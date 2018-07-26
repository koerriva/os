#ifndef _ASM_MACROS_H_
#define _ASM_MACROS_H_

#define ASM_EXIT_IRQ \
    asm("leave; IRET;" : : :);
//    asm("pop    %%ebp; IRET;" : : :);

#define ASM_SET_IDT(V) \
    asm("lidt (%0);" : : "r"(&V) : );

#define ASM_GET_IDT(V) \
    asm("sidt (%0);" : : "r"(&V) : );

#define ASM_SET_GDT(V) \
    asm("lgdt (%0);" : : "r"(&V) : );

#define ASM_GET_GDT(V) \
    asm("sgdt (%0);" : : "r"(&V) : );

#define ASM_HALT \
    asm("hlt;" : : :);

#define ASM_GET_DS(V) \
    asm("mov %%ds, %0" : "=r"(V) : : );

#define ASM_GET_CS(V) \
    asm("mov %%cs, %0" : "=r"(V) : : );

/* TODO: check againsg: ../glibc-2.17-78/glibc-2.17-c758a686/sysdeps/unix/sysv/linux/x86/sys/io.h */
#define ASM_OUTPORT_B(p, d) \
    asm("outb %b0, %w1" : : "a" (d), "Nd" (p) :);

#define ASM_OUTPORT_W(p, d) \
    asm("outw %w0, %w1" : : "a" (d), "Nd" (p) :);
    // asm("outw %1, %0" : : "dN" (p), "a" (d) :);

#define ASM_OUTPORT_L(p, d) \
    asm("outl %0, %w1" : : "a" (d), "Nd" (p) :);
    // asm("outl %1, %0" : : "dN" (p), "a" (d) :);

#define ASM_INPORT_B(p, d) \
    asm("inb %w1, %b0" : "=a" (d) : "Nd" (p) :);

#define ASM_INPORT_W(p, d) \
    asm("inw %w1, %w0" : "=a" (d) : "Nd" (p) :);
    // asm("inw %1, %0" : "=a" (d) : "dN" (p) :);

#define ASM_INPORT_L(p, d) \
    asm("inl %w1, %0" : "=a" (d) : "Nd" (p) :);
    // asm("inl %1, %0" : "=a" (d) : "dN" (p) :);

static inline void ASM_OUTSPORT_B(int port, const void *addr, unsigned long count) {
    asm("rep; outsb" :  "+S"(addr), "+c"(count) : "d"(port));
}

static inline void ASM_INSPORT_B(int port, void *addr, unsigned long count) {
    asm("rep; insb" : "+D"(addr), "+c"(count) : "d"(port));
}

static inline void ASM_OUTSPORT_W(int port, const void *addr, unsigned long count) {
    asm("rep; outsw" :  "+S"(addr), "+c"(count) : "d"(port));
}

static inline void ASM_INSPORT_W(int port, void *addr, unsigned long count) {
    asm("rep; insw" : "+D"(addr), "+c"(count) : "d"(port));
}

static inline void ASM_OUTSPORT_L(int port, const void *addr, unsigned long count) {
    asm("rep; outsl" :  "+S"(addr), "+c"(count) : "d"(port));
}

static inline void ASM_INSPORT_L(int port, void *addr, unsigned long count) {
    asm("rep; insl" : "+D"(addr), "+c"(count) : "d"(port));
}

#endif