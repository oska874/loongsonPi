#include "pwmconfig.h"

int main(int argc, char **argv)
{
    int MyPeriod = 1000000000; //period 设置为1s
    float rate;
    int MyDuty;

    while(1){
        scanf("%f",&rate);
        if( rate > 1){
            printf("exit\n");
            break;
        }
        MyDuty = MyPeriod * rate;
        for(int i=1;i<4;i++){
            if(pwm_export(i)<0){
            perror("PWM export err");
            printf("pwm %d\n",i);
            return -1;
            }
            if(pwm_disable(i) < 0) {
                printf("PWM disable error!\n");
                printf("pwm %d\n",i);
                return(-1);
            }
            /* set period and duty cycle time in ns */
            if(pwm_config(i, MyPeriod, MyDuty*i) < 0) {
                printf("PWM configure error!\n");
                printf("pwm %d\n",i);
                return(-1);
            }
            if(pwm_polarity(i, 0) < 0) {
                printf("PWM polarity error!\n");
                printf("pwm %d\n",i);
                return(-1);
            }
            /* enable corresponding PWM Channel */
            if(pwm_enable(i) < 0) {
                printf("PWM enable error!\n");
                printf("pwm %d\n",i);
                return(-1);
            }
        }
        printf("PWM_a successfully enabled with period - %dms, duty cycle - %2.1f%%\n", MyPeriod/1000000, rate*100);
    }
    return 0;
}
