#ifndef __DATA_LANGUAGERESOURCE_H__
#define __DATA_LANGUAGERESOURCE_H__

#include "Data.h"

#include "../Util/SmartPtr.h"

#pragma warning(disable: 4786)
#include <string>

namespace Data
{
    enum Language {
        lEnglish,
        lChinese,
    };

    enum ResourceId {
        RI_MAINBUTTON,
        RI_CONTACTBUTTON,

                //comn
        RI_COMN_YEARSTC,
        RI_COMN_MONTHSTC,
        RI_COMN_DAYSTC,
        RI_COMN_HOURSTC,
        RI_COMN_MINUTSSTC,
        RI_COMN_SECONDSTC,
        RI_COMN_SUNSTC,
        RI_COMN_MONSTC,
        RI_COMN_TUESSTC,
        RI_COMN_WEDSTC,
        RI_COMN_TURSSTC,
        RI_COMN_FRISTC,
        RI_COMN_SATSTC,
        RI_COMN_ENTRIESSTC,
        RI_COMN_MISSTELSTC,
        RI_COMN_RECORDETELSTC,
        RI_COMN_TOBOTTOM,
        RI_COMN_FINDBTN,
        RI_COMN_SELECTBTN,
        RI_COMN_OKBTN,
        RI_COMN_CANCELBTN,              

                //主界面                         
        RI_MAIN_VIDEOBTN,
        RI_MAIN_POTOBTN,
        RI_MAIN_MP3BTN,
        RI_MAIN_CALENDARBTN,
        RI_MAIN_CALCULATORBTN,
        RI_MAIN_SCREENSAVERBTN,
        RI_MAIN_TODAYSTC,
        RI_MAIN_LUNARCALENDARSTC,
        RI_MAIN_PROTECTEDWALL_OPEN_STC,
        RI_MAIN_PROTECTEDWALL_CLOSE_STC,
        RI_MAIN_NONOTE,
        RI_MAIN_INPUTMATHBTN,
        RI_MAIN_COMMUNICATIONLINEBTN,
        RI_MAIN_MAINBTN,
        RI_MAIN_CARDCASEBTN,
        RI_MAIN_CALLLISTBTN,
        RI_MAIN_STOCKINFOBTN,
        RI_MAIN_SYSTTEMSETTING,
        RI_MAIN_STOCKINFO,

                //mhq add
        RI_MAIN_SOUND,

                //桌面影院
        RI_VEDIO_ALLBTN,
        RI_VEDIO_DELETEALLBTN,
        RI_VEDIO_OPENBTN,
        RI_VEDIO_ALLSCREENBTN,
        RI_VEDIO_LEFTROTATEBTN,
        RI_VEDIO_RIGHTROTATEBTN,
        RI_PHOTO_SCREENSAVE,

                //万年历
        RI_LANUDER_WEEK,
        RI_LANUDER_LOOP,
        RI_LANUDER_MONTH1,
        RI_LANUDER_MONTH2,
        RI_LANUDER_MONTH3,
        RI_LANUDER_MONTH4,
        RI_LANUDER_MONTH5,
        RI_LANUDER_MONTH6,
        RI_LANUDER_MONTH7,
        RI_LANUDER_MONTH8,
        RI_LANUDER_MONTH9,
        RI_LANUDER_MONTH10,
        RI_LANUDER_MONTH11,
        RI_LANUDER_MONTH12,
        RI_LANUDER_DAY10,
        RI_LANUDER_DAY11,
        RI_LANUDER_DAY12,
        RI_LANUDER_DAY13,
        RI_LANUDER_DAY0,
        RI_LANUDER_DAY1,
        RI_LANUDER_DAY2,
        RI_LANUDER_DAY3,
        RI_LANUDER_DAY4,
        RI_LANUDER_DAY5,
        RI_LANUDER_DAY6,
        RI_LANUDER_DAY7,
        RI_LANUDER_DAY8,
        RI_LANUDER_DAY9,

                //名片                           
        RI_CARD_TYPESTC,
        RI_CARD_NAMESTC,
        RI_CARD_COMPANYSTC,
        RI_CARD_COMPANYADDRESSSTC,
        RI_CARD_DEPARTSTC,
        RI_CARD_TITLESTC,
        RI_CARD_OPERATORSTC,
        RI_CARD_ALLCARDSTC,
        RI_CARD_FINDREAULTSTC,
        RI_CARD_NEWCARDBTN,
        RI_CARD_NEWTYPEBTN,
        RI_CARD_EDITTYPEBTN,
        RI_CARD_DELETETYPEBTN,
        RI_CARD_DELETECARDBTN,
        RI_CARD_MOBILESTC,
        RI_CARD_TELPHONESTC,
        RI_CARD_EMAILSTC,
        RI_CARD_WEBSTC,
        RI_CARD_POSTCODESTC,
        RI_CARD_MOTOSTC,
        RI_CARD_COMNSTC,
        RI_CARD_VIPSTC,
        RI_CARD_BLACKLISTSTC,
        RI_CARD_CARDFINDSTC,
        RI_CARD_FINDCONTENTSTC,
        RI_CARD_TELCODESTC,
        RI_CARD_GROUPSTC,
        RI_CARD_NEWCARDSTC,
        RI_CARD_EDITCARDSTC,
        RI_CARD_TYPENAME,
        RI_CARD_RING,
        RI_CARD_SOUNDTIP,
        RI_CARD_DEFAULT,
        RI_CARD_FIND_TITLE,
        RI_CARD_FIND_TIP,
        RI_CARD_FIND_NAME,
        RI_CARD_FIND_NUMBER,
        RI_CARD_FIND_COMPANY,
        RI_CARD_FIND_DUTY,


                //电话                           
        RI_TEL_NEWTELSTC,
        RI_TEL_HIDEBTN,
        RI_TEL_ACCEPTBTN,
        RI_TEL_DIALTELSTC,
        RI_TEL_TELINGSTC,
        RI_TEL_NOTEBTN,
        RI_TEL_RECORDEBTN,
        RI_TEL_STOPRECORDEBTN,
        RI_TEL_HUNGOFFBTN,
        RI_TEL_RECORDING,
        RI_TEL_DURATION,

                //记事                           
        RI_NOTE_TITLE,
        RI_NOTE_PAGE,
        RI_NOTE_NEXTBTN,
        RI_NOTE_BACKBTN,
        RI_NOTE_CLEAR,

                //通话记录
        RI_RECORD_TYPE,
        RI_RECORD_NAMENO,
        RI_RECORD_TIME,
        RI_RECORD_DURATION,
        RI_RECORD_SOUND,
        RI_RECORD_OPERATION,
        RI_RECORD_IN,
        RI_RECORD_INED,
        RI_RECORD_OUT,
        RI_RECORD_SEARCHRESULT,
        RI_RECORD_DIAL,
        RI_RECORD_LISTENSOUND,
        RI_RECORD_NOTE,
        RI_RECORD_NEWCARD,
        RI_RECORD_SEARCH,
        RI_RECORD_DELETE,
        RI_RECORD_FIND_TITLE,
        RI_RECORD_FIND_TIP,
        RI_RECORD_FIND_TIME,
        RI_RECORD_FIND_TIMEFROM,
        RI_RECORD_FIND_TIMETO,
        RI_RECORD_FIND_NAME,
        RI_RECORD_FIND_NUMBER,

                //录音
        RI_SOUND_SOUND,
        RI_SOUND_RECORD,
        RI_SOUND_PROTECT,
        RI_SOUND_PLAY,
        RI_SOUND_FIND_TITLE,

                //录音播放
        RI_SOUND_PLAY_TITLE,
        RI_SOUND_PLAY_CLOSE,
        RI_SOUND_PLAY_PREV,
        RI_SOUND_PLAY_TEXT,
        RI_SOUND_PLAY_PLAY,
        RI_SOUND_PLAY_CONTINUE,

                //设置
        RI_SETTING_DEFAULT,
        RI_SETTING_DETAILTITLE,
        RI_SETTING_TELEPHONE,
        RI_SETTING_INTERNET,
        RI_SETTING_SYSTEM,
        RI_SETTING_INFORMATION,
        RI_SETTING_AREACODE,
        RI_SETTING_OUTLINE,
        RI_SETTING_IPDIAL,
        RI_SETTING_TELVOLUME,
        RI_SETTING_SYSTEMTIME,
        RI_SETTING_ENABLERING,
        RI_SETTING_ENABLERINGSTC,
        RI_SETTING_ENABLEAUTOLEAVEWORD,
        RI_SETTING_RINGTIMES,
        RI_SETTING_LEAVEWORD,
        RI_SETTING_RECORDLEAVEWORD,
        RI_SETTING_DEFINESPEEDBUTTON,
        RI_SETTING_DEFINESPEEDBUTTONSTC,
        RI_SETTING_CONNECTAUTO,
        RI_SETTING_CONNECTMANUAL,
        RI_SETTING_ADSLNAME,
        RI_SETTING_ADSLPASSWORD,
        RI_SETTING_USEDYNAMICIP,
        RI_SETTING_USESTATICIIP,
        RI_SETTING_IP,
        RI_SETTING_MASK,
        RI_SETTING_GATEWAY,
        RI_SETTING_DNS,
        RI_SETTING_ENABLESP,
        RI_SETTING_SPTYPE,
        RI_SETTING_TIMESP,
        RI_SETTING_IMAGESP,
        RI_SETTING_WAITTIME,
        RI_SETTING_ENABLEPASSWORD,
        RI_SETTING_SETPASSWORD,
        RI_SETTING_SOUNDSAVEPATH,
        RI_SETTING_LOCAL,
        RI_SETTING_SDCARD,
        RI_SETTING_SCREENADJUSTSTC,
        RI_SETTING_SCREENADJUST,
        RI_SETTING_SYSTOMTITLE,
        RI_SETTING_SOFTVERSION,
        RI_SETTING_HARDVERSION,
        RI_SETTING_FLASHSIZE,
        RI_SETTING_SDSTORAGESIZE,
        RI_SETTING_CONTACTSIZE,
        RI_SETTING_CONTACTINFOSIZE,
        RI_SETTING_AVAILABLESIZE,
        RI_SETTING_SOUNDSIZE,
        RI_SETTING_INFORMATIONSIZE,
        RI_SETTING_VERSIONRIGHT,
        RI_SETTING_CLEARCONTACT,
        RI_SETTING_CLEARCONTACTINFO,
        RI_SETTING_CLEARSOUND,
        RI_SETTING_CLEARINFORMATION,
        RI_SETTING_TITILEBACKUP,
        RI_SETTING_TITLERESTORE,
        RI_SETTING_TITLE,
        RI_SETTING_BACKUP,
        RI_SETTING_RESTORE,
        RI_SETTING_IN,
        RI_SETTING_OUT,
        RI_SETTING_RING,
        RI_SETTING_PHOTO,
        RI_SETTING_MP3,
        RI_SETTING_VIDEO,
        RI_SETTING_RECORDE,
        RI_SETTING_LOCALFILE,
        RI_SETTING_USBFILE,
        RI_SETTING_COPYFILE,
        RI_SETTING_COPYFILE_ERROR1,
        RI_SETTING_COPYFILE_ERROR2,
        RI_SETTING_COPYFILE_ERROR3,
        RI_SETTING_COPYFILE_FINISH,
        RI_SETTING_COPYFILE_DOWITH,

                //速拨
        RI_FASTDIALS_TITLE,
        RI_FASTDIALS_NUMBER01,
        RI_FASTDIALS_NUMBER02,
        RI_FASTDIALS_NUMBER03,
        RI_FASTDIALS_NUMBER04,
        RI_FASTDIALS_NUMBER05,
        RI_FASTDIALS_NUMBER06,
        RI_FASTDIALS_NUMBER07,
        RI_FASTDIALS_NUMBER08,
        RI_FASTDIALS_NUMBER09,
        RI_FASTDIALS_NUMBER10,
        RI_FASTDIALS_NUMBER11,
        RI_FASTDIALS_NUMBER12,

                //防火墙
        RI_FIREWALL_TITLE,
        RI_PASSWORD_TITLE1,
        RI_FIREWALL_ENABLE,
        RI_FIREWALL_BLACKLIST,
        RI_FIREWALL_CONTACT,
        RI_FIREWALL_VIP,
        RI_FIREWALL_ALL,
        RI_FIREWALL_PART1,
        RI_FIREWALL_PART2,

                //密码保护
        RI_PASSWORD_TITLE,
        RI_PASSWORD_TIP,
        RI_PASSWORD_TIP1,
        RI_PASSWORD_TIP2,
        RI_PASSWORD_TIP3,
        RI_PASSWORD_OLDPASSWORD,
        RI_PASSWORD_NEWPASSWORD1,
        RI_PASSWORD_NEWPASSWORD2,
        RI_DELETETIP_TITLE,
        RI_DELETETIP_TIP,
        RI_DELETETIP_PASSWORD,
        RI_DELETETIP_CONTACT,
        RI_DELETETIP_CONTACTINFO,
        RI_DELETETIP_SOUND,
        RI_DELETETIP_CONTACTGROUP,
        RI_DELETETIP_CONTACT2,
        RI_DELETETIP_CONTACTINFO2,
        RI_DELETETIP_SOUND2,
                //屏保
        RI_SCREENSAVE_PASSWORDINPUT,
        RI_SYSTEM_DISK,

                //股票
        RI_STOCKS_NETERROR,
        RI_STOCKS_NETCONNECT,
        RI_SOUND_RECORD_STOPPLAY,
        RI_SOUND_LOCALRECORD,
        RI_SOUND_PLAY_PAUSE,
        RI_SOUND_LEAVEWORDTIP
    };

    struct LangResource {
        LangResource(ResourceId rid, std::string v)
        : id(rid)
        , value(v) {
        }

        ResourceId id;
        std::string value;
    };

    //extern LangResource langResource[];

    class LanguageResource : public DataAccess<LanguageResource> {
    public:
        std::string const value(void) const {
            return value_;
        }
        void value(std::string const v) {
            value_ = v;
        }
        static Language const language() {
            return LanguageResource::language_;
        }
        static void language(Language const language) {
            LanguageResource::language_ = language;
        }
    private:
        //int id_;
        std::string value_;
        /*
        //meta info
        Util::Timestamp creationTime_;
        Util::Timestamp lastReadTime_;
        Util::Timestamp lastWriteTime_;
        std::vector<Util::Timestamp> accessTimes_;
        Util::Timestamp destroyTime_;
        */
    private:
        static Language language_;
    public:
        static void InsertToDatabase();
        LanguageResource(); //create
        ~LanguageResource(); //destroy
        static std::vector<boost::shared_ptr<LanguageResource> > GetFromDatabase(std::string filter, Direction const dir = dNull, int const id = 0, int const pageSize = 0); //get from database
        static std::string Get(int const id); //get from database
        void Update() const; //sync to database
        void Insert() const; //insert new instance to database
        static void Remove(std::string filter); //delete form database
    private:
        static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<LanguageResource> item);
    };
}

#endif //__DATA_LANGUAGERESOURCE_H__