#include "LanguageResource.h"
#include "StringOp.h"

namespace Util {
    std::vector<LangResource> langResource() {
	    static const LangResource data[] = {
		    //LangResource(RI_MAINBUTTON, "��ҳ"),
		    //LangResource(RI_CONTACTBUTTON, "��Ƭ��"),

		    //������
		    LangResource(RI_COMN_YEARSTC,					"��"),
		    LangResource(RI_COMN_MONTHSTC,					"��"),
		    LangResource(RI_COMN_DAYSTC,					"��"),
		    LangResource(RI_COMN_HOURSTC,					"ʱ"),
		    LangResource(RI_COMN_MINUTSSTC,					"��"),
		    LangResource(RI_COMN_SECONDSTC,					"��"),
		    LangResource(RI_COMN_SUNSTC,					"������"),
		    LangResource(RI_COMN_MONSTC,					"����һ"),
		    LangResource(RI_COMN_TUESSTC,					"���ڶ�"),
		    LangResource(RI_COMN_WEDSTC,					"������"),
		    LangResource(RI_COMN_TURSSTC,					"������"),
		    LangResource(RI_COMN_FRISTC,					"������"),
		    LangResource(RI_COMN_SATSTC,					"������"),
		    LangResource(RI_COMN_ENTRIESSTC,				"��"),
		    LangResource(RI_COMN_MISSTELSTC,				"δ�ӵ绰"),
		    LangResource(RI_COMN_RECORDETELSTC,				"���Ե绰"),
		    LangResource(RI_COMN_TOBOTTOM,					"��һ��"),

		    LangResource(RI_COMN_FINDBTN,					"����"),
		    LangResource(RI_COMN_SELECTBTN,					"ѡ��"),
		    LangResource(RI_COMN_OKBTN,						"ȷ��"),
		    LangResource(RI_COMN_CANCELBTN,					"ȡ��"),

		    //������
		    LangResource(RI_MAIN_VIDEOBTN,					"����ӰԺ"),
		    LangResource(RI_MAIN_POTOBTN,					"�������"),
		    LangResource(RI_MAIN_MP3BTN,					"MP3 ����"),
		    LangResource(RI_MAIN_CALENDARBTN,				"��    ��"),
		    LangResource(RI_MAIN_CALCULATORBTN,				"�� �� ��"),
		    LangResource(RI_MAIN_SCREENSAVERBTN,			"��Ļ����"),
		    LangResource(RI_MAIN_TODAYSTC,					"����"),
		    LangResource(RI_MAIN_LUNARCALENDARSTC,			"ũ��"),
		    LangResource(RI_MAIN_PROTECTEDWALL_OPEN_STC,	"����ǽ����"),
		    LangResource(RI_MAIN_PROTECTEDWALL_CLOSE_STC,	"����ǽ�ر�(������������)"),
		    LangResource(RI_MAIN_NONOTE,					"�����޼���"),

		    LangResource(RI_MAIN_INPUTMATHBTN,				"���뷨"),
		    LangResource(RI_MAIN_COMMUNICATIONLINEBTN,		"ͨѶ��·"),
		    LangResource(RI_MAIN_MAINBTN,					"����"),
		    LangResource(RI_MAIN_CARDCASEBTN,				"��Ƭ"),
		    LangResource(RI_MAIN_CALLLISTBTN,				"��¼"),
		    LangResource(RI_MAIN_STOCKINFOBTN,				"��Ʊ"),
		    LangResource(RI_MAIN_SYSTTEMSETTING,			"����"),
		    LangResource(RI_MAIN_SOUND,			            "¼��"),
		    LangResource(RI_MAIN_STOCKINFO,					"ͬ��˳��Ʊ����"),

		    //����ӰԺ
		    LangResource(RI_VEDIO_ALLBTN,					"ȫѡ"),
		    LangResource(RI_VEDIO_DELETEALLBTN,				"���"),
		    LangResource(RI_VEDIO_OPENBTN,					"��"),
		    LangResource(RI_VEDIO_ALLSCREENBTN,				"ȫ��"),
		    LangResource(RI_VEDIO_LEFTROTATEBTN,			"����"),
		    LangResource(RI_VEDIO_RIGHTROTATEBTN,			"����"),
		    LangResource(RI_PHOTO_SCREENSAVE,				"����������"),

		    //������		
		    LangResource(RI_LANUDER_WEEK,					"  ��     һ     ��     ��     ��     ��     ��"),
		    LangResource(RI_LANUDER_LOOP,					"��"),
		    LangResource(RI_LANUDER_MONTH1,					"����"),
		    LangResource(RI_LANUDER_MONTH2,					"����"),
		    LangResource(RI_LANUDER_MONTH3,					"����"),
		    LangResource(RI_LANUDER_MONTH4,					"����"),
		    LangResource(RI_LANUDER_MONTH5,					"����"),
		    LangResource(RI_LANUDER_MONTH6,					"����"),
		    LangResource(RI_LANUDER_MONTH7,					"����"),
		    LangResource(RI_LANUDER_MONTH8,					"����"),
		    LangResource(RI_LANUDER_MONTH9,					"����"),
		    LangResource(RI_LANUDER_MONTH10,				"ʮ��"),
		    LangResource(RI_LANUDER_MONTH11,				"����"),
		    LangResource(RI_LANUDER_MONTH12,				"����"),
		    LangResource(RI_LANUDER_DAY10,					"��"),
		    LangResource(RI_LANUDER_DAY11,					"ʮ"),
		    LangResource(RI_LANUDER_DAY12,					"إ"),
		    LangResource(RI_LANUDER_DAY13,					"ئ"),

		    LangResource(RI_LANUDER_DAY0,					"ʮ"),
		    LangResource(RI_LANUDER_DAY1,					"һ"),
		    LangResource(RI_LANUDER_DAY2,					"��"),
		    LangResource(RI_LANUDER_DAY3,					"��"),
		    LangResource(RI_LANUDER_DAY4,					"��"),
		    LangResource(RI_LANUDER_DAY5,					"��"),
		    LangResource(RI_LANUDER_DAY6,					"��"),
		    LangResource(RI_LANUDER_DAY7,					"��"),
		    LangResource(RI_LANUDER_DAY8,					"��"),
		    LangResource(RI_LANUDER_DAY9,					"��"),

		    //��Ƭ
		    LangResource(RI_CARD_TYPESTC,					"���"),
		    LangResource(RI_CARD_NAMESTC,					"����"),
		    LangResource(RI_CARD_COMPANYSTC,				"��λ"),
		    LangResource(RI_CARD_COMPANYADDRESSSTC,			"��ַ"),
		    LangResource(RI_CARD_DEPARTSTC,					"����"),
		    LangResource(RI_CARD_TITLESTC,					"ְ��"),
		    LangResource(RI_CARD_OPERATORSTC,				"����"),
		    LangResource(RI_CARD_ALLCARDSTC,				"������Ƭ"),
		    LangResource(RI_CARD_FINDREAULTSTC,				"���ҽ��"),
		    LangResource(RI_CARD_NEWCARDBTN,				"�½���Ƭ"),
		    LangResource(RI_CARD_NEWTYPEBTN,				"�½����"),
		    LangResource(RI_CARD_EDITTYPEBTN,				"�༭���"),
		    LangResource(RI_CARD_DELETETYPEBTN,				"ɾ�����"),
		    LangResource(RI_CARD_DELETECARDBTN,				"ɾ����Ƭ"),
		    LangResource(RI_CARD_MOBILESTC,					"�ֻ�:"),
		    LangResource(RI_CARD_TELPHONESTC,				"�̻�:"),
		    LangResource(RI_CARD_EMAILSTC,					"�ʼ�:"),
		    LangResource(RI_CARD_WEBSTC,					"��ַ:"),
		    LangResource(RI_CARD_POSTCODESTC,				"�ʱ�:"),
		    LangResource(RI_CARD_MOTOSTC,					"��ע:"),
		    LangResource(RI_CARD_COMNSTC,					"��ͨ"),
		    LangResource(RI_CARD_VIPSTC,					"VIP"),
		    LangResource(RI_CARD_BLACKLISTSTC,				"������"),
		    LangResource(RI_CARD_CARDFINDSTC,				"��Ƭ����"),
		    LangResource(RI_CARD_FINDCONTENTSTC,			"��ѡ������Ҫ���ҵ�����"),
		    LangResource(RI_CARD_TELCODESTC,				"�绰����"),
		    LangResource(RI_CARD_GROUPSTC,				    "��Ⱥ:"),
		    LangResource(RI_CARD_NEWCARDSTC,				"�½���Ƭ"),
		    LangResource(RI_CARD_EDITCARDSTC,				"�༭��Ƭ"),

		    LangResource(RI_CARD_TYPENAME,			    	"�������:"),
		    LangResource(RI_CARD_RING,						"��ɫ����:"),
		    LangResource(RI_CARD_SOUNDTIP,					"������ʾ:"),
		    LangResource(RI_CARD_DEFAULT,					"Ĭ��"),
		    LangResource(RI_CARD_FIND_TITLE,				"��Ƭ����"),
		    LangResource(RI_CARD_FIND_TIP,				    "������Ҫ���ҵ�����"),
		    LangResource(RI_CARD_FIND_NAME,					"��      ��:"),
		    LangResource(RI_CARD_FIND_NUMBER,				"�绰����:"),
		    LangResource(RI_CARD_FIND_COMPANY,				"��      ˾:"),
		    LangResource(RI_CARD_FIND_DUTY,				    "ְ      ��:"),

		    //�绰
		    LangResource(RI_TEL_NEWTELSTC,					"����������"),
		    LangResource(RI_TEL_HIDEBTN,					"����"),
		    LangResource(RI_TEL_ACCEPTBTN,					"����"),
		    LangResource(RI_TEL_DIALTELSTC,					"����绰"),
		    LangResource(RI_TEL_TELINGSTC,					"ͨ����"),
		    LangResource(RI_TEL_NOTEBTN,					"����"),
		    LangResource(RI_TEL_RECORDEBTN,					"¼��"),
		    LangResource(RI_TEL_STOPRECORDEBTN,				"ֹͣ¼��"),
		    LangResource(RI_TEL_HUNGOFFBTN,					"�Ҷ�"),
		    LangResource(RI_TEL_RECORDING,            	    "����¼��"),
		    LangResource(RI_TEL_DURATION,                 	"ͨ��ʱ��"),

		    //����
		    LangResource(RI_NOTE_NEXTBTN,					"��һҳ"),
		    LangResource(RI_NOTE_BACKBTN,					"��һҳ"),
		    LangResource(RI_NOTE_TITLE,					    "����"),
		    LangResource(RI_NOTE_PAGE,				    	"ҳ"),
		    LangResource(RI_NOTE_CLEAR,				    	"���"),

		    //ͨ����¼
		    LangResource(RI_RECORD_TYPE,                     "����"),
		    LangResource(RI_RECORD_NAMENO,                   "����/����"),
		    LangResource(RI_RECORD_TIME,                     "ʱ��"),
		    LangResource(RI_RECORD_DURATION,                 "ʱ��"),
		    LangResource(RI_RECORD_SOUND,                    "¼��"),
		    LangResource(RI_RECORD_OPERATION,                "����"),
		    LangResource(RI_RECORD_IN,                       "δ�ӵ绰"),
		    LangResource(RI_RECORD_INED,                     "�ѽӵ绰"),
		    LangResource(RI_RECORD_OUT,                      "�Ѳ��绰"),
		    LangResource(RI_RECORD_SEARCHRESULT,             "���ҽ��"),
		    LangResource(RI_RECORD_DIAL,                     "����"),
		    LangResource(RI_RECORD_LISTENSOUND,              "��������"),
		    LangResource(RI_RECORD_NOTE,                     "����"),
		    LangResource(RI_RECORD_NEWCARD,                  "�½���Ƭ"),
		    LangResource(RI_RECORD_SEARCH,                   "����"),
		    LangResource(RI_RECORD_DELETE,                   "ɾ��"),

		    LangResource(RI_RECORD_FIND_TITLE,               "ͨ����¼����"),
		    LangResource(RI_RECORD_FIND_TIP,                 "��ѡ������Ҫ���ҵ�����:"),
		    LangResource(RI_RECORD_FIND_TIME,                "ʱ�䷶Χ:"),
		    LangResource(RI_RECORD_FIND_TIMEFROM,            "��"),
		    LangResource(RI_RECORD_FIND_TIMETO,              "��"),
		    LangResource(RI_RECORD_FIND_NAME,                "��      ��:"),
		    LangResource(RI_RECORD_FIND_NUMBER,              "�绰����:"),
    		
		    //¼��
		    LangResource(RI_SOUND_SOUND,                     "��������"),
		    LangResource(RI_SOUND_RECORD,                    "ͨ��¼��"),
		    LangResource(RI_SOUND_PROTECT,                   "����"),
		    LangResource(RI_SOUND_PLAY,                      "����"),
		    LangResource(RI_SOUND_FIND_TITLE,                "¼������"),

		    LangResource(RI_SOUND_PLAY_TITLE,                "������"),
		    LangResource(RI_SOUND_PLAY_CLOSE,                "�ر�"),
		    LangResource(RI_SOUND_PLAY_PREV,                 "��һ��"),
		    LangResource(RI_SOUND_PLAY_TEXT,                 "��һ��"),
		    LangResource(RI_SOUND_PLAY_PLAY,                 "����"),
		    LangResource(RI_SOUND_PLAY_CONTINUE,             "����������һ��¼��"),

		    //����
		    LangResource(RI_SETTING_TELEPHONE,               "�绰ͨѶ"),
		    LangResource(RI_SETTING_INTERNET,                "��·����"),
		    LangResource(RI_SETTING_SYSTEM,                  "ϵͳ����"),
		    LangResource(RI_SETTING_INFORMATION,             "ϵͳ��Ϣ"),
		    LangResource(RI_SETTING_DEFAULT,                 "�ָ�Ĭ��"),
		    LangResource(RI_SETTING_DETAILTITLE,             "������Ŀ"),
    				
		    LangResource(RI_SETTING_AREACODE,				 "��������:"),
		    LangResource(RI_SETTING_OUTLINE,				 "�������߼Ӳ�"),
		    LangResource(RI_SETTING_IPDIAL,					 "����IP����"),
		    LangResource(RI_SETTING_TELVOLUME,			     "��������"),
		    LangResource(RI_SETTING_SYSTEMTIME,				 "����ϵͳʱ��:"),

		    LangResource(RI_SETTING_ENABLERING,              "������ɫ���幦��"),
		    LangResource(RI_SETTING_ENABLERINGSTC,           "Ĭ�ϱ�׼����"),
		    LangResource(RI_SETTING_ENABLEAUTOLEAVEWORD,     "�����Զ����Թ���"),
		    LangResource(RI_SETTING_RINGTIMES,               "�����������"),
		    LangResource(RI_SETTING_LEAVEWORD,               "�Զ�������ʾ"),
		    LangResource(RI_SETTING_RECORDLEAVEWORD,         "¼������"),
		    LangResource(RI_SETTING_DEFINESPEEDBUTTON,       "�����ٲ�"),
		    LangResource(RI_SETTING_DEFINESPEEDBUTTONSTC,    "�������õ绰��12���ٲ�����"),
    		
		    LangResource(RI_SETTING_CONNECTAUTO,             "ֱ����������"),
		    LangResource(RI_SETTING_CONNECTMANUAL,           "ADSL������������"),
		    LangResource(RI_SETTING_ADSLNAME,                "�û���:"),
		    LangResource(RI_SETTING_ADSLPASSWORD,            "����:"),
		    LangResource(RI_SETTING_USEDYNAMICIP,            "�Զ����IP��ַ"),
		    LangResource(RI_SETTING_USESTATICIIP,            "ʹ�������IP��ַ:"),
		    LangResource(RI_SETTING_IP,                      "IP��ַ:"),
		    LangResource(RI_SETTING_MASK,                    "��������:"),
		    LangResource(RI_SETTING_GATEWAY,                 "����:"),
		    LangResource(RI_SETTING_DNS,                     "DNS������:"),
    				
		    LangResource(RI_SETTING_ENABLESP,                "������Ļ����"),
		    LangResource(RI_SETTING_SPTYPE,                  "��������:"),
		    LangResource(RI_SETTING_TIMESP,                  "ʱ������"),
		    LangResource(RI_SETTING_IMAGESP,                 "�����������"),
		    LangResource(RI_SETTING_WAITTIME,                "�ȴ�ʱ��:"),
		    LangResource(RI_SETTING_ENABLEPASSWORD,          "�ڻָ�ʱʹ������"),
		    LangResource(RI_SETTING_SETPASSWORD,             "��������"),
		    LangResource(RI_SETTING_SOUNDSAVEPATH,           "¼���洢λ��:"),
		    LangResource(RI_SETTING_LOCAL,					 "����"),
		    LangResource(RI_SETTING_SDCARD,                  "SD��"),
		    LangResource(RI_SETTING_SCREENADJUSTSTC,         "�������µ�����Ļ�������"),
		    LangResource(RI_SETTING_SCREENADJUST,            "У����Ļ"),

		    LangResource(RI_SETTING_SYSTOMTITLE,             "��ý�����绰ϵͳ"),
		    LangResource(RI_SETTING_SOFTVERSION,             "�����汾:"),
		    LangResource(RI_SETTING_HARDVERSION,             "Ӳ���汾:"),
		    LangResource(RI_SETTING_FLASHSIZE,               "���ؿռ�:"),
		    LangResource(RI_SETTING_SDSTORAGESIZE,           "SD���ռ�:"),
		    LangResource(RI_SETTING_AVAILABLESIZE,           "����ʹ��"),
		    LangResource(RI_SETTING_CONTACTSIZE,             "�绰��Ƭ:"),
		    LangResource(RI_SETTING_CONTACTINFOSIZE,         "ͨ����¼:"),
		    LangResource(RI_SETTING_SOUNDSIZE,               "¼������:"),
		    LangResource(RI_SETTING_INFORMATIONSIZE,         "��Ϣ�㲥:"),
		    LangResource(RI_SETTING_VERSIONRIGHT,            "��Ȩ����:"),
		    LangResource(RI_SETTING_CLEARCONTACT,            "�����Ƭ"),
		    LangResource(RI_SETTING_CLEARCONTACTINFO,        "��ռ�¼"),
		    LangResource(RI_SETTING_CLEARSOUND,              "���¼��"),
		    LangResource(RI_SETTING_CLEARINFORMATION,        "��ձ��"),

		    LangResource(RI_FASTDIALS_TITLE,                 "�����Զ����ٲ�����"),
		    LangResource(RI_FASTDIALS_NUMBER01,              "�ٲ�01:"),
		    LangResource(RI_FASTDIALS_NUMBER02,              "�ٲ�02:"),
		    LangResource(RI_FASTDIALS_NUMBER03,              "�ٲ�03:"),
		    LangResource(RI_FASTDIALS_NUMBER04,              "�ٲ�04:"),
		    LangResource(RI_FASTDIALS_NUMBER05,              "�ٲ�05:"),
		    LangResource(RI_FASTDIALS_NUMBER06,              "�ٲ�06:"),
		    LangResource(RI_FASTDIALS_NUMBER07,              "�ٲ�07:"),
		    LangResource(RI_FASTDIALS_NUMBER08,              "�ٲ�08:"),
		    LangResource(RI_FASTDIALS_NUMBER09,              "�ٲ�09:"),
		    LangResource(RI_FASTDIALS_NUMBER10,              "�ٲ�10:"),
		    LangResource(RI_FASTDIALS_NUMBER11,              "�ٲ�11:"),
		    LangResource(RI_FASTDIALS_NUMBER12,              "�ٲ�12:"),
    		
		    LangResource(RI_SETTING_TITILEBACKUP,			"���ݿ�(��Ƭ, ͨ����¼, ����) ����"),
		    LangResource(RI_SETTING_TITLERESTORE,			"���ݿ�(��Ƭ, ͨ����¼, ����) �ָ�"),
		    LangResource(RI_SETTING_TITLE,					"¼���ļ�����"),//"�ļ�����"),
		    LangResource(RI_SETTING_BACKUP,					"����"),
		    LangResource(RI_SETTING_RESTORE,				"�ָ�"),
		    LangResource(RI_SETTING_IN,						"����"),
		    LangResource(RI_SETTING_OUT,					"����"),
		    LangResource(RI_SETTING_RING,					"��������"),
		    LangResource(RI_SETTING_PHOTO,					"�������"),
		    LangResource(RI_SETTING_MP3,					"MP3����"),
		    LangResource(RI_SETTING_VIDEO,					"��Ƶ�ļ�"),
		    LangResource(RI_SETTING_RECORDE,				"¼���ļ�"),
		    LangResource(RI_SETTING_LOCALFILE,				"�����ļ�"),
		    LangResource(RI_SETTING_USBFILE,				"U���ļ�"),

		    LangResource(RI_SETTING_COPYFILE,				"�ļ�������ָ�"),
		    LangResource(RI_SETTING_COPYFILE_ERROR1,		"�ƶ��洢�豸�����ڣ����������!"),
		    LangResource(RI_SETTING_COPYFILE_ERROR2,		"�˲����Ḳ�����е��ļ�!"),
		    LangResource(RI_SETTING_COPYFILE_ERROR3,		"�ļ�������!"),
		    LangResource(RI_SETTING_COPYFILE_FINISH,		"���������!"),
		    LangResource(RI_SETTING_COPYFILE_DOWITH,		"���ڿ����ļ�,��Ҫ�ϵ�, ���Ժ�..."),

		    //����ǽ
		    LangResource(RI_FIREWALL_TITLE,                  "����ǽ"),
		    LangResource(RI_FIREWALL_ENABLE,                 "��������ǽ"),
		    LangResource(RI_FIREWALL_BLACKLIST,              "�ܾ��������绰����"),
		    LangResource(RI_FIREWALL_CONTACT,                "ֻ������Ƭ����"),   //�������������
		    LangResource(RI_FIREWALL_VIP,                    "ֻ����VIP�绰����"),
		    LangResource(RI_FIREWALL_ALL,                    "�ܾ���������,�޶�ʱ��"),
		    LangResource(RI_FIREWALL_PART1,                  "�޶�ʱ��"),
		    LangResource(RI_FIREWALL_PART2,                  "Сʱ"),

		    //���뱣��
	    	//LangResource(RI_PASSWORD_TITLE,                  "¼����������"),
		    LangResource(RI_PASSWORD_TITLE,                  "��������"),
		    LangResource(RI_PASSWORD_TITLE1,                 "��֤����"),
		    LangResource(RI_PASSWORD_TIP,                    "����������:"),
		    LangResource(RI_PASSWORD_TIP1,                   "������֤����, ���������������:"),
		    LangResource(RI_PASSWORD_TIP2,                   "���������벻���, ����������������:"),
		    LangResource(RI_PASSWORD_TIP3,                   "�������, ����������:"),
		    LangResource(RI_PASSWORD_OLDPASSWORD,            "������:"),
		    LangResource(RI_PASSWORD_NEWPASSWORD1,           "������:"),
		    LangResource(RI_PASSWORD_NEWPASSWORD2,           "ȷ��������:"),

		    LangResource(RI_DELETETIP_TITLE,                 "ɾ��ȷ��"),
		    LangResource(RI_DELETETIP_TIP,                   "ȷ��Ҫɾ����¼����¼��"),
		    LangResource(RI_DELETETIP_PASSWORD,              "��������:"),
		    LangResource(RI_DELETETIP_CONTACT,               "ȷ��Ҫɾ������Ƭ��"),
		    LangResource(RI_DELETETIP_CONTACTINFO,           "ȷ��Ҫɾ����ͨ����¼��"),
		    LangResource(RI_DELETETIP_CONTACTGROUP,          "ȷ��Ҫɾ������Ƭ�����(�������Ƭһͬ��ɾ��)"),
		    LangResource(RI_DELETETIP_PASSWORD,              "��������:"),

		    LangResource(RI_DELETETIP_CONTACT2,               "ȷ��Ҫ���������Ƭ��"),
		    LangResource(RI_DELETETIP_CONTACTINFO2,           "ȷ��Ҫ�������ͨ����¼��"),
		    LangResource(RI_DELETETIP_SOUND2,                 "ȷ��Ҫ�������¼����¼��"),

		    //��Ļ����
		    LangResource(RI_SCREENSAVE_PASSWORDINPUT,        "����������:"),
		    LangResource(RI_SYSTEM_DISK,                     "���̿ռ䲻��,����������!"),

		    //��Ʊ
		    LangResource(RI_STOCKS_NETERROR,                 "�������,����!"),
		    LangResource(RI_STOCKS_NETCONNECT,               "������������......"),	


		    LangResource(RI_SOUND_RECORD_STOPPLAY,           "ֹͣ����"),
		    LangResource(RI_SOUND_LOCALRECORD,               "����¼��"),
		    LangResource(RI_SOUND_LEAVEWORDTIP,              "��ʾ��"),
		    LangResource(RI_SOUND_PLAY_PAUSE,				 "��ͣ"),
	    };
        std::vector<LangResource> result(data, data + sizeof data / sizeof data[0]);
        return result;
    }

	std::wstring DataAccess<LanguageResource>::tableName_ = L"chineseResource";
    int DataAccess<LanguageResource>::count_ = 0;
	int DataAccess<LanguageResource>::offset_ = 0;

	Language LanguageResource::language_ = lChinese;

    void LanguageResource::InsertToDatabase() {
        std::vector<LangResource> r = langResource();
        for (size_t i = 0; i < r.size(); ++i) {
            LanguageResource lr;
            lr.id(r[i].id);
            lr.value(r[i].value);
            lr.Insert();
        }
    }

    LanguageResource::LanguageResource()
        : DataAccess<LanguageResource>()
        //, value_()
    {
    }

    std::vector<Util::shared_ptr<LanguageResource> > LanguageResource::GetFromDatabase(std::wstring const& filter, std::wstring const& orderFieldName, SeqenceRelation const dir, int const offset, int const pageSize) {
		return DataAccess<LanguageResource>::GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, dir, offset, pageSize);
	}

	std::string LanguageResource::Get(int const id) {
        // method1:
		//Util::shared_ptr<LanguageResource> lr = DataAccess<LanguageResource>::GetDataById(id, modifyFieldByDB_);
		//return lr->value();
        // 
        // method2:
        //find from langResource array
        std::string result = "";
        std::vector<LangResource> lr = langResource();
        for (size_t i = 0; i < lr.size(); ++i) {
            if (lr[i].id == id) {
                result = lr[i].value;
                break;
            }
        }
        return result;
	}

	void LanguageResource::Update() const {
        std::wstring cmd = L"UPDATE ";
        cmd += tableName_;
        cmd += L" SET [value] = '";
        cmd += StringOp::FromUTF8(value_);
        cmd += L"' WHERE id = ";
        cmd += StringOp::FromInt(id());
        ExecCommand(cmd);
	}

	void LanguageResource::Insert() const {
        std::wstring cmd = L"INSERT INTO ";
        cmd += tableName_;
        cmd += L" ( id, value ) VALUES ( ";
        cmd += StringOp::FromInt(id());
        cmd += L", '";
        cmd += StringOp::FromUTF8(value_);
        cmd += L"' )";
        ExecCommand(cmd);
	}

	void LanguageResource::Remove(std::wstring filter) {
		DataAccess<LanguageResource>::RemoveDatasByFilter(filter);
	}

    void LanguageResource::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<LanguageResource> item) {
        item->id(atoi(argv[BaseData::GetIndexByName(argc, columnName, "id")]));
        item->value_ = argv[BaseData::GetIndexByName(argc, columnName, "value")];
	}
}