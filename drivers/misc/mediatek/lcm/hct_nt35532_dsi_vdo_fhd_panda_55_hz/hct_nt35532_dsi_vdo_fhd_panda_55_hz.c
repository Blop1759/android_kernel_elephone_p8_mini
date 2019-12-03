#define LOG_TAG "LCM"

#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif

#include "lcm_drv.h"

#ifdef BUILD_LK
#include <platform/upmu_common.h>
#include <platform/mt_gpio.h>
#include <platform/mt_i2c.h>
#include <platform/mt_pmic.h>
#include <string.h>
#include <cust_gpio_usage.h>
#include <cust_i2c.h>
#endif

#ifdef BUILD_LK
#define LCM_LOGI(string, args...)  dprintf(0, "[LK/"LOG_TAG"]"string, ##args)
#define LCM_LOGD(string, args...)  dprintf(1, "[LK/"LOG_TAG"]"string, ##args)
#else
#define LCM_LOGI(fmt, args...)  pr_notice("[KERNEL/"LOG_TAG"]"fmt, ##args)
#define LCM_LOGD(fmt, args...)  pr_debug("[KERNEL/"LOG_TAG"]"fmt, ##args)
#endif

#define LCM_ID_NT35532 (0xf5)

static LCM_UTIL_FUNCS lcm_util;

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))
#define MDELAY(n)       (lcm_util.mdelay(n))
#define UDELAY(n)       (lcm_util.udelay(n))

/* --------------------------------------------------------------------------- */
/* Local Functions */
/* --------------------------------------------------------------------------- */
#define dsi_set_cmdq_V22(cmdq, cmd, count, ppara, force_update) \
  lcm_util.dsi_set_cmdq_V22(cmdq, cmd, count, ppara, force_update)
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update) \
  lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update) \
  lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd) lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums) \
  lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd) \
  lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size) \
  lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)
#define set_gpio_lcd_enp(cmd) \
  lcm_util.set_gpio_lcd_enp_bias(cmd)

/* --------------------------------------------------------------------------- */
/* Local Constants */
/* --------------------------------------------------------------------------- */
#define FRAME_WIDTH                                     (1080)
#define FRAME_HEIGHT                                    (1920)

#define GPIO_65132_EN GPIO_LCD_BIAS_ENP_PIN

#define REGFLAG_DELAY       0xFFFC
#define REGFLAG_UDELAY  0xFFFB
#define REGFLAG_END_OF_TABLE    0xFFFD
#define REGFLAG_RESET_LOW   0xFFFE
#define REGFLAG_RESET_HIGH  0xFFFF

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

static void NT35596_DCS_write_1A_1P(unsigned char cmd, unsigned char para)
{
  unsigned int data_array[16];

  data_array[0] = (0x00022902);
  data_array[1] = (0x00000000 | (para << 8) | (cmd));
  dsi_set_cmdq(data_array, 2, 1);
}

static void NT35596_DCS_write_1A_0P(unsigned char cmd)
{
  unsigned int data_array[16];

  data_array[0]=(0x00000500 | (cmd<<16));
  dsi_set_cmdq(data_array, 1, 1);

}

static void init_lcm_registers(void)
{
  SET_RESET_PIN(1);
  MDELAY(5);
  SET_RESET_PIN(0);
  MDELAY(5);
  SET_RESET_PIN(1);
  MDELAY(30);

  NT35596_DCS_write_1A_1P(0xff, 0x1);
  NT35596_DCS_write_1A_1P(0x6e, 0x80);
  NT35596_DCS_write_1A_1P(104, 19);
  NT35596_DCS_write_1A_1P(251, 1);
  NT35596_DCS_write_1A_1P(255, 2);
  NT35596_DCS_write_1A_1P(251, 1);
  NT35596_DCS_write_1A_1P(255, 5);
  NT35596_DCS_write_1A_1P(251, 1);
  NT35596_DCS_write_1A_1P(215, 49);
  NT35596_DCS_write_1A_1P(216, 112);
  NT35596_DCS_write_1A_1P(255, 0);
  NT35596_DCS_write_1A_1P(251, 1);
  NT35596_DCS_write_1A_1P(186, 3);
  NT35596_DCS_write_1A_1P(53, 0);
  NT35596_DCS_write_1A_1P(54, 0);
  NT35596_DCS_write_1A_1P(176, 0);
  NT35596_DCS_write_1A_1P(211, 10);
  NT35596_DCS_write_1A_1P(212, 15);
  NT35596_DCS_write_1A_1P(213, 15);
  NT35596_DCS_write_1A_1P(214, 72);
  NT35596_DCS_write_1A_1P(215, 0);
  NT35596_DCS_write_1A_1P(217, 0);
  NT35596_DCS_write_1A_1P(255, 238);
  NT35596_DCS_write_1A_1P(2, 0);
  NT35596_DCS_write_1A_1P(64, 0);
  NT35596_DCS_write_1A_1P(2, 0);
  NT35596_DCS_write_1A_1P(65, 0);
  NT35596_DCS_write_1A_1P(2, 0);
  NT35596_DCS_write_1A_1P(66, 0);
  NT35596_DCS_write_1A_1P(85, 144);
  NT35596_DCS_write_1A_1P(251, 1);
  NT35596_DCS_write_1A_1P(255, 1);
  NT35596_DCS_write_1A_1P(251, 1);
  NT35596_DCS_write_1A_1P(1, 85);
  NT35596_DCS_write_1A_1P(4, 12);
  NT35596_DCS_write_1A_1P(5, 58);
  NT35596_DCS_write_1A_1P(6, 80);
  NT35596_DCS_write_1A_1P(7, 208);
  NT35596_DCS_write_1A_1P(10, 15);
  NT35596_DCS_write_1A_1P(12, 6);
  NT35596_DCS_write_1A_1P(13, 107);
  NT35596_DCS_write_1A_1P(14, 107);
  NT35596_DCS_write_1A_1P(15, 112);
  NT35596_DCS_write_1A_1P(16, 99);
  NT35596_DCS_write_1A_1P(17, 60);
  NT35596_DCS_write_1A_1P(18, 92);
  NT35596_DCS_write_1A_1P(19, 77);
  NT35596_DCS_write_1A_1P(20, 77);
  NT35596_DCS_write_1A_1P(21, 96);
  NT35596_DCS_write_1A_1P(22, 21);
  NT35596_DCS_write_1A_1P(23, 21);
  NT35596_DCS_write_1A_1P(91, 202);
  NT35596_DCS_write_1A_1P(92, 0);
  NT35596_DCS_write_1A_1P(93, 0);
  NT35596_DCS_write_1A_1P(94, 38);
  NT35596_DCS_write_1A_1P(95, 27);
  NT35596_DCS_write_1A_1P(96, 213);
  NT35596_DCS_write_1A_1P(97, 247);
  NT35596_DCS_write_1A_1P(108, 171);
  NT35596_DCS_write_1A_1P(109, 68);
  NT35596_DCS_write_1A_1P(110, 128);
  NT35596_DCS_write_1A_1P(255, 5);
  NT35596_DCS_write_1A_1P(251, 1);
  NT35596_DCS_write_1A_1P(0, 63);
  NT35596_DCS_write_1A_1P(1, 63);
  NT35596_DCS_write_1A_1P(2, 63);
  NT35596_DCS_write_1A_1P(3, 63);
  NT35596_DCS_write_1A_1P(4, 56);
  NT35596_DCS_write_1A_1P(5, 63);
  NT35596_DCS_write_1A_1P(6, 63);
  NT35596_DCS_write_1A_1P(7, 25);
  NT35596_DCS_write_1A_1P(8, 27);
  NT35596_DCS_write_1A_1P(9, 63);
  NT35596_DCS_write_1A_1P(10, 29);
  NT35596_DCS_write_1A_1P(11, 23);
  NT35596_DCS_write_1A_1P(12, 63);
  NT35596_DCS_write_1A_1P(13, 2);
  NT35596_DCS_write_1A_1P(14, 8);
  NT35596_DCS_write_1A_1P(15, 12);
  NT35596_DCS_write_1A_1P(16, 63);
  NT35596_DCS_write_1A_1P(17, 16);
  NT35596_DCS_write_1A_1P(18, 63);
  NT35596_DCS_write_1A_1P(19, 63);
  NT35596_DCS_write_1A_1P(20, 63);
  NT35596_DCS_write_1A_1P(21, 63);
  NT35596_DCS_write_1A_1P(22, 63);
  NT35596_DCS_write_1A_1P(23, 63);
  NT35596_DCS_write_1A_1P(24, 56);
  NT35596_DCS_write_1A_1P(25, 24);
  NT35596_DCS_write_1A_1P(26, 26);
  NT35596_DCS_write_1A_1P(27, 63);
  NT35596_DCS_write_1A_1P(28, 63);
  NT35596_DCS_write_1A_1P(29, 28);
  NT35596_DCS_write_1A_1P(30, 22);
  NT35596_DCS_write_1A_1P(31, 63);
  NT35596_DCS_write_1A_1P(32, 63);
  NT35596_DCS_write_1A_1P(33, 2);
  NT35596_DCS_write_1A_1P(34, 6);
  NT35596_DCS_write_1A_1P(35, 10);
  NT35596_DCS_write_1A_1P(36, 63);
  NT35596_DCS_write_1A_1P(37, 14);
  NT35596_DCS_write_1A_1P(38, 63);
  NT35596_DCS_write_1A_1P(39, 63);
  NT35596_DCS_write_1A_1P(84, 8);
  NT35596_DCS_write_1A_1P(85, 7);
  NT35596_DCS_write_1A_1P(86, 26);
  NT35596_DCS_write_1A_1P(88, 25);
  NT35596_DCS_write_1A_1P(89, 54);
  NT35596_DCS_write_1A_1P(90, 27);
  NT35596_DCS_write_1A_1P(91, 1);
  NT35596_DCS_write_1A_1P(92, 50);
  NT35596_DCS_write_1A_1P(94, 39);
  NT35596_DCS_write_1A_1P(95, 40);
  NT35596_DCS_write_1A_1P(96, 43);
  NT35596_DCS_write_1A_1P(97, 44);
  NT35596_DCS_write_1A_1P(98, 24);
  NT35596_DCS_write_1A_1P(99, 1);
  NT35596_DCS_write_1A_1P(100, 50);
  NT35596_DCS_write_1A_1P(101, 0);
  NT35596_DCS_write_1A_1P(102, 68);
  NT35596_DCS_write_1A_1P(103, 17);
  NT35596_DCS_write_1A_1P(104, 1);
  NT35596_DCS_write_1A_1P(105, 1);
  NT35596_DCS_write_1A_1P(106, 6);
  NT35596_DCS_write_1A_1P(107, 34);
  NT35596_DCS_write_1A_1P(108, 8);
  NT35596_DCS_write_1A_1P(109, 8);
  NT35596_DCS_write_1A_1P(120, 0);
  NT35596_DCS_write_1A_1P(121, 0);
  NT35596_DCS_write_1A_1P(126, 0);
  NT35596_DCS_write_1A_1P(127, 0);
  NT35596_DCS_write_1A_1P(128, 0);
  NT35596_DCS_write_1A_1P(129, 0);
  NT35596_DCS_write_1A_1P(141, 0);
  NT35596_DCS_write_1A_1P(142, 0);
  NT35596_DCS_write_1A_1P(143, 192);
  NT35596_DCS_write_1A_1P(144, 115);
  NT35596_DCS_write_1A_1P(145, 16);
  NT35596_DCS_write_1A_1P(146, 9);
  NT35596_DCS_write_1A_1P(150, 17);
  NT35596_DCS_write_1A_1P(151, 20);
  NT35596_DCS_write_1A_1P(152, 0);
  NT35596_DCS_write_1A_1P(153, 0);
  NT35596_DCS_write_1A_1P(154, 0);
  NT35596_DCS_write_1A_1P(155, 97);
  NT35596_DCS_write_1A_1P(156, 21);
  NT35596_DCS_write_1A_1P(157, 48);
  NT35596_DCS_write_1A_1P(159, 15);
  NT35596_DCS_write_1A_1P(162, 176);
  NT35596_DCS_write_1A_1P(167, 10);
  NT35596_DCS_write_1A_1P(169, 0);
  NT35596_DCS_write_1A_1P(170, 112);
  NT35596_DCS_write_1A_1P(171, 218);
  NT35596_DCS_write_1A_1P(172, 255);
  NT35596_DCS_write_1A_1P(174, 244);
  NT35596_DCS_write_1A_1P(175, 64);
  NT35596_DCS_write_1A_1P(176, 127);
  NT35596_DCS_write_1A_1P(177, 22);
  NT35596_DCS_write_1A_1P(178, 83);
  NT35596_DCS_write_1A_1P(179, 0);
  NT35596_DCS_write_1A_1P(180, 42);
  NT35596_DCS_write_1A_1P(181, 58);
  NT35596_DCS_write_1A_1P(182, 240);
  NT35596_DCS_write_1A_1P(188, 133);
  NT35596_DCS_write_1A_1P(189, 248);
  NT35596_DCS_write_1A_1P(190, 59);
  NT35596_DCS_write_1A_1P(191, 19);
  NT35596_DCS_write_1A_1P(192, 119);
  NT35596_DCS_write_1A_1P(193, 119);
  NT35596_DCS_write_1A_1P(194, 119);
  NT35596_DCS_write_1A_1P(195, 119);
  NT35596_DCS_write_1A_1P(196, 119);
  NT35596_DCS_write_1A_1P(197, 119);
  NT35596_DCS_write_1A_1P(198, 119);
  NT35596_DCS_write_1A_1P(199, 119);
  NT35596_DCS_write_1A_1P(200, 170);
  NT35596_DCS_write_1A_1P(201, 42);
  NT35596_DCS_write_1A_1P(202, 0);
  NT35596_DCS_write_1A_1P(203, 170);
  NT35596_DCS_write_1A_1P(204, 146);
  NT35596_DCS_write_1A_1P(205, 0);
  NT35596_DCS_write_1A_1P(206, 24);
  NT35596_DCS_write_1A_1P(207, 136);
  NT35596_DCS_write_1A_1P(208, 170);
  NT35596_DCS_write_1A_1P(209, 0);
  NT35596_DCS_write_1A_1P(210, 0);
  NT35596_DCS_write_1A_1P(211, 0);
  NT35596_DCS_write_1A_1P(214, 2);
  NT35596_DCS_write_1A_1P(237, 0);
  NT35596_DCS_write_1A_1P(238, 0);
  NT35596_DCS_write_1A_1P(239, 112);

  NT35596_DCS_write_1A_1P(0xFA, 0x3);
  NT35596_DCS_write_1A_1P(0xFF,0);

  NT35596_DCS_write_1A_0P(0x11);
  MDELAY(120);
  NT35596_DCS_write_1A_0P(0x29);
  MDELAY(20);
}

/* --------------------------------------------------------------------------- */
/* LCM Driver Implementations */
/* --------------------------------------------------------------------------- */

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
  memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

static void lcm_get_params(LCM_PARAMS *params)
{
	memset(params, 0, sizeof(LCM_PARAMS));

	params->dsi.mode = BURST_VDO_MODE;
	params->dsi.LANE_NUM = LCM_FOUR_LANE;
	params->dsi.packet_size = 256;
	params->dsi.word_count = FRAME_WIDTH*3;
	params->dsi.vertical_backporch = 8;
	params->dsi.vertical_frontporch = 13;
	params->dsi.PLL_CLOCK = 470;
	params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9Cu;
	params->physical_width = 68;
	params->physical_height = 122;
	params->type = LCM_TYPE_DSI;
	params->dsi.data_format.format = LCM_DPI_FORMAT_RGB888;
	params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;
	params->dsi.vertical_sync_active = 2;
	params->width = FRAME_WIDTH;
	params->dsi.horizontal_active_pixel = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;
	params->dsi.vertical_active_line = FRAME_HEIGHT;
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding = LCM_DSI_PADDING_ON_LSB;
	params->dsi.intermediat_buffer_num = 0;
	params->dsi.cont_clock = 0;
	params->dsi.horizontal_sync_active = 10;
	params->dsi.lcm_esd_check_table[0].cmd = 10;
	params->dsi.horizontal_backporch = 118;
	params->dsi.horizontal_frontporch = 118;
	params->dsi.ssc_disable = 1;
	params->dsi.clk_lp_per_line_enable = 1;
	params->dsi.esd_check_enable = 1;
	params->dsi.customization_esd_check_enable = 1;
	params->dsi.lcm_esd_check_table[0].count = 1;
}

static void lcm_init(void)
{
  init_lcm_registers();
}

static void lcm_suspend(void)
{

  NT35596_DCS_write_1A_0P(0x28);
  MDELAY(10);
  NT35596_DCS_write_1A_0P(0x10);
  MDELAY(120);

  NT35596_DCS_write_1A_1P(0xFF,0x5);
  NT35596_DCS_write_1A_1P(0xFB,0x1);
  NT35596_DCS_write_1A_1P(0xD7,0x30);
  NT35596_DCS_write_1A_1P(0xD8,0x70);
  NT35596_DCS_write_1A_1P(0xFF,0x0);
//  NT35596_DCS_write_1A_1P(0x4F,0x1);
}

static void lcm_resume(void)
{
  lcm_init();
}

static unsigned int lcm_compare_id(void)
{
  unsigned int id = 0;
  unsigned char buffer[2];
  unsigned int array[16];

  SET_RESET_PIN(1);
  SET_RESET_PIN(0);
  MDELAY(1);

  SET_RESET_PIN(1);
  MDELAY(20);

  array[0] = 0x00023700;  /* read id return two byte,version and id */
  dsi_set_cmdq(array, 1, 1);

  read_reg_v2(0xF4, buffer, 2);
  id = buffer[0];     /* we only need ID */

  LCM_LOGI("%s,nt35532 debug: nt35532 id = 0x%08x\n", __func__, id);

  if (id == LCM_ID_NT35532)
    return 1;
  else
    return 0;

}

LCM_DRIVER hct_nt35532_dsi_vdo_fhd_panda_55_hz = {
  .name = "hct_nt35532_dsi_vdo_fhd_panda_55_hz",
  .set_util_funcs = lcm_set_util_funcs,
  .get_params = lcm_get_params,
  .init = lcm_init,
  .suspend = lcm_suspend,
  .resume = lcm_resume,
  .compare_id = lcm_compare_id,
};
