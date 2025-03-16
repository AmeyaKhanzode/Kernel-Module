#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

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
	{ 0x122c3a7e, "_printk" },
	{ 0xd3eb51ff, "proc_create" },
	{ 0xf84d1d0d, "single_open" },
	{ 0xb3daf1a5, "remove_proc_entry" },
	{ 0xf88dc246, "init_task" },
	{ 0xdcb764ad, "memset" },
	{ 0xa65c6def, "alt_cb_patch_nops" },
	{ 0x385c7481, "__tracepoint_mmap_lock_start_locking" },
	{ 0x668b19a1, "down_read" },
	{ 0xcf2ea86b, "__tracepoint_mmap_lock_acquire_returned" },
	{ 0x9ce185b9, "seq_printf" },
	{ 0x524879e0, "mas_find" },
	{ 0x5f9c8d15, "seq_puts" },
	{ 0xd469bccc, "__tracepoint_mmap_lock_released" },
	{ 0x53b954a2, "up_read" },
	{ 0xa9ecc4ee, "mmput" },
	{ 0x159e7d29, "__mmap_lock_do_trace_released" },
	{ 0x44601e88, "__mmap_lock_do_trace_acquire_returned" },
	{ 0xaa3f0ae4, "__mmap_lock_do_trace_start_locking" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x5f8ee070, "seq_read" },
	{ 0x69fc8b8e, "seq_lseek" },
	{ 0x39a4ebc8, "single_release" },
	{ 0x773354b7, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "DCA1E7774E90918C5F54CF0");
