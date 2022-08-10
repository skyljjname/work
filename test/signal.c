#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define CMD "AT+ALIVE" //8+4
#define CMD1 "AT+NOT+ALIVE"//12+4


int main(int argc, char const *argv[])
{
    char *_buf={"AT+ALIVE"};
    char *_buf1={"AT+NOT+ALIVE"};
    if(0 == memcmp(_buf,CMD,strlen("AT+ALIVE")))
    {
        printf("AT+ALIV=%d\n",(int)strlen("AT+ALIVE"));
    }
    if(0 == memcmp(_buf1,CMD1,strlen("AT+NOT+ALIVE")))
    {
        printf("AT+NOT+ALIVE=%d\n",(int)strlen("AT+NOT+ALIVE"));
    }
    
    return 0;
}
