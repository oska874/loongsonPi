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
#include <stdint.h>

#define I2C_DEV "/dev/i2c-0"//i2c_dev为i2c　adapter创建的别名
static int iic_read(int fd, char buff[], unsigned char addr, int count)
{
    int res;
    char sendbuffer1[2];
    sendbuffer1[1]=0x53;
    sendbuffer1[0]=addr;
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
    sendbuffer[1]=0x52;
    sendbuffer[0]=addr;
    res=write(fd,sendbuffer,count+2);
    printf("write %d byte at 0x%x\n", res, addr);
}


uint16_t makeuint16(int lsb, int msb) {
    return ((msb & 0xFF) << 8) | (lsb & 0xFF);
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
    if(res <0) {
        perror("dis ten ");
    }
    res = ioctl(fd,I2C_SLAVE,slaveaddr);    //设置I2C从设备地址[6:0]
    if(res <0) {
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

            regaddr = VL53L0X_REG_IDENTIFICATION_MODEL_ID;
            res=iic_read(fd,buf,regaddr,1);
            printf("model %d bytes read: ",res);
            for(i=0; i<res; i++) {
                printf("%x ",buf[i]);
            }
            printf("\n");
            regaddr = VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD;
            res=iic_read(fd,buf,regaddr,1);
            printf("PRE_RANGE_CONFIG_VCSEL_PERIOD %d bytes read: ",res);
            for(i=0; i<res; i++) {
                printf("%x ",buf[i]);
            }
            printf("\n");

            regaddr = VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD;
            res=iic_read(fd,buf,regaddr,1);
            printf("VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD %d bytes read: ",res);
            for(i=0; i<res; i++) {
                printf("%x ",buf[i]);
            }
            printf("\n");
            break;
        case '3':
            printf("config and start\n");
            regaddr = VL53L0X_REG_SYSRANGE_START;
            buf[0] = 0x01;
            res = iic_write(fd, buf, regaddr, 1);
            if(res <0) {
                perror("iic write fail ");
                break;
            }
            printf("wait config ok\n");
            int32_t cnt = 100;
            while(cnt-->0) {
                usleep(10000);
                res = iic_read(fd, buf, VL53L0X_REG_RESULT_RANGE_STATUS,1);
                if(res == 1) {
                    if(buf[0]&0x1) {
                        printf("ready!\n");
                        break;
                    }
                } else {
                    perror("read err");
                    break;
                }
            }
            break;
        case '4':
            printf("read disance\n");
            regaddr = VL53L0X_REG_RESULT_RANGE_STATUS;
            res = iic_read(fd,buf,regaddr,12);
            printf("read %d byte:", res);
            for(i =0;i<12;i++){
                printf("%d ",buf[i]);
            }
            printf("\n");
            uint16_t acnt = makeuint16(buf[7], buf[6]);
              uint16_t scnt = makeuint16(buf[9], buf[8]);
              uint16_t dist = makeuint16(buf[11], buf[10]);
              uint8_t drsi = (buf[0]&0x78)>>3;
              printf("ambient count: %d\n",acnt);
              printf("signal count: %d\n",scnt);
              printf("distance %d\n",dist);
              printf("status: %d\n",drsi);
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
