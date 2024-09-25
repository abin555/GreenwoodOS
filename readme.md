# Greenwood Operating System

### Why the name?

I don't know, I think it sounds cool.

### Whats it for?

My own personal exploration of low level system concepts.

### So you've got questions
* Is it good? - Probably not.
* Should I use it? - Absolutely not.
* Will it beat Linux? - I sure hope it won't.
* What does it do? - Definitely will try to nuke your hard drive on virtual hardware. Maybe overheat your CPU. Otherwise, terrible UI, program management, and awful memory manager.

# Requirements

* grub-mkrescue
* xorriso
* gcc
* qemu
* make (gnu version)

# How to build

Simple compilation:
```bash
make build
```

Compile and run in Qemu:
```bash
make
```

Building the filesystem
```bash
make -C filesystem

sudo make make_ext2
#/\ mounts an image to the filesystem, needs sudo
```
