#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <galileo2io.h>
#include <12cutil.h>
#include <jhdlcd.h>

static volatile int run=1;

void quit(int signal){ run=0; }

static void i2c_error(const char *msg)
{
	perror(msg);
	exit(-errno);
}

int main(int argc, char *argv[])
{
	int duty_cycle;
	double angle;
	double angleref = 0;
	char str[100], str2[100];
	int fd1, fd;
	char raw_str[80];
	int raw;
	int i;
	int button_fd;
	char s;
	int n;
	struct sigaction act;

	/* Pull-up on IO18 */
	pputs("/sys/class/gpio/export", "57");
	pputs("/sys/class/gpio/gpio57/direction", "out");
	pputs("/sys/class/gpio/gpio57/value", "1");

	/* Select I2C on IO18 and IO19 */
	pputs("/sys/class/gpio/export", "60");
	pputs("/sys/class/gpio/gpio60/direction", "out");
	pputs("/sys/class/gpio/gpio60/value", "0");

	/* Pull-up on IO19 */
	pputs("/sys/class/gpio/export", "59");
	pputs("/sys/class/gpio/gpio59/direction", "out");
	pputs("/sys/class/gpio/gpio59/value", "1");
	
	if((fd = open("/dev/i2c-0", O_WRONLY)) < 0)
		i2c_error("Opening /dev/i2c-0");
	
	usleep(30000); //Espera por 30 ms após ligar

	//------ LAB10 - writelcd e clklcd ----------------	

	/* LCD initialization */
        if(ioctl(fd,I2C_SLAVE,LCD_ADDR) < 0) i2c_error("ioctl on /dev/i2c-0");

        i2c_write_reg(fd,LCD_C0,LCD_FUNCTIONSET | LCD_2LINE);
        usleep(40);  /* Wait for more than 39 us */

        i2c_write_reg(fd,LCD_C0,LCD_DISPLAYSWITCH | LCD_DISPLAYON | 
                LCD_CURSOROFF | LCD_BLINKOFF);
        usleep(40);	/* Wait for more then 39 us */

        i2c_write_reg(fd,LCD_C0,LCD_SCREENCLEAR);
        usleep(1600);	/* Wait for more then 1.53 ms */
        
        i2c_write_reg(fd,LCD_C0,LCD_INPUTSET | LCD_ENTRYLEFT | LCD_DECREMENT);

        /* Backlight initialization */
        if(ioctl(fd,I2C_SLAVE,BL_ADDR) < 0) i2c_error("ioctl on /dev/i2c-0");
        i2c_write_reg(fd,BL_MODE1,0);
        i2c_write_reg(fd,BL_LEDOUT,BL_RED_GRPPWM | BL_GREEN_GRPPWM | 
                BL_BLUE_GRPPWM);
        i2c_write_reg(fd,BL_MODE2,BL_DMBLNK);
        
        i2c_write_reg(fd,BL_RED,255);
        i2c_write_reg(fd,BL_GREEN,0);
	i2c_write_reg(fd,BL_BLUE,0);

	act.sa_handler=quit;
        sigaction(SIGINT,&act,NULL);
	sigaction(SIGTERM,&act,NULL);

	/* IO3 = pwm1 */
	pputs("/sys/class/pwm/pwmchip0/export", "1");
	pputs("/sys/class/pwm/pwmchip0/device/pwm_period", "20000000"); //20.000.000 ns = 20 ms
	
	/* gpio16 = 0 = out */
	pputs("/sys/class/gpio/export", "16");
	pputs("/sys/class/gpio/gpio16/direction", "out");
	pputs("/sys/class/gpio/gpio16/value", "0");

	/* gpio 34 = ?? */
	pputs("/sys/class/gpio/export", "34");
	pputs("/sys/class/gpio/gpio34/direction", "out");
	pputs("/sys/class/gpio/gpio34/value", "1");

	/* gpio 35 = ?? */
	pputs("/sys/class/gpio/export", "35");
	pputs("/sys/class/gpio/gpio35/direction", "out");
	pputs("/sys/class/gpio/gpio35/value", "0");	

	/* gpio 35 = ?? */
	pputs("/sys/class/gpio/export", "70");
	pputs("/sys/class/gpio/gpio70/direction", "out");
	pputs("/sys/class/gpio/gpio70/value", "0");

	button_fd=open("/sys/class/gpio/gpio13/value", O_RDONLY);

	/* gpio49 = in = neither pull-up nor pull down */
	pputs("/sys/class/gpio/export", "49");
	pputs("/sys/class/gpio/gpio49/direction", "in");

	if((fd1=open("/sys/bus/iio/devices/iio:device0/in_voltage0_raw", O_RDONLY)) < 0)
	{
		perror("Opening in_voltage0raw:");
		return -1;
	}

	//Main loop - LAB06 - PWM
	for(;;)
	{
		lseek(button_fd, 0, SEEK_SET);
		read(button_fd, &s, sizeof s);
		
		lseek(fd1, 0, SEEK_SET);
		read(fd1, &raw_str, sizeof raw_str);
		raw = atoi(raw_str);

		angle = raw / 14.444;
		angle -= 90;
		angle *= M_PI / 180;

		if(angle > M_PI_2) angle = M_PI_2;
		if(angle < -M_PI_2) angle = -M_PI_2;
		
		duty_cycle = angle / M_PI_2 * 850000 + 1500000;

		pputs("/sys/class/pwm/pwmchip0/device/pwm_period","20000000");
        	
        	snprintf(str,sizeof str,"%d\n",duty_cycle);
        	pputs("/sys/class/pwm/pwmchip0/pwm1/duty_cycle",str);
		pputs("/sys/class/pwm/pwmchip0/pwm1/enable","1");

		if(ioctl(fd, I2C_SLAVE, LCD_ADDR) < 0) 
			i2c_error("ioctl on /dev/12c-0");
		
		i2c_write_reg(fd, LCD_C0, LCD_DDRAMADDRSET | 0);
		sprintf(str, "%f %f", angle * 180 / M_PI, angle);
		sprintf(str2, "%f %f", angleref * 180 / M_PI, angleref);
		n = strlen(str);
		for(i=0; i < n; i++)
			i2c_write_reg(fd, LCD_RS, str[i]);

		if(ioctl(fd, I2C_SLAVE, LCD_ADDR) < 0) 
			i2c_error("ioctl on /dev/12c-0");

		double diff = (angle - angleref) * 180 / M_PI;

		if(diff > 18)
		{
			i2c_write_reg(fd, BL_RED, 255);
			i2c_write_reg(fd, BL_BLUE, 0);
			i2c_write_reg(fd, BL_GREEN, 0);
		}
		else if(diff < -18)
		{
			i2c_write_reg(fd, BL_RED, 255);
			i2c_write_reg(fd, BL_BLUE, 255);
			i2c_write_reg(fd, BL_GREEN, 0);
		}
		else
		{
			i2c_write_reg(fd, BL_RED, 0);
			i2c_write_reg(fd, BL_BLUE, 255);
			i2c_write_reg(fd, BL_GREEN, 0);
		}

		if(s==49) angleref = angle;
		usleep(99000);
	}
	close(fd1);
	close(fd);
	close(button_fd);

	pputs("/sys/class/gpio/unexport", "13");
	pputs("/sys/class/gpio/unexport", "34");
	pputs("/sys/class/gpio/unexport", "35");
	pputs("/sys/class/gpio/unexport", "77");
	
	return 0;
}	