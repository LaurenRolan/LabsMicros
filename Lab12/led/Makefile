MODULEDIR=/lib/modules/`uname -r`/char

TARGET=led.ko
SRCS=$(TARGET:.ko=.c)

#KERNEL_SOURCE=/lib/modules/`uname -r`/build
KERNEL_SOURCE=../linux-4.17
EXTRA_CFLAGS+=-D__KERNEL__ -DMODULE

obj-m+=$(TARGET:.ko=.o)

all: $(TARGET) $(UDEV_RULES)

$(TARGET): $(TARGET:.ko=.c)
	$(MAKE) -C $(KERNEL_SOURCE) O=. M=`pwd` modules

clean:
	rm -rf *~ *.bak *.o *.mod.c .*.ko.cmd .*.o.cmd .tmp* \
		Module.symvers modules.order

distclean: clean
	rm -f $(TARGET)


install:
	install -m 0755 -d $(MODULEDIR)
	install -m 0644 $(TARGET) $(MODULEDIR)

uninstall:
	rm -f $(MODULEDIR)/$(TARGET)
