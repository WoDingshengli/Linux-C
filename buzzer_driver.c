/*
 *蜂鸣器驱动程序，学习MOOC课程时所录
 */

#include<linux/module.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/errno.h>
#include<linux/types.h>
#include<linux/delay.h>
#include<linux/proc_fs.h>
#include<asm/uaccess.h>
#include<asm/hardware.h>
#include<asm/arch-s3c2410/s3c2410.h>

#define BUZZER_MAJOR 222 /*主设备号*/
#define BUZZER_DEVNAME "buzzer"
#define GPB_CTL_BASE io_p2v(0x56000010)
#define S3C2410_GPBCON (GPB_CTL_BASE + 0x0)
#define S3C2410_GPBDAT (GPB_CTL_BASE + 0x4)

struct unit
{
    u32 *GPB_CON;
    u32 *GPB_DAT;
}; /*设备数据结构*/

static struct unit buzzer_unit = {
    .GPB_CON = (u32*)S3C2410_GPBCON,
    .GPB_DAT = (u32*)S3C2410_GPBDAT,
};

static void buzzer_set_value(struct unit *unit, u8 val)
{
    if(val == 1)
        *unit->GPB_DAT = *unit->GPB_DAT & (~0x1);/*PB0置低电平*/
    else
        *unit->GPB_DAT = *unit->GPB_DAT | 0x1;/*PB0置高电平*/
}

ssize_t Buzzer_open(struct inode *innode, struct file *file)
{
    MOD_INC_USE_COUNT;
    file->private_data = &buzzer_unit;
    return 0;
}

ssize_t Buzzer_release(struct inode *inode, struct file *file)
{
    MOD_DEC_USE_COUNT;
    return 0;
}

ssize_t Buzzer_write(struct file *file, const char *buf, size_t count, loff_t *offset)
{
    char temp;
    int ret;
    struct unit *unit = (struct unit *)file->private_data;
    ret = copy_from_user(&temp, buf, count);
    if(ret != 0)
    {
        printk("wrong!\n"); return -EFAULT;
    }
    else
        buzzer_set_value(unit, temp); /*把值写到私有数据*/

    return ret;
}

struct file_operations Buzzer_Ctl_ops =
{
    open:     Buzzer_open,
    write:    Buzzer_write,
    release:  Buzzer_release,
};

static devfs_handle_t devfs_handle, devfs_buzzer_dor;
static void_init init_hardware(struct unit *unit)
{
    *unit->GPB_CON &= ~0x2;
    *unit->GPB_CON |= 0x1; /*PBCON0 = 1, PB0为输出*/
    *unit->GPB_DAT |= 0x1; /*PB0初始值为1,蜂鸣器静音*/
}

static int__init init_buzzer()
{
    int res;
    printk("This is my Buzzer driver!\n");
    res = devfs_register_chrdev(BUZZER_MAJOR, BUZZER_DEVNAME, &Buzzer_Ctl_ops);
    devfs_buzzer_dir = devfs_mk_dir(NULL, BUZZER_DEVNAME, NULL):
    devfs_handle = devfs_register(devfs_buzzer_dir, "0", DEVFS_FL_DEFAULT, BUZZER_MAJOR, 0, S_IFCHR|S_IRUSR|S_IWUSR, &Buzzer_Ctl_ops, NULL);
    init_hardware(&buzzer_unit);
    return res;
}

static void_exit clean_buzzer()
{
    devfs_unregister_chrdev(BUZZER_MAJOR, BUZZER_DEVNAME):
    devfs_unregister(devfs_handle);
    devfs_unregister(devfs_buzzer_dir);
}

module_init(init_buzzer);
module_exit(clean_buzzer);
MODULE_DESCRIIPTION("EduKit lll-2410 buzzer driver");
MODULE_AUTHOR("SSDUT MOOC");
MODULE_LICENSE("GPL");










