Block Device driver 

STEP1: Download the main.c and Makefile

STEP2 :make : to make the kernel object file

STEP 3 :Go to the directory in which code is downloaded and give the command $ make all

STEP 4 : Insert the Object file into the kernel using sudo insmod main.ko

STEP 5 :Check if module is loaded using lsmod command

STEP 6 :Check the partition details that we have created using cat /proc/partitions also check using ls -l /dev/dof* or sudo fdisk -l

STEP 7 :Take root access using sudo -s command

STEP 8 :To enter input into disk use cat > /dev/dof ,type something & press enter to read back from the disk on command line use command xxd /dev/dof | less

STEP 9 :Remove the module from kernel using sudo rmmod main.ko