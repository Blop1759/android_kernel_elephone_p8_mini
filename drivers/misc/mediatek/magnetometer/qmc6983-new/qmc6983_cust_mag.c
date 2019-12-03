#include <cust_mag.h>
#include <linux/types.h>
#include <mach/upmu_sw.h>
#include <mt-plat/upmu_common.h>
#include <cust_mag.h>
#include "mag.h"



static struct mag_hw cust_mag_hw = {
    .i2c_num = 2,
//    .i2c_addr = {0x2c,0,0,0},
    .direction = 3,
    .power_id = MT65XX_POWER_NONE,  /*!< LDO is not used */
    .power_vol= VOL_DEFAULT,        /*!< LDO is not used */
    .is_batch_supported = false,
};
struct mag_hw* qmc6983_get_cust_mag_hw(void) 
{
    return &cust_mag_hw;
}
