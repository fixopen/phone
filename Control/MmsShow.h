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
#include "MidiPlayerControl.h"


#define		DISPOSAL_NO 0
#define		DISPOSAL_NOT 4
#define		DISPOSAL_RESTBACK 8
#define		DISPOSAL_RESTORE 12

#define		PAR_SHOW_TIMER		5
#define		IMAGE_TIMER			1
#define		TXT_TIMER			2
#define		DEF_IMAGE_SHOW		3
#define		DEF_TXT_SHOW		4
#define		AUDIO_TIMER			6
#define		DEF_AUDIO_TIMER		7
#define		VIDEO_TIMER			14
#define		DEF_VIDEO_TIMER		15

#define		WM_AUDIO			1234
#define		WM_IMAGE			1235

#define		BTN_INTERVAL		25
#define		BTN_HEITHT			235
#define		BTN_WIDTH			130
#define		TIME_BASE			1
#define		PIC_BTN_HEIGHT		85	
#define		PIC_BTN_WIDTH		100
#define		PIC_SAVE_TIMER		6
#define		IMAGE_REGION_WIDTH	200
#define		IMAGE_REGION_HEIGHT	172
#define		TEXT_REGION_WIDTH	290
#define		TEXT_REGION_HEIGHT	172
#define		TEXT_TOP			60

#define		PAR_SIZE			20

#define		IDC_BTN_PIC1		0x2000
#define		IDC_BTN_PIC2		0x2001
#define		MMS_BTN_OK			0x2002
#define		MMS_BTN_CANCEL		0x2003
#define		IDC_LIST_CONTRAL	0x2004
#define		IDC_BTN_MUSIC		0x2005
#define		IDC_BTN_PICTURE		0x2006
#define		IDC_BTN_VEDIO		0x2007
#define		IDC_BTN_NEXT_PAGE	0x2008
#define		IDC_BTN_UP_PAGE		0x2009
#define     IDC_BTN_TEXT_DOWN	0x200A
#define     IDC_BTN_TEXT_UP		0x200B

#ifndef IDC_BTN_OK
#define IDC_BTN_OK	0x2010
#endif
#ifndef IDC_BTN_CANCEL
#define IDC_BTN_CANCEL	0x2011
#endif

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

	int m_uSaveType;       //������Ƶ����Ƶ��ͼƬ
		
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
	HBITMAP DIBTohBitmap(HDC hDC,LPSTR  lpSrcDIB);
	WORD GetCode(void);
	BYTE GetByte(void);
	void Output(BYTE bit);
	void SetMainWnd(CWnd * wnd);
};



#pragma once

class CImageViewer : public CStatic
{
public:
	CString sTitle;
	void SetTitle(CString s);
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

	int		FindType(const CString ext);
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
	virtual void PreSubclassWindow();
	afx_msg void OnClicked();
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
	void FromListName(CString const allpath);
	void SetImageFileName(std::wstring const& filename);//����ͼƬ·��
	void SetImageRegion(CPoint const& origin, CSize const& size);//��ȡͼƬ�̶��Ĵ�С��
	void SetImageShow(BOOL show = true);//
	
	void SetTextContent(std::wstring const& text);//�����ı�������
	void SetTextFont(const LOGFONT &font);//�ı�����
	void SetTextColor(COLORREF bkclor,COLORREF txtclor);//���ñ���ɫ �� �ı�ɫ
	void SetTextSize(CPoint const& origin, CSize const& size);//�����ı�����λ��
	void SetTextShow( BOOL show = true); //����
	void SetTextUpdate();//�����ı�����
	void ShowTextBtn();
	void TextScrollUp();//���Ϲ���
	void TextScrollDown();//���¹���
	void SetImagePos(CRect &rct);
	void SetMmsEdit();//�����ı��ɱ༭���Լ���ť����
	void SetMmsRead();//�����ı�Ϊ�ɶ�״̬,
	void SetMmsInsert();//���ݲ���
	void SetpWnd(CWnd const * pWnd);
	
	BOOL FindFileSmil(std::wstring const filename);
	void InitialDefRegion();
	void InitialRegion();
	void InitialPar();
	
	void Clear();
	void GetMmsRegionInfo();
	void GetDefRegionInfo();
	void GetHouZhui(CString &houzui,CString const fl);
	void GetImage(CString &fl);
	void InsertPicture();
	void ClosePicture();
	void SavePicture();//����ͼƬ

	void GetAllFileInfo(std::wstring &image,std::wstring &text,std::wstring &aduio,std::wstring &video);//��������ļ�����Ϣ
	void DeleteAudio();
	void SaveAudio();
	void HandleAudio();
	void SetAudioShow();//��ʾ��Ƶ�ļ�
	void InsertAudio(CString const audioPath);//������Ƶ
	void PlayAudio(bool bplay);//����
	CString GetAudio();

	void SetVideoShow(BOOL show = TRUE);  //������Ƶ��ʾ
	void PlayVideo();
	void DeleteVideo();
	void SaveVideo();
	void InsertVideo(CString const videoPath);
	CString GetVideo();
	void StopVideo();

	BOOL GetParInfo(std::vector<MMS::MMSWarp::MMS_PAR> &mmsPar,MMS::MMSWarp::MMS_LAYOUT &mmsLayout,std::wstring &title);//���par�������
	void SaveParInfo();
	void AddParInfo(bool binsert = false);//��par�����һ��par
	void SetTitle(std::wstring const title);
	void SetTransit(std::wstring const filename);//����ת��
	CString	GetFileName(CString const allpath);
	void MsgPost();//������Ƶ���ı���״̬
	void NewPar();//�½�һ��Par
	void AllFileSize();//par����ļ���С
	void GetCurrentPageFileSize(long double &filesize,CString &text);//��õ�ǰҳ���ļ�С
	void AutoPlay(bool bauto);//�Զ����ţ������ֶ�����
	void ShowParInfo();//չʾpar����Ϣ
	void GetText();//����ı�����
	bool ReadName(std::string name,CString &content);//����ı��������
	BOOL GetPicMp3Count();

public:
	MediaPlayer::MidiPlayer *m_pMidiPlayer;
	BOOL  m_isMidInit;
	CString	m_cAudioName	;//��Ƶ·��
	CString	m_cImagePath	;//ͼƬȫ·��
	CString m_cVideoName	;//��Ƶ�ļ���
	CPicPathSet	m_picPathDlg	;//·������
	std::vector<MMS::MMSWarp::MMS_PAR>		m_cMmsPar		;//par
	double m_addtionSize;

	void SetReadPage();
protected:
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CImageViewer							m_cPic			;
	CCELineEdit								m_cTextView		;
	CCEStatic								m_cstcTitle		;
//	CColorStatic							m_cstcAudio		;//��Ƶ
	CCEMoveTxtStatic						m_cstcAudio		;//��Ƶ
	CCELineEdit								m_cMmsTitle		;//����
	//CCERectEdit								m_cMmsTitle;
	UINT									m_imageWidth	;//ͼƬ�ĸ߶�
	UINT									m_imageHeight	;//ͼƬ�Ŀ��
	CRect									m_rctImage		;//ͼƬ������
	UINT									m_itextWidth	;//�ı��ĸ߶�
	UINT									m_itextHeight	;//�ı��Ŀ��
	CRect									m_rcttext		;//�ı�������
	std::vector<MMS::MMSWarp::MMS_REGION>	m_cMmsRegion	;//�ļ���·��
	MMS::MMSWarp::MMS_LAYOUT				m_cMmsLayOut	;//mms layout
	
	int										m_uMmsParPlay	;//��ǰ���ڲ��ŵ�PAR
	CString									m_cSmilPath		;//SMIL�ļ���·��
	
	UINT									m_cImageTime	;//ͼƬ��ʾ����
	UINT									m_uImageDur		;//ͼƬ����ʾʱ��

	UINT									m_uParDur		;//Par��ʾ����ʱ��
	UINT									m_uParTime		;//��ʾ��ʱ��

	UINT									m_uTxtDur		;//�ı���ʾʱ��
	CString									m_cTxtContent	;//�ı�����
	UINT									m_uTxtTime		;//�ı���ʾ�˶೤ʱ��
	
	UINT									m_uAudioDur		;//��Ƶ�ļ���ʾ��ʱ��
	UINT									m_uAudioTime	;//��Ƶ�ļ�

	UINT									m_uVideoDur		;//��Ƶ�ļ���ʾʱ��
	UINT									m_uVideoTime	;

	std::vector<CString>					m_cDefImagePath	;//û��SMIL�ļ������ظ�·����ȫ��ͼƬ
	std::vector<CString>					m_cDefTxtContent;//û��SMIL�ļ��������ı�
	std::vector<CString>					m_cDefAudio		;//��Ƶ�ļ�
	std::vector<CString>					m_cDefVideo		;//��Ƶ�ļ�

	UINT									m_uDefTxtTime	;//�ı���ʾ�˶೤ʱ��
	UINT									m_uDefImageTime	;//��ʾ��ʱ��
	UINT									m_uDefAudioTime	;//��Ƶ�ļ���ʾʱ��
	UINT									m_uDefVideoItem ;//��Ƶ�ļ���ʾ��ʱ��
	int										m_uDefTxtItem	;//����
	int										m_uDefImageItem	;//��ʾ��ʱ��
	int										m_uDefAudioItem ;//��Ƶ
	
	BOOL									m_bFindSiml		;
	BOOL									m_bIsTextClick ;
		
	UINT									m_uState		;//1,readonly,2,edit
	std::vector<CString>					m_cInsertList	;//����ͼƬ���б�
	std::vector<CString>					m_cInsertAudio	;//����
	std::vector<CString>					m_cInsertvideo	;//��Ƶ
	CString									m_cVedioName	;//
	CString									m_cTitleName	;//��������
	int										m_iPageNum		;//ҳ��

	CRect									m_cMmscRect		;//�ؼ��Ĵ�С
	HWND									m_pHwnd			;//���ô��ھ��

	CCEButtonST								m_btnUp			;
	CCEButtonST								m_btnDown		;

	bool									m_bHaveInsertPic;//�Ƿ����ͼƬ

	std::vector<std::pair<int,std::wstring > > m_vText		;//�����ı�Text
	
//	CListContral							m_listCtral		;//�ļ��б�

protected:
	DECLARE_MESSAGE_MAP()

public:

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd * pParentWnd, UINT nID, const DWORD dwFlag = 0);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void  OnBtnPicture();
	afx_msg void  OnBtnUp();
	afx_msg void  OnBtnNext();
	afx_msg void  OnBtnTextUp();
	afx_msg void  OnBtnTextDown();
	afx_msg void  OnPicClicked(WPARAM w, LPARAM l);
	afx_msg void  OnLButtonDown(UINT nFlags, CPoint point);

};

#endif



