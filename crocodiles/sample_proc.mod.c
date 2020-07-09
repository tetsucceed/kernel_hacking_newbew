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
__used
__attribute__((section("__versions"))) = {
	{ 0xd76291be, "struct_module" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xc9c55f8e, "per_cpu__current_task" },
	{ 0x268c6758, "proc_symlink" },
	{ 0x4c29b36d, "create_proc_entry" },
	{ 0x7fd06529, "proc_mkdir" },
	{ 0x7d11c268, "jiffies" },
	{ 0x3432d76f, "remove_proc_entry" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "8040A4564721CEE96906F84");
