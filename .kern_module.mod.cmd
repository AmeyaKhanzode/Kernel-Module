savedcmd_/home/ameya/Kernel-Module/kern_module.mod := printf '%s\n'   kern_module.o | awk '!x[$$0]++ { print("/home/ameya/Kernel-Module/"$$0) }' > /home/ameya/Kernel-Module/kern_module.mod
