
ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m  := obmsk_gpio.o
ccflags-y:=-std=gnu99 -Wno-declaration-after-statement
else
# normal makefile

KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD

clean:
	$(MAKE)  -C $(KDIR) M=$$PWD clean
endif
