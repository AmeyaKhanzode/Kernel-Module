savedcmd_/home/ameya/Kernel-Module/module.mod := printf '%s\n'   module.o | awk '!x[$$0]++ { print("/home/ameya/Kernel-Module/"$$0) }' > /home/ameya/Kernel-Module/module.mod
