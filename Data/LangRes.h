#ifndef __DATA_LANGRES_H__
#define __DATA_LANGRES_H__

#include "LanguageResource.h"

namespace Data
{
    LangResource* langResource(size_t& count) {
        static LangResource result[] = {
            //LangResource(RI_MAINBUTTON, "主页"),
            //LangResource(RI_CONTACTBUTTON, "名片夹"),

            //主界面
            LangResource(RI_COMN_YEARSTC, "年"), LangResource(RI_COMN_MONTHSTC, "月"), LangResource(RI_COMN_DAYSTC, "日"), LangResource(RI_COMN_HOURSTC, "时"), LangResource(RI_COMN_MINUTSSTC, "分"), LangResource(RI_COMN_SECONDSTC, "秒"), LangResource(RI_COMN_SUNSTC, "星期日"), LangResource(RI_COMN_MONSTC, "星期一"), LangResource(RI_COMN_TUESSTC, "星期二"), LangResource(RI_COMN_WEDSTC, "星期三"), LangResource(RI_COMN_TURSSTC, "星期四"), LangResource(RI_COMN_FRISTC, "星期五"), LangResource(RI_COMN_SATSTC, "星期六"), LangResource(RI_COMN_ENTRIESSTC, "个"), LangResource(RI_COMN_MISSTELSTC, "未接电话"), LangResource(RI_COMN_RECORDETELSTC, "留言电话"), LangResource(RI_COMN_TOBOTTOM, "上一级"), LangResource(RI_COMN_FINDBTN, "查找"), LangResource(RI_COMN_SELECTBTN, "选择"), LangResource(RI_COMN_OKBTN, "确定"), LangResource(RI_COMN_CANCELBTN, "取消"),

            //主界面
            LangResource(RI_MAIN_VIDEOBTN, "桌面影院"), LangResource(RI_MAIN_POTOBTN, "电子相册"), LangResource(RI_MAIN_MP3BTN, "MP3 播放"), LangResource(RI_MAIN_CALENDARBTN, "日    历"), LangResource(RI_MAIN_CALCULATORBTN, "计 算 器"), LangResource(RI_MAIN_SCREENSAVERBTN, "屏幕保护"), LangResource(RI_MAIN_TODAYSTC, "今日"), LangResource(RI_MAIN_LUNARCALENDARSTC, "农历"), LangResource(RI_MAIN_PROTECTEDWALL_OPEN_STC, "防火墙启动"), LangResource(RI_MAIN_PROTECTEDWALL_CLOSE_STC, "防火墙关闭(接听所有来电)"), LangResource(RI_MAIN_NONOTE, "今天无记事"), LangResource(RI_MAIN_INPUTMATHBTN, "输入法"), LangResource(RI_MAIN_COMMUNICATIONLINEBTN, "通讯线路"), LangResource(RI_MAIN_MAINBTN, "桌面"), LangResource(RI_MAIN_CARDCASEBTN, "名片"), LangResource(RI_MAIN_CALLLISTBTN, "记录"), LangResource(RI_MAIN_STOCKINFOBTN, "股票"), LangResource(RI_MAIN_SYSTTEMSETTING, "设置"), LangResource(RI_MAIN_SOUND, "录音"), LangResource(RI_MAIN_STOCKINFO, "同花顺股票软件"),

            //桌面影院
            LangResource(RI_VEDIO_ALLBTN, "全选"), LangResource(RI_VEDIO_DELETEALLBTN, "清空"), LangResource(RI_VEDIO_OPENBTN, "打开"), LangResource(RI_VEDIO_ALLSCREENBTN, "全屏"), LangResource(RI_VEDIO_LEFTROTATEBTN, "左旋"), LangResource(RI_VEDIO_RIGHTROTATEBTN, "右旋"), LangResource(RI_PHOTO_SCREENSAVE, "保存至屏保"),

            //万年历        
            LangResource(RI_LANUDER_WEEK, "  日     一     二     三     四     五     六"), LangResource(RI_LANUDER_LOOP, "闰"), LangResource(RI_LANUDER_MONTH1, "正月"), LangResource(RI_LANUDER_MONTH2, "二月"), LangResource(RI_LANUDER_MONTH3, "三月"), LangResource(RI_LANUDER_MONTH4, "四月"), LangResource(RI_LANUDER_MONTH5, "五月"), LangResource(RI_LANUDER_MONTH6, "六月"), LangResource(RI_LANUDER_MONTH7, "七月"), LangResource(RI_LANUDER_MONTH8, "八月"), LangResource(RI_LANUDER_MONTH9, "九月"), LangResource(RI_LANUDER_MONTH10, "十月"), LangResource(RI_LANUDER_MONTH11, "冬月"), LangResource(RI_LANUDER_MONTH12, "腊月"), LangResource(RI_LANUDER_DAY10, "初"), LangResource(RI_LANUDER_DAY11, "十"), LangResource(RI_LANUDER_DAY12, "廿"), LangResource(RI_LANUDER_DAY13, "卅"), LangResource(RI_LANUDER_DAY0, "十"), LangResource(RI_LANUDER_DAY1, "一"), LangResource(RI_LANUDER_DAY2, "二"), LangResource(RI_LANUDER_DAY3, "三"), LangResource(RI_LANUDER_DAY4, "四"), LangResource(RI_LANUDER_DAY5, "五"), LangResource(RI_LANUDER_DAY6, "六"), LangResource(RI_LANUDER_DAY7, "七"), LangResource(RI_LANUDER_DAY8, "八"), LangResource(RI_LANUDER_DAY9, "九"),

            //名片
            LangResource(RI_CARD_TYPESTC, "类别"), LangResource(RI_CARD_NAMESTC, "姓名"), LangResource(RI_CARD_COMPANYSTC, "单位"), LangResource(RI_CARD_COMPANYADDRESSSTC, "地址"), LangResource(RI_CARD_DEPARTSTC, "部门"), LangResource(RI_CARD_TITLESTC, "职务"), LangResource(RI_CARD_OPERATORSTC, "操作"), LangResource(RI_CARD_ALLCARDSTC, "所有名片"), LangResource(RI_CARD_FINDREAULTSTC, "查找结果"), LangResource(RI_CARD_NEWCARDBTN, "新建名片"), LangResource(RI_CARD_NEWTYPEBTN, "新建类别"), LangResource(RI_CARD_EDITTYPEBTN, "编辑类别"), LangResource(RI_CARD_DELETETYPEBTN, "删除类别"), LangResource(RI_CARD_DELETECARDBTN, "删除名片"), LangResource(RI_CARD_MOBILESTC, "手机:"), LangResource(RI_CARD_TELPHONESTC, "固话:"), LangResource(RI_CARD_EMAILSTC, "邮件:"), LangResource(RI_CARD_WEBSTC, "网址:"), LangResource(RI_CARD_POSTCODESTC, "邮编:"), LangResource(RI_CARD_MOTOSTC, "备注:"), LangResource(RI_CARD_COMNSTC, "普通"), LangResource(RI_CARD_VIPSTC, "VIP"), LangResource(RI_CARD_BLACKLISTSTC, "黑名单"), LangResource(RI_CARD_CARDFINDSTC, "名片查找"), LangResource(RI_CARD_FINDCONTENTSTC, "请选择输入要查找的内容"), LangResource(RI_CARD_TELCODESTC, "电话号码"), LangResource(RI_CARD_GROUPSTC, "组群:"), LangResource(RI_CARD_NEWCARDSTC, "新建名片"), LangResource(RI_CARD_EDITCARDSTC, "编辑名片"), LangResource(RI_CARD_TYPENAME, "类别名称:"), LangResource(RI_CARD_RING, "特色振铃:"), LangResource(RI_CARD_SOUNDTIP, "留言提示:"), LangResource(RI_CARD_DEFAULT, "默认"), LangResource(RI_CARD_FIND_TITLE, "名片查找"), LangResource(RI_CARD_FIND_TIP, "请输入要查找的内容"), LangResource(RI_CARD_FIND_NAME, "姓      名:"), LangResource(RI_CARD_FIND_NUMBER, "电话号码:"), LangResource(RI_CARD_FIND_COMPANY, "公      司:"), LangResource(RI_CARD_FIND_DUTY, "职      务:"),

            //电话
            LangResource(RI_TEL_NEWTELSTC, "您有新来电"), LangResource(RI_TEL_HIDEBTN, "隐藏"), LangResource(RI_TEL_ACCEPTBTN, "接听"), LangResource(RI_TEL_DIALTELSTC, "拨打电话"), LangResource(RI_TEL_TELINGSTC, "通话中"), LangResource(RI_TEL_NOTEBTN, "记事"), LangResource(RI_TEL_RECORDEBTN, "录音"), LangResource(RI_TEL_STOPRECORDEBTN, "停止录音"), LangResource(RI_TEL_HUNGOFFBTN, "挂断"), LangResource(RI_TEL_RECORDING, "正在录音"), LangResource(RI_TEL_DURATION, "通话时长"),

            //记事
            LangResource(RI_NOTE_NEXTBTN, "下一页"), LangResource(RI_NOTE_BACKBTN, "上一页"), LangResource(RI_NOTE_TITLE, "记事"), LangResource(RI_NOTE_PAGE, "页"), LangResource(RI_NOTE_CLEAR, "清空"),

            //通话记录
            LangResource(RI_RECORD_TYPE, "类型"), LangResource(RI_RECORD_NAMENO, "姓名/号码"), LangResource(RI_RECORD_TIME, "时间"), LangResource(RI_RECORD_DURATION, "时长"), LangResource(RI_RECORD_SOUND, "录音"), LangResource(RI_RECORD_OPERATION, "操作"), LangResource(RI_RECORD_IN, "未接电话"), LangResource(RI_RECORD_INED, "已接电话"), LangResource(RI_RECORD_OUT, "已拨电话"), LangResource(RI_RECORD_SEARCHRESULT, "查找结果"), LangResource(RI_RECORD_DIAL, "拨打"), LangResource(RI_RECORD_LISTENSOUND, "播放留言"), LangResource(RI_RECORD_NOTE, "记事"), LangResource(RI_RECORD_NEWCARD, "新建名片"), LangResource(RI_RECORD_SEARCH, "查找"), LangResource(RI_RECORD_DELETE, "删除"), LangResource(RI_RECORD_FIND_TITLE, "通话记录查找"), LangResource(RI_RECORD_FIND_TIP, "请选择输入要查找的内容:"), LangResource(RI_RECORD_FIND_TIME, "时间范围:"), LangResource(RI_RECORD_FIND_TIMEFROM, "从"), LangResource(RI_RECORD_FIND_TIMETO, "到"), LangResource(RI_RECORD_FIND_NAME, "姓      名:"), LangResource(RI_RECORD_FIND_NUMBER, "电话号码:"),

            //录音
            LangResource(RI_SOUND_SOUND, "语音留言"), LangResource(RI_SOUND_RECORD, "通话录音"), LangResource(RI_SOUND_PROTECT, "保护"), LangResource(RI_SOUND_PLAY, "播放"), LangResource(RI_SOUND_FIND_TITLE, "录音查找"), LangResource(RI_SOUND_PLAY_TITLE, "播放器"), LangResource(RI_SOUND_PLAY_CLOSE, "关闭"), LangResource(RI_SOUND_PLAY_PREV, "上一条"), LangResource(RI_SOUND_PLAY_TEXT, "下一条"), LangResource(RI_SOUND_PLAY_PLAY, "播放"), LangResource(RI_SOUND_PLAY_CONTINUE, "连续播放下一条录音"),

            //设置
            LangResource(RI_SETTING_TELEPHONE, "电话通讯"), LangResource(RI_SETTING_INTERNET, "网路设置"), LangResource(RI_SETTING_SYSTEM, "系统参数"), LangResource(RI_SETTING_INFORMATION, "系统信息"), LangResource(RI_SETTING_DEFAULT, "恢复默认"), LangResource(RI_SETTING_DETAILTITLE, "设置项目"), LangResource(RI_SETTING_AREACODE, "本地区号:"), LangResource(RI_SETTING_OUTLINE, "启用外线加拨"), LangResource(RI_SETTING_IPDIAL, "启用IP拨号"), LangResource(RI_SETTING_TELVOLUME, "话机音量"), LangResource(RI_SETTING_SYSTEMTIME, "设置系统时间:"), LangResource(RI_SETTING_ENABLERING, "启用特色振铃功能"), LangResource(RI_SETTING_ENABLERINGSTC, "默认标准振铃"), LangResource(RI_SETTING_ENABLEAUTOLEAVEWORD, "启动自动留言功能"), LangResource(RI_SETTING_RINGTIMES, "启动振铃次数"), LangResource(RI_SETTING_LEAVEWORD, "自动留言提示"), LangResource(RI_SETTING_RECORDLEAVEWORD, "录制留言"), LangResource(RI_SETTING_DEFINESPEEDBUTTON, "设置速拨"), LangResource(RI_SETTING_DEFINESPEEDBUTTONSTC, "可以设置电话的12个速拨按键"), LangResource(RI_SETTING_CONNECTAUTO, "直接连接网络"), LangResource(RI_SETTING_CONNECTMANUAL, "ADSL拨号连接网络"), LangResource(RI_SETTING_ADSLNAME, "用户名:"), LangResource(RI_SETTING_ADSLPASSWORD, "密码:"), LangResource(RI_SETTING_USEDYNAMICIP, "自动获得IP地址"), LangResource(RI_SETTING_USESTATICIIP, "使用下面的IP地址:"), LangResource(RI_SETTING_IP, "IP地址:"), LangResource(RI_SETTING_MASK, "子网掩码:"), LangResource(RI_SETTING_GATEWAY, "网关:"), LangResource(RI_SETTING_DNS, "DNS服务器:"), LangResource(RI_SETTING_ENABLESP, "启用屏幕保护"), LangResource(RI_SETTING_SPTYPE, "屏保类型:"), LangResource(RI_SETTING_TIMESP, "时间屏保"), LangResource(RI_SETTING_IMAGESP, "电子相册屏保"), LangResource(RI_SETTING_WAITTIME, "等待时间:"), LangResource(RI_SETTING_ENABLEPASSWORD, "在恢复时使用密码"), LangResource(RI_SETTING_SETPASSWORD, "设置密码"), LangResource(RI_SETTING_SOUNDSAVEPATH, "录音存储位置:"), LangResource(RI_SETTING_LOCAL, "本地"), LangResource(RI_SETTING_SDCARD, "SD卡"), LangResource(RI_SETTING_SCREENADJUSTSTC, "可以重新调整屏幕点击精度"), LangResource(RI_SETTING_SCREENADJUST, "校正屏幕"), LangResource(RI_SETTING_SYSTOMTITLE, "多媒体会议电话系统"), LangResource(RI_SETTING_SOFTVERSION, "软件版本:"), LangResource(RI_SETTING_HARDVERSION, "硬件版本:"), LangResource(RI_SETTING_FLASHSIZE, "本地空间:"), LangResource(RI_SETTING_SDSTORAGESIZE, "SD卡空间:"), LangResource(RI_SETTING_AVAILABLESIZE, "可以使用"), LangResource(RI_SETTING_CONTACTSIZE, "电话名片:"), LangResource(RI_SETTING_CONTACTINFOSIZE, "通话记录:"), LangResource(RI_SETTING_SOUNDSIZE, "录音管理:"), LangResource(RI_SETTING_INFORMATIONSIZE, "信息点播:"), LangResource(RI_SETTING_VERSIONRIGHT, "版权所有:"), LangResource(RI_SETTING_CLEARCONTACT, "清空名片"), LangResource(RI_SETTING_CLEARCONTACTINFO, "清空记录"), LangResource(RI_SETTING_CLEARSOUND, "清空录音"), LangResource(RI_SETTING_CLEARINFORMATION, "清空便笺"), LangResource(RI_FASTDIALS_TITLE, "配置自定义速拨按键"), LangResource(RI_FASTDIALS_NUMBER01, "速拨01:"), LangResource(RI_FASTDIALS_NUMBER02, "速拨02:"), LangResource(RI_FASTDIALS_NUMBER03, "速拨03:"), LangResource(RI_FASTDIALS_NUMBER04, "速拨04:"), LangResource(RI_FASTDIALS_NUMBER05, "速拨05:"), LangResource(RI_FASTDIALS_NUMBER06, "速拨06:"), LangResource(RI_FASTDIALS_NUMBER07, "速拨07:"), LangResource(RI_FASTDIALS_NUMBER08, "速拨08:"), LangResource(RI_FASTDIALS_NUMBER09, "速拨09:"), LangResource(RI_FASTDIALS_NUMBER10, "速拨10:"), LangResource(RI_FASTDIALS_NUMBER11, "速拨11:"), LangResource(RI_FASTDIALS_NUMBER12, "速拨12:"), LangResource(RI_SETTING_TITILEBACKUP, "数据库(名片, 通话记录, 记事) 操作"), LangResource(RI_SETTING_TITLERESTORE, "数据库(名片, 通话记录, 记事) 恢复"), LangResource(RI_SETTING_TITLE, "录音文件导出"),//"文件操作"),
            LangResource(RI_SETTING_BACKUP, "备份"), LangResource(RI_SETTING_RESTORE, "恢复"), LangResource(RI_SETTING_IN, "导入"), LangResource(RI_SETTING_OUT, "导出"), LangResource(RI_SETTING_RING, "振铃音乐"), LangResource(RI_SETTING_PHOTO, "电子相册"), LangResource(RI_SETTING_MP3, "MP3音乐"), LangResource(RI_SETTING_VIDEO, "视频文件"), LangResource(RI_SETTING_RECORDE, "录音文件"), LangResource(RI_SETTING_LOCALFILE, "本地文件"), LangResource(RI_SETTING_USBFILE, "U盘文件"), LangResource(RI_SETTING_COPYFILE, "文件备份与恢复"),
            /*
            LangResource(RI_SETTING_COPYFILE_ERROR1,        "移动存储设备不存在，请重试!"),
            LangResource(RI_SETTING_COPYFILE_ERROR2,        "此操作会覆盖已有的文件!"),
            LangResource(RI_SETTING_COPYFILE_ERROR3,        "文件不存在!"),
            LangResource(RI_SETTING_COPYFILE_FINISH,        "操作已完成!"),
            LangResource(RI_SETTING_COPYFILE_DOWITH,        "正在拷贝文件,不要断电, 请稍后..."),
            */
            LangResource(RI_SETTING_COPYFILE_ERROR1, ".\\adv\\mjpg\\k1\\common\\移动存储不存在.bmp"), LangResource(RI_SETTING_COPYFILE_ERROR2, ".\\adv\\mjpg\\k1\\common\\覆盖已有文件.bmp"), LangResource(RI_SETTING_COPYFILE_ERROR3, ".\\adv\\mjpg\\k1\\common\\文件不存在.bmp"), LangResource(RI_SETTING_COPYFILE_FINISH, ".\\adv\\mjpg\\k1\\common\\操作完成.bmp"), LangResource(RI_SETTING_COPYFILE_DOWITH, ".\\adv\\mjpg\\k1\\common\\正在拷贝.bmp"),

            //防火墙
            LangResource(RI_FIREWALL_TITLE, "防火墙"), LangResource(RI_FIREWALL_ENABLE, "开启防火墙"), LangResource(RI_FIREWALL_BLACKLIST, "拒绝黑名单电话来电"), LangResource(RI_FIREWALL_CONTACT, "只接听名片来电"),   //除黑名单以外的
            LangResource(RI_FIREWALL_VIP, "只接听VIP电话来电"), LangResource(RI_FIREWALL_ALL, "拒绝所有来电,限定时间"), LangResource(RI_FIREWALL_PART1, "限定时间"), LangResource(RI_FIREWALL_PART2, "小时"),

            //密码保护
            //  LangResource(RI_PASSWORD_TITLE,                  "录音保护设置"),
            LangResource(RI_PASSWORD_TITLE, "设置密码"), LangResource(RI_PASSWORD_TITLE1, "验证密码"), LangResource(RI_PASSWORD_TIP, "请输入密码:"), LangResource(RI_PASSWORD_TIP1, "密码验证错误, 请重新输入旧密码:"), LangResource(RI_PASSWORD_TIP2, "新密码输入不相符, 请重新输入新密码:"),
            //      LangResource(RI_PASSWORD_TIP3,                   "密码错误, 请重新输入:"),
            LangResource(RI_PASSWORD_OLDPASSWORD, "旧密码:"), LangResource(RI_PASSWORD_NEWPASSWORD1, "新密码:"), LangResource(RI_PASSWORD_NEWPASSWORD2, "确认新密码:"), LangResource(RI_DELETETIP_TITLE, "删除确认"), LangResource(RI_DELETETIP_PASSWORD, "保护密码:"), LangResource(RI_DELETETIP_PASSWORD, "保护密码:"),
            /*
            LangResource(RI_DELETETIP_TIP,                   "确定要删除该录音记录吗？"),
            LangResource(RI_DELETETIP_CONTACT,               "确定要删除该名片吗？"),
            LangResource(RI_DELETETIP_CONTACTINFO,           "确定要删除该通话记录吗？"),
            LangResource(RI_DELETETIP_CONTACTGROUP,          "确定要删除该名片类别吗？(该类别名片一同被删除)"),
            LangResource(RI_DELETETIP_CONTACT2,              "确定要永久清空名片吗？"),
            LangResource(RI_DELETETIP_CONTACTINFO2,          "确定要永久清空通话记录吗？"),
            LangResource(RI_DELETETIP_SOUND2,                "确定要永久清空录音记录吗？"),
            */
            LangResource(RI_DELETETIP_TIP, ".\\adv\\mjpg\\k1\\common\\删除录音记录.bmp"), LangResource(RI_DELETETIP_CONTACT, ".\\adv\\mjpg\\k1\\common\\删除名片.bmp"), LangResource(RI_DELETETIP_CONTACTINFO, ".\\adv\\mjpg\\k1\\common\\删除通话记录.bmp"), LangResource(RI_DELETETIP_CONTACTGROUP, ".\\adv\\mjpg\\k1\\common\\删除名片类别.bmp"), LangResource(RI_DELETETIP_CONTACT2, ".\\adv\\mjpg\\k1\\common\\清空名片.bmp"), LangResource(RI_DELETETIP_CONTACTINFO2, ".\\adv\\mjpg\\k1\\common\\清空通话记录.bmp"), LangResource(RI_DELETETIP_SOUND2, ".\\adv\\mjpg\\k1\\common\\清空录音记录.bmp"), LangResource(RI_PASSWORD_TIP3, ".\\adv\\mjpg\\k1\\common\\password_checkerror.bmp"),

            //屏幕保护
            LangResource(RI_SCREENSAVE_PASSWORDINPUT, "请输入密码:"), LangResource(RI_SYSTEM_DISK, "磁盘空间不足,请整理磁盘!"),

            //股票
            LangResource(RI_STOCKS_NETERROR, "网络错误,请检测!"), LangResource(RI_STOCKS_NETCONNECT, "网络正在连接......"), LangResource(RI_SOUND_RECORD_STOPPLAY, "停止播放"), LangResource(RI_SOUND_LOCALRECORD, "本地录音"), LangResource(RI_SOUND_LEAVEWORDTIP, "提示语"), LangResource(RI_SOUND_PLAY_PAUSE, "暂停"),
        };
        count = sizeof result / sizeof result[0];
        return result;
    }
}

#endif