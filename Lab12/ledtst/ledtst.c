/*
  ledtst.c: User program to use the Linux driver for a LED on IO13
  
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

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	int fd;
	
	if(argc != 3)
	{
		fprintf(stderr,"ledtst\n");
		fprintf(stderr,"\tUsage:\t%s <device> <value>\n",argv[0]);
		return -1;
	}
	if((fd=open(argv[1],O_WRONLY))==-1)
	{
		perror("ledtst");
		return -errno;
	}
	
	if(write(fd,&argv[2][0],1)==-1)
	{
		perror("ledtst");
		return -errno;
	}
	
	sleep(5);
	
	if(close(fd))
	{
		perror("ledtst");
		return -errno;
	}
	
	return 0;
}
