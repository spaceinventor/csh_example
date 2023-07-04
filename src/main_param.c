
#include <stdio.h>
#include <stdlib.h>
#include <csp/csp.h>
#include <param/param.h>
#include <vmem/vmem.h>
#include <vmem/vmem_ram.h>


PARAM_SECTION_INIT(param_addinexample)

static uint8_t _ram_u8 = 0;
PARAM_SEC_DEFINE_STATIC_RAM(param_addinexample, 100, ram_u8, PARAM_TYPE_UINT8, -1, sizeof(uint8_t), PM_CONF, NULL, "", &_ram_u8, "Test RAM U8");

static uint16_t _ram_u16 = 0;
PARAM_SEC_DEFINE_STATIC_RAM(param_addinexample, 101, ram_u16, PARAM_TYPE_UINT16, -1, sizeof(uint8_t), PM_CONF, NULL, "", &_ram_u16, "Test RAM U16");


VMEM_SECTION_INIT(vmem_addinexample);

VMEM_SEC_DEFINE_STATIC_RAM(vmem_addinexample, addintest, "addintest", 1024);

PARAM_SEC_DEFINE_STATIC_VMEM(param_addinexample, 150, vmem_u8, PARAM_TYPE_UINT8, 0, sizeof(uint8_t), PM_CONF, NULL, "", addintest, 0x0, "Test VMEM U8");
PARAM_SEC_DEFINE_STATIC_VMEM(param_addinexample, 151, vmem_u16, PARAM_TYPE_UINT16, 0, sizeof(uint16_t), PM_CONF, NULL, "", addintest, 0x1, "Test VMEM U16");


/* libmain and libinfo are not required */
