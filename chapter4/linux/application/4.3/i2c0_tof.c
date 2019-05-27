#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include "vl53l0x.h"

#define I2C_DEV "/dev/i2c-0"//i2c_dev为i2c　adapter创建的别名
static int iic_read(int fd, char buff[], unsigned char addr, int count)
{
    int res;
    char sendbuffer1[2];
    sendbuffer1[0]=0x53;
    sendbuffer1[1]=addr;
    write(fd,sendbuffer1,2);
    res=read(fd,buff,count);
    printf("read %d byte at 0x%x\n", res, addr);
    return res;
}

static int iic_write(int fd, char buff[], unsigned char addr, int count)
{
    int res;
    int i,n;
    static char sendbuffer[100];
    memcpy(sendbuffer+2, buff, count);
    sendbuffer[0]=0x52;
    sendbuffer[1]=addr;
    res=write(fd,sendbuffer,count+2);
    printf("write %d byte at 0x%x\n", res, addr);
}

int main(void)
{
    int fd;
    int res;
    char ch;
    char buf[50];
    int regaddr,i;
    int slaveaddr = VL53L0X_ADDR;
    fd = open(I2C_DEV, O_RDWR);// I2C_DEV /dev/i2c-0
    if(fd < 0) {
        printf("####i2c test device open failed####\n");
        return (-1);
    }
    res = ioctl(fd,I2C_TENBIT,0);   //not 10bit
    if(res <0){
	    perror("dis ten ");
    }
    res = ioctl(fd,I2C_SLAVE,slaveaddr);    //设置I2C从设备地址[6:0]
    if(res <0){
	    perror("set slave  ");
    }
    printf("%x\n",slaveaddr);
    while((ch=getchar())!='0') {
        switch(ch) {
        case '1':
            printf("getch test success\n");
            break;
        case '2':
	    //read version
            regaddr = VL53L0X_REG_IDENTIFICATION_REVISION_ID;
            printf("read tof id \n");
            res=iic_read(fd,buf,regaddr,1);
            printf("revision %d bytes read: ",res);
            for(i=0; i<res; i++) {
                printf("%x ",buf[i]);
            }
            printf("\n");
            break;
        case '3':
	    //modle id
	    regaddr = VL53L0X_REG_IDENTIFICATION_MODEL_ID;
            res=iic_read(fd,buf,regaddr,1);
            printf("model %d bytes read: ",res);
            for(i=0; i<res; i++) {
                printf("%x ",buf[i]);
            }
            printf("\n");
            break;
	case '9':
	    printf("quit\n");
	    close(fd);
	    exit(0);
	    break;
        default:
            printf("bad command\n");
            break;
        }
    }
    close(fd);
    printf("test done\n");
    return 0;
}
