#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include "shmcommon.h"
#include "shmobject.h"

int main()
{
	CShmObject read_obj;
	if(read_obj.read_init(4321) == 0)
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
        unsigned char* readdata = read_obj.get_read_data();
        if(NULL != readdata)
        {
            struBlkHead* pstru = (struBlkHead*)readdata;
            int *p = (int*)pstru->data;
            printf("read shm:%d\n", *p);
            read_obj.update_read_shm(readdata);
        }
		
        sleep(1);
    }

    read_obj.read_release();

    return 0;
}
