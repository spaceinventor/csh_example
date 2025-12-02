#define _GNU_SOURCE
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <csp/drivers/eth_linux.h>
#include <csp/drivers/can_socketcan.h>
#include <csp/interfaces/csp_if_zmqhub.h>
#include <vmem/vmem_server.h>
#include <param/param_server.h>

#include <csp/csp_debug.h>

#include <param/param.h>

#define PARAMID_CSP_DBG_BUFFER_OUT          51
#define PARAMID_CSP_DBG_CONN_OUT            52
#define PARAMID_CSP_DBG_CONN_OVF            53
#define PARAMID_CSP_DBG_CONN_NOROUTE        54
#define PARAMID_CSP_DBG_INVAL_REPLY         55
#define PARAMID_CSP_DBG_ERRNO               56
#define PARAMID_CSP_DBG_CAN_ERRNO           57
#define PARAMID_CSP_DBG_RDP_PRINT           58
#define PARAMID_CSP_DBG_PACKET_PRINT        59



PARAM_DEFINE_STATIC_RAM(PARAMID_CSP_DBG_BUFFER_OUT,   csp_buf_out,         PARAM_TYPE_UINT8,  0, 0, PM_DEBUG | PM_ERRCNT, NULL, "", &csp_dbg_buffer_out, "Number of buffer overruns");
PARAM_DEFINE_STATIC_RAM(PARAMID_CSP_DBG_CONN_OUT,     csp_conn_out,        PARAM_TYPE_UINT8,  0, 0, PM_DEBUG | PM_ERRCNT, NULL, "", &csp_dbg_conn_out, "Number of connection overruns");
PARAM_DEFINE_STATIC_RAM(PARAMID_CSP_DBG_CONN_OVF,     csp_conn_ovf,        PARAM_TYPE_UINT8,  0, 0, PM_DEBUG | PM_ERRCNT, NULL, "", &csp_dbg_conn_ovf, "Number of rx-queue overflows");
PARAM_DEFINE_STATIC_RAM(PARAMID_CSP_DBG_CONN_NOROUTE, csp_conn_noroute,    PARAM_TYPE_UINT8,  0, 0, PM_DEBUG | PM_ERRCNT, NULL, "", &csp_dbg_conn_noroute, "Number of packets dropped due to no-route");
PARAM_DEFINE_STATIC_RAM(PARAMID_CSP_DBG_INVAL_REPLY,  csp_inval_reply,     PARAM_TYPE_UINT8,  0, 0, PM_DEBUG | PM_ERRCNT, NULL, "", &csp_dbg_inval_reply, "Number of invalid replies from csp_transaction");
PARAM_DEFINE_STATIC_RAM(PARAMID_CSP_DBG_ERRNO,        csp_errno,           PARAM_TYPE_UINT8,  0, 0, PM_DEBUG, NULL, "", &csp_dbg_errno, "Global CSP errno, enum in csp_debug.h");
PARAM_DEFINE_STATIC_RAM(PARAMID_CSP_DBG_CAN_ERRNO,    csp_can_errno,       PARAM_TYPE_UINT8,  0, 0, PM_DEBUG, NULL, "", &csp_dbg_can_errno, "CAN driver specific errno, enum in csp_debug.h");
PARAM_DEFINE_STATIC_RAM(PARAMID_CSP_DBG_RDP_PRINT,    csp_print_rdp,       PARAM_TYPE_UINT8,  0, 0, PM_DEBUG, NULL, "", &csp_dbg_rdp_print, "Turn on csp_print of rdp information");
PARAM_DEFINE_STATIC_RAM(PARAMID_CSP_DBG_PACKET_PRINT, csp_print_packet,    PARAM_TYPE_UINT8,  0, 0, PM_DEBUG, NULL, "", &csp_dbg_packet_print, "Turn on csp_print of packet information");

static unsigned int eth_addr = 0;
static unsigned int eth_mask = 12;
static unsigned int can_addr = 0;
static unsigned int can_mask = 12;
static char *eth_dev = "eth0";
static unsigned int zmq_addr = 110;
static unsigned int zmq_mask = 8;
static char *zmq_proxy = "localhost";

static struct option itf_options[] = {
    {"eth_addr", required_argument, 0, 1},
    {"eth_mask", required_argument, 0, 2},
    {"can_addr", required_argument, 0, 3},
    {"can_mask", required_argument, 0, 4},
    {"eth_dev", required_argument, 0, 5},
    {"zmq_addr", required_argument, 0, 7},
    {"zmq_mask", required_argument, 0, 8},
    {"zmq_proxy", required_argument, 0, 9},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}};


void *router_task(void *param)
{
    while (1)
    {
        csp_route_work();
    }
}

void * vmem_server_task(void * param) {
    vmem_server_loop(param);
    return NULL;
}


static int run_in_thread(void *(*routine)(void *), const char *name, void *ctx)
{

    pthread_attr_t attributes;
    pthread_t handle;
    int ret = 0;

    if (pthread_attr_init(&attributes) != 0)
    {
        return -1;
    }

    /* Create the thread as a non-time sliced thread */
    pthread_attr_setschedpolicy(&attributes, SCHED_FIFO);

    ret = pthread_create(&handle, &attributes, routine, ctx);

    if (ret != 0)
    {
        return -1;
    }

    /* Set thread name for debugging purposes */
    pthread_setname_np(handle, name);
    /* Thread has completed with a status */
    pthread_attr_destroy(&attributes);

    return ret;
}


/* Choose an VMEM address which doesn't overlap any other VMEM areas (None in this case).
    If the user read/write or peek/poke an address which doesn't correspond to a VMEM area,
    a naive `memcpy()` will be called on that address instead. */
#define VMEM_CONF_EXAMPLE_ADDR  0x32000700
/* Size (arbitrarily) chosen to fit 8*uint8 + 8*uint16 + 8*uint32 + 8*uint64 */
#define VMEM_CONF_EXAMPLE_SIZE  120

static uint8_t vmem_example_buffer[VMEM_CONF_EXAMPLE_SIZE] = {0};

static void vmem_example_read(vmem_t * vmem, uint64_t addr, void * dataout, uint32_t len) {
    /* Bounds checking already performed by `vmem_read_direct()` */
    // if (addr + len >= VMEM_CONF_EXAMPLE_SIZE) {
    //     return;
    // }

    memcpy(dataout, &vmem_example_buffer[addr], len);
}

static void vmem_example_write(vmem_t * vmem, uint64_t addr, const void * datain, uint32_t len) {
    /* Bounds checking already performed by `vmem_write_direct()` */
    // if (addr + len >= VMEM_CONF_EXAMPLE_SIZE) {
    //     return;
    // }

    memcpy(&vmem_example_buffer[addr], datain, len);
}

/* By supplying `__attribute__((section("vmem")))`,
    we can make the VMEM area show up with CSH's `vmem` command. */
static vmem_t vmem_example __attribute__ ((section("vmem"), used)) = {
    .type = VMEM_TYPE_DRIVER,
    .name = "exmpl",
    .size = VMEM_CONF_EXAMPLE_SIZE,
    .read = vmem_example_read,
    .write = vmem_example_write,
    .big_endian = 0,
    .vaddr = VMEM_CONF_EXAMPLE_ADDR,
    .ack_with_pull = true,
};


/* ----------------------vvv ID (and name vv) should be unique within a CSP process ------------------ vvvvvvv prepends `vmem_` to name specified here. */
PARAM_DEFINE_STATIC_VMEM(200, vmem_custom_u8, PARAM_TYPE_UINT8, 8, sizeof(uint8_t), PM_CONF, NULL, "", example, 0x0, "Test VMEM U8");
PARAM_DEFINE_STATIC_VMEM(201, vmem_custom_u16, PARAM_TYPE_UINT16, 8, sizeof(uint16_t), PM_CONF, NULL, "", example, 0x8, "Test VMEM U8");
PARAM_DEFINE_STATIC_VMEM(202, vmem_custom_u32, PARAM_TYPE_UINT32, 8, sizeof(uint32_t), PM_CONF, NULL, "", example, 0x18, "Test VMEM U8");
PARAM_DEFINE_STATIC_VMEM(203, vmem_custom_u64, PARAM_TYPE_UINT64, 8, sizeof(uint64_t), PM_CONF, NULL, "", example, 0x38, "Test VMEM U8");


int main(int argc, char *argv[])
{
    int c;
    while (1)
    {
        int option_index = 0;
        c = getopt_long(argc, argv, "",
                        itf_options, &option_index);
        if (c == -1)
            break;
        switch (c)
        {
        case 1:
            eth_addr = atoi(optarg);
            break;
        case 2:
            eth_mask = atoi(optarg);
            break;
        case 3:
            can_addr = atoi(optarg);
            break;
        case 4:
            can_mask = atoi(optarg);
            break;
        case 5:
            eth_dev = optarg;
            break;
        case 7:
            zmq_addr = atoi(optarg);
            break;
        case 8: 
            zmq_mask = atoi(optarg);
            break;
        case 9:
            zmq_proxy = optarg;
            break;
        case 'h':
        case '?':
            printf("Options are:\n");
            printf("\t- eth_dev: %s\n", eth_dev);
            printf("\t- eth_addr: %d\n", eth_addr);
            printf("\t- eth_mask: %d\n", eth_mask);
            printf("\t- can_addr: %d\n", can_addr);
            printf("\t- can_mask: %d\n", can_mask);
            printf("\t- zmq_addr: %d\n", zmq_addr);
            printf("\t- zmq_mask: %d\n", zmq_mask);
            printf("\t- zmq_proxy: %s\n", zmq_proxy);
        default:
            exit(1);
        }
    }

    extern const char * csh_example_version_string;

    csp_conf.hostname = "param_example_server";
    csp_conf.model = "Example Libparam Server";
    csp_conf.revision = csh_example_version_string;
    csp_init();


    if(can_addr) {
        csp_iface_t *can_itf;
        if (CSP_ERR_NONE != csp_can_socketcan_open_and_add_interface("can0", "CAN", can_addr, 1000000, 1, &can_itf))
        {
            fprintf(stderr, "Cannot instantiate CAN interface!\n");
        } else {
            can_itf->is_default = 1;
            can_itf->addr = can_addr;
            can_itf->netmask = can_mask;
        }
    }
    if(eth_addr) {
        csp_iface_t *eth_itf;
        if (CSP_ERR_NONE != csp_eth_init(eth_dev, "ETH", 1200, eth_addr, 0, &eth_itf))
        {
            fprintf(stderr, "Cannot instantiate ETH interface!\n");
        } else {
            eth_itf->is_default = 1;
            eth_itf->addr = eth_addr;
            eth_itf->netmask = eth_mask;
        }
    }
    if(zmq_addr) {
        csp_iface_t *zmq_itf;
            if (CSP_ERR_NONE != csp_zmqhub_init_filter2((const char *)"ZMQ", zmq_proxy, zmq_addr, zmq_mask, 1, &zmq_itf, 0, 6000, 7000))
        {
            fprintf(stderr, "Cannot instantiate ZMQ interface!\n");
        } else {
            zmq_itf->is_default = 1;
            zmq_itf->addr = zmq_addr;
            zmq_itf->netmask = zmq_mask;
        }
    }
    csp_bind_callback(csp_service_handler, CSP_ANY);
    static pthread_t router_handle;
    pthread_create(&router_handle, NULL, &router_task, NULL);

    run_in_thread(vmem_server_task, "VMEM server", NULL);
    csp_bind_callback(param_serve, PARAM_PORT_SERVER);

    while (1) {
        sleep(10);
    }
    
    return 0;
}
