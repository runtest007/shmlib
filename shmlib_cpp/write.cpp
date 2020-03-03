#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#include "shmcommon.h"
#include "shmobject.cpp"

int main(int argc, char** argv)
{
    CShmObject write_obj;
    if( write_obj.write_init(sizeof(struBlkHead)+2048, 30, 2020) == 0 )
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
        shmdata = write_obj.get_write_data();
        if(NULL != shmdata)
        {
            ++i;
            struBlkHead* phead = (struBlkHead*)shmdata;
            phead->len = sizeof(int);
            memcpy(phead->data, (char*)(&i), sizeof(int));
            printf("write shm:%d\n", i);
            write_obj.update_write_shm(shmdata);
        }
        sleep(1);
    }

    write_obj.write_release();

    return 0;
}
