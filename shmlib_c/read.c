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
    if(init(1, 2020, sizeof(struBlkHead)+2048, 30) == 0)
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
        else
        {
            printf("无可读数据，请等待写入\n");
            sleep(1);
        }
    }

    read_release();

    return 0;
}
