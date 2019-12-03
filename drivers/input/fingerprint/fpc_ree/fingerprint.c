#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <linux/device.h>
#include <linux/compat.h>
#include <linux/platform_device.h>



struct pinctrl *fpc_finger_pinctrl;
struct pinctrl_state *fpc_finger_int_as_int,*fpc_finger_3v3_on,*fpc_finger_3v3_off,*fpc_finger_1v8_on,*fpc_finger_1v8_off,*fpc_finger_reset_high,*fpc_finger_reset_low;

int fpc_finger_get_gpio_info(struct platform_device *pdev)
{
	struct device_node *node;
	int ret;
	node = of_find_compatible_node(NULL, NULL, "mediatek,fpc_finger");
	pr_debug("node.name %s full name %s",node->name,node->full_name);

		fpc_finger_pinctrl = devm_pinctrl_get(&pdev->dev);
		if (IS_ERR(fpc_finger_pinctrl)) {
			ret = PTR_ERR(fpc_finger_pinctrl);
			dev_err(&pdev->dev, "fpc_finger cannot find pinctrl\n");
				return ret;
		}
    pr_debug("[%s] fpc_finger_pinctrl+++++++++++++++++\n",pdev->name);

	fpc_finger_3v3_on = pinctrl_lookup_state(fpc_finger_pinctrl, "finger_power_en1");
	if (IS_ERR(fpc_finger_3v3_on)) {
		ret = PTR_ERR(fpc_finger_3v3_on);
		dev_err(&pdev->dev, " Cannot find fpc_finger pinctrl fpc_finger_power_on!\n");
		return ret;
	}
	fpc_finger_3v3_off = pinctrl_lookup_state(fpc_finger_pinctrl, "finger_power_en0");
	if (IS_ERR(fpc_finger_3v3_off)) {
		ret = PTR_ERR(fpc_finger_3v3_off);
		dev_err(&pdev->dev, " Cannot find fpc_finger pinctrl fpc_finger_power_off!\n");
		return ret;
	}
	fpc_finger_1v8_on = pinctrl_lookup_state(fpc_finger_pinctrl, "finger_power1v8_en1");
	if (IS_ERR(fpc_finger_1v8_on)) {
		ret = PTR_ERR(fpc_finger_1v8_on);
		dev_err(&pdev->dev, " Cannot find fpc_finger pinctrl fpc_finger_power_on!\n");
		return ret;
	}
	fpc_finger_1v8_off = pinctrl_lookup_state(fpc_finger_pinctrl, "finger_power1v8_en0");
	if (IS_ERR(fpc_finger_1v8_off)) {
		ret = PTR_ERR(fpc_finger_1v8_off);
		dev_err(&pdev->dev, " Cannot find fpc_finger pinctrl fpc_finger_power_off!\n");
		return ret;
	}

	fpc_finger_reset_high = pinctrl_lookup_state(fpc_finger_pinctrl, "finger_reset_en1");
	if (IS_ERR(fpc_finger_reset_high)) {
		ret = PTR_ERR(fpc_finger_reset_high);
		dev_err(&pdev->dev, " Cannot find fpc_finger pinctrl fpc_finger_reset_high!\n");
		return ret;
	}
	fpc_finger_reset_low = pinctrl_lookup_state(fpc_finger_pinctrl, "finger_reset_en0");
	if (IS_ERR(fpc_finger_reset_low)) {
		ret = PTR_ERR(fpc_finger_reset_low);
		dev_err(&pdev->dev, " Cannot find fpc_finger pinctrl fpc_finger_reset_low!\n");
		return ret;
	}
#if 0
	fpc_finger_int_as_int = pinctrl_lookup_state(fpc_finger_pinctrl, "finger_int_as_int");
	if (IS_ERR(fpc_finger_int_as_int)) {
		ret = PTR_ERR(fpc_finger_int_as_int);
		dev_err(&pdev->dev, " Cannot find fpc_finger pinctrl fpc_finger_int_as_int!\n");
		return ret;
	}
    fpc_finger_spi0_mi_as_spi0_mi = pinctrl_lookup_state(fpc_finger_pinctrl, "finger_spi0_mi_as_spi0_mi");
    if (IS_ERR(fpc_finger_spi0_mi_as_spi0_mi)) {
        ret = PTR_ERR(fpc_finger_spi0_mi_as_spi0_mi);
        dev_err(&pdev->dev, " Cannot find fpc_finger pinctrl fpc_finger_spi0_mi_as_spi0_mi!\n");
        return ret;
    }   
    fpc_finger_spi0_mi_as_gpio = pinctrl_lookup_state(fpc_finger_pinctrl, "finger_spi0_mi_as_gpio");
    if (IS_ERR(fpc_finger_spi0_mi_as_gpio)) {
        ret = PTR_ERR(fpc_finger_spi0_mi_as_gpio);
        dev_err(&pdev->dev, " Cannot find fpc_finger pinctrl fpc_finger_spi0_mi_as_gpio!\n");
        return ret;
    }   
    fpc_finger_spi0_mo_as_spi0_mo = pinctrl_lookup_state(fpc_finger_pinctrl, "finger_spi0_mo_as_spi0_mo");
    if (IS_ERR(fpc_finger_spi0_mo_as_spi0_mo)) {
        ret = PTR_ERR(fpc_finger_spi0_mo_as_spi0_mo);
        dev_err(&pdev->dev, " Cannot find fpc_finger pinctrl fpc_finger_spi0_mo_as_spi0_mo!\n");
        return ret;
    }
    fpc_finger_spi0_mo_as_gpio = pinctrl_lookup_state(fpc_finger_pinctrl, "finger_spi0_mo_as_gpio");
    if (IS_ERR(fpc_finger_spi0_mo_as_gpio)) {
        ret = PTR_ERR(fpc_finger_spi0_mo_as_gpio);
        dev_err(&pdev->dev, " Cannot find fpc_finger pinctrl fpc_finger_spi0_mo_as_gpio!\n");
        return ret;
    }
    fpc_finger_spi0_clk_as_spi0_clk = pinctrl_lookup_state(fpc_finger_pinctrl, "finger_spi0_clk_as_spi0_clk");
    if (IS_ERR(fpc_finger_spi0_clk_as_spi0_clk)) {
        ret = PTR_ERR(fpc_finger_spi0_clk_as_spi0_clk);
        dev_err(&pdev->dev, " Cannot find fpc_finger pinctrl fpc_finger_spi0_clk_as_spi0_clk!\n");
        return ret;
    }
    fpc_finger_spi0_clk_as_gpio = pinctrl_lookup_state(fpc_finger_pinctrl, "finger_spi0_clk_as_gpio");
    if (IS_ERR(fpc_finger_spi0_clk_as_gpio)) {
        ret = PTR_ERR(fpc_finger_spi0_clk_as_gpio);
        dev_err(&pdev->dev, " Cannot find fpc_finger pinctrl fpc_finger_spi0_clk_as_gpio!\n");
        return ret;
    }
    fpc_finger_spi0_cs_as_spi0_cs = pinctrl_lookup_state(fpc_finger_pinctrl, "finger_spi0_cs_as_spi0_cs");
    if (IS_ERR(fpc_finger_spi0_cs_as_spi0_cs)) {
        ret = PTR_ERR(fpc_finger_spi0_cs_as_spi0_cs);
        dev_err(&pdev->dev, " Cannot find fpc_finger pinctrl fpc_finger_spi0_cs_as_spi0_cs!\n");
        return ret;
    }
    fpc_finger_spi0_cs_as_gpio = pinctrl_lookup_state(fpc_finger_pinctrl, "finger_spi0_cs_as_gpio");
    if (IS_ERR(fpc_finger_spi0_cs_as_gpio)) {
        ret = PTR_ERR(fpc_finger_spi0_cs_as_gpio);
        dev_err(&pdev->dev, " Cannot find fpc_finger pinctrl fpc_finger_spi0_cs_as_gpio!\n");
        return ret;
    }
#endif
	pr_debug("fpc_finger get gpio info ok--------");
	return 0;
}

int fpc_finger_set_power(int cmd)
{
	switch (cmd)
		{
		case 0 : 		
			pinctrl_select_state(fpc_finger_pinctrl, fpc_finger_3v3_off);
			pinctrl_select_state(fpc_finger_pinctrl, fpc_finger_1v8_off);
		break;
		case 1 : 		
			pinctrl_select_state(fpc_finger_pinctrl, fpc_finger_3v3_on);
			pinctrl_select_state(fpc_finger_pinctrl, fpc_finger_1v8_on);
		break;
		}
	return 0;
}

int fpc_finger_set_reset(int cmd)
{
	switch (cmd)
		{
		case 0 : 		
			pinctrl_select_state(fpc_finger_pinctrl, fpc_finger_reset_low);
		break;
		case 1 : 		
			pinctrl_select_state(fpc_finger_pinctrl, fpc_finger_reset_high);
		break;
		}
	return 0;
}

int fpc_finger_set_eint(int cmd)
{
	switch (cmd)
		{
		case 0 : 		
			return -1;
		break;
		case 1 : 		
			//pinctrl_select_state(fpc_finger_pinctrl, fpc_finger_int_as_int);
		break;
		}
	return 0;
}

