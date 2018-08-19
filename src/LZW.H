/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             lzw.h                                                          */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技lzw压缩算法类的头文件                                  */
/*             HanWang Technology E-PhoneII lzw compress method class header  */
/*                                                                            */
/*  组    成                                                                  */
/*             CLZWEncode类声明                                                */
/*             CLZWDecode类声明                                               */
/*                                                                            */
/*  作    者                                                                  */
/*             郝庆丰                                                          */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-12                                                     */
/*                                                                            */
/******************************************************************************/
#ifndef  _LZW_H_ 
#define  _LZW_H_

#include "hwfcl.h"
#include <memory.h> 
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define   TAB_SIZE         2048             //编码表的大小
#define   NOT_FIND         0xFFFF
#define   NO_PREV          0xFFFF
#define   CC               256              //码表清空标记
#define   CEND             257              //编码结束标记
#define   INIT_SIZE        258    
#define   INIT_DECBUF_SIZE    512			//初始解码的缓冲大小
#define   INIT_ENCBUF_SIZE    512          //初始编码的缓冲大小
#define   INC_SIZE         64
#define   DEC_INC_SIZE     3072
#define   EMPTY            0xFFFF
#define   UEOF             0xFFFF
//以上是LZW解压缩算法中用到的常量

typedef struct
{
	char used ;
	UINT32 prev;
	BYTE c;
}ENTRY;   //LZW编码的结构


class CLZWEncode
{
public:
	CLZWEncode();
	~CLZWEncode();
	UINT32  Encode(BYTE *pSrcBuf, UINT32 nBufSize , BYTE **ppDstBuf);
    //函数功能：压缩数据。
	//参数说明：pSrcBuf为待压缩的数据缓冲， nBufSize 为缓冲大小，
	//          *ppDstBuf为压完后的数据的缓冲，由类内部分配。
	//返回值：  压完的缓冲大小
	void    ReleaseBuf(BYTE *pBuffer);
	//函数功能：释放保存压完的数据的缓冲
	//参数说明：pBuffer是缓冲指针
protected:
	UINT32  QueryTable(UINT32 code , BYTE c);
	void    InitTable();
    void    InitData();
	void    UpdateTable(UINT32 prevcode , BYTE c);
    UINT32  GetChars(UINT32 code , BYTE *buffer , UINT32 size);
	void    putcode(UINT32 code);
	UINT32  readc();
protected:
	BYTE          *m_pSrcPicdata;
    UINT32         m_nSrcPicsize;
	    
	UINT32         m_curtablepos ;
	
	int            m_inpindex;
	
    int            m_tempcode;
	UINT32         m_oupindex;
	BYTE           *m_outbuf;
    int            m_totalsize;
}; //lzw编码类

class  CLZWDecode
{
public:
	CLZWDecode();
	~CLZWDecode();
protected:
	BYTE          *m_pSrcPicdata;
    UINT32         m_nSrcPicsize;
    
	UINT32         m_curtablepos ;

	int            m_inpindex;
	
    int            m_tempcode;
	UINT32         m_oupindex;

    int            m_totalsize;
protected:
    void    InitTable();
	BOOL    InitData(BYTE **ppDstBuf);
	void    UpdateTable(UINT32 prevcode , BYTE c);
	UINT32  GetChars(UINT32 code , char *buffer , UINT32 size);
    UINT32  readc();
	UINT32  getcode();
public:
	UINT32  Decode(BYTE *pSrcBUf , BYTE **ppDstBuf);
	//函数功能：解压缩数据。
	//参数说明：pSrcBuf为lzw压缩的数据缓冲
	//          *ppDstBuf为解完后的数据的缓冲，由类内部分配。
	//返回值：  解完的缓冲大小
	UINT32  Decode(BYTE *pSrcBuf , BYTE *pDstBuf, UINT32 nBufSize);
	//函数功能：解压缩数据。
	//参数说明：pSrcBuf为lzw压缩的数据缓冲；
	//          pDstBuf为解完后的数据的缓冲，由用户分配；
    //          nBufSize为缓冲大小。
	//返回值：  解完的数据长度
	void    ReleaseBuf(BYTE *pBuffer);
	//函数功能：释放解完的数据的缓冲
	//参数说明：pBuffer是缓冲指针
};//lzw解码类

#endif