#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


#define ErrLog(errmsg) do{perror(errmsg);\
					   	  printf("%s %d %s\n", __FILE__, __LINE__, __FUNCTION__);\
					   	  return -1;}while(0)
#define main_serial_name "/dev/ttyUSB1"
#define control_serial_name "/dev/ttyS1"

int serial_open(const char *DevName, int nSpeed, int nBits, int nEvent, int nStop)
{
	struct termios attr;
	int fd = -1;
    printf("%s\n",DevName);
	/* open uart */
	fd = open(DevName, O_RDWR|O_NOCTTY|O_NDELAY);
	//fd = open(DevName, O_RDWR|O_NOCTTY);
	if(-1 == fd)
	{
        perror("打开串口失败");
		goto ERR_OUT;
	}

	/* get uart configuration */
	if(0 != tcgetattr(fd, &attr))
	{
		goto ERR_OUT;
	}

	//set console attr
	attr.c_cflag = CLOCAL | CREAD;
	attr.c_cflag &= ~CSIZE;
	attr.c_lflag &= ~(ICANON|ECHO|ECHOE|ECHOK|ECHONL|NOFLSH);		//No echo

//add zln for special control characters
#if 1
	attr.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	attr.c_lflag &= ~(ISIG | IEXTEN);
	attr.c_oflag &= ~(OPOST | ONLCR);
#endif

	//baudrate setting
	switch( nSpeed )
	{
  	  	case 2400:
  			cfsetispeed(&attr, B2400);
  			cfsetospeed(&attr, B2400);
  			break;
  	  	case 4800:
  			cfsetispeed(&attr, B4800);
  			cfsetospeed(&attr, B4800);
  			break;
  	  	case 9600:
  			cfsetispeed(&attr, B9600);
  			cfsetospeed(&attr, B9600);
  			break;
  	  	case 115200:
  			cfsetispeed(&attr, B115200);
  			cfsetospeed(&attr, B115200);
  			break;
  	  	case 460800:
  			cfsetispeed(&attr, B460800);
  			cfsetospeed(&attr, B460800);
  			break;
  	  	default:
  			cfsetispeed(&attr, B115200);
  			cfsetospeed(&attr, B115200);
  			break;
	}

	switch( nBits )	//data bit setting
	{
		case 7:
  			attr.c_cflag |= CS7;
  			break;
  	  	case 8:
  			attr.c_cflag |= CS8;
  			break;
		default:
			attr.c_cflag |= CS8;
	}

	//odd-even check setting
	switch( nEvent )
 	{
  	  	case 'O': 
  	  	case 'o': 
  			attr.c_cflag |= PARENB;
  			attr.c_cflag |= PARODD;
  			attr.c_iflag |= (INPCK | ISTRIP);
  			break;
  			
  	  	case 'E': 
  	  	case 'e': 
  			attr.c_iflag |= (INPCK | ISTRIP);
  			attr.c_cflag |= PARENB;
  			attr.c_cflag &= ~PARODD;
  			break;
  			
  	  	case 'N':  
  	  	case 'n':  
  			attr.c_cflag &= ~PARENB;
  			break;
  			
		default:
			attr.c_cflag &= ~PARENB;
  			break;
	}

	//stop bit setting
	switch( nStop )		
  	{
		case 1:
			attr.c_cflag &=  ~CSTOPB;
			break;
		case 2:
			attr.c_cflag |=  CSTOPB;
			break;
		default:
			attr.c_cflag &=  ~CSTOPB;
			break;
	}

	//min receive bytes 
  	attr.c_cc[VMIN]	= 0;	
	//max receive timeout (unit : 0.1s)
	attr.c_cc[VTIME] = 1;	

	//reset uart config
	if( tcsetattr(fd, TCSANOW, &attr) )
	{
		goto ERR_OUT;
	}

	//success out
	return fd;

ERR_OUT:
	if( fd != -1 )
	{
		close(fd);
	}
	return -1;
}

void _signal_handle(int signo)
{
	switch (signo) 
	{
		case SIGINT:
		case SIGTERM:
			printf("siganl sigtrem\n");
			exit(0);
            break;
		case SIGKILL:
			/* exit immediately*/
			printf("signal kill \n");
			exit(0);
			break;
		case SIGUSR1:
			break;
	}
}

int main(int argc, char const *argv[])
{
    /* code */
    int serial_fd,serial_bytes;
    if(argc < 2)
    {
        ErrLog("输入有误，需要参数\n");
        return -1;
    }
    printf("%s\n",argv[1]);
	serial_fd=serial_open(main_serial_name,115200,8,'N',1);
	#if 0
    if(argv[1] == "1")
    {
        //ErrLog("test");
        serial_fd=serial_open(main_serial_name,115200,8,'N',1);
    }
    else
        ErrLog("test");
        serial_fd=serial_open(control_serial_name,115200,8,'N',1);
	#endif
    if(serial_fd < 0)
    {
        ErrLog("打开串口失败");
        return -1;
    }

	int ret;
    char read_buf[64] = {0};
    char *write_buf = "hello serila";
	struct timeval timeout;
	fd_set read_fdset;
	signal(SIGKILL, _signal_handle);
    while(1)
    {
		FD_ZERO(&read_fdset); 
		FD_SET(serial_fd, &read_fdset); 
		timeout.tv_sec = 4; 
		timeout.tv_usec = 0; 

		ret = select(serial_fd+1 , &read_fdset,  NULL ,  NULL , NULL);
		if(ret < 0)
		{
			ErrLog("select is fail");
			ret = -4;
		}
		else if(ret == 0)
		{
			ErrLog("select timeout!");
			continue;
		}
		if(FD_ISSET(serial_fd,&read_fdset))
		{
			memset(read_buf,0,sizeof(read_buf));
			serial_bytes = read(serial_fd,read_buf,sizeof(read_buf));
        	printf("%s %d 读取的内容是:%s 字节数是:%d\n",__FILE__,__LINE__,read_buf,serial_bytes);
        	if(serial_bytes < 0)
        	{
            	ErrLog("读取/写入串口数据失败");
            	break;
        	}
        	else if(serial_bytes == 0)
        	{
            	//ErrLog("串口数据读/写完成或没有数据");
            	continue;
        	}
        	else
        	{
				if(strncmp(read_buf,write_buf,strlen(write_buf)))
				{
					printf("%s %d 读取的内容是:%s\n",__FILE__,__LINE__,read_buf);
					continue;
				}
				else
				{
					//break;
				}
        	}
		}
    }
    return 0;
}
