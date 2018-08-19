// SABTD_Param_Type.h
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
//	赛声忙音检测器，参数定义
//  SoftAcoustic Parameter Type
// 
//  Copyright (c) 2007
//  北京赛声科技有限公司
//  版权所有
//  网址：www.softacoustic.com
//  技术支持：support@softacoustic.com
//  
//////////////////////////////////////////////////////////////////////////
#ifndef SABTD_PARAM_TYPE
#define SABTD_PARAM_TYPE

//////////////////////////////////////////////////////////////////////////
// 注意：一旦设定某项参数，系统将以设定的参数为准，仅在小范围内浮动，
//      不再进行自适应调整!!
//////////////////////////////////////////////////////////////////////////

enum ParamType {

    ///////////////////////////// 频率设定 ////////////////////////////////
    ////////////// 请注意：过宽的频率范围会降低系统性能!/////////////////////
    // 说明：最大频率
    // 范围：大于SABTD_PARAMTYPE_FREQ_MIN，小于700。(缺省值：550)
    SABTD_PARAMTYPE_FREQ_MAX = 0,
    // 说明：忙音点频率
    // 范围：在MAX与MIN之间，标准为450。(缺省值：450，自适应)
    SABTD_PARAMTYPE_FREQ,
    // 说明：最小频率(小于Max，大于300)
    // 范围：小于SABTD_PARAMTYPE_FREQ_MAX，大于300。(缺省值：350)
    SABTD_PARAMTYPE_FREQ_MIN,


    //////////////////////////// 占空比 /////////////////////////////////
    // 说明：忙音持续时间(s)
    // 范围：0.2-1.0。(缺省值：自适应)
    SABTD_PARAMTYPE_BUSYTONE_LENGTH,
    // 说明：静音持续时间(s)
    // 范围：0.2-1.0。(缺省值：自适应)
    SABTD_PARAMTYPE_SILENCETONE_LENGTH,


    ///////////////////////// 能量与静音 /////////////////////////////////
    // 能量值(敏感度，0.1为最敏感，1.0为最迟钝。过低的设定会降低系统抗噪能力)
    // 范围：0.1-1.0。(缺省值：0.5)
    SABTD_PARAMTYPE_ENERGY,
    // 说明：多少秒检测不到声音即判断为静音
    // 范围：3.0-20.0。(缺省值：10.0)
    SABTD_PARAMTYPE_SILENCE_LENGTH,
    // 前若干秒的数据不进行忙音判断
    // 范围：0.0-10.0。(缺省值：4.0)
    SABTD_PARAMTYPE_FRONT_NOTTONE,


    ///////////////////////// 忙音检测个数 ///////////////////////////////
    // 说明：检测多少个忙音后判断挂机(过短会导致不稳定，降低系统抗噪能力)
    // 范围：3-15。(缺省值：5)
    SABTD_PARAMTYPE_BUSYTONE_NUM,


    ////////////////////////////// 其它 //////////////////////////////////
    // 参数个数
    SABTD_PARAMTYPE_NUMBER

};

#endif // SABTD_PARAM_TYPE