# make iso
`sudo apt install grub-pc-bin`

# c call nasm
* 当参数少于7个时， 参数从左到右放入寄存器: rdi, rsi, rdx, rcx, r8, r9。
* 当参数为 7 个以上时， 前 6 个与前面一样， 但后面的依次从 "右向左" 放入栈中。
[Comment1](https://www.cnblogs.com/chenchenluo/archive/2012/04/02/2421457.html)

# set VGA without BIOS
Sets VGA-compatible video modes without using the BIOS
[Comment2](https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c)

# 使用in和out指令对端口进行读写：

* 和端口通信的数据只能用al和ax，8位数据用al，16位数据用ax，如果用其它寄存器会报错；

* in用于从端口读数据，使用格式是：in al/ax, 端口号

* out用于向端口写数据，使用格式时：out 端口号, al/ax

* 其中端口号只能用立即数或者dx表示（dx用于存放端口号，端口号毕竟是16位的嘛），并且如果用寄存器表示端口号就必须用dx，用其它的就会报错；


# vga 教程

* [paper](https://github.com/AlgorithMan-de/wyoos/blob/master/src/drivers/vga.cpp)