#include <stdio.h>
#include <stdlib.h>
#include <termios.h>


int main()
{
    struct termios new_setting,init_setting;
    char passwd[20];
    tcgetattr(0,&init_setting); // 0 是fd 这里保存现在的终端设置
    new_setting = init_setting;
    new_setting.c_lflag=!ECHO; // new_setting.c_lflag&=~ECHO;都是可以的
    tcsetattr(0,TCSANOW,&new_setting);
    fflush(NULL);
    printf("Please enter password:\n");
    fgets(passwd,sizeof(passwd),stdin);

    printf("you have enter : %s",passwd);
    tcsetattr(0,TCSANOW,&init_setting);
    return 0;
}