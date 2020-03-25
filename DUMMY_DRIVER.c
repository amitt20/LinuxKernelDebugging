#include <linux/modules.h>
#include <stdio.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kernel.h>

//Creating the structure for my dummy device.
struct sample_device {
    char data [100];
    
}dummy_device;

#define DEVICE_NAME "mydevice"
int major_number, minor_number, ret;
struct cdev *mycdev;
dev_t dev_num;

//Allocating major number dynamically.
static int __init driver_start(void) {
    ret = alloc_chrdev_region(&dev_num, 0,1,DEVICE_NAME);
    if(ret ==NULL) {
        printk("Failed to allocte mojor number for this device")
    }
    return 0;

major_number = MAJOR(dev_num);  //Getting the Major No.
minor_number = MINOR(dev_num);  //Getting the minor no.

//Adding the device to chrdev structure
mycdev->ops = &fops;
mycdev = cedv_alloc();          
mycdev->OWNER = THIS_MODULE;
ret = cdev_add(mycdev, dev_num, 1);

//Different operations which need to be performed on the device. 
//Structure is defined in fs.h. There is function pointer
//For different operation. VFS will call this function on 
//the operations supported by the device. 

static int fops = {
    .owner = THIS_MODULE;
    .open = device_open();
    .read = device_read();
    .write = device_write();
    .release = device_close();
}

//opening of the device for doing operation on device.skipping failure case
int device_open(struct inode *i, struct file *f) {
    printk("device is open");
    return 0;
}

//Writing of data from userspace to kernel space
ssize_t device_write(struct file *f, const char *data, ssize_t Count, l_offt *offset) {
    ret = copy_from_user(dummy_device.data, data, bufferCount, loffset); 
    printk(KERN_ALERT, "Writing into the device");
    return ret;
}

//Reading of data from kernelspace to userspace.
ssize int device_read(struct inode *i, bufferlenth, bufferCount, loffset) {
    ret = copy_to_user(dummy_device.data, data, bufferCount, loffset); 
    printk(KERN_ALERT, "Reading from the device");
    return ret;
}

//closing of the device
static void __exit device_close() {
    cdev_del(mycdev);
    unregister_chrdev_region(dev_num, 1);
   
}
