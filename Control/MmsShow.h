#ifndef IMAGESHOW_H
#define IMAGESHOW_H
//////GIFShow.h////////

#include "../util/MMSWarp.h"
#include "CELineEdit.h"
#include "CEListCtrl.h"
#include "CeBtnST.h"
#include "CEStatic.h"
#include "CERadioButton.h"
#include "../Resource.h"
#include "ximage.h"


#define		DISPOSAL_NO 0
#define		DISPOSAL_NOT 4
#define		DISPOSAL_RESTBACK 8
#define		DISPOSAL_RESTORE 12

#define		PAR_SHOW_TIMER		5
#define		IMAGE_TIMER			1
#define		TXT_TIMER			2
#define		DEF_IMAGE_SHOW		3
#define		DEF_TXT_SHOW		4
#define		BTN_INTERVAL		25
#define		BTN_HEITHT			205
#define		BTN_WIDTH			130
#define		TIME_BASE			1
#define		PIC_BTN_HEIGHT		85	
#define		PIC_BTN_WIDTH		100
#define		PIC_SAVE_TIMER		6
#define		IMAGE_REGION_WIDTH	200
#define		IMAGE_REGION_HEIGHT	172
#define		TEXT_REGION_WIDTH	290
#define		TEXT_REGION_HEIGHT	172
#define		TEXT_TOP			29

#define		IDC_BTN_PIC1		0x2000
#define		IDC_BTN_PIC2		0x2001
#define		IDC_BTN_OK			0x2002
#define		IDC_BTN_CANCEL		0x2003
#define		IDC_LIST_CONTRAL	0x2004
#define		IDC_BTN_MUSIC		0x2005
#define		IDC_BTN_PICTURE		0x2006
#define		IDC_BTN_VEDIO		0x2007
#define		IDC_BTN_NEXT_PAGE	0x2008
#define		IDC_BTN_UP_PAGE		0x2009
#define     IDC_BTN_SAVE		0x200A



/////////////////////////////////////////////////////////////////////////////
// CPicPathSet dialog

/////////////////////////////////////////////////////////////////////////////
// CListContral dialog

class CListContral : public CDialog
{
public:
	CListContral(CWnd* pParent = NULL);   // standard constructor
	
	enum { IDD = IDD_LIST_DLG };
		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
private:
	CCEListCtrl		m_lslist	 ;
	CImageList		m_cImageList ;
	UINT			m_uState	 ;
	CWnd			*m_pWnd		 ;
	CCEButtonST		m_btnOK		 ;
	CCEButtonST		m_btnCancel	 ;
	UINT			m_clickItem	 ;
	UINT			m_clickNum	 ;
	CString			m_callpath	 ;
	
public:
	void	SetList();
	void	SetState(UINT state);
	void    SetMwnd(CWnd *pwnd);
	void	Clear();
	CString	GetPath();
protected:
	afx_msg void OnbtnOk();
	afx_msg void OnbtnCancel();
	afx_msg void OnListCltrlClick(WPARAM w, LPARAM l);
	
protected:
	
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()
};

class CPicPathSet : public CDialog
{

public:
	CPicPathSet(CWnd* pParent = NULL);   // standard constructor	
	enum { IDD = IDD_PIC_SELECT_DLG };
		
protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
	CCERadioButton		m_btnPicSet1;
	CCERadioButton		m_btnPicSet2;
	CCEButtonST			m_btnOK;
	CCEButtonST			m_btnCancel;
	CString				m_cAllPath;
	CString				m_cFileName;
	CWnd				*m_pWnd;
	UINT				m_uState;
	CListContral		m_listCtral;
	CCEStatic			m_cImageName;//文件名
	CCEStatic			m_cImagePath;//文件路径
	CCEStatic			m_cSaveClue ;//文件保存提示
	
public:
	void	SetCopyPath(CString &allpath,CString &filename);
	void	SetState(UINT state);
	void	GetMainWnd(CWnd *mwnd);
	void    SetClue(CString const clue);
	void    OnShow();
	void    OnHide();
	
protected:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnbtnPic1();
	afx_msg void OnbtnPic2();
	afx_msg void OnbtnOk();
	afx_msg void OnbtnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	
	DECLARE_MESSAGE_MAP()
};



typedef struct
{
	BYTE bit;
	WORD previouscode;
	WORD nextcode;
}GIFTABLE;

class CGIFShow
{
private:
	CWnd	*m_pwnd;
	HDC     m_hWndHDC;
	bool    m_bPause;
	BOOL    m_bAutoStart;
	BOOL    m_bEmbed;
	BYTE    m_cCurentByte,m_cPackedField;
	UINT    m_uBitSize,m_uPrimaryBitSize;
	UINT    m_uRemain,m_uReadByte,m_uBlockSize;
	int     m_iWidth,m_iHeight;
	int     m_iTop,m_iLeft;
	int     m_iFinishCode,m_iResetCode;
	int     m_iPass,m_iRow;
	int     m_iWidth1;
	int     m_iBackgroundColor;
	int     m_iGifSize;
	int     m_x,m_y;
	int     m_iGlobalColorSize;
	int     m_iDisposalMethod;
	BOOL    m_bTransparentIndex;
	int     m_iTransparentIndex;
	int     m_iDelayTime;
	int     m_iTotalReadByte;
	int     m_iMaxByte;
	DWORD   m_dwSpeed;
	COLORREF m_TransparentColor;
	HDC     m_hDC;
	BYTE*   m_pcGlobalColorTable;
	BYTE*   m_pcBitmap;
	BYTE*   m_pcGif;
	BYTE*   m_pcGifTrack;
	BOOL    m_bGlass;
	volatile int m_EndRun;
	HBITMAP m_hRedrawBitmap;
	int     m_iGifWidth,m_iGifHeight;
	volatile BOOL   m_bLockBitmap;
	TCHAR   filename[_MAX_PATH];
	int     flag;
	BOOL    m_bRunMode;
	BOOL    m_bAutoSize1;
	int     m_nPosX;
	int     m_nPosY;
	
public:
	CGIFShow(HDC pWnd);
	~CGIFShow();
	void Play();
	void Pause(bool status);///暂停运行
	void Stop();
	void SetPosition(int x,int y);
	BOOL Play1(void);
	BOOL Load(LPCTSTR filename);
	HBITMAP FirstImage(void);
	HBITMAP NextImage(void);
	HBITMAP TakeIt(void);
	HBITMAP DIBTohBitmap(HDC hDC,LPSTR   lpSrcDIB);
	WORD GetCode(void);
	BYTE GetByte(void);
	void Output(BYTE bit);
	void SetMainWnd(CWnd * wnd);
};



#pragma once

class CImageViewer : public CStatic
{
public:

	static enum EImageSize{ OriginalSize, FitControl};

struct TFrame    // structure that keeps a single frame info
{
	CxImage* m_pPicture;  // pointer to the interface used for drawing
	SIZE     m_frameSize;
	SIZE     m_frameOffset;
	UINT     m_nDelay;     // delay (in 1/100s of a second)
	UINT     m_nDisposal;  // disposal method
};
public:
	enum RenderMode {
		BIT_SHOW,		//渐现
		SHUTTERS_SHOW,	//百叶窗
		ZOOM_SHOW,		//缩放
	};
	void	SetImageFilename(std::wstring const& filename);
	void	SetImageRegion(CPoint const& origin, CSize const& size);
	void	SetEnterMode(RenderMode const renderMode);
	void	SetLightness(unsigned int const lightness);
	void	SetContract(unsigned int const contract);
	BOOL    SetGifShow(LPCTSTR lpszResName);
	void	SetImagePos(CRect &rct);
	void	GifStop();
private:
	CPoint		m_cImagePt			;
	CSize		m_Imagesize			;
	bool		m_bSetRegion		;
	CGIFShow	*m_cGifShow			;
protected:
//	afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );

public:
	BOOL GetPaintRect(RECT *lpRect);
	BOOL SetPaintRect(const RECT *lpRect);
	CImageViewer();
	virtual ~CImageViewer();
	void Stop();   // stops animation
	void UnLoad(); // stops animation plus releases all resources



	BOOL	IsGIF() const;
	BOOL	IsPlaying() const;
	BOOL	IsAnimatedGIF() const;
	SIZE	GetSize() const;
	int		GetFrameCount() const;
	COLORREF GetBkColor() const;


	BOOL	SetBitmap(HBITMAP hBitmap, EImageSize Emode = OriginalSize);
	BOOL	SetBitmap(UINT nIDResource, EImageSize Emode = OriginalSize);
	BOOL	SetBitmap(LPCTSTR lpszResName, EImageSize Emode = OriginalSize);
	BOOL	SetBitmap(const CByteArray& ImgBuffer, CString csFName, EImageSize Emode = OriginalSize);
	

	void	SetOriginalSize();


protected:
	BOOL			m_bNotifyParent;
	BOOL			m_bTransparentBk;
	int				m_nSizeMode;
	UINT			m_nFrameCount;
	RECT			m_PaintRect;
	SIZE			m_PictureSize;
	COLORREF		m_clrBackground;
	UINT			m_nCurrFrame;
	UINT			m_nDataSize;
	UINT			m_nCurrOffset;
	UINT			m_nGlobalCTSize;
	BOOL			m_bIsGIF;
	BOOL			m_bIsPlaying;
	volatile BOOL	m_bExitThread;
	BOOL			m_bIsInitialized;
	HDC				m_hMemDC;

	HDC				m_hDispMemDC;
	HBITMAP			m_hDispMemBM;
	HBITMAP			m_hDispOldBM;

	HBITMAP			m_hBitmap;
	HBITMAP			m_hOldBitmap;
	CWinThread*		m_pThread;
	HANDLE			m_hExitEvent;
	CxImage*		m_pPicture;
	CxMemFile		m_MemFile;

	// FILL ATTRIBUTES
	CBrush			m_brush;
	CBrush*			m_pBrush;
	COLORREF		m_rgbBkgnd;
	COLORREF		m_crHiColor;
	COLORREF		m_crLoColor;

	// TEXT ATTRIBUTES
	COLORREF		m_rgbText;
	BOOL			m_bAutoWrapping;
	CString			m_strText;

	// FONT ATTRIBUTES
	BOOL			m_bAutoAdjustFont;
	BOOL			m_bFont3d;
	LOGFONT			m_DefLogFont;
	LOGFONT			m_lf;
	CFont			m_font;
	int				m_nFontSize;
	int				m_nFontSizeVar;
	CString			m_csFontFamilly;

	std::vector<TFrame> m_arrFrames;

	int		FindType(const CString& ext);
	BOOL	DrawBitmap();
	BOOL	Load(int nImgType);
	void	ThreadAnimation();
	static UINT WINAPI _ThreadAnimation(LPVOID pParam);


	BOOL	PrepareDC(int nWidth, int nHeight);
	BOOL	RedrawWindow();


	// Generated message map functions
	//{{AFX_MSG(CImageViewer)
	afx_msg void	OnDestroy();
	afx_msg void	OnPaint();
	afx_msg void	OnSize(UINT nType, int cx, int cy);

	afx_msg HBRUSH CtlColor(CDC* pDC,UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

// CTextViewer

class CTextViewer : public CStatic
{
	DECLARE_DYNAMIC(CTextViewer)
public:
	
	CTextViewer();
	virtual ~CTextViewer();
	void SetTextContent(std::wstring const& content);
	void SetFont(LOGFONT const &font);
	void OnDraw(CRect rt_, std::wstring ws);//显示文字区域大小 和 内容
	void SetTextColor(COLORREF bkcolor,COLORREF ftcolor);//设置颜色
	void SetTextSize(CPoint const& origin, CSize const& size);//位置和大小
	void Update();//刷新
	
private:
	CFont				m_cFontText ;//文本的字体大小
	LOGFONT				m_cLogFont  ;//字体大小 
	std::wstring		m_cwStrContent ;//要显示的内容
	UINT				m_uiBitmapID ; //是否要画位图
	COLORREF			m_txtRGB ; //文本颜色
	COLORREF			m_bkRGB		;
	CPoint				m_cTextPt	;//设置文本坐标
	CSize				m_cTextSize	;//文本宽度
	
protected:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	
protected:
	DECLARE_MESSAGE_MAP()
};


class MmsShow : public CWnd
{
	DECLARE_DYNAMIC(MmsShow)
	
public:
	MmsShow();
	virtual ~MmsShow();
public:
	void FromListName(CString const& allpath);
	void SetImageFileName(std::wstring const& filename);//设置图片路径
	void SetImageRegion(CPoint const& origin, CSize const& size);//截取图片固定的大小。
	void SetImageShow(BOOL show = true);//
	void SetTextContent(std::wstring const& text);//设置文本的内容
	void SetTextFont(const LOGFONT &font);//文本字体
	void SetTextColor(COLORREF bkclor,COLORREF txtclor);//设置背景色 和 文本色
	void SetTextSize(CPoint const& origin, CSize const& size);//设置文本窗口位置
	void SetTextShow( BOOL show = true); //设置
	void SetTextUpdate();//设置文本更新
	void SetImagePos(CRect &rct);
	void SetMmsEdit();//设置文本可编辑，以及按钮性质
	void SetMmsRead();//设置文本为可读状态,
	BOOL LoadTextBMP(UINT bmpID);
	BOOL FindFileSmil(std::wstring const& filename);
	void InitialDefRegion();
	void InitialDefPar();
	void InitialRegion();
	void InitialPar();
	void Clear();
	void GetMmsRegionInfo();
	void GetDefRegionInfo();
	void GetHouZhui(CString &houzui,CString const fl);
	void GetName(CString &fl);
	void InsertPicture();
	void ClosePicture();
	void GetAllFileInfo(std::wstring &image,std::wstring &text,std::wstring &aduio,std::wstring &vedio);//获得所有文件的信息
	BOOL GetParInfo(std::vector<MMS::MMSWarp::MMS_PAR> &mmsPar,MMS::MMSWarp::MMS_LAYOUT &mmsLayout,std::wstring &title);//获得par里的内容
	void SaveParInfo();
	void AddParInfo();//往par里添加一条par
	void SetTitle(std::wstring const title);
	CString	GetFileName(CString const allpath);
public:

protected:
	afx_msg void OnTimer(UINT nIDEvent);

private:
	CImageViewer							m_cPic			;
//	CTextViewer								m_cTextView		;
	CCELineEdit								m_cTextView		;
	CCEStatic								m_cstcTitle		;
	CCELineEdit								m_cMmsTitle		;//标题
	UINT									m_imageWidth	;//图片的高度
	UINT									m_imageHeight	;//图片的宽度
	CRect									m_rctImage		;//图片的区域
	UINT									m_itextWidth	;//文本的高度
	UINT									m_itextHeight	;//文本的宽度
	CRect									m_rcttext		;//文本的区域
	std::vector<MMS::MMSWarp::MMS_REGION>	m_cMmsRegion	;//文件的路径
	std::vector<MMS::MMSWarp::MMS_PAR>		m_cMmsPar		;//par
	MMS::MMSWarp::MMS_LAYOUT				m_cMmsLayOut	;//mms layout
	
	UINT									m_cImageTime	;//图片显示次数
	int										m_uMmsParPlay	;//当前正在播放的PAR
	CString									m_cSmilPath		;//SMIL文件的路径
	CString									m_cImagePath	;//图片全路径
	UINT									m_uImageDur		;//图片的显示时间
	UINT									m_uTxtDur		;//文本显示时间
	UINT									m_uParDur		;//Par显示的总时间
	CString									m_cTxtContent	;//文本内容
	UINT									m_uTxtTime		;//文本显示了多长时间
	UINT									m_uParTime		;//显示的时间

	std::vector<CString>					m_cDefImagePath	;//没有SMIL文件，加载该路径的全部图片
	std::vector<CString>					m_cDefTxtContent;//没有SMIL文件，加载文本
	UINT									m_uDefTxtTime	;//文本显示了多长时间
	UINT									m_uDefImageTime	;//显示的时间
	int										m_uDefTxtItem	;//正在
	int										m_uDefImageItem	;//显示的时间
	BOOL									m_bFindSiml		;
	CCEButtonST								m_btnPicture	;//保存图片
	CCEButtonST								m_btnMusic		;//保存音频
	CCEButtonST								m_btnVedio		;//保存视频
	CCEButtonST								m_btnNextPage	;//下一页
	CCEButtonST								m_btnUpPage		;//上一页
	CCEButtonST								m_btnSave		;//保存
	UINT									m_uState		;//1,readonly,2,edit
	std::vector<CString>					m_cInsertList	;//插入图片的列表
	std::vector<CString>					m_cInsertAudio	;//音乐
	std::vector<CString>					m_cInsertvedio	;//视频
	CString									m_cAudioName	;//音频路径
	CString									m_cVedioName	;//
	CString									m_cTitleName	;//标题名字
	CPicPathSet								m_picPathDlg	;//路径设置
	int										m_iPageNum		;//页数
//	CListContral							m_listCtral		;//文件列表

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd * pParentWnd, UINT nID, const DWORD dwFlag = 0);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void  OnBtnPicture();
	afx_msg void  OnBtnMusic();
	afx_msg void  OnBtnVedio();
	afx_msg void  OnBtnUp();
	afx_msg void  OnBtnNext();
	afx_msg void  OnBtnSave();

};

#endif



