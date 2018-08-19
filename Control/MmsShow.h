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
	CCEStatic			m_cImageName;//�ļ���
	CCEStatic			m_cImagePath;//�ļ�·��
	CCEStatic			m_cSaveClue ;//�ļ�������ʾ
	
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
	void Pause(bool status);///��ͣ����
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
		BIT_SHOW,		//����
		SHUTTERS_SHOW,	//��Ҷ��
		ZOOM_SHOW,		//����
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
	void OnDraw(CRect rt_, std::wstring ws);//��ʾ���������С �� ����
	void SetTextColor(COLORREF bkcolor,COLORREF ftcolor);//������ɫ
	void SetTextSize(CPoint const& origin, CSize const& size);//λ�úʹ�С
	void Update();//ˢ��
	
private:
	CFont				m_cFontText ;//�ı��������С
	LOGFONT				m_cLogFont  ;//�����С 
	std::wstring		m_cwStrContent ;//Ҫ��ʾ������
	UINT				m_uiBitmapID ; //�Ƿ�Ҫ��λͼ
	COLORREF			m_txtRGB ; //�ı���ɫ
	COLORREF			m_bkRGB		;
	CPoint				m_cTextPt	;//�����ı�����
	CSize				m_cTextSize	;//�ı����
	
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
	void SetImageFileName(std::wstring const& filename);//����ͼƬ·��
	void SetImageRegion(CPoint const& origin, CSize const& size);//��ȡͼƬ�̶��Ĵ�С��
	void SetImageShow(BOOL show = true);//
	void SetTextContent(std::wstring const& text);//�����ı�������
	void SetTextFont(const LOGFONT &font);//�ı�����
	void SetTextColor(COLORREF bkclor,COLORREF txtclor);//���ñ���ɫ �� �ı�ɫ
	void SetTextSize(CPoint const& origin, CSize const& size);//�����ı�����λ��
	void SetTextShow( BOOL show = true); //����
	void SetTextUpdate();//�����ı�����
	void SetImagePos(CRect &rct);
	void SetMmsEdit();//�����ı��ɱ༭���Լ���ť����
	void SetMmsRead();//�����ı�Ϊ�ɶ�״̬,
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
	void GetAllFileInfo(std::wstring &image,std::wstring &text,std::wstring &aduio,std::wstring &vedio);//��������ļ�����Ϣ
	BOOL GetParInfo(std::vector<MMS::MMSWarp::MMS_PAR> &mmsPar,MMS::MMSWarp::MMS_LAYOUT &mmsLayout,std::wstring &title);//���par�������
	void SaveParInfo();
	void AddParInfo();//��par�����һ��par
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
	CCELineEdit								m_cMmsTitle		;//����
	UINT									m_imageWidth	;//ͼƬ�ĸ߶�
	UINT									m_imageHeight	;//ͼƬ�Ŀ��
	CRect									m_rctImage		;//ͼƬ������
	UINT									m_itextWidth	;//�ı��ĸ߶�
	UINT									m_itextHeight	;//�ı��Ŀ��
	CRect									m_rcttext		;//�ı�������
	std::vector<MMS::MMSWarp::MMS_REGION>	m_cMmsRegion	;//�ļ���·��
	std::vector<MMS::MMSWarp::MMS_PAR>		m_cMmsPar		;//par
	MMS::MMSWarp::MMS_LAYOUT				m_cMmsLayOut	;//mms layout
	
	UINT									m_cImageTime	;//ͼƬ��ʾ����
	int										m_uMmsParPlay	;//��ǰ���ڲ��ŵ�PAR
	CString									m_cSmilPath		;//SMIL�ļ���·��
	CString									m_cImagePath	;//ͼƬȫ·��
	UINT									m_uImageDur		;//ͼƬ����ʾʱ��
	UINT									m_uTxtDur		;//�ı���ʾʱ��
	UINT									m_uParDur		;//Par��ʾ����ʱ��
	CString									m_cTxtContent	;//�ı�����
	UINT									m_uTxtTime		;//�ı���ʾ�˶೤ʱ��
	UINT									m_uParTime		;//��ʾ��ʱ��

	std::vector<CString>					m_cDefImagePath	;//û��SMIL�ļ������ظ�·����ȫ��ͼƬ
	std::vector<CString>					m_cDefTxtContent;//û��SMIL�ļ��������ı�
	UINT									m_uDefTxtTime	;//�ı���ʾ�˶೤ʱ��
	UINT									m_uDefImageTime	;//��ʾ��ʱ��
	int										m_uDefTxtItem	;//����
	int										m_uDefImageItem	;//��ʾ��ʱ��
	BOOL									m_bFindSiml		;
	CCEButtonST								m_btnPicture	;//����ͼƬ
	CCEButtonST								m_btnMusic		;//������Ƶ
	CCEButtonST								m_btnVedio		;//������Ƶ
	CCEButtonST								m_btnNextPage	;//��һҳ
	CCEButtonST								m_btnUpPage		;//��һҳ
	CCEButtonST								m_btnSave		;//����
	UINT									m_uState		;//1,readonly,2,edit
	std::vector<CString>					m_cInsertList	;//����ͼƬ���б�
	std::vector<CString>					m_cInsertAudio	;//����
	std::vector<CString>					m_cInsertvedio	;//��Ƶ
	CString									m_cAudioName	;//��Ƶ·��
	CString									m_cVedioName	;//
	CString									m_cTitleName	;//��������
	CPicPathSet								m_picPathDlg	;//·������
	int										m_iPageNum		;//ҳ��
//	CListContral							m_listCtral		;//�ļ��б�

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



