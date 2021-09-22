#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x2ae8735, "module_layout" },
	{ 0x16bd2233, "cdev_del" },
	{ 0x5f575f8d, "device_destroy" },
	{ 0x7ce62d80, "cdev_add" },
	{ 0x5175e3c0, "cdev_init" },
	{ 0x655f555e, "class_destroy" },
	{ 0x8e1984a8, "device_create" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x21a54c1d, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0xbb72d4fe, "__put_user_1" },
	{ 0xc5850110, "printk" },
	{ 0x79aa04a2, "get_random_bytes" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xf7802486, "__aeabi_uidivmod" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "7117951A1624137181E57A0");
