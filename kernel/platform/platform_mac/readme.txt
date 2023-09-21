参考：https://www.cnblogs.com/yikoulinux/p/15841343.html

该宏定义的文件如下：
include/linux/device.h

struct driver_attribute {
	struct attribute attr;
	ssize_t (*show)(struct device_driver *driver, char *buf);
	ssize_t (*store)(struct device_driver *driver, const char *buf,
			 size_t count);
};

#define DRIVER_ATTR(_name, _mode, _show, _store) \
	struct driver_attribute driver_attr_##_name = __ATTR(_name, _mode, _show, _store)

__ATTR定义于文件
include/linux/sysfs.h

#define __ATTR(_name, _mode, _show, _store) {				\
	.attr = {.name = __stringify(_name), .mode = _mode },		\
	.show	= _show,						\
	.store	= _store,						\
}

说明

	_name：名称，也就是将在sys fs中生成的文件名称。
	_mode：上述文件的访问权限，与普通文件相同，UGO的格式，最高权限0644，否则会报错。
	_show：显示函数，cat该文件时，此函数被调用。
	_store：写函数，echo内容到该文件时，此函数被调用。

test:
1. cat /sys/bus/platform/devices/duang/driver/peng， 或 cat /sys/bus/platform/drivers/duang/peng
效果相同，因为devices里面包含的drivers

2. sudo echo 2 > /sys/bus/platform/devices/duang/driver/peng， 或 sudo echo 2 > /sys/bus/platform/drivers/duang/peng
也许需要chmod 777 打开权限才能写
