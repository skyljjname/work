#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CMD_GET_PASS "AT+CIP:"

//字符串拼接
int  string_spilt()
{

    FILE *fp=NULL;
    unsigned char get_pass[72]={0};
    unsigned char device_num[64]={0};
    fp=fopen("device_num","r");
    if(fp == NULL)
	{
        printf("111111111111\n");
		return -4;
	}

    if(NULL == fgets((char *)device_num,64,fp))
	{
        printf("222222222\n");
		fclose(fp);
		return -5;
	}
    fclose(fp);
	sprintf((char*)get_pass,"%s%s",CMD_GET_PASS,device_num);
    printf("%s\n",device_num);
    printf("%s\n",get_pass);
    return 0;
}

//if判断字符串
void if_string()
{
    char *p;
    malloc(10);
    printf("p = %s p = %d\n",p,p);
    if(!p)
    {
        printf("test1\n");
    }
    printf("test2\n");
}
int main(int argc, char const *argv[])
{
    if_string();
    return 0;
}
