#include<galileo2io.h>
#include<stdio.h>

//Talvez tenha algo a ver, talvez não
//https://stackoverflow.com/questions/33853714/generating-a-tone-with-pwm-signal-to-a-speaker-on-a-pic32-microcontroller?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa

//O que está abaixo pode não fazer o menor sentido, mas foi de coração.
int main(int argc,char * argv[])
{
        int duty_cycle = 0;
        int perido = 1000000;
        int vary = 0;
        char str[100];
        
        snprintf(str,sizeof str,"%d\n",periodo);                
        pputs("/sys/class/pwm/pwmchip0/device/pwm_period",str); //Período de 1ms
        
        while(1)
        {                 
          snprintf(str,sizeof str,"%d\n",duty_cycle);
        
          pputs("/sys/class/pwm/pwmchip0/pwm1/duty_cycle",str);
          pputs("/sys/class/pwm/pwmchip0/pwm1/enable","1");
          duty_cycle += 10000;
          if(duty_cycle == 1000000)
            duty_cycle = 0;
        }        
}
