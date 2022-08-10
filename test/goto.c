#include <stdio.h>
#define  DEBUG(fmt, args...)  	printf("%s-%s-%d "fmt"\n",\
        __FILE__,__FUNCTION__,__LINE__,##args)

int main(int argc, char const *argv[])
{

    printf("test1\n");
test:
    printf("test\n");
    return 0;
}
