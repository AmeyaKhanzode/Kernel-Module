savedcmd_/home/ameyak/Desktop/os/kernel_module/project/module.mod := printf '%s\n'   module.o | awk '!x[$$0]++ { print("/home/ameyak/Desktop/os/kernel_module/project/"$$0) }' > /home/ameyak/Desktop/os/kernel_module/project/module.mod
