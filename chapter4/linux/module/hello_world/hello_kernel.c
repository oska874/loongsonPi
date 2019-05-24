#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init lkp_init( void )
{
printk("Hello,Kernel! \n");
return 0;
}
static void __exit lkp_cleanup( void )
{
printk("Goodbye, Kernel! \n");
}
module_init(lkp_init);
module_exit(lkp_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("opensource");
MODULE_DESCRIPTION("a demo of kernel module");
MODULE_VERSION("0.0.1");
