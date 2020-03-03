#include "shmobject.h"
#ifndef _cplusplus
#define _cplusplus
#endif
#include "shminterface.h"

int init(int flag, int shmkey, int datalen, int datanum)
{
    return CShmObject::instance()->init(flag, shmkey, datalen, datanum);
}

void write_release()
{
    CShmObject::instance()->write_release();
}

void read_release()
{
    CShmObject::instance()->read_release();
}

unsigned char* get_write_data()
{
    return CShmObject::instance()->get_write_data();
}

void update_write_shm(unsigned char* getdata)
{
    CShmObject::instance()->update_write_shm(getdata);
}

unsigned char* get_read_data()
{
    return CShmObject::instance()->get_read_data();
}

void update_read_shm(unsigned char* getdata)
{
    CShmObject::instance()->update_read_shm(getdata);
}

