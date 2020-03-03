#include "shminterface.h"
#include "shmcommon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    if(init(0, 2020, sizeof(struBlkHead)+2048, 30) == 0)
	{
        printf("init write shm success!\n");
	}
    else
    {
        printf("write shm init failed!\n");
        return -1;
    }

    int i = 0;
    unsigned char* shmdata = NULL;
    for(;;)
    {
        shmdata = get_write_data();
        if(NULL != shmdata)
        {
            ++i;
            struBlkHead* phead = (struBlkHead*)shmdata;
            phead->len = sizeof(int);
            memcpy(phead->data, (char*)(&i), sizeof(int));
            printf("write shm:%d\n", i);
            update_write_shm(shmdata);
        }
        sleep(1);
    }

    write_release();

    return 0;
}
