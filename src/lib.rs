#![feature(lang_items)]
#![feature(unique)]
#![feature(const_fn)]
#![feature(alloc, collections)]
#![no_std]

extern crate rlibc;
extern crate volatile;
extern crate spin;
extern crate multiboot2;
extern crate x86_64;
#[macro_use]
extern crate bitflags;


//extern crate bump_allocator;
extern crate hole_list_allocator;
extern crate alloc;
#[macro_use]
extern crate collections;
#[macro_use]
extern crate once;

#[macro_use]
mod vga_buffer;
mod memory;

#[no_mangle]
#[warn(unused_must_use)]
pub extern "C" fn rust_main(multiboot_information_address: usize) {
    // ATTENTION: we have a very small stack and no guard page
    vga_buffer::clear_screen();
    println!("WELCOME FRIEND !");

    let boot_info = unsafe {
        multiboot2::load(multiboot_information_address)
    };
    enable_nxe_bit();
    enable_write_protect_bit();

    // set up guard page and map the heap pages
    memory::init(boot_info);

    println!("memory init successfully!");

    use alloc::boxed::Box;
    let mut heap_test = Box::new(42);
    *heap_test -= 15;
    let heap_test2 = Box::new("hello");
    println!("{:?} {:?}", heap_test, heap_test2);

    let mut vec_test = vec![1,2,3,4,5,6,7];
    vec_test[3] = 42;
    for i in &vec_test {
        print!("{} ", i);
    }

    println!("work well.");


    for i in 0..10000 {
        format!("Some String");
    }

    loop {}
}

fn enable_nxe_bit() {
    use x86_64::registers::msr::{IA32_EFER, rdmsr, wrmsr};

    let nxe_bit = 1 << 11;
    unsafe {
        let efer = rdmsr(IA32_EFER);
        wrmsr(IA32_EFER, efer | nxe_bit);
    }
}

fn enable_write_protect_bit() {
    use x86_64::registers::control_regs::{cr0, cr0_write, Cr0};

    unsafe { cr0_write(cr0() | Cr0::WRITE_PROTECT) };
}

#[lang = "eh_personality"] extern fn eh_personality() {}
#[lang = "panic_fmt"]
#[no_mangle]

pub extern fn panic_fmt(fmt: core::fmt::Arguments, file: &'static str,
                        line: u32) -> ! {
    println!("\n\nPANIC in {} at line {}:", file, line);
    println!("    {}", fmt);
    loop{}
}