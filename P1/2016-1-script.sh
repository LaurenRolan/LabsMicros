#! /bin/sh
### BEGIN INIT INFO
# Provides:          eng10032lab05_1
# Required-Start:    
# Should-Start:      
# Required-Stop:     
# Default-Start:     S
# Default-Stop:
# Short-Description: Configures GPIO and interrupts for Lab 05.
# Description:       Configures GPIO and interrupts for Lab 05.
### END INIT INFO

case "$1" in
    start|restart|force-reload)
   	# IO5 = gpio0
	if [ ! -d /sys/class/gpio/gpio0 ] ; then
	    echo -n "0" > /sys/class/gpio/export
	fi
	echo -n "in" > /sys/class/gpio/gpio0/direction
	chgrp gpio /sys/class/gpio/gpio0/value
	chmod g+r /sys/class/gpio/gpio0/value
	chgrp gpio /sys/class/gpio/gpio0/edge
	chmod g+r /sys/class/gpio/gpio0/edge

	# Level Shifter GPIO = gpio18 = high (input)
	if [ ! -d /sys/class/gpio/gpio18 ] ; then
	    echo -n "18" > /sys/class/gpio/export
	fi
	echo -n "high" > /sys/class/gpio/gpio18/direction

	# 22k Pull-Up GPIO = gpio19 = I => alta impedância
	if [ ! -d /sys/class/gpio/gpio19 ] ; then
	    echo -n "19" > /sys/class/gpio/export
	fi
	echo -n "I" > /sys/class/gpio/gpio19/direction
  
  # Pin Mux 1 GPIO = gpio66 = 'low' => saída para gpio
	if [ ! -d /sys/class/gpio/gpio66 ] ; then
	    echo -n "66" > /sys/class/gpio/export
	fi
	echo -n "low" > /sys/class/gpio/gpio66/direction

	;;
    stop)
    	echo -n "out" > /sys/class/gpio/gpio0/direction
    	echo -n "0" > /sys/class/gpio/unexport
	echo -n "18" > /sys/class/gpio/unexport
	echo -n "19" > /sys/class/gpio/unexport      
	echo -n "66" > /sys/class/gpio/unexport
	;;
    status)
    	ls -d /sys/class/gpio/gpio*
	;;
    *)
	echo -n "Usage: $0 " 
	echo "{start|stop|restart|force-reload|status}"
	exit 1
	;;
esac

exit 0
