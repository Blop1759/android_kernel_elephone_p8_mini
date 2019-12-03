# In case the platform does NOT support this type of sensors
ccflags-y += -I$(srctree)/drivers/misc/mediatek/hwmon/include
# Vanzo:yucheng on: Sat, 15 Oct 2016 12:49:53 +0800
# added for aosp management to import our variable
project_name:=$(shell echo $(VANZO_INNER_PROJECT_NAME))
ifneq ($(strip $(project_name)),)
-include $(srctree)/../zprojects/$(project_name)/$(project_name).mk
VANZO_PROJECT_HEADERS:=$(srctree)/../zprojects/$(project_name)/kernel-headers
ccflags-y += -I$(VANZO_PROJECT_HEADERS)
endif
# End of Vanzo: yucheng
ifeq ($(CONFIG_CUSTOM_KERNEL_ALSPS_MODULE),y)
obj-y   +=  alsps_module.o
else
obj-y   +=  alsps.o
endif
obj-y   +=  alsps_factory.o
obj-y   +=  aal_control.o
# Vanzo:yucheng on: Sat, 15 Oct 2016 12:50:45 +0800
project_name:=$(shell echo $(VANZO_INNER_PROJECT_NAME))
 
ifneq ($(project_name),)
custom_kernel_alsps:= $(addsuffix /, $(shell echo $(CUSTOM_KERNEL_ALSPS)))
$(info "custom_kernel_alsps:$(custom_kernel_alsps)")
ifneq ($(custom_kernel_alsps),)
obj-y += $(custom_kernel_alsps)
endif
else
# End of Vanzo: yucheng

ifeq ($(CONFIG_MTK_SCP_SENSORHUB_V1),y)
obj-$(CONFIG_MTK_ALSPSHUB)      +=  alspshub/
else
obj-$(CONFIG_MTK_GP2AP002S00F)	+=  gp2ap002s00f/
obj-$(CONFIG_MTK_CM36283)	+=  cm36283/
obj-$(CONFIG_MTK_CM36558)	+=  cm36558/
obj-$(CONFIG_MTK_CM36283_NEW)	+=  cm36283-new/
obj-$(CONFIG_MTK_APM_16D)	+=  apm_16d/
obj-$(CONFIG_MTK_EPL2182)	+=  epl2182/
obj-$(CONFIG_MTK_EPL2182_NEW)	+=  EPL2182-new/
obj-$(CONFIG_MTK_LTR559_NEW)	+=  ltr559_new/
obj-$(CONFIG_MTK_STK3X1X)	+=  stk3x1x/
obj-$(CONFIG_MTK_CM36652_NEW)	+=  cm36652-new/
obj-$(CONFIG_MTK_CM36686_NEW)	+=  cm36686-new/
obj-$(CONFIG_MTK_APDS9930)	+=  APDS9930/
obj-$(CONFIG_MTK_CM3232_NEW)	+=  cm3232-new/
obj-$(CONFIG_MTK_APDS9930_NEW)	+=  APDS9930-new/
obj-$(CONFIG_MTK_STK3X1X_NEW)	+=  stk3x1x-new/
obj-$(CONFIG_MTK_RPR410)	+=  rpr410/
obj-$(CONFIG_MTK_TMD2771)	+=  tmd2771/
obj-$(CONFIG_MTK_TMD2771_AUTO)	+=  tmd2771_auto/
obj-$(CONFIG_MTK_STK3X1X_NEW)	+=  stk3x1x-new/
endif
# Vanzo:yucheng on: Sat, 15 Oct 2016 12:51:46 +0800
# added for new structure config alsps
endif
# End of Vanzo: yucheng

