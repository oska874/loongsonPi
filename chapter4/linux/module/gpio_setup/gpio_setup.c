#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init lkp_init( void )
{
printk("Hello,Kernel! \n");
//mmap  通用配置寄存器 0 0x1fe10420 
off_t paddr = 0x1fe00000;
size_t psize = 0x20000;
uint8_t *vaddr = ioremap(paddr,psize);
if( vaddr == NULL){
	printk("ioremap fail 0x%lx\n",paddr);
}
else{
	uint64_t reg0 = *(uint64_t*)(vaddr+0x10420);
	printk("reg0 0x%llx\n",reg0);
	reg0 |= 0x3<<10;//set i2c0/1
	reg0 |= 0xf<<12;//set pwm0-3
	reg0 &= ~(1<<20);//set 37/38 gpio
	*(uint64_t*)(vaddr+0x10420) = reg0;
	reg0 = *(uint64_t*)(vaddr+0x10420);
	printk("2reg0 0x%llx\n",reg0);
	iounmap(vaddr);
}
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
