#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0xa024e4b1, "struct_module" },
	{ 0xb9efbbcd, "seq_release" },
	{ 0x702e312e, "seq_read" },
	{ 0xd0f9a308, "seq_lseek" },
	{ 0x20000329, "simple_strtoul" },
	{ 0xd6c963c, "copy_from_user" },
	{ 0x2ff5a11a, "seq_open" },
	{ 0x8680f95d, "create_proc_entry" },
	{ 0xfb306753, "kmem_cache_alloc" },
	{ 0xab03816e, "kmem_cache_create" },
	{ 0x1b7d4074, "printk" },
	{ 0xb7e4fb5c, "remove_proc_entry" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "8191D6536FF05794021CF27");
