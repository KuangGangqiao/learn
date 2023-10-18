1. make
2. make install
3. dmesg查看一下系统为设备分配的设备号MAJOR, MINOR是多少?
4. sudo mknod /dev/test_chr_dev c MAJOR MINOR
5. sudo ./app
