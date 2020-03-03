#ifndef _SHM_COMMON_H
#define _SHM_COMMON_H

typedef struct _struBlkHead
{
    unsigned char   flag;   //0为可写，1为可读
    unsigned short  len;
    unsigned char data[0];
}struBlkHead;

#endif
