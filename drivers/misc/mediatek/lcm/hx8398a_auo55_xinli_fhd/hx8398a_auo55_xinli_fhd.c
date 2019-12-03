/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */
/*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2008
 *
 *  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 *  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
 *  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 *  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 *  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 *  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
 *  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
 *  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
 *  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 *  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 *  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
 *  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
 *  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
 *  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
 *  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
 *  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
 *
 *****************************************************************************/
#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>
#elif defined(BUILD_UBOOT)
#include <asm/arch/mt_gpio.h>
#else
#include <mt-plat/mt_gpio.h>
//#include <mt-plat/upmu_common.h>
#include <linux/string.h>
#include <linux/kernel.h>
#endif
#include "lcm_drv.h"
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------
#define FRAME_WIDTH  (1080)
#define FRAME_HEIGHT (1920)
#define REGFLAG_DELAY             							0xFFE
#define REGFLAG_END_OF_TABLE      							0xFFA    //0xFFF ??   // END OF REGISTERS MARKER

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
//static unsigned int lcm_esd_test = FALSE;      ///only for ESD test
#define LCM_DSI_CMD_MODE									0
#define LCM_ID_HX8398 0x98
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------
static LCM_UTIL_FUNCS lcm_util = {0};
#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))
#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))
// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V22(cmdq, cmd, count, ppara, force_update) \
  lcm_util.dsi_set_cmdq_V22(cmdq, cmd, count, ppara, force_update)
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update) \
  lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update) \
  lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)									lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums) \
  lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd) \
  lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size) \
  lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)
#define set_gpio_lcd_enp(cmd) \
  lcm_util.set_gpio_lcd_enp_bias(cmd)
// ---------------------------------------------------------------------------
#define GPIO_65132_EN GPIO_LCD_BIAS_ENP_PIN
struct LCM_setting_table
{
  unsigned cmd;
  unsigned char count;
  unsigned char para_list[64];
};
/*
static struct LCM_setting_table lcm_initialization_setting[] =
{

  {0xB9, 3, {0xFF,0x83,0x98}},

{0xB1,11,{0x28,0x0D,0x8D,0x01,0x02,0x33,0x31,0x31,0xD0,0xC7,0x0D}},

{0xB2,10,{0x40,0x00,0xAE,0x1C,0x0B,0x45,0x11,0x00,0x00,0x00}},

{0xB4,25,{0x00,0xFF,0x30,0xA0,0x30,0xA0,0x30,0xA0,0x00,0x2F,0xA2,0x11,0x07,0x3A,0x30,0xA0,0x30,0xA0,0x30,0xA0,0x00,0x2F,0xA2,0x3A,0x00}},

{0xBD,1,{0x02}},

{0xD3,3,{0x00,0x04,0x50}},

{0xBD,1,{0x00}},
{0xD3,48,{0x20,0x00,0x10,0x08,0x10,0x08,0x00,0x00,0x1F,0x2F,0x54,0x10,0x03,0x00,0x03,0x54,0x17,0xA1,0x07,0xA1,0x00,0x00,0x00,0x00,0x00,0x77,0x07,0x0B,0x0B,0x7F,0x03,0x03,0x7F,0x18,0x40,0x00,0x03,0x29,0x00,0xE2,0x00,0x00,0x00,0x04,0x29,0x30,0xA0}},
{0xD5,40,{0x18,0x18,0x18,0x18,0x21,0x20,0x18,0x18,0x18,0x18,0x17,0x16,0x07,0x06,0x15,0x14,0x05,0x04,0x13,0x12,0x03,0x02,0x11,0x10,0x01,0x00,0x18,0x18,0x39,0x38,0x25,0x24,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xD6,40,{0x18,0x18,0x18,0x18,0x24,0x25,0x18,0x18,0x18,0x18,0x00,0x01,0x10,0x11,0x02,0x03,0x12,0x13,0x04,0x05,0x14,0x15,0x06,0x07,0x16,0x17,0x38,0x39,0x18,0x18,0x20,0x21,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
{0xD8,20,{0x08,0x2A,0xAA,0x82,0x00,0x08,0x2A,0xAA,0x82,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xBD,1,{0x01}},
{0xD8,20,{0x08,0x2A,0xAA,0x82,0x00,0x08,0x2A,0xAA,0x82,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xBD,1,{0x02}},
{0xD8,10,{0xFC,0x3F,0xFF,0xC3,0x00,0xFC,0x3F,0xFF,0xC3,0x00}},
{0xBD,1,{0x00}},
{0xCC,1,{0x02}},
{0xE0,58,{0x3F,0x38,0x3A,0x3F,0x40,0x42,0x44,0x41,0x82,0x8D,0x99,0x96,0x9A,0xAB,0xAF,0xB1,0xBB,0xBC,0xB5,0xBF,0xCF,0x62,0x61,0x60,0x61,0x60,0x6F,0x6F,0x60,0x2F,0x38,0x3A,0x3F,0x40,0x42,0x44,0x41,0x82,0x8D,0x99,0x96,0x9A,0xAB,0xAF,0xB1,0xBB,0xBC,0xB5,0xBF,0xCF,0x62,0x61,0x60,0x61,0x60,0x6F,0x6F,0x60}},
   {0x11, 1 , {0x00}},
   {REGFLAG_DELAY, 120, {}},

  {0x29, 1 , {0x00}},
			  	
  {REGFLAG_DELAY, 20, {0}},    */
static void lcm_register(void)
{
  unsigned int data_array[40];

  data_array[0] = 0x00043902;
data_array[1] = 0x9883ffb9;
dsi_set_cmdq(data_array, 2, 1);
data_array[0] = 0x000c3902;
data_array[1] = 0x8d0d28b1;
data_array[2] = 0x31330201;
data_array[3] = 0x0dc7d031;
dsi_set_cmdq(data_array, 4, 1);
data_array[0] = 0x000b3902;
data_array[1] = 0xae0040b2;
data_array[2] = 0x11450b1c;
data_array[3] = 0x00000000;
dsi_set_cmdq(data_array, 4, 1);
data_array[0] = 0x001a3902;
data_array[1] = 0x30ff00b4;
data_array[2] = 0x30a030a0;
data_array[3] = 0xa22f00a0;
data_array[4] = 0x303a0711;
data_array[5] = 0x30a030a0;
data_array[6] = 0xa22f00a0;
data_array[7] = 0x0000003a;
dsi_set_cmdq(data_array, 8, 1);
data_array[0] = 0x02bd1500;
dsi_set_cmdq(data_array, 1, 1);
data_array[0] = 0x00043902;
data_array[1] = 0x500400d3;
dsi_set_cmdq(data_array, 2, 1);
data_array[0] = 0x00bd1500;
dsi_set_cmdq(data_array, 1, 1);
data_array[0] = 0x00303902;
data_array[1] = 0x100020d3;
data_array[2] = 0x00081008;
data_array[3] = 0x542f1f00;
data_array[4] = 0x03000310;
data_array[5] = 0x07a11754;
data_array[6] = 0x000000a1;
data_array[7] = 0x07770000;
data_array[8] = 0x037f0b0b;
data_array[9] = 0x40187f03;
data_array[10] = 0x00290300;
data_array[11] = 0x000000e2;
data_array[12] = 0xa0302904;
dsi_set_cmdq(data_array, 13, 1);
data_array[0] = 0x00293902;
data_array[1] = 0x181818d5;
data_array[2] = 0x18202118;
data_array[3] = 0x17181818;
data_array[4] = 0x15060716;
data_array[5] = 0x13040514;
data_array[6] = 0x11020312;
data_array[7] = 0x18000110;
data_array[8] = 0x25383918;
data_array[9] = 0x18181824;
data_array[10] = 0x18181818;
data_array[11] = 0x00000018;
dsi_set_cmdq(data_array, 12, 1);
data_array[0] = 0x00293902;
data_array[1] = 0x181818d6;
data_array[2] = 0x18252418;
data_array[3] = 0x00181818;
data_array[4] = 0x02111001;
data_array[5] = 0x04131203;
data_array[6] = 0x06151405;
data_array[7] = 0x38171607;
data_array[8] = 0x20181839;
data_array[9] = 0x18181821;
data_array[10] = 0x18181818;
data_array[11] = 0x00000018;
dsi_set_cmdq(data_array, 12, 1);
data_array[0] = 0x00153902;
data_array[1] = 0xaa2a08d8;
data_array[2] = 0x2a080082;
data_array[3] = 0x000082aa;
data_array[4] = 0x00000000;
data_array[5] = 0x00000000;
data_array[6] = 0x00000000;
dsi_set_cmdq(data_array, 7, 1);
data_array[0] = 0x01bd1500;
dsi_set_cmdq(data_array, 1, 1);
data_array[0] = 0x00153902;
data_array[1] = 0xaa2a08d8;
data_array[2] = 0x2a080082;
data_array[3] = 0x000082aa;
data_array[4] = 0x00000000;
data_array[5] = 0x00000000;
data_array[6] = 0x00000000;
dsi_set_cmdq(data_array, 7, 1);
data_array[0] = 0x02bd1500;
dsi_set_cmdq(data_array, 1, 1);
data_array[0] = 0x000b3902;
data_array[1] = 0xff3ffcd8;
data_array[2] = 0x3ffc00c3;
data_array[3] = 0x0000c3ff;
dsi_set_cmdq(data_array, 4, 1);
data_array[0] = 0x00bd1500;
dsi_set_cmdq(data_array, 1, 1);
data_array[0] = 0x02cc1500;
dsi_set_cmdq(data_array, 1, 1);
data_array[0] = 0x003b3902;
data_array[1] = 0x3a383fe0;
data_array[2] = 0x4442403f;
data_array[3] = 0x998d8241;
data_array[4] = 0xafab9a96;
data_array[5] = 0xb5bcbbb1;
data_array[6] = 0x6162cfbf;
data_array[7] = 0x6f606160;
data_array[8] = 0x382f606f;
data_array[9] = 0x42403f3a;
data_array[10] = 0x8d824144;
data_array[11] = 0xab9a9699;
data_array[12] = 0xbcbbb1af;
data_array[13] = 0x62cfbfb5;
data_array[14] = 0x60616061;
data_array[15] = 0x00606f6f;
dsi_set_cmdq(data_array, 16, 1);
data_array[0] = 0x00110500;
dsi_set_cmdq(data_array, 1, 1);
MDELAY(150);
data_array[0] = 0x00290500;
dsi_set_cmdq(data_array, 1, 1);
MDELAY(50);    
};

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
  memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

static void lcm_init_power(void)
{
}

static void lcm_suspend_power(void)
{
}

static void lcm_resume_power(void)
{
}

static void lcm_get_params(LCM_PARAMS *params)
{
  memset(params, 0, sizeof(LCM_PARAMS));
  params->type   = LCM_TYPE_DSI;
  params->width  = FRAME_WIDTH;
  params->height = FRAME_HEIGHT;
  // enable tearing-free
  params->dbi.te_mode 				= LCM_DBI_TE_MODE_DISABLED;
  params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;
  //params->dsi.mode   = BURST_VDO_MODE;
  params->dsi.mode   = SYNC_EVENT_VDO_MODE; //SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE;
  // DSI
  /* Command mode setting */
  params->dsi.LANE_NUM = LCM_FOUR_LANE;
  //The following defined the fomat for data coming from LCD engine.
  params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
  params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
  params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
  params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;
  params->dsi.intermediat_buffer_num = 0;//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage
  params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
  params->dsi.word_count=1080*3;
  params->dsi.vertical_sync_active				       = 3;//2//
  //params->dsi.vertical_backporch					= 5;//8
  //params->dsi.vertical_frontporch					= 20;//6
  params->dsi.vertical_backporch					= 21;//8
  params->dsi.vertical_frontporch					= 31;//6
  params->dsi.vertical_active_line				= FRAME_HEIGHT;
  params->dsi.horizontal_sync_active				= 15;//86 20
  params->dsi.horizontal_backporch				       = 55;//55 50
  params->dsi.horizontal_frontporch				= 215;//55	50
  params->dsi.horizontal_active_pixel			= FRAME_WIDTH;
  params->dsi.PLL_CLOCK = 485;
}
/*
static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
  unsigned int i;
  for(i = 0; i < count; i++)
  {
    unsigned cmd;
    cmd = table[i].cmd;
    switch (cmd)
    {
      case REGFLAG_DELAY :
        MDELAY(table[i].count);
        break;
      case REGFLAG_END_OF_TABLE :
        break;
      default:
        dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
    }
  }
}
*/
static void lcm_init(void)
{
#ifdef BUILD_LK
  mt_set_gpio_mode(GPIO_65132_EN, GPIO_MODE_00);
  mt_set_gpio_dir(GPIO_65132_EN, GPIO_DIR_OUT);
  mt_set_gpio_out(GPIO_65132_EN, GPIO_OUT_ONE);
  MDELAY(5);
#else
  set_gpio_lcd_enp(1);
#endif
  SET_RESET_PIN(1);
  MDELAY(10);
  SET_RESET_PIN(0);
  MDELAY(10);
  SET_RESET_PIN(1);
  MDELAY(180);	///===>180ms
lcm_register();

 // push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

static void lcm_suspend(void)
{
  //push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
  int data_array[16];
  SET_RESET_PIN(1);
  MDELAY(10);
#ifdef BUILD_LK
  mt_set_gpio_mode(GPIO_65132_EN, GPIO_MODE_00);
  mt_set_gpio_dir(GPIO_65132_EN, GPIO_DIR_OUT);
  mt_set_gpio_out(GPIO_65132_EN, GPIO_OUT_ZERO);
#else
  set_gpio_lcd_enp(0);
#endif
  SET_RESET_PIN(0);
  MDELAY(10);
  SET_RESET_PIN(1);
  MDELAY(150);
  data_array[0]=0x00280500; // Display Off
  dsi_set_cmdq(data_array, 1, 1);
  MDELAY(20);
  data_array[0] = 0x00100500; // Sleep In
  dsi_set_cmdq(data_array, 1, 1);
  MDELAY(120);
}

static void lcm_resume(void)
{
#ifndef BUILD_LK
  printk(KERN_ERR "tcl %s line %d\n",__func__,__LINE__);
  lcm_init();
  //push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
#endif
}

static unsigned int lcm_compare_id(void)
{
  unsigned int id,id0;
  unsigned char buffer[5];
  int array[5];
  MDELAY(10);
  SET_RESET_PIN(1);
  MDELAY(10);
  SET_RESET_PIN(0);
  MDELAY(10);
  SET_RESET_PIN(1);
  MDELAY(150);

  array[0]=0x00043902;
  array[1]=0x9883FFB9;// page enable
  dsi_set_cmdq(array, 2, 1);
  MDELAY(10);
/*
  array[0]=0x00023902;
  array[1]=0x000013ba;
  dsi_set_cmdq(array, 2, 1);
  MDELAY(10);
*/
  array[0] = 0x00033700;// return byte number
  dsi_set_cmdq(array, 1, 1);
  MDELAY(10);

  read_reg_v2(0x04, buffer, 3);
  id0 = buffer[1];
//  read_reg_v2(0xdc, buffer, 2);
//  id1 = buffer[0];//1a 0d
  id = id0;
 
#if defined(BUILD_LK)
    printf("%s, hx8398 id = 0x%08x\n", __func__, id);
#else
    printk("%s, hx8398 id = 0x%08x\n", __func__, id);
#endif
 
  return (LCM_ID_HX8398 == id)?1:0;
}

// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER hx8398a_auo55_xinli_fhd_lcm_drv =
{
  .name           = "hx8398a_auo55_xinli_fhd",
  .set_util_funcs = lcm_set_util_funcs,
  .get_params     = lcm_get_params,
  .init           = lcm_init,
  .suspend        = lcm_suspend,
  .resume         = lcm_resume,
  .compare_id    = lcm_compare_id,
  //.esd_check	= lcm_esd_check,
  //.esd_recover	= lcm_esd_recover,
  .init_power        = lcm_init_power,
  .resume_power = lcm_resume_power,
  .suspend_power = lcm_suspend_power,
};
