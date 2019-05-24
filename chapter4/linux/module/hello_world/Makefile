obj-m += hello_kernel.o        	# 产生hello kernel 模块的目标文件
CURRENT_PATH := $(shell pwd)   #模块所在的当前路径
LINUX_KERNEL_PATH  := /home/loongson/loongson/linux-3.10 #对应的Linux内核源代码的绝对路径，
all:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules   #编译
clean:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean    #清理

