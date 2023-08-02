
#include <stdio.h>
#include <stdlib.h>
#include <csp/csp.h>
#include <param/param.h>
#include <param/param_list.h>
#include <vmem/vmem.h>
#include <vmem/vmem_ram.h>
#include <slash/slash.h>
#include <slash/optparse.h>
#include <slash/dflopt.h>


uint8_t _ram_u8 = 0;
PARAM_DEFINE_STATIC_RAM(100, ram_u8, PARAM_TYPE_UINT8, -1, sizeof(uint8_t), PM_CONF, NULL, "", &_ram_u8, "Test RAM U8");

uint16_t _ram_u16 = 555;
PARAM_DEFINE_STATIC_RAM(101, ram_u16, PARAM_TYPE_UINT16, -1, sizeof(uint16_t), PM_CONF, NULL, "", &_ram_u16, "Test RAM U16");

VMEM_DEFINE_STATIC_RAM(addintest, "addintest", 1024);
PARAM_DEFINE_STATIC_VMEM(150, vmem_u8, PARAM_TYPE_UINT8, 0, sizeof(uint8_t), PM_CONF, NULL, "", addintest, 0x0, "Test VMEM U8");
PARAM_DEFINE_STATIC_VMEM(151, vmem_u16, PARAM_TYPE_UINT16, 0, sizeof(uint16_t), PM_CONF, NULL, "", addintest, 0x1, "Test VMEM U16");

static int apm_example_cmd(struct slash *slash) {
    printf("APM example cmd\n");
    return SLASH_SUCCESS;
}
slash_command(apm_example, apm_example_cmd, "", "An example of adding a slash command in an APM");
