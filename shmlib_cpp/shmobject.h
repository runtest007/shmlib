#ifndef _SHM_OBJECT_H
#define _SHM_OBJECT_H
/*
 * 进程间共享内存
 * 内存的前INFOLEN个数据为信息记录
 * 需要先打开写内存进程再打开读内存进程
 * 2020/02/20
 */

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
    ~CShmObject() {}

public:
    static CShmObject* instance()
    {
        static CShmObject impl;
        return &impl;
    }

    //初始化入口函数
    int init(int flag, int shmkey, int datalen, int datanum); 

    //写进程初始化
    int write_init(int shmkey,int datalen,int datanum);

    //读进程初始化
    int read_init(int shmkey);

    //写操作退出时资源释放
    void write_release();

    //读操作退出时资源释放
    void read_release();

    //获取可写数据块
    unsigned char* get_write_data();
	
    //写操作数据更新
	void update_write_shm(unsigned char *getdata); 
	
    //写数据入口
	unsigned short write_data(unsigned char* buf, unsigned short len);

    //获取可读数据块
	unsigned char* get_read_data();

    //读操作数据更新
    void update_read_shm(unsigned char *getdata);

public:
    int iShmId;	//共享内存ID
    void *pShm;	//共享内存首地址
    unsigned int 	*pHead;		//信息区首地址
    unsigned int 	*pBlkSize;	//块大小
    unsigned int 	*pBlocks;	//总块数
    unsigned int 	*pRdIndex;	//读索引
    unsigned int 	*pWrIndex;	//写索引
    unsigned int 	*pQuit;		//退出标志位
    unsigned int 	*pExist;	//内存存在标志位
    unsigned char 	*pDataBegin;//数据区开始位置
    unsigned char 	**pShmData;	//共享内存数据块
};
#endif
