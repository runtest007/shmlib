#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include "shminterface.h"
#include "shmcommon.h"

int main()
{
    if(init(1,1234, sizeof(struBlkHead)+2048, 64) == 0)
    {
        printf("init read shm success!\n");
    }
    else
    {
        printf("init read shm failed!\n");
        return -1;
    }

    for(;;)
    {
        unsigned char* readdata = get_read_data();
        if(NULL != readdata)
        {
            struBlkHead* pstru = (struBlkHead*)readdata;
            int *p = (int*)pstru->data;
            printf("read shm:%d\n",*p);
            update_read_shm(readdata);
            sleep(1);
        }

		sleep(1);
    }
	
    read_release();
    return 0;
}
