#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init lkp_init( void )
{
printk("Hello,Kernel! \n");
//mmap  通用配置寄存器 0 0x1fe10420 
//通用配置寄存器 1  0x1fe10428 
// 通用配置寄存器 2  0x1fe10430 
//update io pin mux
return 0;
}
static void __exit lkp_cleanup( void )
{
printk("Goodbye, Kernel! \n");
//unmap
//exit
}
module_init(lkp_init);
module_exit(lkp_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("opensource");
MODULE_DESCRIPTION("a demo of kernel module");
MODULE_VERSION("0.0.1");
