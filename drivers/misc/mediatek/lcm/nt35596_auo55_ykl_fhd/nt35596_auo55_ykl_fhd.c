#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif
#include "lcm_drv.h"
#ifdef BUILD_LK
	#include <platform/upmu_common.h>
	#include <platform/upmu_hw.h>
	#include <platform/mt_gpio.h>
	#include <platform/mt_i2c.h>
	#include <platform/mt_pmic.h>
	//#include <string.h>
#else
	/* #include <mach/mt_pm_ldo.h> */	/* hwPowerOn */
	#include <mt-plat/upmu_common.h>
	#include <mach/upmu_sw.h>
	#include <mach/upmu_hw.h>

	#include <mt-plat/mt_gpio.h>
#endif
/*#include <cust_gpio_usage.h>*/

#if defined(BUILD_LK)
	#define LCM_DEBUG  printf
	#define LCM_FUNC_TRACE() printf("huyl [uboot] %s\n",__func__)
#else
	#define LCM_DEBUG  printk
	#define LCM_FUNC_TRACE() printk("huyl [kernel] %s\n",__func__)
#endif
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------
#define FRAME_WIDTH  (1080)
#define FRAME_HEIGHT (1920)
#define LCM_ID_NT35596 (0x96)
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------
static LCM_UTIL_FUNCS lcm_util = {0};
#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))
#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))
#define REGFLAG_DELAY                                           0xFC
#define REGFLAG_END_OF_TABLE                                0xFD   // END OF REGISTERS MARKER
// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)
#define dsi_lcm_set_gpio_out(pin, out)										lcm_util.set_gpio_out(pin, out)
#define dsi_lcm_set_gpio_mode(pin, mode)									lcm_util.set_gpio_mode(pin, mode)
#define dsi_lcm_set_gpio_dir(pin, dir)										lcm_util.set_gpio_dir(pin, dir)
#define dsi_lcm_set_gpio_pull_enable(pin, en)								lcm_util.set_gpio_pull_enable(pin, en)
#define   LCM_DSI_CMD_MODE							0

static void lcm_init_power(void)
{
}

static void lcm_suspend_power(void)
{
}

static void lcm_resume_power(void)
{

}

void TC358768_DCS_write_1A_1P(unsigned char cmd, unsigned char para)
{
  unsigned int data_array[16];
  //unsigned char buffer;
  data_array[0] =(0x00023902);
  data_array[1] =(0x00000000| (para<<8)|(cmd));
  dsi_set_cmdq(data_array, 2, 1);
  //MDELAY(1);
}

#define TC358768_DCS_write_1A_0P(cmd)							data_array[0]=(0x00000500 | (cmd<<16)); \
                                                  dsi_set_cmdq(data_array, 1, 1);
static void init_lcm_registers(void)
{
  unsigned int data_array[16];
  //unsigned char buffer[8];
  TC358768_DCS_write_1A_1P(0xFF,0xEE);
  TC358768_DCS_write_1A_1P(0xFB,0x01);
  TC358768_DCS_write_1A_1P(0x18,0x40);
  MDELAY(10);
  TC358768_DCS_write_1A_1P(0x18,0x00);
  MDELAY(20);
  TC358768_DCS_write_1A_1P(0xFF,0x05);
  TC358768_DCS_write_1A_1P(0xFB,0x01);
  TC358768_DCS_write_1A_1P(0xC5,0x31);  // VSOUT = VDDI
  MDELAY(20);

  TC358768_DCS_write_1A_1P(0xFF,0xEE);
  TC358768_DCS_write_1A_1P(0x7c,0x31);

  TC358768_DCS_write_1A_1P(0xFF,0x01);
  TC358768_DCS_write_1A_1P(0xFB,0x01);
  TC358768_DCS_write_1A_1P(0x00,0x01);
  TC358768_DCS_write_1A_1P(0x01,0x55);
  TC358768_DCS_write_1A_1P(0x02,0x40);
  TC358768_DCS_write_1A_1P(0x05,0x50);
  TC358768_DCS_write_1A_1P(0x06,0x4A);  // VCI & AVDD LV detect enable 
  TC358768_DCS_write_1A_1P(0x07,0x29);
  TC358768_DCS_write_1A_1P(0x08,0x0C);
  TC358768_DCS_write_1A_1P(0x0B,0x9B);  //GVDDP
  TC358768_DCS_write_1A_1P(0x0C,0x9B);   //GVDDN
  TC358768_DCS_write_1A_1P(0x0E,0xB0);
  TC358768_DCS_write_1A_1P(0x0F,0xB3);  //VGL=2Eh= -8V 
  TC358768_DCS_write_1A_1P(0x11,0x10);    //vcom
  TC358768_DCS_write_1A_1P(0x12,0x10);
  TC358768_DCS_write_1A_1P(0x13,0x03);
  TC358768_DCS_write_1A_1P(0x14,0x4A);
  TC358768_DCS_write_1A_1P(0x15,0x12);
  TC358768_DCS_write_1A_1P(0x16,0x12);
  TC358768_DCS_write_1A_1P(0x18,0x00);
  TC358768_DCS_write_1A_1P(0x19,0x77);
  TC358768_DCS_write_1A_1P(0x1A,0x55);
  TC358768_DCS_write_1A_1P(0x1B,0x13);
  TC358768_DCS_write_1A_1P(0x1C,0x00);
  TC358768_DCS_write_1A_1P(0x1D,0x00);
  TC358768_DCS_write_1A_1P(0x1E,0x00);
  TC358768_DCS_write_1A_1P(0x1F,0x00);
  TC358768_DCS_write_1A_1P(0x58,0x82);
  TC358768_DCS_write_1A_1P(0x59,0x02);
  TC358768_DCS_write_1A_1P(0x5A,0x02);
  TC358768_DCS_write_1A_1P(0x5B,0x02);
  TC358768_DCS_write_1A_1P(0x5C,0x82);
  TC358768_DCS_write_1A_1P(0x5D,0x82);
  TC358768_DCS_write_1A_1P(0x5E,0x02);
  TC358768_DCS_write_1A_1P(0x5F,0x02);
  TC358768_DCS_write_1A_1P(0x72,0x31);
  TC358768_DCS_write_1A_1P(0xFF,0x05);
  TC358768_DCS_write_1A_1P(0xFB,0x01);
  TC358768_DCS_write_1A_1P(0x00,0x01);
  TC358768_DCS_write_1A_1P(0x01,0x0B);
  TC358768_DCS_write_1A_1P(0x02,0x0C);
  TC358768_DCS_write_1A_1P(0x03,0x09);
  TC358768_DCS_write_1A_1P(0x04,0x0A);
//CMOS TYPE PANEL
  TC358768_DCS_write_1A_1P(0x05,0x00);
  TC358768_DCS_write_1A_1P(0x06,0x0F);
  TC358768_DCS_write_1A_1P(0x07,0x10);
  TC358768_DCS_write_1A_1P(0x08,0x00);
//
  TC358768_DCS_write_1A_1P(0x09,0x00);
  TC358768_DCS_write_1A_1P(0x0A,0x00);
  TC358768_DCS_write_1A_1P(0x0B,0x00);
  TC358768_DCS_write_1A_1P(0x0C,0x00);
  TC358768_DCS_write_1A_1P(0x0D,0x13);
  TC358768_DCS_write_1A_1P(0x0E,0x15);
  TC358768_DCS_write_1A_1P(0x0F,0x17);
  TC358768_DCS_write_1A_1P(0x10,0x01);
  TC358768_DCS_write_1A_1P(0x11,0x0B);
  TC358768_DCS_write_1A_1P(0x12,0x0C);
  TC358768_DCS_write_1A_1P(0x13,0x09);
  TC358768_DCS_write_1A_1P(0x14,0x0A);
  TC358768_DCS_write_1A_1P(0x15,0x00);
  TC358768_DCS_write_1A_1P(0x16,0x0F);
  TC358768_DCS_write_1A_1P(0x17,0x10);
  TC358768_DCS_write_1A_1P(0x18,0x00);
  TC358768_DCS_write_1A_1P(0x19,0x00);
  TC358768_DCS_write_1A_1P(0x1A,0x00);
  TC358768_DCS_write_1A_1P(0x1B,0x00);
  TC358768_DCS_write_1A_1P(0x1C,0x00);
  TC358768_DCS_write_1A_1P(0x1D,0x13);
  TC358768_DCS_write_1A_1P(0x1E,0x15);
  TC358768_DCS_write_1A_1P(0x1F,0x17);
  TC358768_DCS_write_1A_1P(0x20,0x00);
  TC358768_DCS_write_1A_1P(0x21,0x03);
  TC358768_DCS_write_1A_1P(0x22,0x01);
  TC358768_DCS_write_1A_1P(0x23,0x36);
  TC358768_DCS_write_1A_1P(0x24,0x36);
  TC358768_DCS_write_1A_1P(0x25,0xED);
  TC358768_DCS_write_1A_1P(0x29,0x58);    
  TC358768_DCS_write_1A_1P(0x2A,0x12);
  TC358768_DCS_write_1A_1P(0x2B,0x01);
  TC358768_DCS_write_1A_1P(0x4B,0x06);
  TC358768_DCS_write_1A_1P(0x4C,0x11);
  TC358768_DCS_write_1A_1P(0x4D,0x20);
  TC358768_DCS_write_1A_1P(0x4E,0x02);
  TC358768_DCS_write_1A_1P(0x4F,0x02);
  TC358768_DCS_write_1A_1P(0x50,0x20);
  TC358768_DCS_write_1A_1P(0x51,0x61);
  TC358768_DCS_write_1A_1P(0x52,0x01);
  TC358768_DCS_write_1A_1P(0x53,0x72);
  TC358768_DCS_write_1A_1P(0x54,0x75);
  TC358768_DCS_write_1A_1P(0x55,0xED);
  TC358768_DCS_write_1A_1P(0x5B,0x00);
  TC358768_DCS_write_1A_1P(0x5C,0x00);
  //TC358768_DCS_write_1A_1P(0x5D,0x00);
  //TC358768_DCS_write_1A_1P(0x5E,0x00);
  TC358768_DCS_write_1A_1P(0x5F,0x15);
  TC358768_DCS_write_1A_1P(0x60,0x75);
  //TC358768_DCS_write_1A_1P(0x61,0x00);
  //TC358768_DCS_write_1A_1P(0x62,0x00);
  TC358768_DCS_write_1A_1P(0x63,0x00);
  TC358768_DCS_write_1A_1P(0x64,0x00);
 // TC358768_DCS_write_1A_1P(0x65,0x00);
 // TC358768_DCS_write_1A_1P(0x66,0x00);
  TC358768_DCS_write_1A_1P(0x67,0x00);
  TC358768_DCS_write_1A_1P(0x68,0x04);
  //TC358768_DCS_write_1A_1P(0x69,0x00);
  //TC358768_DCS_write_1A_1P(0x6A,0x00);
  TC358768_DCS_write_1A_1P(0x6C,0x40);
  //TC358768_DCS_write_1A_1P(0x75,0x01);
  //TC358768_DCS_write_1A_1P(0x76,0x01);
  TC358768_DCS_write_1A_1P(0x7A,0x80);
  TC358768_DCS_write_1A_1P(0x7B,0xA3);
  TC358768_DCS_write_1A_1P(0x7C,0xD8);
  TC358768_DCS_write_1A_1P(0x7D,0x60);
  TC358768_DCS_write_1A_1P(0x7E,0x08);
  TC358768_DCS_write_1A_1P(0x7F,0x1C);
  TC358768_DCS_write_1A_1P(0x80,0x00);
  TC358768_DCS_write_1A_1P(0x83,0x00);
  TC358768_DCS_write_1A_1P(0x93,0x08);
  TC358768_DCS_write_1A_1P(0x94,0x0A);
  
  TC358768_DCS_write_1A_1P(0x8A,0x00);
  TC358768_DCS_write_1A_1P(0x9B,0x0f);//0f    inversion
  TC358768_DCS_write_1A_1P(0xE7,0x80);//add  video drop enable  
   //Gamma   
  TC358768_DCS_write_1A_1P(0xFF,0x01);
  TC358768_DCS_write_1A_1P(0xFB,0x01);                            
  TC358768_DCS_write_1A_1P(0x75,0x00);   
  TC358768_DCS_write_1A_1P(0x76,0x18);   
  TC358768_DCS_write_1A_1P(0x77,0x00);   
  TC358768_DCS_write_1A_1P(0x78,0x38);   
  TC358768_DCS_write_1A_1P(0x79,0x00);   
  TC358768_DCS_write_1A_1P(0x7A,0x65);   
  TC358768_DCS_write_1A_1P(0x7B,0x00);   
  TC358768_DCS_write_1A_1P(0x7C,0x84);   
  TC358768_DCS_write_1A_1P(0x7D,0x00);   
  TC358768_DCS_write_1A_1P(0x7E,0x9B);   
  TC358768_DCS_write_1A_1P(0x7F,0x00);   
  TC358768_DCS_write_1A_1P(0x80,0xAF);   
  TC358768_DCS_write_1A_1P(0x81,0x00);   
  TC358768_DCS_write_1A_1P(0x82,0xC1);   
  TC358768_DCS_write_1A_1P(0x83,0x00);   
  TC358768_DCS_write_1A_1P(0x84,0xD2);   
  TC358768_DCS_write_1A_1P(0x85,0x00);   
  TC358768_DCS_write_1A_1P(0x86,0xDF);   
  TC358768_DCS_write_1A_1P(0x87,0x01);   
  TC358768_DCS_write_1A_1P(0x88,0x11);   
  TC358768_DCS_write_1A_1P(0x89,0x01);   
  TC358768_DCS_write_1A_1P(0x8A,0x38);   
  TC358768_DCS_write_1A_1P(0x8B,0x01);   
  TC358768_DCS_write_1A_1P(0x8C,0x76);   
  TC358768_DCS_write_1A_1P(0x8D,0x01);   
  TC358768_DCS_write_1A_1P(0x8E,0xA7);   
  TC358768_DCS_write_1A_1P(0x8F,0x01);   
  TC358768_DCS_write_1A_1P(0x90,0xF3);   
  TC358768_DCS_write_1A_1P(0x91,0x02);   
  TC358768_DCS_write_1A_1P(0x92,0x2F);   
  TC358768_DCS_write_1A_1P(0x93,0x02);   
  TC358768_DCS_write_1A_1P(0x94,0x30);   
  TC358768_DCS_write_1A_1P(0x95,0x02);   
  TC358768_DCS_write_1A_1P(0x96,0x66);   
  TC358768_DCS_write_1A_1P(0x97,0x02);   
  TC358768_DCS_write_1A_1P(0x98,0xA0);   
  TC358768_DCS_write_1A_1P(0x99,0x02);   
  TC358768_DCS_write_1A_1P(0x9A,0xC5);   
  TC358768_DCS_write_1A_1P(0x9B,0x02);   
  TC358768_DCS_write_1A_1P(0x9C,0xF8);   
  TC358768_DCS_write_1A_1P(0x9D,0x03);   
  TC358768_DCS_write_1A_1P(0x9E,0x1B);   
  TC358768_DCS_write_1A_1P(0x9F,0x03);   
  TC358768_DCS_write_1A_1P(0xA0,0x46);   
  TC358768_DCS_write_1A_1P(0xA2,0x03);   
  TC358768_DCS_write_1A_1P(0xA3,0x52);   
  TC358768_DCS_write_1A_1P(0xA4,0x03);   
  TC358768_DCS_write_1A_1P(0xA5,0x62);   
  TC358768_DCS_write_1A_1P(0xA6,0x03);   
  TC358768_DCS_write_1A_1P(0xA7,0x71);   
  TC358768_DCS_write_1A_1P(0xA9,0x03);   
  TC358768_DCS_write_1A_1P(0xAA,0x83);   
  TC358768_DCS_write_1A_1P(0xAB,0x03);   
  TC358768_DCS_write_1A_1P(0xAC,0x94);   
  TC358768_DCS_write_1A_1P(0xAD,0x03);   
  TC358768_DCS_write_1A_1P(0xAE,0xA3);   
  TC358768_DCS_write_1A_1P(0xAF,0x03);   
  TC358768_DCS_write_1A_1P(0xB0,0xAD);   
  TC358768_DCS_write_1A_1P(0xB1,0x03);   
  TC358768_DCS_write_1A_1P(0xB2,0xCC);   
  TC358768_DCS_write_1A_1P(0xB3,0x00);   
  TC358768_DCS_write_1A_1P(0xB4,0x18);   
  TC358768_DCS_write_1A_1P(0xB5,0x00);   
  TC358768_DCS_write_1A_1P(0xB6,0x38);   
  TC358768_DCS_write_1A_1P(0xB7,0x00);   
  TC358768_DCS_write_1A_1P(0xB8,0x65);   
  TC358768_DCS_write_1A_1P(0xB9,0x00);   
  TC358768_DCS_write_1A_1P(0xBA,0x84);   
  TC358768_DCS_write_1A_1P(0xBB,0x00);   
  TC358768_DCS_write_1A_1P(0xBC,0x9B);   
  TC358768_DCS_write_1A_1P(0xBD,0x00);   
  TC358768_DCS_write_1A_1P(0xBE,0xAF);   
  TC358768_DCS_write_1A_1P(0xBF,0x00);   
  TC358768_DCS_write_1A_1P(0xC0,0xC1);   
  TC358768_DCS_write_1A_1P(0xC1,0x00);   
  TC358768_DCS_write_1A_1P(0xC2,0xD2);   
  TC358768_DCS_write_1A_1P(0xC3,0x00);   
  TC358768_DCS_write_1A_1P(0xC4,0xDF);   
  TC358768_DCS_write_1A_1P(0xC5,0x01);   
  TC358768_DCS_write_1A_1P(0xC6,0x11);   
  TC358768_DCS_write_1A_1P(0xC7,0x01);   
  TC358768_DCS_write_1A_1P(0xC8,0x38);   
  TC358768_DCS_write_1A_1P(0xC9,0x01);   
  TC358768_DCS_write_1A_1P(0xCA,0x76);   
  TC358768_DCS_write_1A_1P(0xCB,0x01);   
  TC358768_DCS_write_1A_1P(0xCC,0xA7);   
  TC358768_DCS_write_1A_1P(0xCD,0x01);   
  TC358768_DCS_write_1A_1P(0xCE,0xF3);   
  TC358768_DCS_write_1A_1P(0xCF,0x02);   
  TC358768_DCS_write_1A_1P(0xD0,0x2F);   
  TC358768_DCS_write_1A_1P(0xD1,0x02);   
  TC358768_DCS_write_1A_1P(0xD2,0x30);   
  TC358768_DCS_write_1A_1P(0xD3,0x02);   
  TC358768_DCS_write_1A_1P(0xD4,0x66);   
  TC358768_DCS_write_1A_1P(0xD5,0x02);   
  TC358768_DCS_write_1A_1P(0xD6,0xA0);   
  TC358768_DCS_write_1A_1P(0xD7,0x02);   
  TC358768_DCS_write_1A_1P(0xD8,0xC5);   
  TC358768_DCS_write_1A_1P(0xD9,0x02);   
  TC358768_DCS_write_1A_1P(0xDA,0xF8);   
  TC358768_DCS_write_1A_1P(0xDB,0x03);   
  TC358768_DCS_write_1A_1P(0xDC,0x1B);   
  TC358768_DCS_write_1A_1P(0xDD,0x03);   
  TC358768_DCS_write_1A_1P(0xDE,0x46);   
  TC358768_DCS_write_1A_1P(0xDF,0x03);   
  TC358768_DCS_write_1A_1P(0xE0,0x52);   
  TC358768_DCS_write_1A_1P(0xE1,0x03);   
  TC358768_DCS_write_1A_1P(0xE2,0x62);   
  TC358768_DCS_write_1A_1P(0xE3,0x03);   
  TC358768_DCS_write_1A_1P(0xE4,0x71);   
  TC358768_DCS_write_1A_1P(0xE5,0x03);   
  TC358768_DCS_write_1A_1P(0xE6,0x83);   
  TC358768_DCS_write_1A_1P(0xE7,0x03);   
  TC358768_DCS_write_1A_1P(0xE8,0x94);   
  TC358768_DCS_write_1A_1P(0xE9,0x03);   
  TC358768_DCS_write_1A_1P(0xEA,0xA3);   
  TC358768_DCS_write_1A_1P(0xEB,0x03);   
  TC358768_DCS_write_1A_1P(0xEC,0xAD);   
  TC358768_DCS_write_1A_1P(0xED,0x03);   
  TC358768_DCS_write_1A_1P(0xEE,0xCC);   
  TC358768_DCS_write_1A_1P(0xEF,0x00);   
  TC358768_DCS_write_1A_1P(0xF0,0x18);   
  TC358768_DCS_write_1A_1P(0xF1,0x00);   
  TC358768_DCS_write_1A_1P(0xF2,0x38);   
  TC358768_DCS_write_1A_1P(0xF3,0x00);   
  TC358768_DCS_write_1A_1P(0xF4,0x65);   
  TC358768_DCS_write_1A_1P(0xF5,0x00);   
  TC358768_DCS_write_1A_1P(0xF6,0x84);   
  TC358768_DCS_write_1A_1P(0xF7,0x00);   
  TC358768_DCS_write_1A_1P(0xF8,0x9B);   
  TC358768_DCS_write_1A_1P(0xF9,0x00);   
  TC358768_DCS_write_1A_1P(0xFA,0xAF);   
  TC358768_DCS_write_1A_1P(0xFB,0x01);   
  TC358768_DCS_write_1A_1P(0xFF,0x02);   
  TC358768_DCS_write_1A_1P(0x00,0x00);   
  TC358768_DCS_write_1A_1P(0x01,0xC1);   
  TC358768_DCS_write_1A_1P(0x02,0x00);   
  TC358768_DCS_write_1A_1P(0x03,0xD2);   
  TC358768_DCS_write_1A_1P(0x04,0x00);   
  TC358768_DCS_write_1A_1P(0x05,0xDF);   
  TC358768_DCS_write_1A_1P(0x06,0x01);   
  TC358768_DCS_write_1A_1P(0x07,0x11);   
  TC358768_DCS_write_1A_1P(0x08,0x01);   
  TC358768_DCS_write_1A_1P(0x09,0x38);   
  TC358768_DCS_write_1A_1P(0x0A,0x01);   
  TC358768_DCS_write_1A_1P(0x0B,0x76);   
  TC358768_DCS_write_1A_1P(0x0C,0x01);   
  TC358768_DCS_write_1A_1P(0x0D,0xA7);   
  TC358768_DCS_write_1A_1P(0x0E,0x01);   
  TC358768_DCS_write_1A_1P(0x0F,0xF3);   
  TC358768_DCS_write_1A_1P(0x10,0x02);   
  TC358768_DCS_write_1A_1P(0x11,0x2F);   
  TC358768_DCS_write_1A_1P(0x12,0x02);   
  TC358768_DCS_write_1A_1P(0x13,0x30);   
  TC358768_DCS_write_1A_1P(0x14,0x02);   
  TC358768_DCS_write_1A_1P(0x15,0x66);   
  TC358768_DCS_write_1A_1P(0x16,0x02);   
  TC358768_DCS_write_1A_1P(0x17,0xA0);   
  TC358768_DCS_write_1A_1P(0x18,0x02);   
  TC358768_DCS_write_1A_1P(0x19,0xC5);   
  TC358768_DCS_write_1A_1P(0x1A,0x02);   
  TC358768_DCS_write_1A_1P(0x1B,0xF8);   
  TC358768_DCS_write_1A_1P(0x1C,0x03);   
  TC358768_DCS_write_1A_1P(0x1D,0x1B);   
  TC358768_DCS_write_1A_1P(0x1E,0x03);   
  TC358768_DCS_write_1A_1P(0x1F,0x46);   
  TC358768_DCS_write_1A_1P(0x20,0x03);   
  TC358768_DCS_write_1A_1P(0x21,0x52);   
  TC358768_DCS_write_1A_1P(0x22,0x03);   
  TC358768_DCS_write_1A_1P(0x23,0x62);   
  TC358768_DCS_write_1A_1P(0x24,0x03);   
  TC358768_DCS_write_1A_1P(0x25,0x71);   
  TC358768_DCS_write_1A_1P(0x26,0x03);   
  TC358768_DCS_write_1A_1P(0x27,0x83);   
  TC358768_DCS_write_1A_1P(0x28,0x03);   
  TC358768_DCS_write_1A_1P(0x29,0x94);   
  TC358768_DCS_write_1A_1P(0x2A,0x03);   
//  TC358768_DCS_write_1A_1P(0x2B,0xA3);   
  TC358768_DCS_write_1A_1P(0x2D,0x03);   
  TC358768_DCS_write_1A_1P(0x2F,0xAD);   
  TC358768_DCS_write_1A_1P(0x30,0x03);   
  TC358768_DCS_write_1A_1P(0x31,0xCC);   
  TC358768_DCS_write_1A_1P(0x32,0x00);   
  TC358768_DCS_write_1A_1P(0x33,0x18);   
  TC358768_DCS_write_1A_1P(0x34,0x00);   
  TC358768_DCS_write_1A_1P(0x35,0x38);   
  TC358768_DCS_write_1A_1P(0x36,0x00);   
  TC358768_DCS_write_1A_1P(0x37,0x65);   
  TC358768_DCS_write_1A_1P(0x38,0x00);   
  TC358768_DCS_write_1A_1P(0x39,0x84);   
  TC358768_DCS_write_1A_1P(0x3A,0x00);   
  TC358768_DCS_write_1A_1P(0x3B,0x9B);   
  TC358768_DCS_write_1A_1P(0x3D,0x00);   
  TC358768_DCS_write_1A_1P(0x3F,0xAF);   
  TC358768_DCS_write_1A_1P(0x40,0x00);   
  TC358768_DCS_write_1A_1P(0x41,0xC1);   
  TC358768_DCS_write_1A_1P(0x42,0x00);   
  TC358768_DCS_write_1A_1P(0x43,0xD2);   
  TC358768_DCS_write_1A_1P(0x44,0x00);   
  TC358768_DCS_write_1A_1P(0x45,0xDF);   
  TC358768_DCS_write_1A_1P(0x46,0x01);   
  TC358768_DCS_write_1A_1P(0x47,0x11);   
  TC358768_DCS_write_1A_1P(0x48,0x01);   
  TC358768_DCS_write_1A_1P(0x49,0x38);   
  TC358768_DCS_write_1A_1P(0x4A,0x01);   
  TC358768_DCS_write_1A_1P(0x4B,0x76);   
  TC358768_DCS_write_1A_1P(0x4C,0x01);   
  TC358768_DCS_write_1A_1P(0x4D,0xA7);   
  TC358768_DCS_write_1A_1P(0x4E,0x01);   
  TC358768_DCS_write_1A_1P(0x4F,0xF3);   
  TC358768_DCS_write_1A_1P(0x50,0x02);   
  TC358768_DCS_write_1A_1P(0x51,0x2F);   
  TC358768_DCS_write_1A_1P(0x52,0x02);   
  TC358768_DCS_write_1A_1P(0x53,0x30);   
  TC358768_DCS_write_1A_1P(0x54,0x02);   
  TC358768_DCS_write_1A_1P(0x55,0x66);   
  TC358768_DCS_write_1A_1P(0x56,0x02);   
  TC358768_DCS_write_1A_1P(0x58,0xA0);   
  TC358768_DCS_write_1A_1P(0x59,0x02);   
  TC358768_DCS_write_1A_1P(0x5A,0xC5);   
  TC358768_DCS_write_1A_1P(0x5B,0x02);   
  TC358768_DCS_write_1A_1P(0x5C,0xF8);   
  TC358768_DCS_write_1A_1P(0x5D,0x03);   
  TC358768_DCS_write_1A_1P(0x5E,0x1B);   
  TC358768_DCS_write_1A_1P(0x5F,0x03);   
  TC358768_DCS_write_1A_1P(0x60,0x46);   
  TC358768_DCS_write_1A_1P(0x61,0x03);   
  TC358768_DCS_write_1A_1P(0x62,0x52);   
  TC358768_DCS_write_1A_1P(0x63,0x03);   
  TC358768_DCS_write_1A_1P(0x64,0x62);   
  TC358768_DCS_write_1A_1P(0x65,0x03);   
  TC358768_DCS_write_1A_1P(0x66,0x71);   
  TC358768_DCS_write_1A_1P(0x67,0x03);   
  TC358768_DCS_write_1A_1P(0x68,0x83);   
  TC358768_DCS_write_1A_1P(0x69,0x03);   
  TC358768_DCS_write_1A_1P(0x6A,0x94);   
  TC358768_DCS_write_1A_1P(0x6B,0x03);   
  //TC358768_DCS_write_1A_1P(0x6C,0xA3);   
  TC358768_DCS_write_1A_1P(0x6D,0x03);   
  TC358768_DCS_write_1A_1P(0x6E,0xAD);   
  TC358768_DCS_write_1A_1P(0x6F,0x03);   
  TC358768_DCS_write_1A_1P(0x70,0xCC);   
  TC358768_DCS_write_1A_1P(0x71,0x00);   
  TC358768_DCS_write_1A_1P(0x72,0x18);   
  TC358768_DCS_write_1A_1P(0x73,0x00);   
  TC358768_DCS_write_1A_1P(0x74,0x38);   
  TC358768_DCS_write_1A_1P(0x75,0x00);   
  TC358768_DCS_write_1A_1P(0x76,0x65);   
  TC358768_DCS_write_1A_1P(0x77,0x00);   
  TC358768_DCS_write_1A_1P(0x78,0x84);   
  TC358768_DCS_write_1A_1P(0x79,0x00);   
  TC358768_DCS_write_1A_1P(0x7A,0x9B);   
  TC358768_DCS_write_1A_1P(0x7B,0x00);   
  TC358768_DCS_write_1A_1P(0x7C,0xAF);   
  TC358768_DCS_write_1A_1P(0x7D,0x00);   
  TC358768_DCS_write_1A_1P(0x7E,0xC1);   
  TC358768_DCS_write_1A_1P(0x7F,0x00);   
  TC358768_DCS_write_1A_1P(0x80,0xD2);   
  TC358768_DCS_write_1A_1P(0x81,0x00);   
  TC358768_DCS_write_1A_1P(0x82,0xDF);   
  TC358768_DCS_write_1A_1P(0x83,0x01);   
  TC358768_DCS_write_1A_1P(0x84,0x11);   
  TC358768_DCS_write_1A_1P(0x85,0x01);   
  TC358768_DCS_write_1A_1P(0x86,0x38);   
  TC358768_DCS_write_1A_1P(0x87,0x01);   
  TC358768_DCS_write_1A_1P(0x88,0x76);   
  TC358768_DCS_write_1A_1P(0x89,0x01);   
  TC358768_DCS_write_1A_1P(0x8A,0xA7);   
  TC358768_DCS_write_1A_1P(0x8B,0x01);   
  TC358768_DCS_write_1A_1P(0x8C,0xF3);   
  TC358768_DCS_write_1A_1P(0x8D,0x02);   
  TC358768_DCS_write_1A_1P(0x8E,0x2F);   
  TC358768_DCS_write_1A_1P(0x8F,0x02);   
  TC358768_DCS_write_1A_1P(0x90,0x30);   
  TC358768_DCS_write_1A_1P(0x91,0x02);   
  TC358768_DCS_write_1A_1P(0x92,0x66);   
  TC358768_DCS_write_1A_1P(0x93,0x02);   
  TC358768_DCS_write_1A_1P(0x94,0xA0);   
  TC358768_DCS_write_1A_1P(0x95,0x02);   
  TC358768_DCS_write_1A_1P(0x96,0xC5);   
  TC358768_DCS_write_1A_1P(0x97,0x02);   
  TC358768_DCS_write_1A_1P(0x98,0xF8);   
  TC358768_DCS_write_1A_1P(0x99,0x03);   
  TC358768_DCS_write_1A_1P(0x9A,0x1B);   
  TC358768_DCS_write_1A_1P(0x9B,0x03);   
  TC358768_DCS_write_1A_1P(0x9C,0x46);   
  TC358768_DCS_write_1A_1P(0x9D,0x03);   
  TC358768_DCS_write_1A_1P(0x9E,0x52);   
  TC358768_DCS_write_1A_1P(0x9F,0x03);   
  TC358768_DCS_write_1A_1P(0xA0,0x62);   
  TC358768_DCS_write_1A_1P(0xA2,0x03);   
  TC358768_DCS_write_1A_1P(0xA3,0x71);   
  TC358768_DCS_write_1A_1P(0xA4,0x03);   
  TC358768_DCS_write_1A_1P(0xA5,0x83);   
  TC358768_DCS_write_1A_1P(0xA6,0x03);   
  TC358768_DCS_write_1A_1P(0xA7,0x94);   
  TC358768_DCS_write_1A_1P(0xA9,0x03);   
  TC358768_DCS_write_1A_1P(0xAA,0xA3);   
  TC358768_DCS_write_1A_1P(0xAB,0x03);   
  TC358768_DCS_write_1A_1P(0xAC,0xAD);   
  TC358768_DCS_write_1A_1P(0xAD,0x03);   
  TC358768_DCS_write_1A_1P(0xAE,0xCC);   
  TC358768_DCS_write_1A_1P(0xAF,0x00);   
  TC358768_DCS_write_1A_1P(0xB0,0x18);   
  TC358768_DCS_write_1A_1P(0xB1,0x00);   
  TC358768_DCS_write_1A_1P(0xB2,0x38);   
  TC358768_DCS_write_1A_1P(0xB3,0x00);   
  TC358768_DCS_write_1A_1P(0xB4,0x65);   
  TC358768_DCS_write_1A_1P(0xB5,0x00);   
  TC358768_DCS_write_1A_1P(0xB6,0x84);   
  TC358768_DCS_write_1A_1P(0xB7,0x00);   
  TC358768_DCS_write_1A_1P(0xB8,0x9B);   
  TC358768_DCS_write_1A_1P(0xB9,0x00);   
  TC358768_DCS_write_1A_1P(0xBA,0xAF);   
  TC358768_DCS_write_1A_1P(0xBB,0x00);   
  TC358768_DCS_write_1A_1P(0xBC,0xC1);   
  TC358768_DCS_write_1A_1P(0xBD,0x00);   
  TC358768_DCS_write_1A_1P(0xBE,0xD2);   
  TC358768_DCS_write_1A_1P(0xBF,0x00);   
  TC358768_DCS_write_1A_1P(0xC0,0xDF);   
  TC358768_DCS_write_1A_1P(0xC1,0x01);   
  TC358768_DCS_write_1A_1P(0xC2,0x11);   
  TC358768_DCS_write_1A_1P(0xC3,0x01);   
  TC358768_DCS_write_1A_1P(0xC4,0x38);   
  TC358768_DCS_write_1A_1P(0xC5,0x01);   
  TC358768_DCS_write_1A_1P(0xC6,0x76);   
  TC358768_DCS_write_1A_1P(0xC7,0x01);   
  TC358768_DCS_write_1A_1P(0xC8,0xA7);   
  TC358768_DCS_write_1A_1P(0xC9,0x01);   
  TC358768_DCS_write_1A_1P(0xCA,0xF3);   
  TC358768_DCS_write_1A_1P(0xCB,0x02);   
  TC358768_DCS_write_1A_1P(0xCC,0x2F);   
  TC358768_DCS_write_1A_1P(0xCD,0x02);   
  TC358768_DCS_write_1A_1P(0xCE,0x30);   
  TC358768_DCS_write_1A_1P(0xCF,0x02);   
  TC358768_DCS_write_1A_1P(0xD0,0x66);   
  TC358768_DCS_write_1A_1P(0xD1,0x02);   
  TC358768_DCS_write_1A_1P(0xD2,0xA0);   
  TC358768_DCS_write_1A_1P(0xD3,0x02);   
  TC358768_DCS_write_1A_1P(0xD4,0xC5);   
  TC358768_DCS_write_1A_1P(0xD5,0x02);   
  TC358768_DCS_write_1A_1P(0xD6,0xF8);   
  TC358768_DCS_write_1A_1P(0xD7,0x03);   
  TC358768_DCS_write_1A_1P(0xD8,0x1B);   
  TC358768_DCS_write_1A_1P(0xD9,0x03);   
  TC358768_DCS_write_1A_1P(0xDA,0x46);   
  TC358768_DCS_write_1A_1P(0xDB,0x03);   
  TC358768_DCS_write_1A_1P(0xDC,0x52);   
  TC358768_DCS_write_1A_1P(0xDD,0x03);   
  TC358768_DCS_write_1A_1P(0xDE,0x62);   
  TC358768_DCS_write_1A_1P(0xDF,0x03);   
  TC358768_DCS_write_1A_1P(0xE0,0x71);   
  TC358768_DCS_write_1A_1P(0xE1,0x03);   
  TC358768_DCS_write_1A_1P(0xE2,0x83);   
  TC358768_DCS_write_1A_1P(0xE3,0x03);   
  TC358768_DCS_write_1A_1P(0xE4,0x94);   
  TC358768_DCS_write_1A_1P(0xE5,0x03);   
  TC358768_DCS_write_1A_1P(0xE6,0xA3);   
  TC358768_DCS_write_1A_1P(0xE7,0x03);   
  TC358768_DCS_write_1A_1P(0xE8,0xAD);   
  TC358768_DCS_write_1A_1P(0xE9,0x03);   
  TC358768_DCS_write_1A_1P(0xEA,0xCC);   
  TC358768_DCS_write_1A_1P(0xFB,0x01);   
//gamma end                              
	
  //TC358768_DCS_write_1A_1P(0xFF,0x01);
 // TC358768_DCS_write_1A_1P(0xFB,0x01);
 // TC358768_DCS_write_1A_1P(0x07,0x20);
 // TC358768_DCS_write_1A_1P(0x08,0x06);
  TC358768_DCS_write_1A_1P(0xFF,0x01);
  TC358768_DCS_write_1A_1P(0xFB,0x01);
  TC358768_DCS_write_1A_1P(0xFF,0x02);
  TC358768_DCS_write_1A_1P(0xFB,0x01);
  TC358768_DCS_write_1A_1P(0xFF,0x04);
  TC358768_DCS_write_1A_1P(0xFB,0x01);
  TC358768_DCS_write_1A_1P(0xFF,0x00);
  
  TC358768_DCS_write_1A_1P(0xD3,0x06);
  TC358768_DCS_write_1A_1P(0xD4,0x14);
 
  TC358768_DCS_write_1A_1P(0x35,0x00); 
  
  TC358768_DCS_write_1A_1P(0x51,0x00);
  TC358768_DCS_write_1A_1P(0x53,0x24);
  TC358768_DCS_write_1A_1P(0x55,0x00); //01  cabc off mode
  
 TC358768_DCS_write_1A_0P(0x11);
 MDELAY(150);
 TC358768_DCS_write_1A_0P(0x29);
 MDELAY(20);
}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------
static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
  memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

static void lcm_get_params(LCM_PARAMS *params)
{
  memset(params, 0, sizeof(LCM_PARAMS));
  params->type   = LCM_TYPE_DSI;
  params->width  = FRAME_WIDTH;
  params->height = FRAME_HEIGHT;
  // enable tearing-free
  params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;
  params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;
#if (LCM_DSI_CMD_MODE)
  params->dsi.mode   = CMD_MODE;
#else
		params->dsi.mode   = SYNC_PULSE_VDO_MODE;
#endif
  // DSI
  /* Command mode setting */
  //1 Three lane or Four lane
  params->dsi.LANE_NUM				= LCM_FOUR_LANE;
  //The following defined the fomat for data coming from LCD engine.
  params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
  params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
  params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
  params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;
  // Highly depends on LCD driver capability.
  // Not support in MT6573
  params->dsi.packet_size=256;
  // Video mode setting
  params->dsi.intermediat_buffer_num = 0;//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage
  params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
  //		params->dsi.word_count=720*3;
  params->dsi.vertical_sync_active				= 2;
  params->dsi.vertical_backporch					= 4;
  params->dsi.vertical_frontporch					= 20;
  params->dsi.vertical_active_line				= FRAME_HEIGHT;
  params->dsi.horizontal_sync_active				= 10;
  params->dsi.horizontal_backporch				= 118;
  params->dsi.horizontal_frontporch				= 118;
  params->dsi.horizontal_active_pixel				= FRAME_WIDTH;
  // Bit rate calculation
  //1 Every lane speed

  params->dsi.PLL_CLOCK =465; //450
  params->dsi.ssc_disable = 1;  // ssc disable control (1: disable, 0: enable, default: 0)
}

extern void DSI_clk_HS_mode(bool enter);
static void lcm_init(void)
{
  SET_RESET_PIN(1);
  MDELAY(10);
  SET_RESET_PIN(0);
  MDELAY(10);
  SET_RESET_PIN(1);
  MDELAY(50);
  SET_RESET_PIN(0);
  MDELAY(10);
  SET_RESET_PIN(1);
  MDELAY(120);
  init_lcm_registers();
}

static void lcm_suspend(void)
{
  unsigned int data_array[16];
  //unsigned char buffer[2];
#if 0//ndef BUILD_LK
  data_array[0] = 0x00013700;// read id return two byte,version and id
  dsi_set_cmdq(data_array, 1, 1);
  read_reg_v2(0xFE, buffer, 1);
  printk("%s, kernel nt35596 horse debug: nt35596 id = 0x%08x\n", __func__, buffer[0]);
#endif
  TC358768_DCS_write_1A_1P(0xFF,0x01);
  TC358768_DCS_write_1A_1P(0xB1,0x03);
  TC358768_DCS_write_1A_1P(0xB2,0xFF);
  TC358768_DCS_write_1A_1P(0xED,0x03);
  TC358768_DCS_write_1A_1P(0xEE,0xFF);
  TC358768_DCS_write_1A_1P(0xFF,0x02);
  TC358768_DCS_write_1A_1P(0x30,0x03);
  TC358768_DCS_write_1A_1P(0x31,0xFF);
  TC358768_DCS_write_1A_1P(0x6F,0x03);
  TC358768_DCS_write_1A_1P(0x70,0xFF);
  TC358768_DCS_write_1A_1P(0xAD,0x03);
  TC358768_DCS_write_1A_1P(0xAE,0xFF);
  TC358768_DCS_write_1A_1P(0xE9,0x03);
  TC358768_DCS_write_1A_1P(0xEA,0xFF);
  TC358768_DCS_write_1A_1P(0xFF,0x00); 
  data_array[0]=0x00280500; // Display Off
  dsi_set_cmdq(data_array, 1, 1);
  MDELAY(20);
  data_array[0] = 0x00100500; // Sleep In
  dsi_set_cmdq(data_array, 1, 1);
  MDELAY(120);
  TC358768_DCS_write_1A_1P(0xFF,0x05);
  TC358768_DCS_write_1A_1P(0xFB,0x01);
  TC358768_DCS_write_1A_1P(0xC5,0x30);
  MDELAY(20);  
}

static void lcm_resume(void)
{
  //unsigned int data_array[16];
  //unsigned char buffer[2];
  lcm_init();
#if 0//ndef BUILD_LK
  data_array[0] = 0x00013700;// read id return two byte,version and id
  dsi_set_cmdq(data_array, 1, 1);
  read_reg_v2(0xFE, buffer, 1);
  printk("%s, kernel nt35596 horse debug: nt35596 id = 0x%08x\n", __func__, buffer[0]);
#endif
  //TC358768_DCS_write_1A_0P(0x11); // Sleep Out
  //MDELAY(150);
  //TC358768_DCS_write_1A_0P(0x29); // Display On
}




static unsigned int lcm_compare_id(void)
{
  unsigned int id=0;
  unsigned char buffer[2];
  unsigned int array[16];
  SET_RESET_PIN(1);
  MDELAY(10);
  SET_RESET_PIN(0);
  MDELAY(10);
  SET_RESET_PIN(1);
  MDELAY(120);
  array[0] = 0x00023700;// read id return two byte,version and id
  dsi_set_cmdq(array, 1, 1);
  read_reg_v2(0xF4, buffer, 2);
  id = buffer[0]; //we only need ID
#ifdef BUILD_LK
  printf("%s, LK nt35590 debug: nt35590 id = 0x%08x\n", __func__, id);
#else
  printk("%s, kernel nt35590 horse debug: nt35590 id = 0x%08x\n", __func__, id);
#endif
  if(id == LCM_ID_NT35596)
    return 1;
  else
    return 0;
}


LCM_DRIVER nt35596_auo55_ykl_fhd_lcm_drv =
{
  .name			= "nt35596_auo55_ykl_fhd",
  .set_util_funcs = lcm_set_util_funcs,
  .get_params     = lcm_get_params,
  .init           = lcm_init,
  .suspend        = lcm_suspend,
  .resume         = lcm_resume,
  .compare_id     = lcm_compare_id,
  .init_power        = lcm_init_power,
  .resume_power = lcm_resume_power,
  .suspend_power = lcm_suspend_power,
};
