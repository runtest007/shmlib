#include <string.h>
#include "shmobject.h"
#include "shmcommon.h"

CShmObject::CShmObject()
{
    iShmId = -1;
    void *pShm = NULL;
    pHead = NULL;
    pBlkSize = NULL;
    pBlocks = NULL;
    pRdIndex = NULL;
    pWrIndex = NULL;
    pExist = NULL;
    pQuit = NULL;
    pDataBegin = NULL;
    pShmData = NULL;
}

//共享内存初始化入口
int CShmObject::init(int flag, int shmkey, int datalen, int datanum)
{
    if(flag)
    {
		return read_init(shmkey);
    }
    else
    {
		return write_init(shmkey,datalen, datanum);
    }
}

//写操作初始化
int CShmObject::write_init(int shmkey,int datalen,int datanum)
{
    //生成一个key
    key_t key = ftok("/dev/shm/", shmkey);
    if(key == -1)  
    {  
        perror("ftok");
        return -1;
    }

    //创建共享内存,第二个参数为共享内存大小.前面五个值分别记录共享内存循环队列的块大小,总块数,写指针索引,读指针索引,共享内存存在标志与退出标志.	
    while(true)
    {
    	//创建共享内存,成功则返回一个id
        iShmId = shmget(key, sizeof(int)*INFOLEN+datalen*datanum, IPC_CREAT|0666);//0666是文件权限,不写只有超级用户才能打开
        if(-1 == iShmId)
        {
            perror("shmget failed");
            return -1;
        }
        
        //启动对该共享内存的访问,并把共享内存连接到当前进程的地址空间
        pShm = shmat(iShmId, 0, 0);
        if((void*)-1 == pShm)
        {
            perror("shmat failed");
            return -1;
        }
		
		//获取共享内存内容信息
        pHead = (unsigned int *)pShm;
        pBlkSize=pHead; //块大小
        pBlocks=pHead+1;//总块数
        pRdIndex=pHead+2;//读索引
        pWrIndex=pHead+3;//写索引
        pExist=pHead+4; //内存存在标志位
        pQuit=pHead+5;	//退出标志位
		pShmData=new unsigned char*[datanum];//指针数组,用于操作共享内存数据区指针
        
		//原先的共享内存没有销毁，则恢复内容    
        if (*pExist==99)
		{
            pDataBegin=(unsigned char *)(pHead+INFOLEN);//数据区开始位置
            for(int i=0;i<datanum;i++)
            {
                pShmData[i]=pDataBegin+i*datalen;
            }
            printf("初始化完成\n");
            printf("块大小:%d\n", *pBlkSize);
            printf("总块数:%d\n", *pBlocks);
            printf("读索引:%d\n", *pRdIndex);
            printf("写索引:%d\n", *pWrIndex);

            /*
            printf("原先内存没有销毁，先销毁掉再创建\n");
            if(-1 == shmctl(iShmId, IPC_RMID, NULL))
            {
                perror("shmctl failed");
                return -1;
            }
            */
			break;  
		}
		//否则,初始化共享内存头信息
		else
		{
			//偏移到数据区开始位置,获取每个数据的地址,存储在指针数组中
			pDataBegin=(unsigned char *)(pHead+INFOLEN);
			for(int i=0;i<datanum;i++)
			{
				pShmData[i]=pDataBegin+i*datalen;
			}
			
			//初始化共享内存数据信息
			*pBlkSize=datalen;
			*pBlocks=datanum;
			*pRdIndex=0;
			*pWrIndex=0;
			*pExist=99;//内存存在标志位
			*pQuit=0;
			
			printf("初始化完成，共享内存开辟成功\n");
			printf("块大小%d\n", *pBlkSize);
			printf("总块数%d\n", *pBlocks);
			printf("读索引%d\n", *pRdIndex);
			printf("写索引%d\n", *pWrIndex);
			printf("退出标志位%d\n", *pQuit);			
	
			break;
		}	
	}

    return 0;
}

//获取数据区可写内存块指针
unsigned char* CShmObject::get_write_data()
{
    if(*pQuit==1)//退出标志位
    {
        printf("已收到销毁命令，不再写内存，请及时销毁\n");
        return NULL;
    }
    
    //已经写到末尾，等待读线程读完之后往开头写
    if(*pWrIndex==*pBlocks)
    {
        if(pShmData[0][0]==0)
        {
            *pWrIndex=0;
        }
        else
        {
            //printf("共享内存已满，请稍候\n");
            return NULL;
        }
    }
    
    //已经追上读进程，等待读线程读完之后再写
    if(pShmData[*pWrIndex][0]==1)
    {
        //printf("共享内存已满，请稍候\n");
        return NULL;
    }
    
    return pShmData[*pWrIndex];
}

//写操作数据更新
void CShmObject::update_write_shm(unsigned char *getdata)
{
    getdata[0]=1;//可被读取标志位
    *pWrIndex=*pWrIndex+1;//更新写入索引
}

#if 1
//写数据入口
unsigned short CShmObject::write_data(unsigned char* buf, unsigned short len)
{
	//退出标志位判断
	if(*pQuit==1)
	{
		printf("已收到销毁命令，不再写内存，请及时销毁\n");
		return 0;
	}

	//已经写到末尾，等待读进程读完之后往开头写
	if(*pWrIndex==*pBlocks)
	{
		if(pShmData[0][0]==0)
		{
			*pWrIndex=0;
		}
		else
		{
			//printf("共享内存已满，请稍候\n");
			return 0;
		}
	}

	//已经追上读进程，等待读进程读完之后再写
	if(pShmData[*pWrIndex][0]==1)
	{
		//printf("共享内存已满，请稍候\n");
		return 0;
	}

	struBlkHead* phead = (struBlkHead*)(pShmData[*pWrIndex]);
	memcpy(phead->data, buf, len);
	phead->flag= 1;
	phead->len = len;
	*pWrIndex=*pWrIndex+1;

	return len;
}
#endif

//写操作资源释放
void CShmObject::write_release()
{
    //解除映射
    if(-1 == shmdt(pShm))
    {
        perror("shmdt failed");
        exit(3);
    }
    
	/*
    //销毁共享内存
    printf("解除映射%d\n",iShmId);
    if(-1 == shmctl(iShmId, IPC_RMID, NULL))
    {
        perror("shmctl failed");
        exit(4);
    }
	*/
    
    // 释放已开辟内存
    delete []pShmData;
}

//读操作初始化
int CShmObject::read_init(int shmkey)
{
    // 生成一个key
    key_t key = ftok("/dev/shm/", shmkey);

    //获取共享内存,成功则返回一个id
    iShmId = shmget(key, 0, 0);
    if(-1 == iShmId)
    {
        perror("shmget failed");
        return -1;
    }

    //启动对该共享内存的访问,并把共享内存连接到当前进程的地址空间
    pShm = shmat(iShmId, 0, 0);
    if((void*)-1 == pShm)
    {
        perror("shmat failed");
        return -1;
    }
    
    //获取共享内存内容信息
    pHead = (unsigned int *)pShm;
    pBlkSize=pHead;	//块大小
    pBlocks=pHead+1;//总块数
    pRdIndex=pHead+2;//读索引
    pWrIndex=pHead+3;//写索引
    pExist=pHead+4;//内存存在标志位
    pQuit=pHead+5; //退出标志位
	
    printf("块大小%d\n", *pBlkSize);
    printf("总块数%d\n", *pBlocks);
    printf("读索引%d\n", *pRdIndex);
    printf("写索引%d\n", *pWrIndex);
	printf("内存存在标志位%d\n", *pExist);
    printf("退出标志位%d\n", *pQuit);

    pDataBegin=(unsigned char *)(pHead+INFOLEN);//偏移到数据区开始位置
    pShmData=new unsigned char*[*pWrIndex];		//指针数组，用来操作共享内存数据区指针
    for(int i=0;i<*pBlocks;i++)
    {
        pShmData[i]=pDataBegin+i*(*pBlkSize);
    }

    return 0;
}

//获取数据区可读内存块指针
unsigned char* CShmObject::get_read_data()
{
    unsigned char *readaddr = NULL;
    if (*pRdIndex!=*pBlocks && pShmData[*pRdIndex][0]==1)
    {
        readaddr=pShmData[*pRdIndex];
    }   
    else if(*pRdIndex==*pBlocks && pShmData[0][0]==1)
    {
        *pRdIndex=0;
        readaddr=pShmData[*pRdIndex];
    }
    else
    {
        return NULL;
    }
    
    return readaddr;
}

//读操作数据更新
void CShmObject::update_read_shm(unsigned char *getdata)
{
    getdata[0]=0;         //可被写入标志位
    *pRdIndex=*pRdIndex+1;//更新读取索引
}

//读操作资源释放
void CShmObject::read_release()
{
    //解除映射
    if(-1 == shmdt(pShm))
    {
        perror("shmdt failed");
        exit(3);
    }
    
    //销毁已开辟内存
    delete []pShmData;
}
