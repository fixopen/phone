#ifndef __JT_ASS_WORD_H__
#define __JT_ASS_WORD_H__

#define jtAWAPI

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  tagjtAWInitRes
{
    const signed char *pszTRLib;   //  ������ֵ�
    void *pExtBuf;                 //  �û��ṩ��������ⲿ�ѿռ��׵�ַ
}jtAW_InitRes;

#define jtAW_ASSMODEL_MULTI   0    //  ��������
#define jtAW_ASSMODEL_SINGLE  1    //  ��������

// error return code
typedef enum 
{
    jtAW_ERR_NONE,                 //  �޴���
    jtAW_ERR_LICENCE,              //  ����Ȩ
    jtAW_ERR_INPUT_PARAM,          //  �����������
    jtAW_ERR_NOT_INIT,             //  ����û�г�ʼ��������û����ȷ��ʼ��
    jtAW_ERR_OPEN_DATA             //  ��Դ���ݴ���
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
* @param   unsigned long dwHandle    - [in]  ������ 
* @param   unsigned short *pWord     - [in]  Ҫ��ȡ����ʵĺ��ֻ򵥴�
* @param   int nWordLen              - [in]  Ҫ��ȡ����ʵĺ��ֻ򵥴ʵĳ��ȣ�short��
* @param   unsigned short *pAssWord  - [in/out]  ���������ÿո�������û����٣�
* @param   int *pnRelustLen          - [in/out]  ���ٵĿռ䳤�ȣ�short��/��õ�����ʸ���
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
* @param   unsigned long dwHandle    - [in]  ������ 
* @param   unsigned short *pWord     - [in]  Ҫ��ӵĵ���
* @param   int nWordLen              - [in]  Ҫ��ӵĵ��ʵĳ��ȣ�short��
* @return  jtAWErrCode 
*/
jtAWErrCode jtAWAPI jtAW_AddWord(unsigned long  dwHandle,
                                 unsigned short *pWord,
                                 int            nWordLen); 


#ifdef __cplusplus
}; // extern "C"
#endif

#endif // __JT_ASS_WORD_H__
