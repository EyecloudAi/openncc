
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include "pthread.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sdk.h"
#include "cameraCtrl.h"

#ifndef _WINDOWS
#include <sys/time.h>
#else
#include <windows.h>	
extern "C" int gettimeofday(struct timeval *tp, void *tzp);
#pragma warning(disable:4996)
#endif

extern "C" void os_sleep(int ms);

///////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    int ret;

    //1.load firmware
    printf("==================================\n");
    printf("1.load firmware\n");
    printf("==================================\n");
    ret = load_fw(NULL,NULL);
    
	emmc_control_update_app();

    return 0;
}
