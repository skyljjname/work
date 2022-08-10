#include <stdio.h>

int main(int argc, char const *argv[])
{
    char buf[12]={"hello"};
    char * p ="world";
    sprintf(buf+5,"%s",p);
    printf("%s\n",buf);
    return 0;
}
