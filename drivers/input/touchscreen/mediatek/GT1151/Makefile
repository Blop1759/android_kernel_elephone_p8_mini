# Linux driver folder
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/GT1151/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/GT1151/include/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include/

gt1151-objs := gt1x_extents.o gt1x_generic.o gt1x_tools.o gt1x_tpd.o gt1x_update.o
obj-$(CONFIG_TOUCHSCREEN_MTK_GT1151) += gt1151.o
