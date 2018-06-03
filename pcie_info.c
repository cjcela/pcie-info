
/*
 * Author:  Carlos J. Cela
 *          https://github.com/cjcela/pcie-info
 *
 * License: MIT - See LICENSE file
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/pci.h>

 static struct proc_dir_entry* file;

 static int write_data(struct seq_file *m, void *v)
 {
    u8 reg8, type;
    u16 reg16;
    u32 reg32;
    struct pci_dev *pdev = NULL;

    printk("Collecting PCIe device information...\n");

    for_each_pci_dev(pdev) {
        seq_printf(m, "==============================================================\n");
        if (pdev->driver && pdev->driver->name) {
            seq_printf(m, "Driver name: %s\n", pdev->driver->name);
        } else {
            seq_printf(m, "No driver loaded for this device\n");
        }

        seq_printf(m, "Device ID:        0x%04x\n", pdev->device);
        seq_printf(m, "Vendor ID:        0x%04x\n", pdev->vendor);

        pci_read_config_word(pdev, PCI_STATUS, &reg16);
        seq_printf(m, "Status:           0x%04x\n", reg16);
        pci_read_config_word(pdev, PCI_COMMAND, &reg16);
        seq_printf(m, "Command:          0x%04x\n", reg16);

        pci_read_config_word(pdev, PCI_CLASS_DEVICE, &reg16);
        seq_printf(m, "Class device:     0x%04x\n", reg16);
        pci_read_config_byte(pdev, PCI_CLASS_PROG, &reg8);
        seq_printf(m, "Class API:        0x%02x\n", reg8);
        pci_read_config_byte(pdev, PCI_REVISION_ID, &reg8);
        seq_printf(m, "Revision ID:      0x%02x\n", reg8);

        pci_read_config_byte(pdev, PCI_BIST, &reg8);
        seq_printf(m, "BIST:             0x%02x\n", reg8);
        pci_read_config_byte(pdev, PCI_HEADER_TYPE, &type);
        seq_printf(m, "Header type:      0x%02x (", type);
        seq_printf(m, "%s)\n", (type==0?"normal":(type==1?"bridge":"cardbus")));
        pci_read_config_byte(pdev, PCI_LATENCY_TIMER, &reg8);
        seq_printf(m, "Latency timer:    0x%02x\n", reg8);
        pci_read_config_byte(pdev, PCI_CACHE_LINE_SIZE, &reg8);
        seq_printf(m, "Cache line size:  0x%02x\n", reg8);

        pci_read_config_dword(pdev, PCI_BASE_ADDRESS_0, &reg32);
        seq_printf(m, "BAR 0:            0x%08x\n", reg32);
        pci_read_config_dword(pdev, PCI_BASE_ADDRESS_1, &reg32);
        seq_printf(m, "BAR 1:            0x%08x\n", reg32);
        if(type==0x00) {
            pci_read_config_dword(pdev, PCI_BASE_ADDRESS_2, &reg32);
            seq_printf(m, "BAR 2:            0x%08x\n", reg32);
            pci_read_config_dword(pdev, PCI_BASE_ADDRESS_3, &reg32);
            seq_printf(m, "BAR 3:            0x%08x\n", reg32);
            pci_read_config_dword(pdev, PCI_BASE_ADDRESS_4, &reg32);
            seq_printf(m, "BAR 4:            0x%08x\n", reg32);
            pci_read_config_dword(pdev, PCI_BASE_ADDRESS_5, &reg32);
            seq_printf(m, "BAR 5:            0x%08x\n", reg32);
        }

        pci_read_config_dword(pdev, PCI_CARDBUS_CIS, &reg32);
        seq_printf(m, "Cardbus CIS ptr   0x%08x\n", reg32);

        pci_read_config_word(pdev, PCI_SUBSYSTEM_ID, &reg16);
        seq_printf(m, "Subsystem ID      0x%04x\n", reg16);
        pci_read_config_word(pdev, PCI_SUBSYSTEM_VENDOR_ID, &reg16);
        seq_printf(m, "Subs vendor ID    0x%04x\n", reg16);

        pci_read_config_dword(pdev, PCI_ROM_ADDRESS, &reg32);
        seq_printf(m, "PCI ROM address   0x%08x (%s)\n", reg32, (reg32 & PCI_ROM_ADDRESS_ENABLE?"enabled":"disabled"));

        pci_read_config_byte(pdev, PCI_CAPABILITY_LIST, &reg8);
        seq_printf(m, "Capability ptr    0x%02x\n", reg8);

        pci_read_config_byte(pdev, PCI_MAX_LAT, &reg8);
        seq_printf(m, "Maximum latency   0x%02x\n", reg8);
        pci_read_config_byte(pdev, PCI_MIN_GNT, &reg8);
        seq_printf(m, "Minimum gnt       0x%02x\n", reg8);
        pci_read_config_byte(pdev, PCI_INTERRUPT_PIN, &reg8);
        seq_printf(m, "Interrupt pin     0x%02x\n", reg8);
        pci_read_config_byte(pdev, PCI_INTERRUPT_LINE, &reg8);
        seq_printf(m, "Interrupt line    0x%02x\n", reg8);
     }
    return 0;
 }

 static int open_file(struct inode *inode, struct file *file)
 {
     return single_open(file, write_data, NULL);
 }

 static const struct file_operations fops = {
     .owner	    = THIS_MODULE,
     .open	    = open_file,
     .read	    = seq_read,
     .llseek	= seq_lseek,
     .release	= single_release,
 };

 static int __init init_mod(void)
 {
     file = proc_create("pcie_info", 0, NULL, &fops);

     if (!file) {
         return -ENOMEM;
     }

     return 0;
 }

 static void __exit exit_mod(void)
 {
     remove_proc_entry("pcie_info", NULL);
 }

 module_init(init_mod);
 module_exit(exit_mod);

 MODULE_LICENSE("MIT");
