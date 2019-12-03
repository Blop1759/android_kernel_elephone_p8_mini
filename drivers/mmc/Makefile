#
# Makefile for the kernel mmc device drivers.
#

# Vanzo:zhangweipeng on: Mon, 24 Apr 2017 10:50:10 +0800
# modify build error
#subdir-ccflags-y += -Werror
# End of Vanzo:zhangweipeng
subdir-ccflags-$(CONFIG_MMC_DEBUG) := -DDEBUG

obj-$(CONFIG_MMC)		+= core/
obj-$(CONFIG_MMC)		+= card/
obj-$(subst m,y,$(CONFIG_MMC))	+= host/
