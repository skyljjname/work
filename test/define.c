#include <stdio.h>

#define LOG_FILE "./define.log"
#define ErrLog() do{FILE* fd;\
                    fd = fopen(LOG_FILE,"a");\
                    fprintf(fd,"exit:%s %d %s\n", __FILE__, __LINE__,__func__);\
                    return -1;}while(0)

int main(int argc, char const *argv[])
{
    ErrLog();
    return 0;
}
    
