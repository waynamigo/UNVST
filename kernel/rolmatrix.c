#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
MODULE_LICENSE ("GPL");

int __init init_module (void)
{
  printk (KERN_INFO "The fitst module\n");
  return 0;
}

void __exit cleanup_module (void)
{
  printk (KERN_INFO "The module has uninstalled\n");
}