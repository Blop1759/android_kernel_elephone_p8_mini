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

#ifndef BUILD_LK
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/i2c.h>
#include <linux/irq.h>
/* #include <linux/jiffies.h> */
/* #include <linux/delay.h> */
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>

//#define TPS_I2C_BUSNUM  I2C_I2C_LCD_BIAS_CHANNEL    /* for I2C channel 0 */
#define I2C_ID_NAME "tps65132"
//#define TPS_ADDR 0x3E

//static struct i2c_board_info tps65132_board_info __initdata = { I2C_BOARD_INFO(I2C_ID_NAME, TPS_ADDR) };
static const struct of_device_id lcm_of_match[] = {
  {.compatible = "mediatek,I2C_LCD_BIAS"},
  {},
};

/*static struct i2c_client *tps65132_i2c_client;*/
struct i2c_client *tps65132_i2c_client;

/*****************************************************************************
 * Function Prototype
 *****************************************************************************/
static int tps65132_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int tps65132_remove(struct i2c_client *client);
/*****************************************************************************
 * Data Structure
 *****************************************************************************/
struct tps65132_dev {
  struct i2c_client *client;
};

static const struct i2c_device_id tps65132_id[] = {
  {I2C_ID_NAME, 0},
  {}
};

/* #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)) */
/* static struct i2c_client_address_data addr_data = { .forces = forces,}; */
/* #endif */
static struct i2c_driver tps65132_iic_driver = {
  .id_table = tps65132_id,
  .probe = tps65132_probe,
  .remove = tps65132_remove,
  /* .detect               = mt6605_detect, */
  .driver = {
    .owner = THIS_MODULE,
    .name = "tps65132",
    .of_match_table = lcm_of_match,
  },
};

/*****************************************************************************
 * Function
 *****************************************************************************/
static int tps65132_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
  LCM_LOGI("tps65132_iic_probe\n");
  LCM_LOGI("TPS: info==>name=%s addr=0x%x\n", client->name, client->addr);
  tps65132_i2c_client = client;
  return 0;
}

static int tps65132_remove(struct i2c_client *client)
{
  LCM_LOGI("tps65132_remove\n");
  tps65132_i2c_client = NULL;
  i2c_unregister_device(client);
  return 0;
}

int tps65132_write_bytes(unsigned char addr, unsigned char value)
{
  int ret = 0;
  struct i2c_client *client = tps65132_i2c_client;
  char write_data[2] = { 0 };
  write_data[0] = addr;
  write_data[1] = value;
  ret = i2c_master_send(client, write_data, 2);
  if (ret < 0)
    LCM_LOGI("tps65132 write data fail !!\n");
  return ret;
}

static int __init tps65132_iic_init(void)
{
  LCM_LOGI("tps65132_iic_init\n");
  //i2c_register_board_info(TPS_I2C_BUSNUM, &tps65132_board_info, 1);
  LCM_LOGI("tps65132_iic_init2\n");
  i2c_add_driver(&tps65132_iic_driver);
  LCM_LOGI("tps65132_iic_init success\n");
  return 0;
}

static void __exit tps65132_iic_exit(void)
{
  LCM_LOGI("tps65132_iic_exit\n");
  i2c_del_driver(&tps65132_iic_driver);
}


module_init(tps65132_iic_init);
module_exit(tps65132_iic_exit);

MODULE_AUTHOR("Xiaokuan Shi");
MODULE_DESCRIPTION("MTK TPS65132 I2C Driver");
#endif

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

struct LCM_setting_table {
  unsigned int cmd;
  unsigned char count;
  unsigned char para_list[64];
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
  unsigned int i;

  for (i = 0; i < count; i++) {
    unsigned cmd;
    cmd = table[i].cmd;

    switch (cmd) {

      case REGFLAG_DELAY:
        if (table[i].count <= 10)
          MDELAY(table[i].count);
        else
          MDELAY(table[i].count);
        break;

      case REGFLAG_UDELAY:
        UDELAY(table[i].count);
        break;

      case REGFLAG_END_OF_TABLE:
        break;

      default:
        dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
    }
  }
}

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

static struct LCM_setting_table lcm_suspend_setting[] = {
  {0xFF, 1, {0x00} },
  {0x28, 0, {} },
  {0x10, 0, {} },
  {REGFLAG_DELAY, 120, {} },

  {0xFF, 1, {0x00} },
  {0xFF, 1, {0x05} },
  {0xFB, 1, {0x01} },
  {0xD7, 1, {0x30} },
  {0xD8, 1, {0x70} },

  {REGFLAG_DELAY, 10, {} }
};


static void init_lcm_registers(void)
{
  NT35596_DCS_write_1A_1P(0xFF,0x00);

  //NT35532_SHARP5.5_2015.08.13

  //CMD2 Page4

  //CMD1
  NT35596_DCS_write_1A_1P(0xBA,0x03);
  NT35596_DCS_write_1A_1P(0x36,0x00);
  NT35596_DCS_write_1A_1P(0xB0,0x00);
  NT35596_DCS_write_1A_1P(0xD3,0x08);
NT35596_DCS_write_1A_1P(0xD4,0x0E);
NT35596_DCS_write_1A_1P(0xD5,0x0F);
  NT35596_DCS_write_1A_1P(0xD6,0x48);
  NT35596_DCS_write_1A_1P(0xD7,0x00);
  NT35596_DCS_write_1A_1P(0xD9,0x00);
  NT35596_DCS_write_1A_1P(0xFB,0x01);
  NT35596_DCS_write_1A_1P(0xFF,0xEE);
  NT35596_DCS_write_1A_1P(0x40,0x00);
  NT35596_DCS_write_1A_1P(0x41,0x00);
  NT35596_DCS_write_1A_1P(0x42,0x00);
  NT35596_DCS_write_1A_1P(0xFB,0x01);

  //CMD2 Page0
  NT35596_DCS_write_1A_1P(0xFF,0x01);
  NT35596_DCS_write_1A_1P(0xFB,0x01);
  NT35596_DCS_write_1A_1P(0x01,0x55);
  NT35596_DCS_write_1A_1P(0x04,0x0C);
  NT35596_DCS_write_1A_1P(0x05,0x3A);
  NT35596_DCS_write_1A_1P(0x06,0x50);
  NT35596_DCS_write_1A_1P(0x07,0xD0);
  NT35596_DCS_write_1A_1P(0x0A,0x0F);
  NT35596_DCS_write_1A_1P(0x0D,0x7F);
  NT35596_DCS_write_1A_1P(0x0E,0x7F);
  NT35596_DCS_write_1A_1P(0x0F,0x70);
  NT35596_DCS_write_1A_1P(0x10,0x63);
  NT35596_DCS_write_1A_1P(0x11,0x3C);
  NT35596_DCS_write_1A_1P(0x12,0x5C);
//  NT35596_DCS_write_1A_1P(0x13,0x5E);
//  NT35596_DCS_write_1A_1P(0x14,0x5E);
  NT35596_DCS_write_1A_1P(0x15,0x60);
  NT35596_DCS_write_1A_1P(0x16,0x11);
  NT35596_DCS_write_1A_1P(0x17,0x11);
  NT35596_DCS_write_1A_1P(0x5B,0xCA);
  NT35596_DCS_write_1A_1P(0x5C,0x00);
  NT35596_DCS_write_1A_1P(0x5D,0x00);
  NT35596_DCS_write_1A_1P(0x5F,0x1B);
//  NT35596_DCS_write_1A_1P(0x5E,0x2F);
  NT35596_DCS_write_1A_1P(0x60,0xD5);
  NT35596_DCS_write_1A_1P(0x61,0xF0);
  NT35596_DCS_write_1A_1P(0x6C,0xAB);
  NT35596_DCS_write_1A_1P(0x6D,0x44);

  //page selection cmd start
  //page selection cmd end
  //R(+) MCR cmd
  //R(-) MCR cmd
  //G(+) MCR cmd
  //page selection cmd start
  //page selection cmd end
  //G(-) MCR cmd
  //B(+) MCR cmd
  //B(-) MCR cmd


  //CMD2 Page4
  NT35596_DCS_write_1A_1P(0xFF,0x05);
  NT35596_DCS_write_1A_1P(0xFB,0x01);
  NT35596_DCS_write_1A_1P(0x00,0x3F);
  NT35596_DCS_write_1A_1P(0x01,0x3F);
  NT35596_DCS_write_1A_1P(0x02,0x3F);
  NT35596_DCS_write_1A_1P(0x03,0x3F);
  NT35596_DCS_write_1A_1P(0x04,0x38);
  NT35596_DCS_write_1A_1P(0x05,0x3F);
  NT35596_DCS_write_1A_1P(0x06,0x3F);
  NT35596_DCS_write_1A_1P(0x07,0x19);
  NT35596_DCS_write_1A_1P(0x08,0x1D);
  NT35596_DCS_write_1A_1P(0x09,0x3F);
  NT35596_DCS_write_1A_1P(0x0A,0x3F);
  NT35596_DCS_write_1A_1P(0x0B,0x1B);
  NT35596_DCS_write_1A_1P(0x0C,0x17);
  NT35596_DCS_write_1A_1P(0x0D,0x3F);
  NT35596_DCS_write_1A_1P(0x0E,0x3F);
  NT35596_DCS_write_1A_1P(0x0F,0x08);
  NT35596_DCS_write_1A_1P(0x10,0x3F);
  NT35596_DCS_write_1A_1P(0x11,0x10);
  NT35596_DCS_write_1A_1P(0x12,0x3F);
  NT35596_DCS_write_1A_1P(0x13,0x3F);
  NT35596_DCS_write_1A_1P(0x14,0x3F);
  NT35596_DCS_write_1A_1P(0x15,0x3F);
  NT35596_DCS_write_1A_1P(0x16,0x3F);
  NT35596_DCS_write_1A_1P(0x17,0x3F);
  NT35596_DCS_write_1A_1P(0x18,0x38);
  NT35596_DCS_write_1A_1P(0x19,0x18);
  NT35596_DCS_write_1A_1P(0x1A,0x1C);
  NT35596_DCS_write_1A_1P(0x1B,0x3F);
  NT35596_DCS_write_1A_1P(0x1C,0x3F);
  NT35596_DCS_write_1A_1P(0x1D,0x1A);
  NT35596_DCS_write_1A_1P(0x1E,0x16);
  NT35596_DCS_write_1A_1P(0x1F,0x3F);
  NT35596_DCS_write_1A_1P(0x20,0x3F);
  NT35596_DCS_write_1A_1P(0x21,0x3F);
  NT35596_DCS_write_1A_1P(0x22,0x3F);
  NT35596_DCS_write_1A_1P(0x23,0x06);
  NT35596_DCS_write_1A_1P(0x24,0x3F);
  NT35596_DCS_write_1A_1P(0x25,0x0E);
  NT35596_DCS_write_1A_1P(0x26,0x3F);
  NT35596_DCS_write_1A_1P(0x27,0x3F);
  NT35596_DCS_write_1A_1P(0x54,0x06);
  NT35596_DCS_write_1A_1P(0x55,0x05);
  NT35596_DCS_write_1A_1P(0x56,0x04);
  NT35596_DCS_write_1A_1P(0x58,0x03);
  NT35596_DCS_write_1A_1P(0x59,0x1B);
  NT35596_DCS_write_1A_1P(0x5A,0x1B);
  NT35596_DCS_write_1A_1P(0x5B,0x01);
  NT35596_DCS_write_1A_1P(0x5C,0x32);
  NT35596_DCS_write_1A_1P(0x5E,0x18);
  NT35596_DCS_write_1A_1P(0x5F,0x20);
  NT35596_DCS_write_1A_1P(0x60,0x2B);
  NT35596_DCS_write_1A_1P(0x61,0x2C);
  NT35596_DCS_write_1A_1P(0x62,0x18);
  NT35596_DCS_write_1A_1P(0x63,0x01);
  NT35596_DCS_write_1A_1P(0x64,0x32);
  NT35596_DCS_write_1A_1P(0x65,0x00);
  NT35596_DCS_write_1A_1P(0x66,0x44);
  NT35596_DCS_write_1A_1P(0x67,0x11);
  NT35596_DCS_write_1A_1P(0x68,0x01);
  NT35596_DCS_write_1A_1P(0x69,0x01);
  NT35596_DCS_write_1A_1P(0x6A,0x04);
  NT35596_DCS_write_1A_1P(0x6B,0x2C);
  NT35596_DCS_write_1A_1P(0x6C,0x08);
  NT35596_DCS_write_1A_1P(0x6D,0x08);
  NT35596_DCS_write_1A_1P(0x78,0x00);
  NT35596_DCS_write_1A_1P(0x79,0x00);
  NT35596_DCS_write_1A_1P(0x7E,0x00);
  NT35596_DCS_write_1A_1P(0x7F,0x00);
  NT35596_DCS_write_1A_1P(0x80,0x00);
  NT35596_DCS_write_1A_1P(0x81,0x00);
  NT35596_DCS_write_1A_1P(0x8D,0x00);
  NT35596_DCS_write_1A_1P(0x8E,0x00);
  NT35596_DCS_write_1A_1P(0x8F,0xC0);
  NT35596_DCS_write_1A_1P(0x90,0x73);
  NT35596_DCS_write_1A_1P(0x91,0x10);
  NT35596_DCS_write_1A_1P(0x92,0x07);
  NT35596_DCS_write_1A_1P(0x96,0x11);
  NT35596_DCS_write_1A_1P(0x97,0x14);
  NT35596_DCS_write_1A_1P(0x98,0x00);
  NT35596_DCS_write_1A_1P(0x99,0x00);
  NT35596_DCS_write_1A_1P(0x9A,0x00);
  NT35596_DCS_write_1A_1P(0x9B,0x61);
  NT35596_DCS_write_1A_1P(0x9C,0x15);
  NT35596_DCS_write_1A_1P(0x9D,0x30);
  NT35596_DCS_write_1A_1P(0x9F,0x0F);
  NT35596_DCS_write_1A_1P(0xA2,0xB0);
  NT35596_DCS_write_1A_1P(0xA7,0x0A);
  NT35596_DCS_write_1A_1P(0xA9,0x00);
  NT35596_DCS_write_1A_1P(0xAA,0x70);
  NT35596_DCS_write_1A_1P(0xAB,0xDA);
  NT35596_DCS_write_1A_1P(0xAC,0xFF);
  NT35596_DCS_write_1A_1P(0xAE,0xF4);
  NT35596_DCS_write_1A_1P(0xAF,0x40);
  NT35596_DCS_write_1A_1P(0xB0,0x7F);
  NT35596_DCS_write_1A_1P(0xB1,0x16);
  NT35596_DCS_write_1A_1P(0xB2,0x53);
  NT35596_DCS_write_1A_1P(0xB3,0x00);
  NT35596_DCS_write_1A_1P(0xB4,0x2A);
  NT35596_DCS_write_1A_1P(0xB5,0x3A);
  NT35596_DCS_write_1A_1P(0xB6,0xF0);
  NT35596_DCS_write_1A_1P(0xBC,0x85);
  NT35596_DCS_write_1A_1P(0xBD,0xF4);
  NT35596_DCS_write_1A_1P(0xBE,0x33);
  NT35596_DCS_write_1A_1P(0xBF,0x13);
  NT35596_DCS_write_1A_1P(0xC0,0x77);
  NT35596_DCS_write_1A_1P(0xC1,0x77);
  NT35596_DCS_write_1A_1P(0xC2,0x77);
  NT35596_DCS_write_1A_1P(0xC3,0x77);
  NT35596_DCS_write_1A_1P(0xC4,0x77);
  NT35596_DCS_write_1A_1P(0xC5,0x77);
  NT35596_DCS_write_1A_1P(0xC6,0x77);
  NT35596_DCS_write_1A_1P(0xC7,0x77);
  NT35596_DCS_write_1A_1P(0xC8,0xAA);
  NT35596_DCS_write_1A_1P(0xC9,0x2A);
  NT35596_DCS_write_1A_1P(0xCA,0x00);
  NT35596_DCS_write_1A_1P(0xCB,0xAA);
  NT35596_DCS_write_1A_1P(0xCC,0x92);
  NT35596_DCS_write_1A_1P(0xCD,0x00);
  NT35596_DCS_write_1A_1P(0xCE,0x18);
  NT35596_DCS_write_1A_1P(0xCF,0x88);
  NT35596_DCS_write_1A_1P(0xD0,0xAA);
  NT35596_DCS_write_1A_1P(0xD1,0x00);
  NT35596_DCS_write_1A_1P(0xD2,0x00);
  NT35596_DCS_write_1A_1P(0xD3,0x00);
  NT35596_DCS_write_1A_1P(0xD6,0x02);
  NT35596_DCS_write_1A_1P(0xD7,0x31);
  NT35596_DCS_write_1A_1P(0xD8,0x7E);
  NT35596_DCS_write_1A_1P(0xED,0x00);
  NT35596_DCS_write_1A_1P(0xEE,0x00);
  NT35596_DCS_write_1A_1P(0xEF,0x70);
  NT35596_DCS_write_1A_1P(0xFA,0x03);
  NT35596_DCS_write_1A_1P(0xFF,0x00);

  NT35596_DCS_write_1A_0P(0x11);
  MDELAY(150);    

  NT35596_DCS_write_1A_0P(0x29);
  MDELAY(50);
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

  params->type = LCM_TYPE_DSI;

  params->width = FRAME_WIDTH;
  params->height = FRAME_HEIGHT;

  params->dsi.mode = SYNC_EVENT_VDO_MODE;
  params->dsi.switch_mode = CMD_MODE;
  params->dsi.switch_mode_enable = 0;

  /* DSI */
  /* Command mode setting */
  params->dsi.LANE_NUM = LCM_FOUR_LANE;
  /* The following defined the fomat for data coming from LCD engine. */
  params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
  params->dsi.data_format.trans_seq = LCM_DSI_TRANS_SEQ_MSB_FIRST;
  params->dsi.data_format.padding = LCM_DSI_PADDING_ON_LSB;
  params->dsi.data_format.format = LCM_DSI_FORMAT_RGB888;

  /* Highly depends on LCD driver capability. */
  params->dsi.packet_size = 256;
  /* video mode timing */

  params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;

  params->dsi.vertical_sync_active = 2;
  params->dsi.vertical_backporch = 6;
  params->dsi.vertical_frontporch = 14;
  params->dsi.vertical_active_line = FRAME_HEIGHT;

  params->dsi.horizontal_sync_active = 8;
  params->dsi.horizontal_backporch = 16;
  params->dsi.horizontal_frontporch = 72;
  params->dsi.horizontal_active_pixel = FRAME_WIDTH;
  params->dsi.PLL_CLOCK = 430;
  params->dsi.ssc_disable = 1;
  params->dsi.cont_clock=0;
  params->dsi.clk_lp_per_line_enable = 1;
  params->dsi.esd_check_enable = 0;
  params->dsi.customization_esd_check_enable = 0;
  params->dsi.lcm_esd_check_table[0].cmd = 0x53;
  params->dsi.lcm_esd_check_table[0].count = 1;
  params->dsi.lcm_esd_check_table[0].para_list[0] = 0x24;
}

#ifdef BUILD_LK
#define TPS65132_SLAVE_ADDR_WRITE  0x7C
static struct mt_i2c_t TPS65132_i2c;

static int TPS65132_write_byte(kal_uint8 addr, kal_uint8 value)
{
  kal_uint32 ret_code = I2C_OK;
  kal_uint8 write_data[2];
  kal_uint16 len;

  write_data[0] = addr;
  write_data[1] = value;

  TPS65132_i2c.id = 0; /* I2C2; */
  /* Since i2c will left shift 1 bit, we need to set FAN5405 I2C address to >>1 */
  TPS65132_i2c.addr = (TPS65132_SLAVE_ADDR_WRITE >> 1);
  TPS65132_i2c.mode = ST_MODE;
  TPS65132_i2c.speed = 100;
  len = 2;

  ret_code = i2c_write(&TPS65132_i2c, write_data, len);
  /* printf("%s: i2c_write: ret_code: %d\n", __func__, ret_code); */

  return ret_code;
}
#endif

static void lcm_init(void)
{
  unsigned char cmd = 0x0;
  unsigned char data = 0xFF;
  int ret = 0;
  cmd = 0x00;
  data = 0x0E;
  SET_RESET_PIN(0);
#ifdef BUILD_LK
  mt_set_gpio_mode(GPIO_65132_EN, GPIO_MODE_00);
  mt_set_gpio_dir(GPIO_65132_EN, GPIO_DIR_OUT);
  mt_set_gpio_out(GPIO_65132_EN, GPIO_OUT_ONE);
  MDELAY(5);
#else
  set_gpio_lcd_enp(1);
#endif
#ifdef BUILD_LK
  ret = TPS65132_write_byte(cmd, data);
#else
  ret = tps65132_write_bytes(cmd, data);
#endif

  if (ret < 0)
    LCM_LOGI("nt35532----tps6132----cmd=%0x--i2c write error----\n", cmd);
  else
    LCM_LOGI("nt35532----tps6132----cmd=%0x--i2c write success----\n", cmd);

  cmd = 0x01;
  data = 0x0E;

#ifdef BUILD_LK
  ret = TPS65132_write_byte(cmd, data);
#else
  ret = tps65132_write_bytes(cmd, data);
#endif
  MDELAY(10);

  if (ret < 0)
    LCM_LOGI("nt35532----tps6132----cmd=%0x--i2c write error----\n", cmd);
  else
    LCM_LOGI("nt35532----tps6132----cmd=%0x--i2c write success----\n", cmd);

  SET_RESET_PIN(1);
  MDELAY(100);
  SET_RESET_PIN(0);
  MDELAY(10);

  SET_RESET_PIN(1);
  MDELAY(50);

  init_lcm_registers();
}

static void lcm_suspend(void)
{
  push_table(lcm_suspend_setting, sizeof(lcm_suspend_setting) / sizeof(struct LCM_setting_table), 1);
#ifdef BUILD_LK
  mt_set_gpio_mode(GPIO_65132_EN, GPIO_MODE_00);
  mt_set_gpio_dir(GPIO_65132_EN, GPIO_DIR_OUT);
  mt_set_gpio_out(GPIO_65132_EN, GPIO_OUT_ZERO);
#else
  set_gpio_lcd_enp(0);
#endif
  MDELAY(10);
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

LCM_DRIVER nt35532_sharp55_jsj_fhd_lcm_drv = 
{
    .name			= "nt35532_sharp55_jsj_fhd",
  .set_util_funcs = lcm_set_util_funcs,
  .get_params = lcm_get_params,
  .init = lcm_init,
  .suspend = lcm_suspend,
  .resume = lcm_resume,
  .compare_id = lcm_compare_id,
};
