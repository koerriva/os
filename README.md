# make iso
`sudo apt install grub-pc-bin`

# c call nasm
* 当参数少于7个时， 参数从左到右放入寄存器: rdi, rsi, rdx, rcx, r8, r9。
* 当参数为 7 个以上时， 前 6 个与前面一样， 但后面的依次从 "右向左" 放入栈中。
[paper](https://www.cnblogs.com/chenchenluo/archive/2012/04/02/2421457.html)

