#! /bin/sh
printf "Enter your password: "
stty -echo
read passwd1 < /dev/tty
printf "\nEnter your password again: "
read passwd2 < /dev/tty
stty echo
 
if [ "$passwd1" = "$passwd2" ]
then
    passwd=$passwd1
    printf "\nPassword set successfully!\n"
else
    printf "\nThe two passwords you typed do not match!\n"
fi
