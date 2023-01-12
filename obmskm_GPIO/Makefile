
ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m  := obmskm_gpio.o
ccflags-y:=-std=gnu99
else
# normal makefile

KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD

clean:
	$(MAKE)  -C $(KDIR) M=$$PWD clean
endif
