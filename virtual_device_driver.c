/*在内核空间申请一块4kB内存用于模拟一个设备，并在驱动中提供对这块内存的读写、控制和定位
 *以供用户空间到进程能通过Linux系统调用获取或者设置这块内存的内容
 */

 #include<linux/module.h>
 #include<linux/fs.h>
 #include<linux/init.h>
 #include<linux/cdev.h>
 #include<linux/slab.h>
 #include<linux/uaccess.h>

 #define GLOBALMEM_SIZE  0x1000 /*虚拟设备内存4k*/
 #define MEM_CLEAR 0x1 /*控制命令，用于清零虚拟内存*/
 #define GLOBALMEM_MAJOR 230 /*主设备号*/

 static int globalmem_major = GLOBALMEM_MAJOR;
 modual_param(globalmem_major, int, S_IRUGO): /*用于改变设备号*/

 struct globalmem_dev{
     struct cdev cdev;
     unsigned char mem[GLOBALMEM_SIZE];
}/*定义虚拟设备的结构体*/

struct globalmem_dev *globalmem_devp;

static int globalmem_open(struct inode *inode, struct file *filp)
{
    filp->private_data = globalmem_devp;  /*私有数据中存放虚拟设备指针，便于访问虚拟设备*/
    return 0;
}

static int globalmem_release(struct inode *inode, struct file *filp)
{
    return 0;
}/*关闭设备*/

static long globalmem_ioctl(struct file *filp, unsigned int cmd,  unsigned long arg)
{
    struct globalmem_dev *dev = filp->private_data;

    switch(cmd){
    case MEM_CLEAR:/*清内存命令*/
        memset(dev->mem, 0, GLOBALMEM_SIZE):
        printk(KERN_INFO "globalmem is set to zero\n");
        break;

    default:
        return -EINVAL;/*错误标志*/
   }

   return 0;

}

static ssize_t globalmem_read(struct file *filp, char__user *buf, size_t size, loff_t *ppos)
{
    unsigned long p = *ppos; 
    unsigned int count = size;
    int ret = 0;
    struct globalmem_dev *dev = filp->private_data;
    if(p >= GLOBALMEM_SIZE) return 0;
    if(count > GLOBALMEM_SIZE -p)
        count = GLOBALMEM_SIZE - p;
    if(copy_to_user(buf, dev->mem + p, count)){   /*拷贝到用户空间*/
        ret = -EFAULT;
    }else{
           *ppos += count; /*更新当前读写位置*/
           ret = count;
           printk(KERN_INFO "read %u bytes(s) from %lu\n", count, p);
    }

    return ret;
}

static ssize_t globalmem_write(struct file *filp, const char__user *buf, size_t size, loff_t *ppos)
{
       unsigned long p = *ppos; 
       unsigned int count = size;
       int ret = 0;
       struct globalmem_dev *dev = filp->private_data;
       if(p >= GLOBALMEM_SIZE) return 0;
       if(count > GLOBALMEM_SIZE -p)
           count = GLOBALMEM_SIZE - p;
       if(copy_to_user(dev->mem + p, buf, count)){   /*写入内核空间*/
           ret = -EFAULT;
        }else{
               *ppos += count; /*更新当前读写位置*/
               ret = count;
               printk(KERN_INFO "write %u bytes(s) from %lu\n", count, p);
        } 
        
        return ret;                                                                                                                                    
}               

static loff_t globalmem_llseek(struct file *filp, loff_t offest, int orig)
{
    loff_t ret = 0;
    switch(orig){
    case 0:
            if(offest < 0){ ret = -EINVAL; break;}
            if((unsigned int)offest > GLOBALMEM_SIZE){ ret = -EINVAL;break;}
            filp->f_pos = (unsigned int) offest;
            ret = filp->f_pos;
            break;
            /*从文件头开始加偏移量*/
    case 1:
            if((filp->f_pos + offset) > GLOBALMEM_SIZE){ret = -EINVAL; break;}
            if((filp->f_pos +offset) < 0){ret = -EINVAL; break;}
            filp->f_pos += offest;
            ret = filp->f_pos;
            break;
            /*从当前位置开始加偏移量*/
    default:
            ret = -EINVAL;break;
    }

    return ret;
}

/*操作定义及赋值*/
static const struct file_operations globalmem_fops = {
    .owner = THIS_MODULE,
    .llseek = globalmem_llseek,
    .read = globalmem_read,
    .write = globalmem_write,
    .inlocked_ioctl = globalmem_ioctl,
    .open = globalmem_open,
    .release = globalmem_release,
};

/*设置注册函数*/
static void globalmem_setup_cdev(struct globalmem_dev *dev, int index)
{
    int err, devno = MKDEV(globalmem_major, index);

    cdev_init(&dev->cdev, &globalmem_fops);/*操作集给cdev的文件操作结构体*/
    dev->cdev.owner = THIS_MODULE;
    err = cdev_add(&dev->cdev, devno, 1);/*注册*/
    if(err) printk(KERN_NOTICE "Error %d adding globalmem %d", err, index);
}

/*模块加载函数*/
static int__init globalmem_init(void)
{
    int ret;
    dev_t devno = MKDEV(globalmem_major, 0)/*生成主设备号*/
    if(globalmem_major) ret = register_chrdev_region(devno, 1, "globalmem");
    else{
        ret = alloc_chrdev_region(&devno, 0, 1, "globalmem");/*申请主设备号*/
        globalmem_major = MAJOR(devno);
    }/*根据是否已有主设备号，选用不同函数向系统申请注册设备号*/
    if(ret < 0) return ret;
    globalmem_devp = kzalloc(sizeof(struct globalmem_dev), GFP_KERNEL):/*申请设备内存*/
    if(!globalmem_devp){
        ret = -ENOMEM;
        goto fail_malloc;
    }
    globalmem_setup_cdev(globalmem_devp, 0);
    return o;
  fail_malloc:
    unregister_chrdev_region(devno, 1);
    return ret;
}

module_init(globalmem_init);

/*模块卸载函数*/
static void__exit globalmem_exit(void)
{
    cdev_del(&globalmem_devp->cdev);
    kfree(globalmem_devp);
    unregister_chrdev_region(MKDEV(globalmem_major, 0), 1);
}

module_exit(globalmem_exit);

MODULE_AUTHOR("WoDingshengli");
MODELE_LICENSE("GPL v2");
