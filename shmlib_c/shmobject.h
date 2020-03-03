#ifndef _SHM_OBJECT_H
#define _SHM_OBJECT_H
/**
 * **进程间共享内存
 * **内存的前INFOLEN个数据为信息记录，后面为数据块
 * **2020/02/20
 * **/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h> 
#include <stdlib.h>
#include <stdio.h>

#define INFOLEN 10

class CShmObject
{
public:
    CShmObject();

    static CShmObject* instance()
    {
        static CShmObject impl;
        return &impl;
    }
    
    int init(int flag, int shmkey, int datalen, int datanum); //初始化入口函数
    int write_init(int datalen,int datanum,int shmkey);//写操作初始化
    int read_init(int shmkey);//读操作初始化
    void write_release();//写操作退出时资源释放
    void read_release();//读操作退出时资源释放
    unsigned char* get_write_data();////获取数据区可写内存块指针
    unsigned char* get_read_data();////获取数据区可读内存块指针
    void update_write_shm(unsigned char *getdata);       //写操作数据更新
    void update_read_shm(unsigned char *getdata);       //读操作数据更新

    
public:
    int iShmId;//共享内存ID
    void *pShm;//共享内存首地址
    unsigned int *pHead;//信息区首地址
    unsigned int *pBlkSize;//块大小
    unsigned int *pBlocks;//总块数
    unsigned int *pRdIndex;//读索引
    unsigned int *pWrIndex;//写索引
    unsigned int *pExist;//内存存在标志位
    unsigned int *pQuit;//退出标志位
    unsigned char *pDataBegin;//数据区开始位置
    unsigned char **pShmData;//共享内存数据块
};

#endif
