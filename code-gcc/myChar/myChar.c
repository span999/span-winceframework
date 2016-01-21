/**
 * @file   myChar.c
 * @author 
 * @date   
 * @version 0.1
 * @brief   
 */

#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <asm/uaccess.h>          // Required for the copy to user function
#include <linux/mutex.h>	         /// Required for the mutex functionality

#include <linux/version.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include "myChar.h"

#define  DEVICE_NAME "mychar"    ///< The device will appear at /dev/mychar using this value
#define  CLASS_NAME  "spclass"        ///< The device class -- this is a character device driver


MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("Some One");    ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("A Linux char driver");  ///< The description -- see modinfo
MODULE_VERSION("0.1");            ///< A version number to inform users

static DEFINE_MUTEX(mychar_mutex);  /// A macro that is used to declare a new mutex that is visible in this file
                                     /// results in a semaphore variable ebbchar_mutex with value 1 (unlocked)
                                     /// DEFINE_MUTEX_LOCKED() results in a variable with value 0 (locked)

static int    majorNumber;                  ///< Stores the device number -- determined automatically
static char   message[256] = {0};           ///< Memory for the string that is passed from userspace
static short  size_of_message;              ///< Used to remember the size of the string stored
static int    numberOpens = 0;              ///< Counts the number of times the device is opened
static struct class*  charClass  = NULL; ///< The device-driver class struct pointer
static struct device* charDevice = NULL; ///< The device-driver device struct pointer

// The prototype functions for the character driver -- must come before the struct definition
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int dev_ioctl(struct inode *, struct file *, unsigned int, unsigned long);
#else
static long dev_ioctl(struct file *, unsigned int, unsigned long);
#endif

/** @brief Devices are represented as file structure in the kernel. The file_operations structure from
 *  /linux/fs.h lists the callback functions that you wish to associated with your file operations
 *  using a C99 syntax structure. char devices usually implement open, read, write and release calls
 */
static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
   .ioctl = dev_ioctl,
#else
   .unlocked_ioctl = dev_ioctl,
#endif
};

/** @brief The LKM initialization function
 *  The static keyword restricts the visibility of the function to within this C file. The __init
 *  macro means that for a built-in driver (not a LKM) the function is only used at initialization
 *  time and that it can be discarded and its memory freed up after that point.
 *  @return returns 0 if successful
 */
static int __init mychar_init(void){
   printk(KERN_INFO "myChar: Initializing the myChar LKM\n");

   // Try to dynamically allocate a major number for the device -- more difficult but worth it
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "myChar failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "myChar: registered correctly with major number %d\n", majorNumber);

   // Register the device class
   charClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(charClass)){                // Check for error and clean up if there is
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(charClass);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "myChar: device class registered correctly\n");

   // Register the device driver
   charDevice = device_create(charClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(charDevice)){               // Clean up if there is an error
      class_destroy(charClass);           // Repeated code but the alternative is goto statements
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(charDevice);
   }
   printk(KERN_INFO "myChar: device class created correctly\n"); // Made it! device was initialized
   
   mutex_init(&mychar_mutex);       /// Initialize the mutex lock dynamically at runtime
   
   return 0;
}

/** @brief The LKM cleanup function
 *  Similar to the initialization function, it is static. The __exit macro notifies that if this
 *  code is used for a built-in driver (not a LKM) that this function is not required.
 */
static void __exit mychar_exit(void){
   device_destroy(charClass, MKDEV(majorNumber, 0));     // remove the device
   class_unregister(charClass);                          // unregister the device class
   class_destroy(charClass);                             // remove the device class
   unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister the major number
   mutex_destroy(&mychar_mutex);        /// destroy the dynamically-allocated mutex
   printk(KERN_INFO "myChar: Goodbye!\n");
}

/** @brief The device open function that is called each time the device is opened
 *  This will only increment the numberOpens counter in this case.
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_open(struct inode *inodep, struct file *filep){
   
   if(!mutex_trylock(&mychar_mutex)){    /// Try to acquire the mutex (i.e., put the lock on/down)
                                          /// returns 1 if successful and 0 if there is contention
      printk(KERN_ALERT "myChar: Device in use by another process");
      return -EBUSY;
   }
   
   numberOpens++;
   printk(KERN_INFO "myChar: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}

/** @brief This function is called whenever device is being read from user space i.e. data is
 *  being sent from the device to the user. In this case is uses the copy_to_user() function to
 *  send the buffer string to the user and captures any errors.
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param len The length of the b
 *  @param offset The offset if required
 */
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   int error_count = 0;
   // copy_to_user has the format ( * to, *from, size) and returns 0 on success
   error_count = copy_to_user(buffer, message, size_of_message);

   if (error_count==0){            // if true then have success
      printk(KERN_INFO "myChar: Sent %d characters to the user\n", size_of_message);
      return (size_of_message=0);  // clear the position to the start and return 0
   }
   else {
      printk(KERN_INFO "myChar: Failed to send %d characters to the user\n", error_count);
      return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
   }
}

/** @brief This function is called whenever the device is being written to from user space i.e.
 *  data is sent to the device from the user. The data is copied to the message[] array in this
 *  LKM using the sprintf() function along with the length of the string.
 *  @param filep A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param len The length of the array of data that is being passed in the const char buffer
 *  @param offset The offset if required
 */
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   sprintf(message, "%s(%d letters)", buffer, (int)len);   // appending received string with its length
   size_of_message = strlen(message);                 // store the length of the stored message
   printk(KERN_INFO "myChar: Received %d characters from the user\n", (int)len);
   return len;
}


#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int dev_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param) 
#else
static long dev_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
	int iRet = (-1);
	static struct mychar_arg_t prv_arg;
	
	printk(KERN_INFO "myChar: dev_ioctl, cmd=%d arg=0x%x\n", cmd, arg);
	
	switch (cmd)
	{
		case MYCHAR_GET_VALUE:
			printk(KERN_INFO "myChar: dev_ioctl, MYCHAR_GET_VALUE...arg->data=%u\n", ((struct mychar_arg_t *)arg)->data);
			if (copy_to_user((struct mychar_arg_t *)arg, &prv_arg, sizeof(struct mychar_arg_t)))
			{
				iRet = -EACCES;
			}
			break;

		case MYCHAR_CLR_VALUE:
			printk(KERN_INFO "myChar: dev_ioctl, MYCHAR_CLR_VALUE...\n");
			prv_arg.data = 0;
			iRet = 0;
			break;

		case MYCHAR_SET_VALUE:
			printk(KERN_INFO "myChar: dev_ioctl, MYCHAR_SET_VALUE...arg->data=%u\n", ((struct mychar_arg_t *)arg)->data);
			if (copy_from_user(&prv_arg, (struct mychar_arg_t *)arg, sizeof(struct mychar_arg_t)))
			{
				iRet = -EACCES;
			}
			break;

		default:
			printk(KERN_INFO "myChar: dev_ioctl, nothing to do...\n");
			iRet = -EINVAL;
	}	///switch
	
	return iRet;
}

/** @brief The device release function that is called whenever the device is closed/released by
 *  the userspace program
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_release(struct inode *inodep, struct file *filep){
   mutex_unlock(&mychar_mutex);          /// Releases the mutex (i.e., the lock goes up)
   printk(KERN_INFO "myChar: Device successfully closed\n");
   return 0;
}

/** @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(mychar_init);
module_exit(mychar_exit);


