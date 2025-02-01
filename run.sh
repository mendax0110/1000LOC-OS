#!/bin/bash
set -xue

# QEMU file path
QEMU=qemu-system-riscv32

# Path to clang and compiler flags
CC=/opt/homebrew/opt/llvm/bin/clang
OBJCOPY=/opt/homebrew/opt/llvm/bin/llvm-objcopy
CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib"

# absolute path to project root
SCRIPT_DIR=$(dirname "$(realpath "$0")")
BUILD_DIR="$SCRIPT_DIR/build"

# check we are in the build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Build the shell
$CC $CFLAGS -Wl,-T"$SCRIPT_DIR/user.ld" -Wl,-Map=shell.map -o shell.elf \
    "$SCRIPT_DIR/shell.c" "$SCRIPT_DIR/user.c" "$SCRIPT_DIR/common.c"

$OBJCOPY --set-section-flags .bss=alloc,contents -O binary shell.elf shell.bin
$OBJCOPY -I binary -O elf32-littleriscv shell.bin shell.bin.o

# Build the kernel
$CC $CFLAGS -Wl,-T"$SCRIPT_DIR/kernel.ld" -Wl,-Map=kernel.map -o kernel.elf \
    "$SCRIPT_DIR/kernel.c" "$SCRIPT_DIR/common.c" shell.bin.o

# Create disk image
mkdir -p disk
cp -r "$SCRIPT_DIR/disk/"*.txt disk/
tar cf disk.tar --format=ustar -C disk .

# Start QEMU
$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot    \
    -d unimp,guest_errors,int,cpu_reset -D qemu.log                           \
    -drive id=drive0,file=disk.tar,format=raw,if=none                         \
    -device virtio-blk-device,drive=drive0,bus=virtio-mmio-bus.0              \
    -kernel kernel.elf
