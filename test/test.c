#include <stdio.h>
#define  DEBUG(fmt, args...)  	printf("%s-%s-%d "fmt"\n",\
        __FILE__,__FUNCTION__,__LINE__,##args)

int main(int argc, char const *argv[])
{
    unsigned int __attribute__ ((unused))i = 0;
    //DEBUG("this is, test=%d test2=%d",i,i);
    return 0;
}
