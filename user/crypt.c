#include <sys/types.h>
#include <pwd.h>
#include <uuid/uuid.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char ** argv)
{
    char * input_pass,* encrypted_pass;
    input_pass = getpass("Password:");
    encrypted_pass = crypt("$6$Heisenberg$",input_pass);
    printf("Original password:%40s\nEncrtpted password:%40s\n",input_pass,encrypted_pass);
    exit(0);
}