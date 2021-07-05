#ifndef INCLUDE_H_
#define INCLUDE_H_

#include <stdio.h> 
#include <ctype.h> 
#include <errno.h> 

#ifndef _WINDOWS
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/file.h> 
#include <arpa/inet.h>
#include <netdb.h> 
#include <sys/ioctl.h> 
#include <sys/time.h>
#include <sys/shm.h> 
#else
#include <windows.h>
typedef int socklen_t;
#endif

#include <string.h> 
#include <pthread.h>

#include <sys/types.h> 

#include <stdlib.h>
#include <stdarg.h>
#include<sys/stat.h>
#include <stdint.h>
#include <pthread.h>
#include <stddef.h>

#define BYTE uint8_t
#define INT8U uint8_t
#define INT16U uint16_t
#define INT16 int16_t
#define INT32U uint32_t
#define INT32 int32_t
#define UINT  uint32_t
#define BOOL  unsigned char 


#define SUCCESS            0
#define FAILED             -1
#define FALSE              0
#define TRUE               1

#define  _PACKED_			__attribute__((packed))   //单字节内存位对齐

#endif

