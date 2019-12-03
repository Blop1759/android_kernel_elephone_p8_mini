# In case the platform does NOT support this type of sensors
ccflags-y += -I$(srctree)/drivers/misc/mediatek/hwmon/include
# Vanzo:yucheng on: Wed, 12 Nov 2014 21:08:36 +0800
# added for aosp management to import our variable
project_name:=$(shell echo $(VANZO_INNER_PROJECT_NAME))
ifneq ($(strip $(project_name)),)
-include $(srctree)/../zprojects/$(project_name)/$(project_name).mk
VANZO_PROJECT_HEADERS:=$(srctree)/../zprojects/$(project_name)/kernel-headers
ccflags-y += -I$(VANZO_PROJECT_HEADERS)
endif
# End of Vanzo:yucheng
ifeq ($(CONFIG_CUSTOM_KERNEL_GYROSCOPE_MODULE),y)
obj-y   +=  gyroscope_module.o gyro_factory.o
else
obj-y   +=  gyroscope.o gyro_factory.o
endif
# Vanzo:yucheng on: Wed, 12 Nov 2014 21:06:42 +0800
project_name:=$(shell echo $(VANZO_INNER_PROJECT_NAME))
ifneq ($(project_name),)
custom_kernel_gyroscope:= $(addsuffix /, $(shell echo $(CUSTOM_KERNEL_GYROSCOPE)))
ifneq ($(custom_kernel_gyroscope),)
obj-y += $(custom_kernel_gyroscope)
endif
else
# End of Vanzo:yucheng

ifeq ($(CONFIG_MTK_SCP_SENSORHUB_V1),y)
obj-$(CONFIG_MTK_GYROHUB)      +=  gyrohub/
else
obj-$(CONFIG_MTK_MPU3050C)	+=  mpu3050c/
obj-$(CONFIG_MTK_MPU6050C)	+=  mpu6050c/
obj-$(CONFIG_MTK_MPU3000)   +=  mpu3000/
obj-$(CONFIG_MTK_MPU3000_NEW)   +=  mpu3000-new/
obj-$(CONFIG_MTK_MPU6050GY_NEW)   +=  mpu6050gy-new/
obj-$(CONFIG_MTK_MPU6050GY)   +=  mpu6050gy/
obj-$(CONFIG_MTK_BMG160)   +=  bmg160/
obj-$(CONFIG_MTK_MPU6515G)   +=  mpu6515/
obj-$(CONFIG_MTK_MPU3050C_NEW)   +=  mpu3050c-new/
obj-y   +=  ITG1010-new/
obj-$(CONFIG_MTK_L3G4200D)   +=  l3g4200d/
obj-$(CONFIG_MTK_L3GD20)   +=  l3gd20/
obj-$(CONFIG_MTK_LSM6DS0)   +=  lsm6ds0/
obj-$(CONFIG_MTK_LSM6DS3)   +=  lsm6ds3gy/
obj-$(CONFIG_MTK_ICM20645GY)   +=  icm20645gy/
obj-$(CONFIG_MTK_LSM6DS3HGY)   +=  lsm6ds3hgy/
endif
# Vanzo:yucheng on: Tue, 06 Jan 2015 10:34:15 +0800
# added for new structure config alsps
endif
# End of Vanzo:yucheng
