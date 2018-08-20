// SABTD_Return_Codes.h
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//
//	赛声语音引擎，返回代码
//  SoftAcoustic Return Codes
// 
//  Copyright (c) 2007
//  北京赛声科技有限公司
//  版权所有
//  网址：www.softacoustic.com
//  技术支持：support@softacoustic.com
//  
//////////////////////////////////////////////////////////////////////
#ifndef SABTD_RETURN_CODES
#define SABTD_RETURN_CODES

// 正常
#define SARTC_NORMAL 0

// 授权出错
#define SARTC_ERR_UNAUTHORIZED_LICENSE  -1000
#define SARTC_ERR_UNAUTHORIZED_MACHINE  (SARTC_ERR_UNAUTHORIZED_MACHINE-10)
#define SARTC_ERR_UNAUTHORIZED_PRODUCT  (SARTC_ERR_UNAUTHORIZED_MACHINE-20)
#define SARTC_ERR_UNAUTHORIZED_SYSTEM   (SARTC_ERR_UNAUTHORIZED_MACHINE-30)
#define SARTC_ERR_UNAUTHORIZED_SYS_VERSION  (SARTC_ERR_UNAUTHORIZED_MACHINE-40)
#define SARTC_ERR_PRODUCT_EXPIRED       (SARTC_ERR_UNAUTHORIZED_MACHINE-50)

// 函数调用出错
#define SARTC_ERR_FUNCTION      -10000
#define SARTC_ERR_UNINITIALIZED (SARTC_ERR_FUNCTION-10)
#define SARTC_ERR_PARAM_RANGE   (SARTC_ERR_FUNCTION-20)

// 内存分配出错
#define SARTC_ERR_MEMORY_ALLOC  -20000


#endif // SABTD_RETURN_CODES