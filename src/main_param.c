
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
#include <apm/apm.h>

void param_ramu8_cb(param_t * param, int offset) {

    printf("Parameter %s[%d] is set to %d\n", param->name, offset, param_get_uint8_array(param, offset));
}

uint8_t _ram_u8[10];
PARAM_DEFINE_STATIC_RAM(155, ram_u8, PARAM_TYPE_UINT8, 10, sizeof(uint8_t), PM_CONF, param_ramu8_cb, "", &_ram_u8[0], "Test RAM U8");

uint16_t _ram_u16 = 555;
PARAM_DEFINE_STATIC_RAM(156, ram_u16, PARAM_TYPE_UINT16, -1, sizeof(uint16_t), PM_CONF, NULL, "", &_ram_u16, "Test RAM U16");

VMEM_DEFINE_STATIC_RAM(apmtest, "apmtest", 1024);
PARAM_DEFINE_STATIC_VMEM(157, vmem_u8, PARAM_TYPE_UINT8, 0, sizeof(uint8_t), PM_CONF, NULL, "", apmtest, 0x0, "Test VMEM U8");
PARAM_DEFINE_STATIC_VMEM(158, vmem_u16, PARAM_TYPE_UINT16, 0, sizeof(uint16_t), PM_CONF, NULL, "", apmtest, 0x1, "Test VMEM U16");
