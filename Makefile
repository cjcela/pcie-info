obj-m += pcie_info.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
insert:
	insmod ./pcie_info.ko
remove:
	rmmod pcie_proc
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
