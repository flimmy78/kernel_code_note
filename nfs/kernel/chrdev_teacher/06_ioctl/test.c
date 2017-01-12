#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include "ioctl_led.h"

#define	DEV_NAME	"test-mode"

struct test_dev
{
	unsigned int gpio[4];

	int major;
	struct file_operations fops;	
};

///////////////////////裸板驱动//////////////////////////

#define LED1	EXYNOS4X12_GPM4(0)
#define LED2	EXYNOS4X12_GPM4(1)
#define LED3	EXYNOS4X12_GPM4(2)
#define LED4	EXYNOS4X12_GPM4(3)

void led_init(struct test_dev *tdev)
{
	int i;
#if 1
	for(i = 0; i < ARRAY_SIZE(tdev->gpio); i++)
		gpio_free(tdev->gpio[i]);	
#endif

	for(i = 0; i < ARRAY_SIZE(tdev->gpio); i++)
		gpio_request(tdev->gpio[i], "test-led");	
}

void led_uninit(struct test_dev *tdev)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(tdev->gpio); i++)
		gpio_free(tdev->gpio[i]);	
}

void led_all_on(struct test_dev *tdev)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(tdev->gpio); i++)
		gpio_direction_output(tdev->gpio[i], 0);
}

void led_all_off(struct test_dev *tdev)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(tdev->gpio); i++)
		gpio_direction_output(tdev->gpio[i], 1);
}

/////////////////////字符设备框架///////////////////////

int test_open (struct inode *inode, struct file *filp)
{
	struct test_dev *tdev;
	printk("test open\n");
	tdev = container_of(filp->f_op, struct test_dev, fops);
	led_init(tdev);
	led_all_off(tdev);
	return 0;
}

int test_release (struct inode *inode, struct file *filp)
{
	struct test_dev *tdev;
	printk("test release\n");
	tdev = container_of(filp->f_op, struct test_dev, fops);
	led_uninit(tdev);
	led_all_off(tdev);
	return 0;
}

long test_ioctl (struct file *filp, unsigned int cmd, unsigned long args)
{
	struct test_dev *tdev;
	tdev = container_of(filp->f_op, struct test_dev, fops);
	printk("test ioctl\n");
	switch(cmd)
	{
		case LED_ALL_ON:
			printk("led all on\n");
			led_all_on(tdev);
			break;
		case LED_ALL_OFF:
			printk("led all off\n");
			led_all_off(tdev);
			break;
	}
	return 0;
}


/////////////////////////模块/////////////////////////////
struct test_dev tdev = {
	.gpio = {
		LED1, LED2, LED3, LED4
	},
	.major = 0,
	.fops = {
		.owner = THIS_MODULE,		//引用计数; 
		.open = test_open,			//可选实现;
		.release = test_release,	//可选实现;
		.unlocked_ioctl = test_ioctl,
	},
};

int test_init(void)
{
	int ret = 0;
	printk("module init\n");

	ret = register_chrdev(tdev.major, DEV_NAME, &tdev.fops);
	if(ret < 0)
	{
		return ret;
	}
	if(tdev.major == 0)
	{
		tdev.major = ret;
		printk("major = %d\n", tdev.major);
	}
	else
	{
		printk("major = %d\n", tdev.major);
	}

	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
	unregister_chrdev(tdev.major, DEV_NAME);
}

module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");

