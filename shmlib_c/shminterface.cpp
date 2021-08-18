#include "shmobject.h"
#include "shminterface.h"

int init(int flag, int shmkey, int datalen, int datanum)
{
    return CShmObject::instance()->init(flag, shmkey, datalen, datanum);
}

void write_release(void)
{
    CShmObject::instance()->write_release();
}

void read_release(void)
{
    CShmObject::instance()->read_release();
}

unsigned char* get_write_data(void)
{
    return CShmObject::instance()->get_write_data();
}

void update_write_shm(unsigned char* getdata)
{
    CShmObject::instance()->update_write_shm(getdata);
}

unsigned short write_shm(unsigned char* buf, unsigned short len)
{
    return CShmObject::instance()->write_data(buf, len);
}

unsigned char* get_read_data(void)
{
    return CShmObject::instance()->get_read_data();
}

void update_read_shm(unsigned char* getdata)
{
    CShmObject::instance()->update_read_shm(getdata);
}

