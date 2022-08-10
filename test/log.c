#include <stdio.h>
int main(int argc, char const *argv[])
{
    FILE* fd;
    fd = fopen("./test.log","a+");
    fprintf(fd,"你猜怎么看： %s, %d\n", __func__,__LINE__);
    fclose(fd);
    return 0;
}
