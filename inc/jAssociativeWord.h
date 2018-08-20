#ifndef __JT_ASS_WORD_H__
#define __JT_ASS_WORD_H__

#define jtAWAPI

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  tagjtAWInitRes
{
    const signed char *pszTRLib;   //  联想词字典
    void *pExtBuf;                 //  用户提供给引擎的外部堆空间首地址
}jtAW_InitRes;

#define jtAW_ASSMODEL_MULTI   0    //  多字联想
#define jtAW_ASSMODEL_SINGLE  1    //  单字联想

// error return code
typedef enum 
{
    jtAW_ERR_NONE,                 //  无错误
    jtAW_ERR_LICENCE,              //  无授权
    jtAW_ERR_INPUT_PARAM,          //  传入参数错误
    jtAW_ERR_NOT_INIT,             //  引擎没有初始化，或者没有正确初始化
    jtAW_ERR_OPEN_DATA             //  资源数据错误
} jtAWErrCode;

// indexs of parameter type
typedef enum 
{
    jtAW_PARAM_ASSMODEL
}jtAWParam;

/** 
* @brief   jtAW_GetVersion
* 
* Get Version of Engine of MultiFunction HZRecognition 
* @param   unsigned char*    pbyMajor    - [out] major version number
* @param   unsigned char*    pbyMajor    - [out] minor version number
* @param   unsigned char*    pbyMajor    - [out] revision number
* @return  jtAWErrCode 
*/
jtAWErrCode jtAWAPI jtAW_GetVersion(unsigned char *pbyMajor,        
                                    unsigned char *pbyMinor,        
                                    unsigned long *piRevision);

/** 
* @brief   jtAW_GetExtBufSize
* 
* Get extend buffer size
* @param   jtAW_InitRes *pInitRes   - [in]  Init resources struct
* @param   long *pSize              - [out] size of extend buffer size
* @return  jtAWErrCode 
*/
jtAWErrCode jtAWAPI jtAW_GetExtBufSize(jtAW_InitRes *pInitRes, long *pSize);

/** 
* @brief   jtAW_Init
* 
* init
* @param   jtAW_InitRes *pInitRes   - [in]  Init resources struct
* @param   unsigned long dwHandle   - [out] handle to TTS engine 
* @return  jtAWErrCode 
*/
jtAWErrCode jtAWAPI jtAW_Init(jtAW_InitRes *pInitRes, unsigned long *pdwHandle);

/** 
* @brief   jtAW_Uninit
* 
* init
* @param   unsigned long dwHandle   - [in] handle to engine 
* @return  jtAWErrCode 
*/
jtAWErrCode jtAWAPI jtAW_Uninit(unsigned long  dwHandle);

/** 
* @brief   jtAW_SetParam
* 
* init
* @param   unsigned long dwHandle   - [in] handle to engine
* @param   jtAWParam     nParam     - [in] parameter index
* @param   long          iValue     - [in] parameter value
* @return  jtAWErrCode 
*/
jtAWErrCode jtAWAPI jtAW_SetParam(unsigned long dwHandle,
                                  jtAWParam     nParam,
                                  long          iValue);

/** 
* @brief   jtAW_GetParam
* 
* init
* @param   unsigned long dwHandle   - [in] handle to engine
* @param   jtAWParam     nParam     - [in] parameter index
* @param   long          *piValue   - [out] parameter value
* @return  jtAWErrCode 
*/
jtAWErrCode jtAWAPI jtAW_GetParam(unsigned long dwHandle,
                                  jtAWParam     nParam,
                                  long          *piValue);

/** 
* @brief   jtAW_GetAssWord
* 
* Get characters Associative-Words
* @param   unsigned long dwHandle    - [in]  引擎句柄 
* @param   unsigned short *pWord     - [in]  要获取联想词的汉字或单词
* @param   int nWordLen              - [in]  要获取联想词的汉字或单词的长度（short）
* @param   unsigned short *pAssWord  - [in/out]  联想结果，用空格隔开（用户开辟）
* @param   int *pnRelustLen          - [in/out]  开辟的空间长度（short）/获得的联想词个数
* @return  jtAWErrCode   
*/
jtAWErrCode jtAWAPI jtAW_GetAssWord(unsigned long  dwHandle,
                                    unsigned short *pWord,
                                    int            nWordLen,
                                    unsigned short *pAssWord,
                                    int            *pnRelustLen);

/** 
* @brief   jtAW_AddWord
* 
* Add words to associative-word data package
* @param   unsigned long dwHandle    - [in]  引擎句柄 
* @param   unsigned short *pWord     - [in]  要添加的单词
* @param   int nWordLen              - [in]  要添加的单词的长度（short）
* @return  jtAWErrCode 
*/
jtAWErrCode jtAWAPI jtAW_AddWord(unsigned long  dwHandle,
                                 unsigned short *pWord,
                                 int            nWordLen); 


#ifdef __cplusplus
}; // extern "C"
#endif

#endif // __JT_ASS_WORD_H__
