#include <linux/init.h>
#include <linux/module.h>

// 模組資訊宣告
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MIN");
MODULE_DESCRIPTION("Hello World Kernel Module");

// 模組初始化函式
static int __init hello_init(void) {
    printk(KERN_INFO "Hello, Kernel!\n");
    return 0;
}

// 模組清除函式
static void __exit hello_exit(void) {
    printk(KERN_INFO "Goodbye, Kernel!\n");
}

//告訴 Kernel 哪些函式是模組的進入點與退出點
module_init(hello_init);
module_exit(hello_exit);
