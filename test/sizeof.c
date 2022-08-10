#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char const *argv[])
{
    char *_argv[6] = {"truecrypt","/system_enc/system.bin", "/system", "--password=null",\
    "--keyfiles=/system_enc/Readme.txt","--non-interactive"};
    char str_argv0[200];
    char str_argv1[200];
    char str_argv2[200];
    char str_argv3[200];
    char str_argv4[200];
    char str_argv5[200];

    _argv[0]=str_argv0;
    _argv[1]=str_argv1;
    _argv[2]=str_argv2;
    _argv[3]=str_argv3;
    _argv[4]=str_argv4;
    _argv[5]=str_argv5;
    memset(_argv[0], 0, sizeof(str_argv0));

    printf("%ld\n",sizeof(const char *));
    printf("%ld\n",sizeof(unsigned char *));

    return 0;
}
