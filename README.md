# pcie-info

Linux kernel module that goes through each PCI device present (including PCIe)
and shows the standard PCI configuration space information for each device
on the file /proc/pcie_info

This is a nice small demo of how to create /proc entries and how to access PCI
device information, including configuration space data.

Similar information can be obtained using lspci, but lspci will not show all
the actual hex values for each configuration register; the raw configuration
information of a known device (in the example below, device with PCI address
0000:00:16.0) can also be obtained doing:

    /sys/bus/pci/devices/0000\:00\:16.0/config

but this approach requires knowing the device address, and one has to manually
parse and flip the bytes of the configuration register values.

The make file works for Ubuntu 16.04 LTS, building the module may be
different on other distros/versions. To use:

Build the module:

    make

Insert the module in kernel:

	make insert

Read PCIe configuration information:

	cat /proc/pcie_info

Remove kernel module:

	make remove
