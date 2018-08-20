#define READWRITEBUFF   2048
#define BAUDRATE        9600
#define DATAITS         8
#define CMDREQUEST      0x20000000
#define CMDRESPONSE     0x20000001
#define AUCID           0x30000000
#define CHECKID         0x30000001
#define TRANSFEIID      0x30000002
// CCSerialPort document
///////////////////////zzw 缓冲大小
#define  MAXBLOCK  1024
/////////////////////////Protocol///////
/*
#define  AUTHENTICATIONTRAN 0x10000001
#define  CHECKPASSWORDTRAN  0x10000002
#define  TRANSFERFILETRAN   0x10000003

#define  AUTHCMDREQ         0x20000001
#define  AUTHCMDRSP         0x80000001
#define  CHECKPASSREQ       0x20000002
#define  CHECKPASSRSP       0x80000002
#define  TRANSFERFILEREQ    0x20000003
#define  TRANSFERFILERSP    0x80000003
#define  REQDATATRANSFER    0x20000004
#define  RSPDATATRANSFER    0x80000004
#define  REQDATATRANSEND    0x20000005
#define  RSPDATATRANSEND    0x80000005
#define  REQRETRYDATA       0x20000006
#define  RSPRETRYDATA       0x80000006



/*
“文件类型”字段有：
1）	通话记录数据库VIEW  : 0x1
2）	录音文件              : 0x2
3）	铃音文件列表          : 0x3
4）	铃音文件              : 0x4
5）	名片信息数据库        : 0x5
*/
/*
#define   CONTACTINFO     0x10000000
#define   RECORDFILE      0x20000000
#define   MIDIFILELIST    0x30000000
#define   MIDIFILE        0x40000000
#define   CONTACTDB       0x50000000
*/

#define CMDREQUEST      0x207
#define CMDRESPONSE     0x201
#define AUCID           0x300
#define CHECKID         0x301
#define TRANSFEIID      0x302
// CCSerialPort document
///////////////////////zzw 缓冲大小
#define  MAXBLOCK  1024
/////////////////////////Protocol///////
#define  AUTHENTICATIONTRAN 0x101
#define  CHECKPASSWORDTRAN  0x102
#define  TRANSFERFILETRAN   0x103

#define  AUTHCMDREQ         0x201
#define  AUTHCMDRSP         0x801
#define  CHECKPASSREQ       0x202
#define  CHECKPASSRSP       0x802
#define  TRANSFERFILEREQ    0x203
#define  TRANSFERFILERSP    0x803
#define  REQDATATRANSFER    0x204
#define  RSPDATATRANSFER    0x804
#define  REQDATATRANSEND    0x205
#define  RSPDATATRANSEND    0x805
#define  REQRETRYDATA       0x206
#define  RSPRETRYDATA       0x806

/*
“文件类型”字段有：
1）	通话记录数据库VIEW  : 0x1
2）	录音文件              : 0x2
3）	铃音文件列表          : 0x3
4）	铃音文件              : 0x4
5）	名片信息数据库        : 0x5
*/
#define   CONTACTINFO     0x100
#define   RECORDFILE      0x200
#define   MIDIFILELIST    0x300
#define   MIDIFILE        0x400
#define   CONTACTDB       0x500
#define   DOWNLOAD        0X600
#define   UPLOAD          0X700
#define   UPDATEINFO      0X800
#define   UPLOADSTOAGE    0X900

#define    MAGICNUMBER      4
#define    TRNSACTIONID     4
#define    CMDTYPE          4
#define    PACKETID         4
#define    PACKETLENGTH     4
#define    RESERVED         12
#define    PACKETCONTENT    1024
#define    CHACKSUM         4
///////////////////////文件内容

typedef struct BASEPROTOCOLS 
{
	int magicNumber ;
	int trasctTionID ;
	int preCmdType ;
	int cmdType ;
	int packetID ;
	int packetLength;
	int operatorType;
	char reserved[RESERVED];
	int checksum;

}BASEPROTOCOL,*PTRBASEPROTOCOL;

#define FILENMAELEN   256
////文件结构
typedef struct FILESTRUTS
{
   int FileType;
   char FileName[FILENMAELEN];
   char FileContent[PACKETCONTENT];
   int FileSize ;
   int packetcount;

}PFILESTRUT,*PTRFILESTRUT;
//////////文件传输结构///////////////
typedef struct PROTOCOLTRANSFER
{
	BASEPROTOCOL baseprotocl;
    PFILESTRUT FileStruct;

}PROTOCOLTFILE,*PTRPROTOCOLTRANSFILE;
/////////////////////////////////////////

///////////////////////AUTHENTICATION
#define PRODUCTTYPE             64
#define PRODUCTSERIALNUMBER     64
#define PRODUCTIONTIME          64
#define SOFTWAREVERSION         64
#define HARDWAREVERSION         64
#define PASSWORDAUTHENTI        32
////////////////////////鉴权/////////////
typedef struct AUTHENTICATION
{
	char  productType[PRODUCTTYPE];
	char  productSertalNumber[PRODUCTSERIALNUMBER];
	char  productTionTime[PRODUCTIONTIME];
	char  softwaveVersion[SOFTWAREVERSION];
	char  handwaveVersion[HARDWAREVERSION];
	char  PasswordDauthenti[PASSWORDAUTHENTI];

}AUTHENTI,*PTRAUTHENT;
/////////////////////////////////////////////

//////////////////////鉴权数据格式/////////////////
typedef struct PROTOCOLAUTH
{
	BASEPROTOCOL baseprotocl;
	AUTHENTI authenti;

}PROTOCOLAU, *PTRPROTOCOLAUTH;
//////////////////////////////////////////////////

///////////////////PASSWORDAUTHENTICATION
#define  USERNAME  128
#define  USERPASSWORD  128
#define  TESTRESULT    32
////////////////////////核对密码//////////////////////
typedef struct PASSWORDAUTHENTICATION
{
	char userName[USERNAME];
	char userPassword[USERPASSWORD];
	char testResult[TESTRESULT];

}PASSWOR,*PTRPASSWORD;
////////////////////////////////////////////////

///////////////核对密码数据格式/////////////////
typedef struct PROTOCOLCHECKPASS
{
	BASEPROTOCOL baseprotocl;
	PASSWOR password;

}PROTOCOLCHECK,*PTRPROTOCOLCHECK;
//////////////////////////////////////////////


