cmd_/nfsroot/kernel/printk/test.ko := arm-linux-ld -EL -r  -T /share/arm/FriendlyARM-4412-201506/FriendlyARM-4412-DVD-A/Linux/linux-3.5/scripts/module-common.lds --build-id  -o /nfsroot/kernel/printk/test.ko /nfsroot/kernel/printk/test.o /nfsroot/kernel/printk/test.mod.o