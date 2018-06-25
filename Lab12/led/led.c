/*
  led.c: Linux driver for a LED on IO13
  
  Copyright (c) 2016, 2017 Walter Fetter Lages <w.fetter@ieee.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    You can also obtain a copy of the GNU General Public License
    at <http://www.gnu.org/licenses>.

*/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "led"
#define CLASS_NAME "eng10032"

static int major=0;
static int minor=0;
static struct class *ledclass=NULL;

MODULE_AUTHOR("Walter Fetter Lages <fetter@ece.ufrgs.br>");
MODULE_DESCRIPTION("LED Driver");
MODULE_SUPPORTED_DEVICE("led");
MODULE_LICENSE("GPL");

module_param(major,int,0);
MODULE_PARM_DESC(major,"major number; default is kernel allocated.");

static ssize_t led_write(struct file *file,const char *buf,
	size_t count,loff_t *ppos)
{
	unsigned char data;
	int error;

	if(count != sizeof(unsigned char)) return -EINVAL;
	if( (error=get_user(data,buf)) ) return error;	
	
	gpio_set_value(7,data);
	
	return sizeof(unsigned char);
}

static struct gpio routers[]={
	{46,GPIOF_OUT_INIT_LOW|GPIOF_EXPORT_DIR_FIXED,"IO13 is GPIO"},
	{30,GPIOF_OUT_INIT_LOW|GPIOF_EXPORT_DIR_FIXED,"IO13 is Out"},
	{7,GPIOF_OUT_INIT_HIGH|GPIOF_EXPORT_DIR_FIXED,"IO13"}
};

static int led_open(struct inode *inode,struct file *file)
{
	return gpio_request_array(routers,ARRAY_SIZE(routers));
}

static int led_release(struct inode *inode,struct file *file)
{
	gpio_free_array(routers,ARRAY_SIZE(routers));
	return 0;
}

static struct file_operations led_fops=
{
	.owner=THIS_MODULE,
	.write=led_write,
	.open=led_open,
	.release=led_release
};

int init_module(void)
{
	dev_t ledno;

	if((major=register_chrdev(major,DEVICE_NAME,&led_fops))==-1)
	{
		printk("major %d can't be registered.\n",major);
		return -EIO;
	}
	ledclass=class_create(THIS_MODULE,CLASS_NAME);
	ledno=MKDEV(major,minor);
	device_create(ledclass,NULL,ledno,NULL,DEVICE_NAME "%d",minor);
	
	return 0;
}

void cleanup_module(void)
{
	dev_t ledno;

	ledno=MKDEV(major,minor);
	device_destroy(ledclass,ledno);
	if(ledclass != NULL) class_destroy(ledclass);
	unregister_chrdev(major,DEVICE_NAME);
}
