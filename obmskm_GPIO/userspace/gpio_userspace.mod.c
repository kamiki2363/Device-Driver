#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xcc75a739, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x43e84308, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x22183e2a, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x3f728273, __VMLINUX_SYMBOL_STR(cdev_alloc) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x67c2fa54, __VMLINUX_SYMBOL_STR(__copy_to_user) },
	{ 0x599dfc14, __VMLINUX_SYMBOL_STR(obmskm_gpio_is_valid) },
	{ 0xb78fed75, __VMLINUX_SYMBOL_STR(obmskm_gpio_get_value) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0xf320265b, __VMLINUX_SYMBOL_STR(obmskm_gpio_request) },
	{ 0x6df29f78, __VMLINUX_SYMBOL_STR(obmskm_gpio_direction_input) },
	{ 0x69f9722c, __VMLINUX_SYMBOL_STR(obmskm_gpio_direction_output) },
	{ 0x21db4a19, __VMLINUX_SYMBOL_STR(obmskm_gpio_set_value) },
	{ 0xfbc74f64, __VMLINUX_SYMBOL_STR(__copy_from_user) },
	{ 0xabd32746, __VMLINUX_SYMBOL_STR(obmskm_gpio_free) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=obmskm_gpio";


MODULE_INFO(srcversion, "6A8F9D2FBF8B709C4F564B8");
