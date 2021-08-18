#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shminterface.h"
#include "shmcommon.h"

int main(int argc, char** argv)
{
    if(init(0, 1234, sizeof(struBlkHead)+2048, 64) == 0)
    {
        printf("create shm success!\n");
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
