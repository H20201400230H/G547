USERFILE ?= userapp
KERN_NAME ?= bme280
DATAFILE ?= data.log
TEXT_EDITOR ?= nano

obj-m := $(KERN_NAME).o
 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
	gcc -o $(USERFILE) $(USERFILE).c

kern:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

user:
	gcc -o $(USERFILE) $(USERFILE).c
 
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	rm userapp

install:
	insmod $(KERN_NAME).ko

remove:
	rmmod $(KERN_NAME).ko

run:
	./$(USERFILE)

log_clean:
	rm $(DATAFILE)

log_display:
	nano $(DATAFILE)
