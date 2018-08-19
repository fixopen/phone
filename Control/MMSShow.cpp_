// ImageShow.cpp : implementation file
//

#include "stdafx.h"
#include "MmsShow.h"

// ImageShow

// UTF-8的unicode表示方法到unicode的值转换函数
static int utf82unicode(unsigned char  *byte, int index, int count, WCHAR &unicode)
{
	int offset = 0;
	if (index >= count) return -1;
	if ( (byte[index] & 0x80) == 0x0)              //  一位
	{
		unicode = byte[index];
		offset = 1;
	}
	else if ((byte[index] & 0xE0) == 0xC0) // 两位
	{
		if (index + 1 >= count ) return false;
		unicode = (((byte[index] & 0x1F)) << 6) 
			| (byte[ index + 1] & 0x3F);
		offset = 2;
	} 
	else if ((byte[index] & 0xF0) == 0xE0) // 三位
	{
		if (index + 2 >= count) return false;
		unicode = (((byte[index] & 0x0F)) << 12) 
			| (((byte[index  + 1] & 0x3F)) << 6) 
			| (byte[index + 2] & 0x3F);
		offset = 3;
	}
	else if ((byte[index] & 0xF8) == 0xF0) // 四位
	{
		if (index + 3 >= count) return false;
		unicode = (((byte[index] & 0x07)) << 18) 
			| (((byte[index + 1] & 0x3F)) << 12) 
			| (((byte[index + 2] & 0x3F)) << 6) 
			| (byte[index + 3] & 0x3F);
		offset = 4;
	}
	else if ((byte[index] & 0xFC) == 0xF8) // 五位
	{
		if (index + 4 >= count) return false;
		unicode = (((byte[index] & 0x03)) << 24) 
			| (((byte[index + 1] & 0x3F)) << 18) 
			| (((byte[index + 2] & 0x3F)) << 12) 
			| (((byte[index + 3] & 0x3F)) << 6) 
			| (byte[index + 4] & 0x3F);
		offset = 5;
	}
	else if ((byte[index] & 0xFE) == 0xFC) // 六位
	{
		if (index + 5 >= count) return false;
		unicode = (((byte[index] & 0x01)) << 30) 
			| (((byte[index + 1] & 0x3F)) << 24) 
			| (((byte[index + 2] & 0x3F)) << 18) 
			| (((byte[index + 3] & 0x3F)) << 12) 
			| (((byte[index + 4] & 0x3F)) << 6) 
			| (byte[index + 5] & 0x3F);
		offset = 6;
	}
	else
	{
		return -1;
	}
	return offset;
} 

std::string const ToUTF8(std::wstring const& unicode) {
	std::string result;
	enum Hex {
		Hex80 = 0x80,
            Hex800 = 0x800,
            Hex10000 = 0x10000,
            Hex200000 = 0x200000,
            Hex4000000 = 0x4000000,
            Hex80000000 = 0x80000000
	};
	/*std::*/size_t length = unicode.length();
	char chr = 0;
	for (/*std::*/size_t i = 0; i < length; ++i) {
		if (unicode[i] < Hex80) { //1byte
			chr = (char)unicode[i];
			result += chr;
		} else if (unicode[i] < Hex800) { //2byte
			chr = (char)((unicode[i] >> 8) & 0x1F);
			chr |= 0xC0;
			result += chr;
			chr = (char)((unicode[i] & 0x3F) | 0x80);
			result += chr;
		} else if (unicode[i] < Hex10000) { //3byte
			chr = ((char)((unicode[i] >> 12 ) & 0x0F)) | 0xE0;
			result += chr;
			chr = ((char)((unicode[i] >> 6 ) & 0x3F)) | 0x80;
			result += chr;
			chr = ((char)(unicode[i] & 0x3F)) | 0x80;
			result += chr;
		} else if (unicode[i] < Hex200000) { //4byte
			//
		} else if (unicode[i] < Hex4000000) { //5byte
			//
		} else if (unicode[i] < Hex80000000) { //6byte
			//
		}
	}
	return result;
 }

std::wstring static FromUTF8(std::string const& value) {
	std::wstring result;
	wchar_t chr;
	for (size_t i = 0; i < value.length(); ++i) {
		if ((value[i] & 0x80) == 0x0) { //1byte
			chr = value[i];
		} else if ((value[i] & 0xE0) == 0xC0) { //2byte
			chr = (((value[i] & 0x1F)) << 6)
				| (value[i + 1] & 0x3F);
			++i;
		} else if ((value[i] & 0xF0) == 0xE0) { //3byte
			chr = (((value[i] & 0x0F)) << 12)
				| (((value[i + 1] & 0x3F)) << 6)
				| (value[i + 2] & 0x3F);
			i += 2;
		} else if ((value[i] & 0xF8) == 0xF0) { //4byte
			chr = (((value[i] & 0x07)) << 18)
				| (((value[i + 1] & 0x3F)) << 12)
				| (((value[i + 2] & 0x3F)) << 6)
				| (value[i + 3] & 0x3F);
			i += 3;
		} else if ((value[i] & 0xFC) == 0xF8) { //5byte
			chr = (((value[i] & 0x03)) << 24)
				| (((value[i + 1] & 0x3F)) << 18)
				| (((value[i + 2] & 0x3F)) << 12)
				| (((value[i + 3] & 0x3F)) << 6)
				| (value[i + 4] & 0x3F);
			i += 4;
		} else if ((value[i] & 0xFE) == 0xFC) { //6byte
			chr = (((value[i] & 0x01)) << 30)
				| (((value[i + 1] & 0x3F)) << 24)
				| (((value[i + 2] & 0x3F)) << 18)
				| (((value[i + 3] & 0x3F)) << 12)
				| (((value[i + 4] & 0x3F)) << 6)
				| (value[i + 5] & 0x3F);
			i += 5;
		}
		result += chr;
	}
	return result;
 }

IMPLEMENT_DYNAMIC(MmsShow, CWnd)

MmsShow::MmsShow()
{  	

//	m_cMmsLayOut.regions.clear();
	m_cAudioName		=	L""		;
	m_cVedioName		=	L""		;
	m_bFindSiml			=	false	;
	m_uMmsParPlay		=	0		;
	m_cImageTime		=	0		;
	m_cImagePath		=   L""		;
	m_uImageDur			=	0		;//图片的显示时间
	m_uTxtDur			=	0		;//文本显示时间
	m_uParDur			=	0		;//Par显示的总时间
	m_cTxtContent		=	L""		;//文本路径
	m_uTxtTime			=	0		;//文本显示了多长时间
	m_uParTime			=	0		;//

	m_cDefImagePath.clear()			;//没有SMIL文件，加载该路径的全部图片
	m_cDefTxtContent.clear()		;//没有SMIL文件，加载文本
	m_uDefTxtTime	= 0				;//文本显示了多长时间
	m_uDefImageTime	= 0				;//显示的时间
	m_uDefTxtItem	= 0				;//正在
	m_uDefImageItem	= 0				;//显示的时间

	
}

MmsShow::~MmsShow()
{

}


BEGIN_MESSAGE_MAP(MmsShow, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_PICTURE,OnBtnPicture)
	ON_BN_CLICKED(IDC_BTN_MUSIC,OnBtnMusic)
	ON_BN_CLICKED(IDC_BTN_VEDIO,OnBtnVedio)
END_MESSAGE_MAP()

// ImageShow message handlers

int MmsShow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_cPic.Create(L"my static", WS_CHILD|WS_VISIBLE|SS_CENTER, 
		CRect(0,0,200,200), this,0xFFFF);


	//m_cPic.SetImageFilename(L"\\Program Files\\VIEW\\4.jpg");//1.bmp,3.png,4.jpg,5.gif	
	//m_cPic.SetImageRegion(CPoint(100,100),CSize(100,100));
	
	//SetImageFileName(L"\\Program Files\\VIEW\\4.jpg");
	//SetImageRegion(CPoint(30,30),CSize(90,90));//截取图片


//	m_cTextView.Create(L"text", WS_CHILD|WS_VISIBLE|SS_CENTER, 
//	CRect(200,0,300,200), this,0xFFFF); 

	m_cTextView.Create(WS_CHILD|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN, 
		CRect(200,0,300,200), this,0xFFFF); 

	m_cMmsTitle.Create(WS_CHILD|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
		CRect(0,0,500,28),this,0xFFFF);

/*	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));	// zero out structure
	lf.lfHeight = 20;					// request a 12-pixel-height font
	_tcscpy(lf.lfFaceName, _T("Arial")); // request a face name "Arial"	
	SetTextFont(lf);//设置字体	
	SetTextColor(RGB(0,255,0),RGB(204, 184, 218));//设置背景色 和 颜色
	SetTextSize(CPoint(100,100),CSize(400,400));
	Invalidate(false);
*/
	m_btnPicture.Create(L"保存图片",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(25,BTN_HEITHT,25+BTN_WIDTH,BTN_HEITHT+40),this,IDC_BTN_PICTURE);
	m_btnPicture.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	m_btnPicture.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	m_btnPicture.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
	m_btnPicture.DrawBorder();

	m_btnMusic.Create(L"保存音乐",WS_CHILD|BS_PUSHBUTTON,CRect(25+BTN_INTERVAL+BTN_WIDTH ,BTN_HEITHT,25+BTN_INTERVAL+BTN_WIDTH*2 ,BTN_HEITHT+40),this,IDC_BTN_MUSIC);
	m_btnMusic.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	m_btnMusic.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	m_btnMusic.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
	m_btnMusic.DrawBorder();

	m_btnVedio.Create(L"保存音频",WS_CHILD|BS_PUSHBUTTON,CRect(25+BTN_INTERVAL*2 + BTN_WIDTH*2 ,BTN_HEITHT,25+BTN_INTERVAL*2 +BTN_WIDTH*3 ,BTN_HEITHT+40),this,IDC_BTN_VEDIO);
	m_btnVedio.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	m_btnVedio.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	m_btnVedio.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
	m_btnVedio.DrawBorder();

	m_picPathDlg.Create(IDD_PIC_SELECT_DLG,this);
	m_picPathDlg.MoveWindow(0,0,320,170);
	m_picPathDlg.CenterWindow();


//	m_listCtral.Create(IDD_LIST_DLG,this);
//	m_listCtral.MoveWindow(30,30,600,400);

	return 0;
}

void MmsShow::GetName(CString &fl)
{
	fl = m_cImagePath ;
}

BOOL MmsShow::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, const DWORD dwFlag)
{	
	
	// load stock cursor, brush, and icon for
	// my own window class

	CString strMyClass;
	try
	{
		strMyClass = AfxRegisterWndClass(
			CS_VREDRAW | CS_HREDRAW,
			NULL,
			(HBRUSH) ::GetStockObject(WHITE_BRUSH),
			NULL);
	}
	catch (CResourceException* pEx)
	{
		AfxMessageBox(_T("Couldn't register class! (Already registered?)"));
		pEx->Delete();
	}
		
	BOOL b =  CWnd::CreateEx(WS_EX_STATICEDGE, (LPCTSTR)strMyClass, NULL, dwStyle|WS_CHILD, rect, pParentWnd, nID);
//	SetImageShow(true);
//	SetTextShow(true);
	return b ;
}

void MmsShow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CWnd::OnPaint();
}

void MmsShow::SetImageFileName(std::wstring const& filename)
{
	m_cPic.SetImageFilename(filename);
}

void MmsShow::SetImageRegion(CPoint const& origin, CSize const& size)
{
	m_cPic.SetImageRegion(origin,size);
}

void MmsShow::SetImagePos(CRect &rct)
{
	m_cPic.SetImagePos(rct);
}

void MmsShow::SetImageShow(BOOL show /* = true */)
{	
	if (show)      //lxz
	{	
		CRect rct ;
		rct.left	= m_cMmsRegion[1].left ;
		rct.top		= m_cMmsRegion[1].top;
		rct.right	= m_cMmsRegion[1].left + m_cMmsRegion[1].width;
		rct.bottom	= m_cMmsRegion[1].top  + m_cMmsRegion[1].height;
		SetImagePos(rct);
		std::wstring ws = m_cImagePath	;
		SetImageFileName(ws);
		
	}
	//SetImageRegion(CPoint(30,30),CSize(90,90));//截取图片
	m_cPic.ShowWindow(show);
}

void MmsShow::SetTextColor(COLORREF bkclor,COLORREF txtclor)
{
//	m_cTextView.SetTextColor(bkclor,txtclor);
}
void MmsShow::SetTextContent(std::wstring const& text)
{
	m_cTextView.SetWindowText(text.c_str());
//	m_cTextView.SetTextContent(text);
}
void MmsShow::SetTextFont(const LOGFONT &font)
{
//	m_cTextView.SetFont(font);
}
void MmsShow::SetTextSize(CPoint const& origin, CSize const& size)
{
//	m_cTextView.SetTextSize(origin,size);
}
void MmsShow::SetTextUpdate()
{
//	m_cTextView.Update();
}

void MmsShow::FromListName(CString const& allpath)
{
	m_cImagePath = allpath ;
}
	
void MmsShow::SetTextShow(BOOL show /* = true */)
{	

	CRect rct ;
	rct.left	= m_cMmsRegion[0].left ;
	rct.top		= m_cMmsRegion[0].top;
	rct.right	= m_cMmsRegion[0].left + m_cMmsRegion[0].width;
	rct.bottom	= m_cMmsRegion[0].top  + m_cMmsRegion[0].height;
	m_cTextView.MoveWindow(rct.left,rct.top,rct.Width(),rct.Height());

	if (show)
	{
		std::wstring ws ;
		ws		=	m_cTxtContent ;
		SetTextContent(ws);	
		m_cTextView.SetWindowText(ws.c_str());
//		m_cTextView.SetTextSize(CPoint(rct.left,rct.top),CSize(m_cMmsRegion[0].width,m_cMmsRegion[0].height));

	}
	m_cTextView.ShowWindow(show);
}
BOOL MmsShow::FindFileSmil(std::wstring const& filename)
{  
	Clear();
	SetMmsRead();
	CString	str = filename.c_str();
	CString str1 = str ;//保存全路径
	m_cSmilPath	 = str ;
	if (str.Find(L"\\",str.GetLength() - 1) <= 0 && str.Find(L"/",str.GetLength() - 1) <= 0 )
	{	
		AfxMessageBox(L"文件的路径设置不正确，重新设置");
		return FALSE ;
	}
	str = str + L"*" ;
	WIN32_FIND_DATA fd;
	HANDLE hfind;
	hfind	  =	 FindFirstFile(str,&fd);
	if(hfind !=	 INVALID_HANDLE_VALUE)
	{
		do
		{
			if(fd.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
			{ 
				//如果是文件夹
			}
			else
			{ 
				//如果是文件
				CString strfl = fd.cFileName ;

				if ( strfl.Find(L".smil") > 0 || strfl.Find(L".smi") > 0 || strfl.Find(L".SMIL") > 0 || strfl.Find(L".SMI") > 0)
				{
					//解析smil	
					std::wstring	wr		= str1 + strfl	;
					std::string		file	= ToUTF8(wr)	;
					MMS::MMSWarp::GetMMSWarp()->DecodeSmil(file, m_cMmsPar, m_cMmsLayOut);
					
					m_bFindSiml	= TRUE ;
					GetMmsRegionInfo();
					InitialRegion();
					SetImageShow(true);
					SetTextShow(true);
					SetTimer(PAR_SHOW_TIMER,1000,NULL);
					SetTimer(IMAGE_TIMER,1000,NULL);
					SetTimer(TXT_TIMER,1000,NULL);
					
					return	TRUE ;
				}
				if(strfl.Find(L".txt") > 0)
				{						
					CFile f;
					CFileException e;
					char wr[1000]	;
					if( f.Open(str1 +strfl, CFile::modeRead, &e ) )
					{
						int length = f.GetLength() ;
						memset(wr,0,1000);
						f.Read(wr,length);
						wr[length] = '\0';
					//	m_cTxtContent = wr;//CString 不能转化成 char *指针
						wchar_t uicode[1000] ;//转化成uicode
						memset(uicode,0,2000);
						int index = 0 ;
						int count = strlen(wr);//
						int iNum =0 ;
						int offset =0 ;
						while( offset!= -1 && index < count)
						{
							offset  = utf82unicode((unsigned char *)wr,index,count,uicode[iNum]);
							index += offset;
							iNum++;					
						}
						m_cTxtContent = uicode ;
						m_cDefTxtContent.push_back(m_cTxtContent);
						f.Close();
					}

				}
				if(strfl.Find(L".jpg") > 0 || strfl.Find(L".bmp") > 0 || strfl.Find(L".png") > 0 || strfl.Find(L".gif") > 0)
				{
					m_cImagePath		= str1 + strfl;
					m_cDefImagePath.push_back(str1 + strfl);
				}
			}
		}
		while(FindNextFile(hfind,&fd));
	}
	if (m_bFindSiml)
	{
		return TRUE ;
	}
	else
	{  	
		m_bFindSiml	= FALSE;
		GetDefRegionInfo();
		InitialDefRegion();
		SetImageShow(true);
		SetTextShow(true);
		SetTimer(DEF_IMAGE_SHOW,1000,NULL);
		SetTimer(DEF_TXT_SHOW,1000,NULL);

	}
	return TRUE	;

}
void MmsShow::InsertPicture()
{
	SetImageShow();
	if (m_cImagePath.Find(L".JPG") >0 ||m_cImagePath.Find(L".jpg") >0 ||
		m_cImagePath.Find(L".PNG") >0 ||m_cImagePath.Find(L".png") >0 ||
		m_cImagePath.Find(L".GIF") >0 ||m_cImagePath.Find(L".gif") >0 ||
		m_cImagePath.Find(L".BMP") >0 ||m_cImagePath.Find(L".bmp") >0)
	{
		m_btnPicture.SetWindowText(L"删除图片");
		m_uState = 3;
	}
	
}
void MmsShow::ClosePicture()
{
	m_cImagePath = L"";
	SetImageShow();//重新加载
	m_cPic.Invalidate(FALSE);
}
void MmsShow::Clear()
{   
	KillTimer(IMAGE_TIMER);
	KillTimer(PAR_SHOW_TIMER);
	KillTimer(TXT_TIMER);
	KillTimer(DEF_TXT_SHOW);
	KillTimer(DEF_IMAGE_SHOW);
	m_uMmsParPlay		=	0		;
	m_cImageTime		=	0		;
	m_cImagePath		=   L""		;
	m_uImageDur			=	0		;//图片的显示时间
	m_uTxtDur			=	0		;//文本显示时间
	m_uParDur			=	0		;//Par显示的总时间
	m_cTxtContent		=	L""		;//文本路径
	m_uTxtTime			=	0		;//文本显示了多长时间
	m_uParTime			=	0		;//
	m_bFindSiml			=	false	;
    
	if (m_cDefImagePath.size() > 0)
	{
		m_cDefImagePath.clear()			;//没有SMIL文件，加载该路径的全部图片

	}
	if (m_cDefTxtContent.size() >0)
	{
		m_cDefTxtContent.clear()		;//没有SMIL文件，加载文本
	}
	m_uDefTxtTime	= 0				;//文本显示了多长时间
	m_uDefImageTime	= 0				;//显示的时间
	m_uDefTxtItem	= 0				;//正在
	m_uDefImageItem	= 0				;//显示的时间

	m_cMmsRegion.clear();
	m_cMmsPar.clear();
	m_cMmsLayOut.regions.clear();

}
void MmsShow::InitialDefRegion()
{
	if (m_cMmsRegion.size() > 0)
	{
		m_cMmsRegion.clear();
	}
	MMS::MMSWarp::MMS_REGION		region			;//文本
	region.id		= "text"						;
	region.width	= 290							;
	region.height	= 200							;
	region.fit		= ""							;
	region.left		= 200							;
	region.top		= 0								;
	m_cMmsRegion.push_back(region)					;		

	MMS::MMSWarp::MMS_REGION		region1			;//图片
	region1.id		= "image"		;
	region1.width	= 200			;
	region1.height	= 200			;
	region1.fit		= ""			;
	region1.left	= 0				;
	region1.top		= 0				;
	m_cMmsRegion.push_back(region1)	;

}
void MmsShow::InitialDefPar()
{	
	MMS::MMSWarp::MMS_SRC		mmspar					;//文本
	std::wstring path	= m_cImagePath					;
	mmspar.name			= ToUTF8(path)					;
	mmspar.region		= "text"						;
	mmspar.type			= MMS::MMSWarp::stText			;
	mmspar.dur			= 0								;
	mmspar.begin		= 0								;
	mmspar.end			= 0								;
//	m_cMmsPar.duration = 0			;
//	m_cMmsPar.srcs.push_back(mmspar);
}

void MmsShow::InitialRegion()
{
	//
	MMS::MMSWarp::MMS_REGION		region			;//文本
	MMS::MMSWarp::MMS_REGION		region1			;//图片


	region.id		= "text"						;
	region.width	= 290							;
	region.height	= 200							;
	region.fit		= ""							;
	region.left		= 200							;
	region.top		= 0								;

	region1.id		= "image"						;
	region1.width	= 200							;
	region1.height	= 200							;
	region1.fit		= ""							;
	region1.left	= 0								;
	region1.top		= 0								;

	// 目前只能显示一张图片，一个文本
	for (int j = 0 ; j < m_cMmsPar[m_uMmsParPlay].srcs.size();j++)
	{   
		if (m_cMmsPar[m_uMmsParPlay].srcs[j].region.compare("image") == 0 )
		{
			if (m_cMmsLayOut.regions["image"].width < 160 && m_cMmsLayOut.regions["image"].height <120)//没有宽度，使用默认的
			{   
				region1.width	= 200									;
				region1.height	= 200									;
			}
			else
			{  
				region1.width	= 200									;
				region1.height	= 200									;
			}
			region1.id		= "image"									;
			region1.fit		= ""										;
			region1.left	= 0		;
			region1.top		= 0		;

		//	region1.left	= m_cMmsLayOut.regions["image"].left		;
		//	region1.top		= m_cMmsLayOut.regions["image"].top			;
		}
	
		if (m_cMmsPar[m_uMmsParPlay].srcs[j].region.compare("text") == 0 )
		{
			//Text
			if (m_cMmsLayOut.regions["text"].width <160 || m_cMmsLayOut.regions["text"].height <120)//没有宽度，使用默认的
			{

				region.width	= 290									;
				region.height	= 200									;
			}
			else
			{
				region.width	= m_cMmsLayOut.regions["text"].width	;
				region.height	= m_cMmsLayOut.regions["text"].height	;								 

			}
			region.id			= "text"								;
			region.fit			= ""									;
			region.left			= 200		;							
			region.top			= 0			;

//			region.left			= m_cMmsLayOut.regions["text"].left		;							
// 			region.top			= m_cMmsLayOut.regions["text"].top		;

		}
		
	}
	
	if ( j == m_cMmsPar[m_uMmsParPlay].srcs.size())
	{
		m_cMmsRegion.push_back(region)	;
		m_cMmsRegion.push_back(region1)	;
	}
		

}
void MmsShow::GetHouZhui(CString &houzui,CString const fl)
{
	CString temp = fl ;
	if (::GetFileAttributes(temp + L".jpg") != 0xFFFFFFFF)
	{
		houzui = L".jpg" ;
	}

	if (::GetFileAttributes(temp + L".png") != 0xFFFFFFFF)
	{
		houzui = L".png" ;
	
	}

	if (::GetFileAttributes(temp + L".bmp") != 0xFFFFFFFF)
	{
		houzui = L".bmp" ;
	
	}	
	if (::GetFileAttributes(temp + L".gif") != 0xFFFFFFFF)
	{
		houzui = L".gif" ;	
	}
					
}
void MmsShow::GetAllFileInfo(std::wstring &image,std::wstring &text,std::wstring &aduio,std::wstring &vedio)
{   
	wchar_t buf[1000];
	memset(buf,0,2000);
	m_cTextView.GetWindowText(buf,1000)	;
	m_cTxtContent = buf				;
	image = m_cImagePath			;
	text  = m_cTxtContent			;
	aduio = m_cAudioName			;
	vedio = m_cVedioName			;
	
}
CString MmsShow::GetFileName(CString const &allpath)
{   
	//从CString的全路径取得filename
	CString str = allpath;
	int a = str.Find(L"/");
	while (str.Find(L"/") != -1)
	{   
		int nstart = str.Find(L"/");
		str = str.Mid(nstart + 1);	
	}
	return str ;
}
void MmsShow::GetMmsRegionInfo()
{
    m_uParDur	= m_cMmsPar[m_uMmsParPlay].duration ;//一个par的显示时间
	m_cTxtContent	= L"";
	m_cImagePath	= L"";
	for (int j = 0 ; j < m_cMmsPar[m_uMmsParPlay].srcs.size();j++)
	{    		
		if (m_cMmsPar[m_uMmsParPlay].srcs[j].region.compare("image") == 0 )
		{
			//找图片的Region
			m_uImageDur		=  m_cMmsPar[m_uMmsParPlay].srcs[j].end ;
			std::string	 s	=  m_cMmsPar[m_uMmsParPlay].srcs[j].name;
			std::wstring ws	=  FromUTF8(s);
			CString fl		=  ws.c_str();
			if (fl.Find(L".jpg") > 0 || fl.Find(L".JPG") > 0 ||
				fl.Find(L".png") > 0 || fl.Find(L".PNG") > 0 ||
				fl.Find(L".bmp") > 0 || fl.Find(L".BMP") > 0 ||
				fl.Find(L".gif") > 0 ||	fl.Find(L".GIF") > 0)//判断从SMIL解析的文件是否有后缀名
			{
			//	m_cImagePath	=	 m_cSmilPath+	fl			;
				m_cImagePath	=	 fl			;
			}
		//	else//没有后缀名自己查找该图片
		//	{
		//		CString allfl	=	m_cSmilPath + fl				;
		//		CString houzhui =	L"";
		//		GetHouZhui(houzhui,allfl)							;
		//		m_cImagePath	=	allfl+ houzhui					;
		//	}

		}
		
		if (m_cMmsPar[m_uMmsParPlay].srcs[j].region.compare("text") == 0 )
		{
			//Text
			m_uTxtDur			=  m_cMmsPar[m_uMmsParPlay].srcs[j].end		;
			std::string	 s		=  m_cMmsPar[m_uMmsParPlay].srcs[j].name	;
			std::wstring wsr	=  FromUTF8(s);
			CString	 fl			=	wsr.c_str() ;//文件名
		//	if (fl.Find(L".txt") <= 0 )//文本没有后缀名自己加
		//	{
		//		fl = fl + L".txt" ;
		//	}
	
			CFile f;
			CFileException e;
			char wr[1000];
			if( f.Open( fl, CFile::modeRead, &e ) )
			{
				int length = f.GetLength() ;
				memset(wr,0,1000);
				f.Read(wr,length);
				wr[length] = '\0';				
				//m_cTxtContent = wr;	
				wchar_t uicode[1000] ;//转化成uicode
				memset(uicode,0,2000);
				int index = 0 ;
				int count = strlen(wr);//
				int iNum =0 ;
				int offset =0 ;
				while( offset!= -1 && index < count)
				{
					offset  = utf82unicode((unsigned char *)wr,index,count,uicode[iNum]);
					index += offset;
					iNum++;					
				}
				m_cTxtContent = uicode ;
			
				f.Close();
			}
			
			
		}
	
	}

	if (m_cImagePath != L"")//每次检查按钮是否可用
	{
		m_btnPicture.ShowWindow(TRUE);
	}
	else
	{
		m_btnPicture.ShowWindow(FALSE);
	}

}
void MmsShow::GetDefRegionInfo()
{
	int a = m_cDefTxtContent.size() ;
	if ( a > 0 )
	{
		m_cTxtContent  = m_cDefTxtContent[m_uDefTxtItem];
	}
	
	int b = m_cDefImagePath.size() 	;
	if (b > 0 )
	{
		m_cImagePath  = m_cDefImagePath[m_uDefImageItem];
	}

}

void MmsShow::SetMmsEdit()
{
	Clear();
	m_uState  =		2 ;
	m_cTextView.SetReadOnly(FALSE);
	m_btnPicture.SetWindowText(L"插入图片");
	m_btnMusic.SetWindowText(L"插入音乐");
	m_btnVedio.SetWindowText(L"插入视频");
	InitialDefRegion();
	SetImageShow();
	SetTextShow();

}
void MmsShow::SetMmsRead()
{	
	m_uState	=    1;
	m_cTextView.SetReadOnly(TRUE);
	m_btnPicture.SetWindowText(L"保存图片");
	m_btnMusic.SetWindowText(L"保存音乐");
	m_btnVedio.SetWindowText(L"保存视频");
}

void MmsShow::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
		
	if (nIDEvent == PAR_SHOW_TIMER)
	{  
		if (m_uParTime >= m_uParDur/TIME_BASE)
		{   
			KillTimer(PAR_SHOW_TIMER);
			KillTimer(TXT_TIMER);
			KillTimer(IMAGE_TIMER);
			if (m_cMmsPar.size() == 1)
			{
				return ;
			}
			m_cPic.ShowWindow(FALSE);
			m_cTextView.ShowWindow(FALSE);
			m_uParTime		=	0		;
			m_cImageTime	=	0		;
			m_uTxtTime		=	0		;
			m_uMmsParPlay++				;//找下一个par
			
			if (m_uMmsParPlay  > m_cMmsPar.size() -1)
			{	
				m_uMmsParPlay		= 0		;
				m_uImageDur			= 0		;
				m_cImagePath		= L""	;
				m_uTxtDur			= 0		;
				m_cTxtContent		= L""	;
				GetMmsRegionInfo();
				InitialRegion();
				SetImageShow();
				SetTextShow();
				SetTimer(PAR_SHOW_TIMER,1000,NULL);
				SetTimer(IMAGE_TIMER,1000,NULL);
				SetTimer(TXT_TIMER,1000,NULL);

			}
			else//读取下一个par
			{	
				m_uImageDur			= 0		;
				m_cImagePath		= L""	;
				m_uTxtDur			= 0		;
				m_cTxtContent		= L""	;
				GetMmsRegionInfo();
				InitialRegion();
				SetImageShow();
				SetTextShow();
				SetTimer(PAR_SHOW_TIMER,1000,NULL);
				SetTimer(IMAGE_TIMER,1000,NULL);
				SetTimer(TXT_TIMER,1000,NULL);

			}

		}
		else
		{
			m_uParTime++;	
		}
	}
	else if (nIDEvent == IMAGE_TIMER)
	{
		if ( 0 == m_cImagePath.GetLength())//路径不存在
		{
			KillTimer(IMAGE_TIMER);
			m_cPic.ShowWindow(false);
			return ;
		}
		
		if (m_cImageTime > m_uImageDur/TIME_BASE)
		{
			KillTimer(IMAGE_TIMER);
			m_cImagePath	= L"";
			SetImageShow();
		}
		else
		{
			m_cImageTime++ ;
		}		
		
	}
	else if(nIDEvent == TXT_TIMER)
	{
		if ( 0 == m_cTxtContent.GetLength())//没内容
		{
			KillTimer(TXT_TIMER);
			return ;
		}
		
		if (m_uTxtTime > m_uTxtDur /TIME_BASE)
		{
			KillTimer(TXT_TIMER);
			m_cTxtContent	= L"";
			SetTextShow();
		}
		else
		{
			m_uTxtTime++ ;
		}
	}
	else if (nIDEvent == DEF_IMAGE_SHOW)
	{
		if (m_cDefImagePath.size() == 0)
		{
			KillTimer(DEF_IMAGE_SHOW);
			return ;
		}
		
		if (m_cDefImagePath.size() == 1)
		{
			KillTimer(DEF_IMAGE_SHOW);
			return ;
		}
		
		if (m_uDefImageItem >= m_cDefImagePath.size() -1 )
		{
			m_uDefImageItem = 0 ;	
		}
		else
		{
			m_uDefImageItem++	;
		}
		GetDefRegionInfo()	;
		InitialDefRegion()	;
		SetImageShow()		;

	}
	else if (nIDEvent == DEF_TXT_SHOW)
	{
		if (m_cDefTxtContent.size() == 0)
		{
			KillTimer(DEF_TXT_SHOW);
			return ;
		}
		
		if (m_cDefTxtContent.size() == 1)
		{
			KillTimer(DEF_TXT_SHOW);
			return ;
		}
		
		if (m_uDefTxtItem >= m_cDefTxtContent.size() -1 )
		{
			m_uDefTxtItem = 0 ;	
		}
		else
		{
			m_uDefTxtItem++	;
		}
		GetDefRegionInfo()	;
		InitialDefRegion()	;
		SetTextShow()		;
	}

	CWnd::OnTimer(nIDEvent);


}

void MmsShow::OnBtnPicture()
{	
//	SetMmsEdit();
	if (1 == m_uState)//只读状态,保存图片有两个路径需要选择。
	{   
		if (m_cImagePath !=L"")
		{
			m_picPathDlg.ShowWindow(TRUE);
			m_picPathDlg.GetMainWnd(this);
			m_picPathDlg.SetState(1);
			CString name = GetFileName(m_cImagePath);
			m_picPathDlg.SetCopyPath(m_cImagePath,name);
		}
	}
	else if (2 == m_uState)//编辑，插入
	{
		//找出FlashDrv/3G/mms/my_photo/路径下的图
		//插入成功显示图片
		m_picPathDlg.ShowWindow(TRUE);
		m_picPathDlg.GetMainWnd(this);
		m_picPathDlg.SetState(2);

	}
	else if (3 == m_uState)//删除
	{
		ClosePicture();
		m_btnPicture.SetWindowText(L"插入图片");
		m_uState = 2;
	}

}

void MmsShow::OnBtnMusic()
{
	if (1 == m_uState)//只读状态
	{
	/*	CreateDirectoryW(L"FlashDrv/3G/mms/my_music/", 0);
		CString name = L"FlashDrv/3G/mms/my_music/" + GetFileName(m_cImagePath);
		if ( 0 == CopyFile(m_cImagePath,name,FALSE))
		{
			
		}
	*/

	}
	else if (2 == m_uState)//编辑，插入
	{
		//找出FlashDrv/3G/mms/my_photo/路径下的图
		
	}
	else if (3 == m_uState)//删除
	{
		
	}
}
void MmsShow::OnBtnVedio()
{
	if (1 == m_uState)//只读状态
	{
	/*	CreateDirectoryW(L"FlashDrv/3G/mms/my_vedio/", 0);
		CString name = L"FlashDrv/3G/mms/my_vedio/" + GetFileName(m_cImagePath);
		if ( 0 == CopyFile(m_cImagePath,name,FALSE))
		{
			
		}
	*/
	}
	else if (2 == m_uState)//编辑，插入
	{
		//找出FlashDrv/3G/mms/my_photo/路径下的图		
	}
	else if (3 == m_uState)//删除
	{
		
	}
}

HBRUSH MmsShow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);	
	return hbr;
}


#include "stdafx.h"
//#include "ImageViewer.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CImageViewer, CStatic)
	//{{AFX_MSG_MAP(CImageViewer)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
//	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CImageViewer::CImageViewer()
{
	m_cGifShow			= NULL ;
	m_bSetRegion		= false ; //add by qi 
	m_bAutoWrapping		= TRUE;
	m_bAutoAdjustFont	= TRUE;
	m_pPicture			= new CxImage();
	m_nFrameCount		= 0;
	m_pThread			= NULL;
	m_hExitEvent		= CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hBitmap			= NULL;
	m_hMemDC			= NULL;

	m_hDispMemDC		= NULL;
	m_hDispMemBM		= NULL;
	m_hDispOldBM		= NULL;

	m_bTransparentBk	= FALSE;
	m_bNotifyParent		= FALSE;
	m_bIsInitialized	= FALSE;
	m_bExitThread		= FALSE;
	m_bIsPlaying		= FALSE;
	m_bIsGIF			= FALSE;
	m_clrBackground		= RGB(255,255,255); // white by default
	m_nGlobalCTSize		= 0;
	m_nCurrOffset		= 0;
	m_nCurrFrame		= 0;
	m_nDataSize			= 0;
	m_PictureSize.cx	= 0;
	m_PictureSize.cy	= 0;
	SetRect(&m_PaintRect,0,0,0,0);


	m_rgbBkgnd		  = ::GetSysColor(COLOR_BTNFACE);
	m_rgbText		  = ::GetSysColor(COLOR_WINDOWTEXT);
	m_crHiColor		  =	m_rgbBkgnd;
	m_crLoColor		  =	m_rgbBkgnd;
	m_pBrush          = new CBrush(m_rgbBkgnd);

}

CImageViewer::~CImageViewer()
{
	UnLoad();
	CloseHandle(m_hExitEvent);


	// Clean up our Brush
	if (m_pBrush){
		delete m_pBrush;
		m_pBrush = NULL;
	}

	// Clean our main Picture
	if (m_pPicture){
		delete m_pPicture;
		m_pPicture = NULL;
	}


}

void CImageViewer::UnLoad()
{

	Stop();
	GifStop();
	if (m_pPicture){
		m_pPicture->Destroy();
		//m_pPicture = NULL;
	};

	std::vector<TFrame>::iterator it;
	for (it=m_arrFrames.begin();it<m_arrFrames.end();it++){
		(*it).m_pPicture->Destroy();
		if ( (*it).m_pPicture ){
			delete (*it).m_pPicture;
		}
	}

	m_arrFrames.clear();

	if (m_hMemDC){
		SelectObject(m_hMemDC,m_hOldBitmap);
		::DeleteDC(m_hMemDC); ::DeleteObject(m_hBitmap);
		m_hMemDC  = NULL;
		m_hBitmap = NULL;
	};

	if (m_hDispMemDC){
		SelectObject(m_hDispMemDC,m_hDispOldBM);
		::DeleteDC(m_hDispMemDC); ::DeleteObject(m_hDispMemBM);
		m_hDispMemDC  = NULL;
		m_hDispMemBM = NULL;
	};

	//SetRect(&m_PaintRect,0,0,0,0);
	m_pThread		   = NULL;
	m_bIsInitialized   = FALSE;
	m_bExitThread	   = FALSE;
	m_bIsGIF		   = FALSE;
	m_clrBackground    = RGB(255,255,255); // white by default
	m_nCurrOffset	   = 0;
	m_nCurrFrame	   = 0;
	m_nDataSize		   = 0;
	m_nSizeMode		   = FitControl;
}

BOOL CImageViewer::PrepareDC(int nWidth, int nHeight)
{
//	change by qi	
//	if (m_nSizeMode == OriginalSize)
//		SetWindowPos(NULL,0,0,nWidth,nHeight,SWP_NOMOVE | SWP_NOZORDER);

	HDC hWinDC = ::GetDC(m_hWnd);
	if (!hWinDC) return FALSE;

	m_hMemDC = CreateCompatibleDC(hWinDC);
	if (!m_hMemDC)
	{
		::ReleaseDC(m_hWnd,hWinDC);
		return FALSE;
	};

	m_hBitmap  = CreateCompatibleBitmap(hWinDC,nWidth,nHeight);
	if (!m_hBitmap)
	{
		::ReleaseDC(m_hWnd,hWinDC);
		::DeleteDC(m_hMemDC);
		return FALSE;
	};

	m_hOldBitmap = reinterpret_cast<HBITMAP> (SelectObject(m_hMemDC,m_hBitmap));

	// fill the background
	
	m_clrBackground = GetSysColor(RGB(255,0,0));
	RECT rect = {0,0,nWidth,nHeight};
	FillRect(m_hMemDC,&rect,(HBRUSH)(COLOR_WINDOW));

	::ReleaseDC(m_hWnd,hWinDC);
	m_bIsInitialized = TRUE;
	return TRUE;

}


BOOL CImageViewer::RedrawWindow()
{
	/*CRect rect;

	GetClientRect( rect );
	InvalidateRect( rect );*/
	Invalidate();
	return TRUE;
}



BOOL CImageViewer::SetBitmap(HBITMAP hBitmap, EImageSize Emode)
{
	UnLoad();

	m_nSizeMode = Emode;

	bool bRet = m_pPicture->CreateFromHBITMAP(hBitmap);
	if (!bRet) return FALSE;

	m_PictureSize.cx = m_pPicture->GetWidth();
	m_PictureSize.cy = m_pPicture->GetHeight();
	m_nFrameCount = m_pPicture->GetNumFrames();

	bRet = (PrepareDC(m_PictureSize.cx, m_PictureSize.cy)!=0);
	if (!bRet) return FALSE;

	DrawBitmap();

	return TRUE;
}


void CImageViewer::SetOriginalSize()
{


}

BOOL CImageViewer::SetBitmap(UINT nIDResource, EImageSize Emode)
{
	UnLoad();

	m_nSizeMode = Emode;

	HBITMAP hBmp = ::LoadBitmap( AfxGetInstanceHandle(),
		MAKEINTRESOURCE(nIDResource));

	return SetBitmap(hBmp, Emode);

}

BOOL CImageViewer::SetBitmap(LPCTSTR lpszFileName, EImageSize Emode)
{
	UnLoad();

	m_nSizeMode = Emode;

	TRACE(_T("m_arrFrames Size = %d\n"), m_arrFrames.size());
	CString csExt = lpszFileName;
	int nDotPos = csExt.ReverseFind('.');
	if (nDotPos != -1)
		csExt = csExt.Mid(++nDotPos);

	int nImgType = FindType(csExt);
	if (nImgType == 2)
	{
		m_bIsGIF = TRUE;
		return	FALSE;
	}

	// Load Image From disk
	bool bRet = m_pPicture->Load(lpszFileName, nImgType);
	if (!bRet) return FALSE;

	m_PictureSize.cx = m_pPicture->GetWidth();
	m_PictureSize.cy = m_pPicture->GetHeight();


	TFrame	frame;
	UINT	nCurFrame = 0;

	m_nFrameCount = m_pPicture->GetNumFrames();
	for(int i=0; i < (int)m_nFrameCount; i++){
		CxImage *newImage = new CxImage();
		newImage->SetFrame(i);
		newImage->Load(lpszFileName,nImgType);

		frame.m_pPicture = newImage;
		frame.m_frameOffset.cx= 0;
		frame.m_frameOffset.cy = 0;
		frame.m_frameSize.cx = newImage->GetWidth();
		frame.m_frameSize.cy = newImage->GetHeight();
		frame.m_nDelay = newImage->GetFrameDelay();
		frame.m_nDisposal = 1;

		m_arrFrames.push_back(frame);
	}	
	
	bRet = (PrepareDC(m_pPicture->GetWidth(), m_pPicture->GetHeight())!=0);
	if (!bRet) return FALSE;

	DrawBitmap();
	return TRUE;
}

BOOL CImageViewer::SetBitmap(const CByteArray& ImgBuffer, CString csFileName, EImageSize Emode)
{
	UnLoad();

	m_nSizeMode = Emode;

	CString csExt = csFileName;
	int nDotPos = csExt.ReverseFind('.');
	if (nDotPos != -1)
		csExt = csExt.Mid(++nDotPos);

	int nImgType = FindType(csExt);
	if (nImgType == 2)
		m_bIsGIF = TRUE;

	ULONG nBufferLen = ImgBuffer.GetSize();
	BYTE* pBuffer = new BYTE [ nBufferLen ];
	memset(pBuffer, 0, nBufferLen);

	for (ULONG i = 0; i < nBufferLen; i++)
		pBuffer[i] = ImgBuffer.GetAt(i);

	CxMemFile memfile((BYTE*)pBuffer, nBufferLen);
	bool bRet = m_pPicture->Decode(&memfile,nImgType);
	m_PictureSize.cx = m_pPicture->GetWidth();
	m_PictureSize.cy = m_pPicture->GetHeight();

	TFrame	frame;
	UINT	nCurFrame = 0;

	m_nFrameCount = m_pPicture->GetNumFrames();
	for(int j=0; j < (int)m_nFrameCount; j++){
		CxImage *newImage = new CxImage();
		CxMemFile memfile((BYTE*)pBuffer, nBufferLen);
		newImage->SetFrame(j);
		bRet = newImage->Decode(&memfile,nImgType);

		frame.m_pPicture = newImage;
		frame.m_frameOffset.cx= 0;
		frame.m_frameOffset.cy = 0;
		frame.m_frameSize.cx = newImage->GetWidth();
		frame.m_frameSize.cy = newImage->GetHeight();
		frame.m_nDelay = newImage->GetFrameDelay();
		frame.m_nDisposal = 1;

		m_arrFrames.push_back(frame);
	}
	bRet = (PrepareDC(m_pPicture->GetWidth(), m_pPicture->GetHeight())!=0);
	if (!bRet) return FALSE;

	DrawBitmap();

	if (pBuffer){
		delete [] pBuffer;
	}

	return TRUE;
}




BOOL CImageViewer::DrawBitmap()
{
	if (!m_bIsInitialized)
		return FALSE;

	if (IsAnimatedGIF()){
	// the picture needs animation
	// we'll start the thread that will handle it for us
	
		m_pThread = AfxBeginThread( _ThreadAnimation, 
									this, 
									THREAD_PRIORITY_NORMAL, 
									0, 
									CREATE_SUSPENDED, 
									NULL);
		if (!m_pThread){
			TRACE(_T("Draw: Couldn't start a GIF animation thread\n"));
			return FALSE;
		} 
		else{ 
			m_pThread->ResumeThread();
		}
	} 
	else{
		if ( m_pPicture ){
			long hmWidth  = m_pPicture->GetWidth();
			long hmHeight = m_pPicture->GetHeight();
			if (m_pPicture->Draw2(m_hMemDC, 0, 0, m_PictureSize.cx, m_PictureSize.cy) == TRUE){
				Invalidate(FALSE);
				return TRUE;
			}
		}
	}

	return FALSE;
}

SIZE CImageViewer::GetSize() const
{
	return m_PictureSize;
}




//============================================================================
// GIF ANIMATION THREAD
//============================================================================


UINT CImageViewer::_ThreadAnimation(LPVOID pParam)
{
	ASSERT(pParam);
	CImageViewer *pPic = reinterpret_cast<CImageViewer *> (pParam);

	pPic->m_bIsPlaying = TRUE;
	pPic->ThreadAnimation();
	pPic->m_bIsPlaying = FALSE;

	// this thread has finished its work so we close the handle
	//CloseHandle(pPic->m_hThread);
	// and init the handle to zero (so that Stop() doesn't Wait on it)
	//pPic->m_hThread = 0;
	
	return 0;
}

void CImageViewer::ThreadAnimation()
{
	while (!m_bExitThread)
	{
		if (m_arrFrames[m_nCurrFrame].m_pPicture)
		{
			long hmWidth = m_arrFrames[m_nCurrFrame].m_pPicture->GetWidth();
			long hmHeight= m_arrFrames[m_nCurrFrame].m_pPicture->GetHeight();
			// add by qi 缓存DC
			CRect rect(0,0,m_pPicture->GetWidth(),m_pPicture->GetHeight());
			FillRect(m_hMemDC,&rect,(HBRUSH)(COLOR_WINDOW));

			if (m_arrFrames[m_nCurrFrame].m_pPicture->Draw2(m_hMemDC,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cx,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
				m_arrFrames[m_nCurrFrame].m_frameSize.cx,
				m_arrFrames[m_nCurrFrame].m_frameSize.cy) == TRUE)
			{   
				Invalidate(FALSE);
			};

			if (m_bExitThread) break;

			// if the delay time is too short (like in old GIFs), wait for 100ms
			if (m_arrFrames[m_nCurrFrame].m_nDelay < 5)
				WaitForSingleObject(m_hExitEvent, 100);
			else
				WaitForSingleObject(m_hExitEvent, 10*m_arrFrames[m_nCurrFrame].m_nDelay);

			if (m_bExitThread) break;

		}
		m_nCurrFrame++;
		if (m_nCurrFrame == m_arrFrames.size())
		{
			m_nCurrFrame = 0;
		
			// init the screen for the first frame,
			
			//  change by qi 2009_05_14
			//HBRUSH hBrush = CreateSolidBrush(m_clrBackground);
			//if (hBrush)
			//{
			//	RECT rect = {0,0,m_PictureSize.cx,m_PictureSize.cy};
			//	FillRect(m_hMemDC,&rect,hBrush);
			//	DeleteObject(hBrush);
			//}

		}
	}
}


void CImageViewer::Stop()
{
	m_bIsPlaying = FALSE;
	m_bExitThread = TRUE;
	SetEvent(m_hExitEvent);
		
	// make it possible to Draw() again
	ResetEvent(m_hExitEvent);
	m_bExitThread = FALSE;
}


BOOL CImageViewer::IsAnimatedGIF() const
{
	return (m_bIsGIF && (m_arrFrames.size() > 1));
}

BOOL CImageViewer::IsPlaying() const
{
	return m_bIsPlaying;
}

int CImageViewer::GetFrameCount() const
{
	if (!IsAnimatedGIF())
		return 0;

	return m_arrFrames.size();
}

COLORREF CImageViewer::GetBkColor() const
{
	return m_clrBackground;
}


//============================================================================
// WINDOWS MESSAGE HANDLING
//============================================================================

void CImageViewer::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(rect);
	

	//change by qi 2009_05_14
	CBrush backBrush(RGB(255,255,255));
	dc.FillRect(&rect, &backBrush);

	// Render background color
	//if (m_fillmode == Normal)
	//	dc.FillRect(&rect, m_pBrush);
	//else
	//	DrawGradientFill(&dc, &rect, m_fillmode);

	// Render Bitmap
	if (m_bSetRegion)//截图
	{
		SetWindowPos(NULL,m_cImagePt.x,m_cImagePt.y,m_Imagesize.cx,m_Imagesize.cy,SWP_NOSIZE);
		CBrush backBrush(RGB(255,255,255));
		dc.FillRect(&rect, &backBrush);
		::StretchBlt(dc.m_hDC, 0, 0, rect.Width(), rect.Height(),
			m_hMemDC, m_cImagePt.x, m_cImagePt.y, m_Imagesize.cx, m_Imagesize.cy, SRCCOPY);
	}
	else
	{	
		if (rect.Width() > m_PictureSize.cx && rect.Height() > m_PictureSize.cy )//图片区域小于设置的区域
		{   
			UINT xbegin = (rect.Width() - m_PictureSize.cx)/2	;
			UINT ybegin	= (rect.Height() - m_PictureSize.cy)/2	;
			::StretchBlt(dc.m_hDC, xbegin, ybegin, m_PictureSize.cx, m_PictureSize.cy,
				m_hMemDC, 0, 0, m_PictureSize.cx, m_PictureSize.cy, SRCCOPY);
		}
		else if (rect.Width() <= m_PictureSize.cx && m_PictureSize.cy <rect.Height() )//
		{   
			UINT ybegin	= (rect.Height() - m_PictureSize.cy)/2	;
			::StretchBlt(dc.m_hDC, 0, ybegin, rect.Width(), m_PictureSize.cy,
				m_hMemDC, 0, 0, m_PictureSize.cx, m_PictureSize.cy, SRCCOPY);

		}
		else if (rect.Width() > m_PictureSize.cx && m_PictureSize.cy >= rect.Height() )
		{   
			UINT xbegin = (rect.Width() - m_PictureSize.cx)/2	;
			::StretchBlt(dc.m_hDC, xbegin, 0, m_PictureSize.cx, rect.Height(),
				m_hMemDC, 0, 0, m_PictureSize.cx, m_PictureSize.cy, SRCCOPY);
	
		}
		else if (rect.Width() <= m_PictureSize.cx && m_PictureSize.cy >=rect.Height() )
		{
			::StretchBlt(dc.m_hDC, 0, 0, rect.Width(), rect.Height(),
				m_hMemDC, 0, 0, m_PictureSize.cx, m_PictureSize.cy, SRCCOPY);
	
		}
		
	//	::StretchBlt(dc.m_hDC, 0, 0, rect.Width(), rect.Height(),
	//	m_hMemDC, 0, 0, m_PictureSize.cx, m_PictureSize.cy, SRCCOPY);
	}	

	//在图片下面画底线框
	COLORREF m_bkRGB = ::GetSysColor(COLOR_STATIC);
	CPen	pen;
	pen.CreatePen(PS_SOLID,1,m_bkRGB);
	dc.SelectObject(pen.m_hObject);
	dc.MoveTo(rect.left,rect.bottom -1);//线自己占一个像素
	dc.LineTo(rect.right,rect.bottom -1);

}


void CImageViewer::OnDestroy()
{
	UnLoad();
	CStatic::OnDestroy();
}

void CImageViewer::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
	Invalidate();
}

BOOL CImageViewer::SetPaintRect(const RECT *lpRect)
{
	return CopyRect(&m_PaintRect, lpRect);
}

BOOL CImageViewer::GetPaintRect(RECT *lpRect)
{
	return CopyRect(lpRect, &m_PaintRect);
}

int CImageViewer::FindType(const CString& ext)
{
//	return CxImage::GetTypeIdFromName(ext);
	if(ext.Compare(L"gif") == 0)
	{
		return 2;
	}
}

void CImageViewer::SetImageFilename(std::wstring const& filename)
{
	SetBitmap(filename.c_str());
	//SetBitmap(L"\\Program Files\\TestCximageLib\\5.gif"); 
	if (m_bIsGIF)//是GIF先用别的显示，现在的cximage不支持gif
	{
		SetGifShow(filename.c_str());
	//	SetGifShow(L"\\Program Files\\TestCximageLib\\5.gif");
	}
}

void CImageViewer::SetImageRegion(CPoint const& origin, CSize const& size)
{
	m_cImagePt		=	origin	;
	m_Imagesize		=	size	;	
	m_bSetRegion	=	true	;
}

BOOL CImageViewer::SetGifShow(LPCTSTR lpszResName)
{

	HDC hdc = ::GetDC(m_hWnd);
	m_cGifShow = new CGIFShow(hdc);
	if(m_cGifShow->Load(lpszResName))
	{
		//	RECT rcClient;
		//	::GetClientRect(m_hWnd, &rcClient);
		m_cGifShow->SetMainWnd(this);
		m_cGifShow->SetPosition(0,0);
		m_cGifShow->Play();
		return TRUE ;
	}
	
}
void CImageViewer::SetImagePos(CRect &rct)
{
	SetWindowPos(NULL,rct.left,rct.top,rct.right,rct.bottom, SWP_NOSIZE);
}

void CImageViewer::GifStop()
{
	if (m_cGifShow != NULL)
	{
		m_cGifShow->Stop();
		delete m_cGifShow ;
		m_cGifShow = NULL ;

	}

}

// TextViewer.cpp : implementation file
//

#include "stdafx.h"
//#include "MmsView.h"
//#include "TextViewer.h"


// CTextViewer

IMPLEMENT_DYNAMIC(CTextViewer, CStatic)

CTextViewer::CTextViewer():m_cwStrContent(L"")
{
	m_bkRGB			= RGB(128,128,128);
	m_txtRGB		= RGB(0,0,0);
	m_uiBitmapID	= 0 ;
}

CTextViewer::~CTextViewer()
{

}


BEGIN_MESSAGE_MAP(CTextViewer, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CTextViewer::SetTextContent(std::wstring const& content)
{
	m_cwStrContent = content ;
}

void CTextViewer::SetFont(const LOGFONT &font)
{
	if (m_cFontText.m_hObject)
	{	
		m_cFontText.Detach();
		DeleteObject(m_cFontText.m_hObject);
	}
	VERIFY(m_cFontText.CreateFontIndirect(&font));
}

void CTextViewer::OnPaint()
{
	CStatic::OnPaint();

	CRect rt;
	GetClientRect(&rt);
	OnDraw(rt, m_cwStrContent);
	return;
}

void CTextViewer::OnDraw(CRect rt_, std::wstring ws)
{
	HDC dc = ::GetDC(m_hWnd);
	::SetBkMode(dc, TRANSPARENT); //这里为什么会有个小黑点,原来是没加载位图

	HFONT oldFont = (HFONT)::SelectObject(dc, m_cFontText.m_hObject);

	CRect rt = rt_;
	CRect rect_;
	GetClientRect(&rect_);
	if(rt.Width() == 0 && rt.Height() == 0)
		rt = rect_;


//	COLORREF m_bkRGB = ::GetSysColor(COLOR_STATIC);
	CBrush	 bBr	 =	m_bkRGB; 
	::FillRect(dc, &rt, (HBRUSH)bBr.m_hObject);

	
//CBrush brush;
//	brush.CreateSysColorBrush(COLOR_GRAYTEXT);
//	::FillRect(dc, &rect_, (HBRUSH)brush.m_hObject);
	

	//贴图
	if(m_uiBitmapID > 0)
	{
		HDC memDC;//内存设备场景
		HBITMAP cb;//位图类对象
		CRect rect;
		GetClientRect(&rect);
		cb = ::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(m_uiBitmapID));//载入资源
		memDC = ::CreateCompatibleDC (dc);//创建内存设备上下文
		HBITMAP oldbitmap = (HBITMAP)::SelectObject (memDC, cb);//将位图选入当前设备场景
		::BitBlt (dc, rect.left, rect.top, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
		::SelectObject (memDC, oldbitmap);
		::DeleteObject(cb);
		::DeleteDC(memDC);

	}
	
	::SetTextColor(dc, m_txtRGB);// 设置颜色
	SelectObject(dc, m_cFontText.m_hObject);
	CString s2 ;
	s2.Format(L"%s",ws.c_str());
	::DrawText(dc, s2, s2.GetLength(), &rt, DT_WORDBREAK | DT_LEFT);

	::SelectObject(dc, oldFont);
	::ReleaseDC(m_hWnd, dc);

}

void CTextViewer::SetTextColor(COLORREF bkcolor, COLORREF ftcolor)
{
	m_bkRGB		=	bkcolor ;
	m_txtRGB	=	ftcolor ;
}
void CTextViewer::SetTextSize(CPoint const& origin, CSize const& size)
{   
	MoveWindow(origin.x,origin.y,size.cx,size.cy);
//	SetWindowPos(NULL,origin.x,origin.y,size.cx,size.cy, SWP_NOSIZE);
}

void CTextViewer::Update()
{
	Invalidate();
}

HBRUSH CTextViewer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CStatic::OnCtlColor(pDC, pWnd, nCtlColor);	
//	HBRUSH	 bk		= (HBRUSH)::GetStockObject(WHITE_BRUSH);//白色背景粉刷
	//COLORREF	 bkclr  = ::GetSysColor(COLOR_WINDOW);
	//HBRUSH	 hbrclr	= ::CreateSolidBrush(bkclr);
	
//	COLORREF m_bkRGB = ::GetSysColor(COLOR_STATIC);

	pDC->SetBkMode(TRANSPARENT); 
	return hbr ;
//	return hbrclr;

}
// CTextViewer message handlers


#include "stdafx.h"
//#include "GifShow.h"

DWORD WINAPI ThreadFunc(CGIFShow* ptr)
{
	ptr->Play1();
	return 0;
}

void CGIFShow::Output(BYTE bit)
{
	int tmp;
	if(m_cPackedField&0x40)
	{
		if(m_x==m_iWidth)
		{
			m_x=0;
			if(m_iPass==1)m_iRow+=8;
			if(m_iPass==2)m_iRow+=8;
			if(m_iPass==3)m_iRow+=4;
			if(m_iPass==4)m_iRow+=2;
			if(m_iRow>=m_iHeight){m_iPass+=1;m_iRow=16>>m_iPass;}

		}
		tmp=m_iRow*m_iWidth1+m_x;
		m_pcBitmap[tmp]=bit;
		m_x++;
	}
	else
	{
		if(m_x==m_iWidth){m_x=0;m_y++;}
		tmp=m_y*m_iWidth1+m_x;
		m_x++;
	}
	if(tmp>m_iMaxByte)return;
	m_pcBitmap[tmp]=bit;
}

BYTE CGIFShow::GetByte(void)
{
	if(m_uReadByte>=m_uBlockSize)//
	{
		m_uBlockSize=*m_pcGifTrack++;
		m_uReadByte=0;
		m_iTotalReadByte +=m_uBlockSize+1;
		if(m_iTotalReadByte>m_iGifSize){m_iTotalReadByte-=m_uBlockSize+1;return 0xFF;}
		if(m_uBlockSize==0){m_pcGifTrack--;m_iTotalReadByte--;return 0xFF;}
	}
	m_uReadByte++;
	return *m_pcGifTrack++;
}

WORD CGIFShow::GetCode(void)
{
	UINT tmp1;
	BYTE tmp;
	tmp=1;
	if(m_uRemain>=m_uBitSize)
	{
		tmp<<=m_uBitSize;
		tmp--;
		tmp1=m_cCurentByte&tmp;
		m_cCurentByte>>=m_uBitSize;
		m_uRemain-=m_uBitSize;
	}
	else
	{
		tmp<<=m_uRemain;
		tmp--;
		tmp1=m_cCurentByte;
		m_cCurentByte=GetByte();
		tmp=1;
		if(8>=(m_uBitSize-m_uRemain))
		{
			tmp<<=(m_uBitSize-m_uRemain);
			tmp--;
			tmp1=(((UINT)(m_cCurentByte&tmp))<<m_uRemain)+tmp1;
			m_cCurentByte>>=(m_uBitSize-m_uRemain);
			m_uRemain=8-(m_uBitSize-m_uRemain);
		}
		else
		{
			tmp1=(((UINT)(m_cCurentByte))<<m_uRemain)+tmp1;
			m_cCurentByte=GetByte();
			tmp<<=m_uBitSize-m_uRemain-8;
			tmp--;
			tmp1=(((UINT)(m_cCurentByte&tmp))<<(m_uRemain+8))+tmp1;
			m_cCurentByte>>=m_uBitSize-m_uRemain-8;
			m_uRemain=8-(m_uBitSize-m_uRemain-8);
		}
	}
	return tmp1;
}
void CGIFShow::SetMainWnd(CWnd * wnd)
{
	m_pwnd = wnd	;
}

void CGIFShow::Play()
{
	HANDLE hThread;
	DWORD ThreadId;
	if(m_hWndHDC==0)return ;
	if(m_pcGif==0)return;
	if(m_EndRun==5)return;
	m_pcGifTrack=m_pcGif;
	m_iTotalReadByte=0;
	m_EndRun=5;
	hThread=CreateThread(NULL,0,(unsigned long(_stdcall*)(void*))ThreadFunc,this,0,&ThreadId);
	CloseHandle(hThread);
	return;
}

BOOL CGIFShow::Play1(void)
{
	HDC hDC,hMemDC,hMemDC1,hPauseMemDC;
	HBITMAP hOldBitmap,hOldBitmap1,hBitmap,hPreviousBitmap,hPauseBitmap;
	DWORD systimer1,systimer2,speed;
	BOOL PauseShow = FALSE;
	
	//add by qi 2009_06_05
	CRect rect;
	m_pwnd->GetClientRect(&rect);
	
	hDC = m_hWndHDC;
	hMemDC = ::CreateCompatibleDC(hDC);
	hMemDC1 = ::CreateCompatibleDC(hDC);
	hPauseMemDC = ::CreateCompatibleDC(hDC);
	m_hDC = hDC;
	hPreviousBitmap = 0;
	while(m_bLockBitmap)
	{

	}
	m_bLockBitmap = TRUE;
	if(m_hRedrawBitmap != 0)
	{
		DeleteObject(m_hRedrawBitmap);
	}
	m_hRedrawBitmap = ::CreateCompatibleBitmap(hDC,m_iGifWidth,m_iGifHeight);
	hOldBitmap1 = (HBITMAP)SelectObject(hMemDC1,m_hRedrawBitmap);
	hPauseBitmap = ::CreateCompatibleBitmap(hDC,m_iGifWidth,m_iGifHeight);
	SelectObject(hPauseMemDC,hPauseBitmap);

	::BitBlt(hMemDC1,0,0,m_iGifWidth,m_iGifHeight,hDC,m_nPosX,m_nPosY,SRCCOPY);
	SelectObject(hMemDC1,hOldBitmap1);
	m_bLockBitmap = FALSE;
	m_iDisposalMethod = DISPOSAL_NOT;
	while(1 != m_EndRun)
	{
		systimer2=systimer1 = GetTickCount();
		while(m_bLockBitmap)
		{
		}
		m_bLockBitmap = TRUE;
		hOldBitmap1 = (HBITMAP)SelectObject(hMemDC1,m_hRedrawBitmap);

		switch(m_iDisposalMethod)
		{
		case DISPOSAL_NO:
			break;
		case DISPOSAL_NOT:
			break;
		case DISPOSAL_RESTBACK:
		case DISPOSAL_RESTORE:
			hOldBitmap = (HBITMAP)SelectObject(hMemDC,hPreviousBitmap);
			::BitBlt(hMemDC1,m_iLeft,m_iTop,m_iWidth,m_iHeight,hMemDC,0,0,SRCCOPY);
			SelectObject(hMemDC,hOldBitmap);
			DeleteObject(hPreviousBitmap);
			hPreviousBitmap = 0;
			break;
		}
		m_iDisposalMethod = DISPOSAL_NO;

		if(!PauseShow)
		{
			hBitmap=NextImage();

			switch(m_iDisposalMethod)
			{
			case DISPOSAL_NO:
				break;
			case DISPOSAL_NOT:
				break;
			case DISPOSAL_RESTBACK:
			case DISPOSAL_RESTORE:
				hPreviousBitmap = ::CreateCompatibleBitmap(hDC,m_iWidth,m_iHeight);
				hOldBitmap = (HBITMAP)SelectObject(hMemDC,hPreviousBitmap);
				::BitBlt(hMemDC,0,0,m_iWidth,m_iHeight,hMemDC1,m_iLeft,m_iTop,SRCCOPY);
				SelectObject(hMemDC,hOldBitmap);
				break;
			}
			hOldBitmap = (HBITMAP)SelectObject(hMemDC,hBitmap);
			if(m_bTransparentIndex)
			{
				HBITMAP    bmAndBack, bmAndObject;
				HBITMAP    bmBackOld, bmObjectOld;
				HDC        hdcBack, hdcObject;
				COLORREF cColor;

				hdcBack = ::CreateCompatibleDC(hDC);
				hdcObject = ::CreateCompatibleDC(hDC);
				bmAndBack = CreateBitmap(m_iWidth,m_iHeight,1,1,NULL);
				bmAndObject = CreateBitmap(m_iWidth,m_iHeight,1,1,NULL);
				bmBackOld = (HBITMAP)SelectObject(hdcBack,bmAndBack);
				bmObjectOld = (HBITMAP)SelectObject(hdcObject,bmAndObject);
				cColor = SetBkColor(hMemDC,m_TransparentColor);
				::BitBlt(hdcObject,0,0,m_iWidth,m_iHeight,hMemDC,0,0,SRCCOPY);
				SetBkColor(hMemDC,cColor);
				::BitBlt(hdcBack,0,0,m_iWidth,m_iHeight,hdcObject,0,0,NOTSRCCOPY);
				::BitBlt(hMemDC1,m_iLeft,m_iTop,m_iWidth,m_iHeight,hdcObject,0,0,SRCAND);
				::BitBlt(hMemDC,0,0,m_iWidth,m_iHeight,hdcBack,0,0,SRCAND);
				::BitBlt(hMemDC1,m_iLeft,m_iTop,m_iWidth,m_iHeight,hMemDC,0,0,SRCPAINT);
				DeleteObject(SelectObject(hdcBack,bmBackOld));
				DeleteObject(SelectObject(hdcObject,bmObjectOld));
				DeleteDC(hdcBack);
				DeleteDC(hdcObject);
			}
			else
			{
				::BitBlt(hMemDC1,m_iLeft,m_iTop,m_iWidth,m_iHeight,hMemDC,0,0,SRCCOPY);
			}
			SelectObject(hMemDC,hOldBitmap);
			DeleteObject(hBitmap);
			
			//add by qi 2009_06_05
			if (rect.Width() >m_iGifWidth && rect.Height() > m_iGifHeight)
			{
				int xstart =  (rect.Width()	- m_iGifWidth)/2;
				int ystart =  (rect.Height()- m_iGifHeight)/2;
				::StretchBlt(hDC, xstart, ystart, m_iGifWidth, m_iGifHeight,
					hMemDC1, 0, 0, m_iGifWidth, m_iGifHeight, SRCCOPY);
			}
			else if (rect.Width() <= m_iGifWidth && rect.Height() > m_iGifHeight)
			{
				int ystart =  (rect.Height()- m_iGifHeight)/2;
				::StretchBlt(hDC, 0, ystart, rect.Width(), m_iGifHeight,
					hMemDC1, 0, 0, m_iGifWidth, m_iGifHeight, SRCCOPY);	
			}
			else if (rect.Width() >m_iGifWidth && rect.Height() <= m_iGifHeight)
			{
				int xstart =  (rect.Width()	- m_iGifWidth)/2;
				::StretchBlt(hDC, xstart, 0, m_iGifWidth, rect.Height(),
					hMemDC1, 0, 0, m_iGifWidth, m_iGifHeight, SRCCOPY);
	
			}
			else if (rect.Width() <= m_iGifWidth && rect.Height() <= m_iGifHeight)
			{
				::StretchBlt(hDC, 0, 0, rect.Width(), rect.Height(),
					hMemDC1, 0, 0, m_iGifWidth, m_iGifHeight, SRCCOPY);
			}
			// add over

		//	add by qi 注释
		//	::BitBlt(hDC,m_nPosX,m_nPosY,m_iGifWidth,m_iGifHeight,hMemDC1,0,0,SRCCOPY);
			::BitBlt(hPauseMemDC,0,0,m_iGifWidth,m_iGifHeight,hMemDC1,0,0,SRCCOPY);

		}
		else
		{
			::BitBlt(hDC,m_nPosX,m_nPosY,m_iGifWidth,m_iGifHeight,hPauseMemDC,0,0,SRCCOPY);
		}

		SelectObject(hMemDC1,hOldBitmap1);
		m_bLockBitmap = FALSE;
		if(0 != m_iDelayTime)
			speed = m_iDelayTime * 10;
		else
			speed=m_dwSpeed;
		while((1 != m_EndRun)&&(speed > systimer2 - systimer1))
		{
			Sleep(10);
			systimer2 = GetTickCount();
		}
		PauseShow = m_bPause;
	}
	if(hPreviousBitmap != 0)
	DeleteObject(hPreviousBitmap);
	DeleteDC(hMemDC);
	DeleteDC(hMemDC1);
	//::ReleaseDC(hDC);
	m_EndRun = 2;
	return TRUE;
}


void CGIFShow::Stop()
{
	if(m_EndRun != 5)
		return;
	m_EndRun = 1;
	while(m_EndRun != 2)
	{
	}
	return;
}

CGIFShow::CGIFShow(HDC pWnd)
{
	m_hWndHDC=pWnd;
	m_bPause=FALSE;
	m_bAutoStart=TRUE;
	m_bAutoSize1=TRUE;
	m_bEmbed=FALSE;
	m_pcGlobalColorTable=0;
	m_pcGif=0;
	m_iGifSize=0;
	m_iGlobalColorSize=0;
	m_bTransparentIndex=FALSE;
	m_iDelayTime=0;
	m_EndRun=0;
	m_dwSpeed=50;
	m_hRedrawBitmap=0;
	m_bLockBitmap=FALSE;
	flag=0;
	m_nPosX=0;
	m_nPosY=0;
	wcscpy(filename,_T(""));
	m_bRunMode=1;
	m_bGlass=FALSE;
}

CGIFShow::~CGIFShow(void)
{
	Stop();
	if(m_hRedrawBitmap!=0)
		DeleteObject(m_hRedrawBitmap);
	if(m_pcGlobalColorTable!=NULL)
		delete[] m_pcGlobalColorTable;
	if(m_pcGif!=NULL)
		delete[] m_pcGif;
}

BOOL CGIFShow::Load(LPCTSTR filename)
{
	HANDLE hFile;
	DWORD size,size1,readbyte;
	BYTE temp[13];

	if(m_bEmbed)
		return FALSE;

	Stop();
	if(m_pcGlobalColorTable!=NULL)
		delete[] m_pcGlobalColorTable;
	if(m_pcGif!=NULL)
		delete[] m_pcGif;
	if(m_hRedrawBitmap!=0)
	{
		DeleteObject(m_hRedrawBitmap);
		m_hRedrawBitmap=0;
	}
	m_pcGlobalColorTable=m_pcGif=0;
	m_iTotalReadByte=0;
	m_iGifSize=m_iGlobalColorSize=0;

	hFile = CreateFile(filename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(INVALID_HANDLE_VALUE==hFile)
		return FALSE;
	size=GetFileSize(hFile,&size1);
	if(size==0xFFFFFFFF)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	ReadFile(hFile,temp,13,&readbyte,NULL);
	if((readbyte!=13)||((temp[0]!='G')||(temp[1]!='I')||
		(temp[2]!='F')||(temp[3]!='8')||((temp[4]!='7')&&
		(temp[4]!='9'))||(temp[5]!='a')))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	m_iGifWidth=*(temp+6)+ (*(temp+7)*256);
	m_iGifHeight=*(temp+8) + (*(temp+9)) *256;
	m_iBackgroundColor=temp[11];
	if(temp[10]&0x80)
	{
		m_iGlobalColorSize=0x01<<((temp[10]&0x07)+1);
		m_pcGlobalColorTable=new BYTE[3*m_iGlobalColorSize];
		ReadFile(hFile,m_pcGlobalColorTable,3*m_iGlobalColorSize,&readbyte,NULL);
		if(readbyte!=(DWORD)3*m_iGlobalColorSize)
		{
			delete[] m_pcGlobalColorTable;
			m_pcGlobalColorTable=0;
			m_iGlobalColorSize=0;
			CloseHandle(hFile);
			return FALSE;
		}
	}

	m_iGifSize=size-3*m_iGlobalColorSize-12;
	m_pcGifTrack=m_pcGif=new BYTE[m_iGifSize];
	ReadFile(hFile,m_pcGif,m_iGifSize,&readbyte,NULL);
	CloseHandle(hFile);
	return TRUE;
}

HBITMAP CGIFShow::NextImage(void)
{
	if(m_pcGif==NULL)return 0;

label2: if(m_iTotalReadByte>m_iGifSize)
		{
			m_pcGifTrack=m_pcGif;
			m_iTotalReadByte=0;
			return 0;
		}
		m_iTotalReadByte++;
		switch(*m_pcGifTrack++)
		{
		case 0x2C:
			return TakeIt();
			break;
		case 0x21:
			BYTE cSize;
			m_iTotalReadByte++;
			switch(*m_pcGifTrack++)
			{
			case 0xF9:
				m_pcGifTrack++;//block size
				m_iDisposalMethod=(*m_pcGifTrack)&28;
				m_bTransparentIndex=(*m_pcGifTrack++)&1;

				m_iDelayTime=*m_pcGifTrack;
				m_pcGifTrack+=2;
				m_iTransparentIndex=*m_pcGifTrack++;
				m_iTotalReadByte+=5;
				break;
			case 0xFE:
				while((cSize=*m_pcGifTrack)!=0)
				{
					m_pcGifTrack+=cSize+1;
					m_iTotalReadByte+=cSize+1;
					if(m_iTotalReadByte>m_iGifSize)
						return 0;
				}
				break;
			case 0x01:
				m_pcGifTrack+=13;
				m_iTotalReadByte+=13;
				while((cSize=*m_pcGifTrack)!=0)
				{
					m_pcGifTrack+=cSize+1;
					m_iTotalReadByte+=cSize+1;
					if(m_iTotalReadByte>m_iGifSize)
						return 0;
				}
				break;
			case 0xFF:
				m_pcGifTrack+=12;
				m_iTotalReadByte+=12;
				while((cSize=*m_pcGifTrack)!=0)
				{
					m_pcGifTrack+=cSize+1;
					m_iTotalReadByte+=cSize+1;
					if(m_iTotalReadByte>m_iGifSize)
						return 0;
				}
				break;
			default:
				return FALSE;
			}
			m_pcGifTrack++;
			m_iTotalReadByte++;
			if(m_iTotalReadByte>m_iGifSize)
				return 0;
			goto label2;
			break;
		case 0x3B:
			m_pcGifTrack=m_pcGif;
			m_iTotalReadByte=0;
			goto label2;
		case 0:
			goto label2;
		default:
			return FALSE;
		}
}

HBITMAP CGIFShow::TakeIt(void)
{
	UINT uLocalColorTableSize;
	WORD code,oldcode,code1;
	int iFinishCode,iResetCode;
	int iPrimaryTableSize,iTableSize;
	BITMAPINFOHEADER *bitmapheader;
	BYTE *pcColorTable;
	BYTE *pcInfo;
	GIFTABLE *pcGifTable;
	HBITMAP hBitmap;
	m_iLeft=*m_pcGifTrack;
	m_pcGifTrack+=2;
	m_iTop=*m_pcGifTrack;
	m_pcGifTrack+=2;
	m_iWidth=*m_pcGifTrack + (*(m_pcGifTrack+1))*256;
	m_pcGifTrack+=2;
	m_iWidth1=((m_iWidth-1)|0x3)+1;
	m_iHeight=*m_pcGifTrack + (*(m_pcGifTrack+1))*256 ;
	m_pcGifTrack+=2;
	m_cPackedField=*m_pcGifTrack++;
	m_iTotalReadByte+=9;
	m_iMaxByte=m_iWidth1*m_iHeight;
	pcInfo=new BYTE[256*sizeof(RGBQUAD)+sizeof(BITMAPINFOHEADER)+m_iMaxByte+sizeof(GIFTABLE)*4096];

	bitmapheader=(BITMAPINFOHEADER*)pcInfo;
	pcColorTable=pcInfo+sizeof(BITMAPINFOHEADER);
	m_pcBitmap=pcColorTable+256*sizeof(RGBQUAD);
	pcGifTable=(GIFTABLE*)(m_pcBitmap+m_iMaxByte);
	for(int i=0;i<4096;i++)
		pcGifTable[i].previouscode=pcGifTable[i].nextcode=0;
	bitmapheader->biSize=sizeof(BITMAPINFOHEADER);
	bitmapheader->biWidth=m_iWidth;
	bitmapheader->biHeight=-m_iHeight;
	bitmapheader->biPlanes=1;
	bitmapheader->biBitCount=8;
	bitmapheader->biCompression=BI_RGB;
	bitmapheader->biSizeImage=0;
	bitmapheader->biXPelsPerMeter=0;
	bitmapheader->biYPelsPerMeter=0;
	bitmapheader->biClrUsed=256;
	bitmapheader->biClrImportant=256;

	if(m_cPackedField&0x80)//是否设置了颜色列表标志
	{
		uLocalColorTableSize=1;
		uLocalColorTableSize<<=(m_cPackedField&7)+1;//颜色列表的位数
		if(m_bTransparentIndex)
		{
			m_TransparentColor=RGB(m_pcGifTrack[m_iTransparentIndex*3],m_pcGifTrack[m_iTransparentIndex*3+1],m_pcGifTrack[m_iTransparentIndex*3+2]);
		}
		m_iTotalReadByte+=uLocalColorTableSize*3;
		for(UINT i=0;i<uLocalColorTableSize;i++)
		{
			pcColorTable[2]=*m_pcGifTrack++;
			pcColorTable[1]=*m_pcGifTrack++;
			pcColorTable[0]=*m_pcGifTrack++;
			pcColorTable[3]=0;
			pcColorTable+=4;
		}

	}
	else
	{
		BYTE *pcGlobalColor=m_pcGlobalColorTable;
		if(m_bTransparentIndex)
		{
			m_TransparentColor=RGB(pcGlobalColor[m_iTransparentIndex*3],pcGlobalColor[m_iTransparentIndex*3+1],pcGlobalColor[m_iTransparentIndex*3+2]);
		}
		for(int i=0;i<m_iGlobalColorSize;i++)
		{
			pcColorTable[2]=*pcGlobalColor++;
			pcColorTable[1]=*pcGlobalColor++;
			pcColorTable[0]=*pcGlobalColor++;
			pcColorTable[3]=0;
			pcColorTable+=4;
		}
	}
	m_uPrimaryBitSize=m_uBitSize=(*m_pcGifTrack++);
	m_iTotalReadByte++;
	iPrimaryTableSize=iTableSize=(1<<m_uBitSize)+2;
	iFinishCode=iTableSize-1;
	iResetCode=iFinishCode-1;

	m_uPrimaryBitSize++;
	m_uBitSize++;
	m_uRemain=0;
	m_cCurentByte=0;
	m_uBlockSize=0;
	m_uReadByte=1;
	m_x=m_y=0;
	m_iPass=1;
	m_iRow=0;
	while((code=GetCode())!=iFinishCode)
	{
		if(code==iResetCode)
		{
			m_uBitSize=m_uPrimaryBitSize;
			iTableSize=iPrimaryTableSize;
			oldcode=GetCode();
			if(oldcode>iTableSize)
			{
				delete[] pcInfo;
				return 0;
			}
			Output((BYTE)oldcode);
			continue;
		}
		if(code<iTableSize) //<code> exist in the string pcGifTable
		{
			code1=code;
			WORD code2=0;
			while(code1>=iPrimaryTableSize)
			{
				pcGifTable[code1].nextcode=code2;
				code2=code1;
				code1=pcGifTable[code1].previouscode;
				if(code1>=code2)
				{
					delete[] pcInfo;
					return 0;
				}
			}
			Output((BYTE)code1);
			while(code2!=0)
			{
				Output(pcGifTable[code2].bit);
				code2=pcGifTable[code2].nextcode;
			}
			pcGifTable[iTableSize].bit=(BYTE)code1;
			pcGifTable[iTableSize].previouscode=oldcode;
			iTableSize++;
			if(iTableSize==(0x0001<<m_uBitSize))
				m_uBitSize++;
			if(m_uBitSize>12)
				m_uBitSize=12;
			oldcode=code;
		}
		else
		{
			code1=oldcode;
			WORD code2=0;
			while(code1>=iPrimaryTableSize)
			{
				pcGifTable[code1].nextcode=code2;
				code2=code1;
				code1=pcGifTable[code1].previouscode;
				if(code1>=code2)
				{
					delete[] pcInfo;
					return 0;
				}
			}
			Output((BYTE)code1);
			while(code2!=0)
			{
				Output(pcGifTable[code2].bit);
				code2=pcGifTable[code2].nextcode;
			}
			Output((BYTE)code1);
			pcGifTable[iTableSize].bit=(BYTE)code1;
			pcGifTable[iTableSize].previouscode=oldcode;
			iTableSize++;
			if(iTableSize==(0x0001<<m_uBitSize))
				m_uBitSize++;
			if(m_uBitSize>12)
				m_uBitSize=12;
			oldcode=code;
		}
	}

	hBitmap = DIBTohBitmap(m_hDC,(LPSTR)pcInfo);
	m_pcGifTrack++;
	m_iTotalReadByte++;
	delete[] pcInfo;
	return hBitmap;
}

void CGIFShow::SetPosition(int x,int y)
{
	m_nPosX=x;
	m_nPosY=y;
}

void CGIFShow::Pause(bool status)
{
	m_bPause=status;
}
HBITMAP CGIFShow::DIBTohBitmap(HDC hDC,LPSTR   lpSrcDIB)
{
	HBITMAP   hBitmap   =   NULL;
	HBITMAP   hOldBmp   =   NULL;
	HDC		  hTmpDC	=   NULL;
	BITMAPINFOHEADER* bitmapheader = (BITMAPINFOHEADER*)lpSrcDIB;
	hBitmap = CreateCompatibleBitmap(hDC,bitmapheader->biWidth,
		-(bitmapheader->biHeight));
	hTmpDC=CreateCompatibleDC(hDC);
	hOldBmp=(HBITMAP)SelectObject(hTmpDC,hBitmap);

	StretchDIBits(hTmpDC,0,0,bitmapheader->biWidth,
		-bitmapheader->biHeight,0,0,bitmapheader->biWidth,
		-bitmapheader->biHeight,lpSrcDIB+40 + 256*sizeof(RGBQUAD),(BITMAPINFO*)lpSrcDIB,
		DIB_RGB_COLORS,SRCCOPY);

	SelectObject(hTmpDC,hOldBmp);
	DeleteDC(hTmpDC);
	return   hBitmap;
}

// PicPathSet.cpp : implementation file
//


/////////////////////////////////////////////////////////////////////////////
// CPicPathSet dialog

CPicPathSet::CPicPathSet(CWnd* pParent /*=NULL*/)
	: CDialog(CPicPathSet::IDD, pParent)
{
	m_uState = 0;
}


void CPicPathSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPicPathSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPicPathSet, CDialog)
	//{{AFX_MSG_MAP(CPicPathSet)
	ON_BN_CLICKED(IDC_BTN_PIC1,OnbtnPic1)
	ON_BN_CLICKED(IDC_BTN_PIC2,OnbtnPic2)
	ON_BN_CLICKED(IDC_BTN_OK,OnbtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL,OnbtnCancel)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicPathSet message handlers

BOOL CPicPathSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	m_btnPicSet1.Create(L"我的照片", WS_CHILD|WS_VISIBLE, CRect(20,PIC_BTN_HEIGHT,150,PIC_BTN_HEIGHT+34), this, IDC_BTN_PIC1);
	m_btnPicSet1.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));

	m_btnPicSet2.Create(L"我的彩信", WS_CHILD|WS_VISIBLE, CRect(20,PIC_BTN_HEIGHT+34+8,150,PIC_BTN_HEIGHT+34+8+34), this, IDC_BTN_PIC2);
	m_btnPicSet2.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));
	
	CButton *pButton[2];
	pButton[0] = &m_btnPicSet1;
	pButton[1] = &m_btnPicSet2;	
	m_btnPicSet1.SetGroupButton(pButton, 2);
	m_btnPicSet2.SetGroupButton(pButton, 2);
	
	COLORREF m_bkRGB = ::GetSysColor(COLOR_STATIC);//背景色是STATIC
	m_cImageName.Create(L"", WS_CHILD, CRect(20,10,300,40), this);//显示文本内容
	m_cImageName.SetColor(RGB(0, 0, 0), m_bkRGB);
	
	m_cImagePath.Create(L"", WS_CHILD, CRect(20,50,300,80), this);//显示文本内容
	m_cImagePath.SetColor(RGB(0, 0, 0), m_bkRGB);

	m_cSaveClue.Create(L"", WS_CHILD, CRect(0,70,140,110), this);//显示文本内容
	m_cSaveClue.SetColor(RGB(0, 0, 0), m_bkRGB);

	m_btnOK.Create(L"确定",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(200,PIC_BTN_HEIGHT,200+PIC_BTN_WIDTH,PIC_BTN_HEIGHT+34),this,IDC_BTN_OK);
	m_btnOK.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	m_btnOK.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	m_btnOK.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
	m_btnOK.DrawBorder();

	m_btnCancel.Create(L"取消",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(200,PIC_BTN_HEIGHT + 32 + 8,200+PIC_BTN_WIDTH,PIC_BTN_HEIGHT+34*2+8),this,IDC_BTN_CANCEL);
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
	m_btnCancel.DrawBorder();

	m_listCtral.Create(IDD_LIST_DLG,this);
	m_listCtral.MoveWindow(30,30,534,400);
	m_listCtral.CenterWindow();
    
	CRect rct;
	GetWindowRect(&rct);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPicPathSet::OnbtnPic1()
{
	//	if (m_btnPicSet2.GetCheck())
	//	{
	//		m_btnPicSet2.SetCheck();
	//	}
}

void CPicPathSet::OnbtnPic2()
{
	//	if (m_btnPicSet1.GetCheck_())
	//	{
	//		m_btnPicSet1.SetCheck();
	//	}
}

void CPicPathSet::OnbtnOk()
{    
	UINT sucess = 0 ;
	if (m_btnPicSet1.GetCheck_())
	{
		
		if ( 1 == m_uState )//状态1 保存图片
		{   
			CreateDirectoryW(L"FlashDrv/my_photo/", 0);
			CString path = L"FlashDrv/my_photo/" + m_cFileName ;
			if ( 0 != CopyFile(m_cAllPath,path,FALSE))
			{
				sucess = 1 ;	
			}
			else
			{
				sucess = 0 ;	
			}
		}
		else if (2 == m_uState )//插入图片
		{
			m_listCtral.SetState(1);//
			m_listCtral.SetMwnd(m_pWnd);//传主窗口路径
			m_listCtral.SetList();
			m_listCtral.ShowWindow(TRUE);
			//加载列表，
		}

	}
	else if (m_btnPicSet2.GetCheck_())
	{   
		if (1 == m_uState)
		{
			CreateDirectoryW(L"FlashDrv/3G/mms/my_photo/", 0);
			CString path = L"FlashDrv/3G/mms/my_photo/" + m_cFileName ;
			if ( 0 != CopyFile(m_cAllPath,path,FALSE))
			{
				sucess = 1 ;	
			}
			else
			{
				sucess = 0 ;	
			}
		}
		else if (2 == m_uState)
		{
			m_listCtral.SetState(2);//
			m_listCtral.SetMwnd(m_pWnd);//传主窗口路径
			m_listCtral.SetList();
			m_listCtral.ShowWindow(TRUE);
		}
	
	}
	
	if (sucess && 1 == m_uState)
	{
		m_cSaveClue.SetWindowText(L"图片保存成功!");
	}
	else if (!sucess && 1 == m_uState)
	{
		m_cSaveClue.SetWindowText(L"图片保存失败!");
	}
	
	if ( 2 == m_uState)
	{
		ShowWindow(false);
	}

	if (1 == m_uState)
	{	
		m_cSaveClue.ShowWindow(TRUE);
		MoveWindow(0,0,200,100);
		CenterWindow();
		m_cSaveClue.CenterWindow();
		SetTimer(PIC_SAVE_TIMER,400,NULL);
		OnHide();

	}

}
void CPicPathSet::OnbtnCancel()
{
	ShowWindow(FALSE);
}

void CPicPathSet::SetCopyPath(CString &allpath,CString &filename)
{
	MoveWindow(0,0,320,170);
	CenterWindow();
	m_cAllPath	=	allpath;
	m_cFileName	=	filename;
	m_cImageName.SetWindowText(L"图片名:"+m_cFileName);
	m_cImagePath.SetWindowText(L"请选择图片保存位置:");
	OnShow();
	m_cImageName.Invalidate(FALSE);
}

void CPicPathSet::GetMainWnd(CWnd *mwnd)
{
	m_pWnd		= mwnd;
	CString		str ;
	((MmsShow*)m_pWnd)->GetName(str);

}

void CPicPathSet::OnShow()
{
	m_btnPicSet1.ShowWindow(TRUE);
	m_btnPicSet2.ShowWindow(TRUE);
	m_btnOK.ShowWindow(TRUE);
	m_btnCancel.ShowWindow(TRUE);
	m_cImageName.ShowWindow(TRUE);
	m_cImagePath.ShowWindow(TRUE);
}

void CPicPathSet::OnHide()
{
	m_btnPicSet1.ShowWindow(FALSE);
	m_btnPicSet2.ShowWindow(FALSE);
	m_btnOK.ShowWindow(FALSE);
	m_btnCancel.ShowWindow(FALSE);
	m_cImageName.ShowWindow(FALSE);
	m_cImagePath.ShowWindow(FALSE);
}
void CPicPathSet::SetState(UINT state)
{
	m_uState = state	;
	if ( 2 == m_uState)//插入状态，窗口改小
	{   
		int iterval = 8  ;
		int ybegin  = 50 ;
		int yheight = 34 ;
		int xbegin	= 20 ;
		int xwidth	= 130; 
	
		m_cSaveClue.MoveWindow(20,10,300,40);
		m_btnPicSet1.MoveWindow(20,ybegin,130,yheight);
		m_btnOK.MoveWindow(200,ybegin,100,yheight);
		ybegin += iterval + yheight;
		m_btnPicSet2.MoveWindow(20,ybegin,130,yheight);
		m_btnCancel.MoveWindow(200,ybegin,100,yheight);
		m_btnOK.DrawBorder();
		m_btnCancel.DrawBorder();
		m_cSaveClue.SetWindowText(L"请选择图片路径:");
		m_cSaveClue.ShowWindow(true);
		MoveWindow(0,0,320,140);
		CenterWindow();
		
	}
}

void CPicPathSet::OnTimer(UINT nIDEvent) 
{
	if ( PIC_SAVE_TIMER == nIDEvent)
	{	
		KillTimer(PIC_SAVE_TIMER);
		m_cSaveClue.ShowWindow(FALSE);
		ShowWindow(FALSE);
	}
}


// ListContral.cpp : implementation file
//

/////////////////////////////////////////////////////////////////////////////
// CListContral dialog


CListContral::CListContral(CWnd* pParent /*=NULL*/)
	: CDialog(CListContral::IDD, pParent)
{
	m_uState		= 0		;
	m_clickItem		= -1	;
	m_clickNum		= 0		;
	m_callpath		= L""	;
}


void CListContral::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListContral)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListContral, CDialog)
	//{{AFX_MSG_MAP(CListContral)
	ON_BN_CLICKED(IDC_BTN_OK,OnbtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL,OnbtnCancel)
	ON_MESSAGE(WM_LISTCTRL_CLICK,OnListCltrlClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListContral message handlers

BOOL CListContral::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_btnOK.Create(L"确定",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(100,330,100+100,330+52),this,IDC_BTN_OK);
	m_btnOK.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	m_btnOK.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	m_btnOK.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
	m_btnOK.DrawBorder();
	
	m_btnCancel.Create(L"取消",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(304,330 ,304+100,330+52),this,IDC_BTN_CANCEL);
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
	m_btnCancel.DrawBorder();

	m_lslist.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(27, 50, 27+450, 50+243), this, IDC_LIST_CONTRAL, TRUE, FALSE);
	m_lslist.SetListColor(RGB(189, 206, 239), RGB(214, 222, 247));
	m_lslist.InsertColumn(0, _T("路径"), LVCFMT_LEFT, 440);

	m_cImageList.Create(32,32,ILC_COLOR32|ILC_MASK,12,2);
	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAP_SELECT1);
	m_cImageList.Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();

	bm.LoadBitmap(IDB_BITMAP_UNSELECT1);
	m_cImageList.Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();

	//插入行	
	m_lslist.SetImageList(&m_cImageList,LVSIL_SMALL );

//	m_lslist.InsertItem(0,L"21232",0);
//	SetList();
//	m_lslist.InsertItem(2, L"34343", 0);
//	m_lslist.SetItemData(2, 0);
//	m_lslist.MoveWindow(20,20,200,100);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CListContral::SetList()
{   
	if ( 0 == m_uState )
	{
		return ;
	}
	UINT	ncount	= 0	;
	CString allpath	= L"";
	allpath = GetPath();
	WIN32_FIND_DATA FindFileData;		//查找文件时要使用的数据结构
	HANDLE hFind = INVALID_HANDLE_VALUE;//定义查找句柄	
	allpath += L"*.*";
	hFind = FindFirstFile(allpath, &FindFileData);//使用FindFirstFile函数来开始文件查找

	if(hFind !=	 INVALID_HANDLE_VALUE)
	{
		do
		{
			if(FindFileData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
			{ 
				//如果是文件夹
			}
			else
			{ 
				//如果是文件
				CString strfl = FindFileData.cFileName ;
				if ( strfl.Find(L".JPG") > 0 || strfl.Find(L".jpg") > 0 ||
					 strfl.Find(L".GIF") > 0 || strfl.Find(L".gif") > 0 ||
					 strfl.Find(L".PNG") > 0 || strfl.Find(L".png") > 0 ||
					 strfl.Find(L".BMP") > 0 || strfl.Find(L".bmp") > 0 )
				{
					m_lslist.InsertItem(ncount, FindFileData.cFileName, 1);
					m_lslist.SetItemData(ncount++, 0);
	
				}

			}
		}
		while(FindNextFile(hFind,&FindFileData));
	}

	m_lslist.SetScrollRagle(TRUE);

}

void CListContral::SetState(UINT state)
{
	m_uState = state ;
}

void CListContral::SetMwnd(CWnd *pwnd)
{
	m_pWnd = pwnd ;
}

void CListContral::OnbtnOk()
{
	m_callpath = GetPath() + m_callpath ;
	if (m_callpath != L"")//
	{		 
		 ((MmsShow*)m_pWnd)->FromListName(m_callpath);
		 ((MmsShow*)m_pWnd)->InitialDefRegion();//设置缺省的区域
		 ((MmsShow*)m_pWnd)->InsertPicture();//插入图片
	}
	Clear();
	ShowWindow(FALSE);

	//没选择的情况
		
}
void CListContral::OnbtnCancel()
{
	Clear();
	ShowWindow(FALSE);
}
void CListContral::Clear()
{
	m_uState		= 0		;
	m_clickItem		= -1	;
	m_clickNum		= 0		;
	m_callpath		= L""	;
	m_lslist.DeleteAllItems();
}
CString CListContral::GetPath()
{  
	CString allpath = L"";
	if (1 == m_uState)//我的相册
	{
		allpath = L"FlashDrv/my_photo/" ;	
	}
	else if (2 == m_uState)//我的彩信相册
	{
		allpath = L"FlashDrv/3G/mms/my_photo/" ;	
	}
	return allpath;
} 

void CListContral::OnListCltrlClick(WPARAM w, LPARAM l)
{
	if ( IDC_LIST_CONTRAL== (UINT)w)//得到当前哪项被点击
	{
		POSITION pos = m_lslist.GetFirstSelectedItemPosition();
		if (pos != NULL)
		{
			int index = m_lslist.GetNextSelectedItem (pos);
			if (m_lslist.GetCheck(index))//其它项清空，因为一次只能插入一张图
			{	
				LVITEM lvitem;
				int count = m_lslist.GetItemCount();
				if (m_clickItem == index )//点了两次，
				{
					m_clickNum++ ;
				}
				else
				{	
					m_clickNum	  =	0	;
				}
				
				lvitem.iImage = 0 ;
				if ( 1 == m_clickNum%2)//
				{
					lvitem.iImage = 1 ;
				}
				
				if ( 0 == lvitem.iImage)
				{
					m_callpath = m_lslist.GetItemText(index,0);
				}
				else
				{
					m_callpath = L"" ;
				}

				lvitem.mask =LVIF_TEXT | LVIF_IMAGE;   
				lvitem.iItem = index;   
				lvitem.iSubItem =0;   
				lvitem.pszText =(LPTSTR)(LPCTSTR)m_lslist.GetItemText(index,0);   
				lvitem.lParam =	0;  
				m_lslist.SetItem(&lvitem);
				m_lslist.SetItemState(index,0,LVIS_SELECTED);//设为全没有被点击

				m_clickItem	= index ;//记载当前哪个被点击
				for (int i = 0 ; i < count ;i++)
				{
					if (i != index)
					{   
						LVITEM lvitem;
						int id	=	m_lslist.GetItemData(i);
						lvitem.mask =LVIF_TEXT | LVIF_IMAGE;   
						lvitem.iItem = i;   
						lvitem.iSubItem = 0;   
						lvitem.pszText =(LPTSTR)(LPCTSTR)m_lslist.GetItemText(i,0);   
						lvitem.lParam =	0;   
						lvitem.iImage =	1;
						m_lslist.SetItem(&lvitem);
						m_lslist.SetItemData(index, id);
						m_lslist.SetItemState(i,0,LVIS_SELECTED);
					}
				}
			}
		}
	}
}
