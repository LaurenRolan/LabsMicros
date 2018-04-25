/*
  pushled.c: Reads the value of a push-button and write to a LED.
  
  Copyright (c) 2016 Walter Fetter Lages <w.fetter@ieee.org>

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

#include <fcntl.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	int led_fd;
	int button_fd;
	char s;
	
	led_fd=open("/sys/class/gpio/gpio62/value",O_WRONLY);
	button_fd=open("/sys/class/gpio/gpio13/value",O_RDONLY);

	for(;;)
	{
		lseek(button_fd,0,SEEK_SET);
		read(button_fd,&s,sizeof s);
	
		lseek(led_fd,0,SEEK_SET);
		write(led_fd,&s,sizeof s);

		sleep(1);
	}
	
	close(led_fd);
	close(button_fd);

	return 0;
}
