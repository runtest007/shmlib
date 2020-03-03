#ifndef _SHM_INTERFACE_H
#define _SHM_INTERFACE_H

#ifdef _cplusplus
extern "C"
{
#endif

/**
 * 初始化
 *
 * @param flag
 *   0:write shm, 1:read shm
 * @param shmkey
 *   the shm key
 * @param datalen
 *   mem block len need by write process
 * @param datanum
 *   mem block num need by write process
 * @return
 *   - 0: Success.
 *   - Negative on error.
 */
int init(int flag, int shmkey, int datalen, int datanum);

/**
 * 写数据handler释放
 */
void write_release();

/**
 * 读数据handler释放
 */
void read_release();

/**
 *写内存获取函数，成功：返回可写内存块指针
 */
unsigned char* get_write_data();

/**
 * 写操作数据更新，等待被读进程读取
 */
void update_write_shm(unsigned char* getdata);

/**
 * 读内存获取，成功：返回可读内存指针
 */
unsigned char* get_read_data();

/**
 * 读操作数据更新，等待被写进程读取
 */
void update_read_shm(unsigned char* getdata);



#ifdef _cplusplus
}
#endif

#endif
