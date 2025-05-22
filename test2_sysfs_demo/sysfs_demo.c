#include <linux/module.h>   // 基本模組功能 (module_init, module_exit, MODULE_*)
#include <linux/kernel.h>   // KERN_INFO, printk 等 log 工具
#include <linux/device.h>   // class_create, device_create, DEVICE_ATTR

// =======================
// 模組內部變數，只能此 .c 檔案內使用
// =======================
static int my_value = 0;  // sysfs 存取的整數變數（初始化為 0）

// =======================
// sysfs read 函式：讀取變數用
// =======================
static ssize_t value_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", my_value);  // 將 my_value 寫入 buf 中回傳給使用者
}

// =======================
// sysfs write 函式：寫入變數用
// =======================
static ssize_t value_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &my_value);  // 從使用者輸入的 buf 轉換成整數存到 my_value
    return count;                  // 回傳處理的字元數
}

// =======================
// 定義一個 read/write 屬性（DEVICE_ATTR_RW：自動綁定 show/store）
// 在 sysfs 裡會出現 /sys/class/sysfs_demo/demo/value
// =======================
static DEVICE_ATTR_RW(value);

// =======================
// class 與 device 指標，供 sysfs 創建使用
// =======================
static struct class *demo_class;
static struct device *demo_device;

// =======================
// 模組初始化（載入模組時呼叫）
// =======================
static int __init sysfs_demo_init(void)
{
    // 建立 class：會在 /sys/class/ 下建立一個資料夾
    demo_class = class_create("sysfs_demo");
    if (IS_ERR(demo_class))
        return PTR_ERR(demo_class);

    // 建立 device：在 /sys/class/sysfs_demo/ 下建立 demo 裝置資料夾
    demo_device = device_create(demo_class, NULL, 0, NULL, "demo");
    if (IS_ERR(demo_device))
        return PTR_ERR(demo_device);

    // 在裝置資料夾下建立 value 檔案（sysfs 屬性）
    return device_create_file(demo_device, &dev_attr_value);
}

// =======================
// 模組卸載（移除模組時呼叫）
// =======================
static void __exit sysfs_demo_exit(void)
{
    // 移除 sysfs 檔案
    device_remove_file(demo_device, &dev_attr_value);
    // 銷毀裝置與 class
    device_destroy(demo_class, 0);
    class_destroy(demo_class);
}

// =======================
// 註冊模組進出點
// =======================
module_init(sysfs_demo_init);
module_exit(sysfs_demo_exit);

// =======================
// 模組描述資訊（作者、授權）
// =======================
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MIN");
MODULE_DESCRIPTION("A simple sysfs variable module");



