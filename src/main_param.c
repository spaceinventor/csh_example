
#include <stdio.h>
#include <stdlib.h>
#include <csp/csp.h>
#include <param/param.h>
#include <param/param_list.h>
#include <vmem/vmem.h>
#include <vmem/vmem_ram.h>


uint8_t _ram_u8 = 0;
PARAM_DEFINE_STATIC_RAM(100, ram_u8, PARAM_TYPE_UINT8, -1, sizeof(uint8_t), PM_CONF, NULL, "", &_ram_u8, "Test RAM U8");

uint16_t _ram_u16 = 555;
PARAM_DEFINE_STATIC_RAM(101, ram_u16, PARAM_TYPE_UINT16, -1, sizeof(uint16_t), PM_CONF, NULL, "", &_ram_u16, "Test RAM U16");

VMEM_DEFINE_STATIC_RAM(addintest, "addintest", 1024);
PARAM_DEFINE_STATIC_VMEM(150, vmem_u8, PARAM_TYPE_UINT8, 0, sizeof(uint8_t), PM_CONF, NULL, "", addintest, 0x0, "Test VMEM U8");
PARAM_DEFINE_STATIC_VMEM(151, vmem_u16, PARAM_TYPE_UINT16, 0, sizeof(uint16_t), PM_CONF, NULL, "", addintest, 0x1, "Test VMEM U16");


extern param_t __start_param;
extern param_t __stop_param;

#if 0
__attribute__((constructor)) void libinit(void) {

}
#endif

void libmain(int argc, char ** argv) {

    /* Check if we have parameter section defined */
    if (&__start_param != &__stop_param) {

        for (param_t * param = &__start_param; param < &__stop_param; param += 1) {
            printf("name %s %p %p %x %p\n", param->name, param, param->addr, param->mask, &param->mask);
            int ret = param_list_add(param);
            printf("Result %d\n", ret);
        }

    }
}

/* libmain and libinfo are not required */
