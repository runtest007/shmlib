#ifndef _SHM_INTERFACE_H
#define _SHM_INTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * 初始化
 *
 * @param flag
 *   1:write shm, 0:read shm
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
void write_release(void);

/**
 * 读数据handler释放
 */
void read_release(void);

/**
 * 获取可写数据块,失败返回NULL,
 */
unsigned char* get_write_data(void);

/**
 * 写操作数据更新，等待被读进程读取
 */
void update_write_shm(unsigned char* getdata);

/**
 * 写数据接口
 */
unsigned short write_data(unsigned char* buf, unsigned short len);

/**
 * 更新数据块可读状态和写索引
 */
void update_shm(unsigned char* getdata);

/**
 * 获取可写数据块,失败返回NULL
 */
unsigned char* get_read_data(void);

/**
 * 读操作数据更新，等待被写进程读取
 */
void update_read_shm(unsigned char* getdata);

#ifdef __cplusplus
}
#endif

#endif
