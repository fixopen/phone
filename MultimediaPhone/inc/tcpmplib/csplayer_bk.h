/*
 * csplayer.h
 *
 * the media player for Windows CE
 *
 */
#ifndef _CSPLAYER_INC_
#define _CSPLAYER_INC_

#ifdef __cplusplus
extern "C" {
#endif

#define TRACKMAX					30000
#define TICKSPERSEC					16384

/* nTick或nDuration与时间之间的转换关系
int Hour,Min,Sec;
Hour = Tick / 3600 / TICKSPERSEC;
Tick -= Hour * 3600 * TICKSPERSEC;
Min = Tick / 60 / TICKSPERSEC;
Tick -= Min * 60 * TICKSPERSEC;
Sec = Tick / TICKSPERSEC;
*/

typedef struct {	
	TCHAR	szFileName[256];
	int		nTick;					/* 当前时间位置 */
	int		nDuration;				/* 总时间长度 */
	int		nFileSize;				/* KB */

	TCHAR	szVidStreamName[256];	
	int		nVidWidth;
	int		nVidHeight;
	int		nVidFrameRate;			/* fps */
	int		nVidBitRate;

	TCHAR	szAudStreamName[256];	
	int		nAudSampleRate;			/* Hz */
	int		nAudChannels;
	int		nAudByteRate;			/* Kbps */
} MEDIA_INFO, *LPMEDIA_INFO;

/*
 * Name: plyCreate
 * Create the media player.
 * Return: Zero indicates success.
 */
int plyCreate(
			int x,					
			int y,					
			int width,				
			int height			
			);

/*
 * Name: plyInit
 * Initialize the player.
 * Return: Zero indicates success.
 */
int plyInit(
			HWND hWnd				/* [IN] Handle to the window, will receive the heartbeat message */
			);

/*
 * Name: plyOpen
 * Open a media file.
 * Return: Zero indicates success.
 */
int plyOpen(
			LPTSTR pszFile			/* [IN] Pointer to a null-terminated string that specifies the name of the file, to open */
			);

/*
 * Name: plyExit
 * Exit the media player.
 * Return: Zero indicates success.
 */
int plyExit(void);

/*
 * Name: plyPlay
 * Play/pause the media file.
 * Return: Zero indicates success.
 */
int plyPlay(
			BOOL fstart				/* [IN] Specifies whether playback or pause */
			);

/*
 * Name: plyStop
 * Stops the media player.
 * Return: Zero indicates success.
 */
int plyStop(void);

/*
 * Name: plyForward
 * Sets the forward setting.
 * Return: Zero indicates success.
 */
int plyForward(void);

/*
 * Name: plySetFullScreen
 * Sets full screen mode.
 * Return: Zero indicates success.
 */
int plySetFullScreen(
					 BOOL isFullScreen	/* [IN] Specifies whether full screen or not */
					 );

/*
 * Name: plySetWndPos
 * Sets video window pos.
 * Return: Zero indicates success.
 */
int plySetWndPos(
				 WORD x, 
				 WORD y, 
				 WORD width, 
				 WORD height
				 );

/*
 * Name: plySetPos
 * Sets the track pos.
 * 
 */
int plySetPos(
			  int percent	/* [IN] percent/TRACKMAX 的值是百分比，比如要设置到 %5 的位置，则 percent = TRACKMAX * %5 */
			  );

/*
 * Name: plySetRotate
 * Sets the rotate setting.
 * Return: Zero indicates success.
 */
int plySetRotate(
				 int rotate			/* [IN] Specifies how the window is to be rotated */
#define ROTATE_GUI					1
#define ROTATE_0					0
#define ROTATE_90					90
#define ROTATE_180					180
#define ROTATE_270					270
				 );

/*
 * Name: plySetZoom
 * Sets the zoom setting.
 * Return: Zero indicates success.
 */
int plySetZoom(
			   int zoom				/* [IN] Specifies how the window is to be zoom */
#define ZOOM_FIT_SCREEN				0
#define ZOOM_STRETCH_SCREEN			1
#define ZOOM_FILL_SCREEN			2
#define ZOOM_50						50
#define ZOOM_100					100
#define ZOOM_150					150
#define ZOOM_200					200
			   );

/*
 * Name: plyGetMediaInfo
 * Get the media information.
 * Return: Zero indicates success.
 */
int plyGetMediaInfo(
					LPMEDIA_INFO info	/* [IN/OUT] Address of a MEDIA_INFO structure */
					);


/*
 * Name: plySetRepeat
 * Sets the repeat setting.
 * Return: Zero indicates success.
 */
int plySetRepeat(
				 BOOL repeat			/* [IN] Specifies whether repeat */
				 );

/*
 * Name: plySetShuffle
 * Sets the shuffle setting.
 * Return: Zero indicates success.
 */
int plySetShuffle(
				  BOOL shuffle			/* [IN] Specifies whether shuffle */
				  );

/*
 * Name: plySetAutoProRotate
 * Sets the auto prerotate setting.
 * Return: Zero indicates success.
 */
int plySetAutoPreRotate(
						BOOL rotate		/* [IN] Specifies whether auto prerotate */
						);

/*
 * Name: plySetVideoOutput
 * Sets the video output setting.
 * Return: Zero indicates success.
 */
int plySetVideoOutput(
					  BOOL vid			/* [IN] Specifies whether video output */
					  );

/*
 * Name: plySetAudioOutput
 * Sets the audio output setting.
 * Return: Zero indicates success.
 */
int plySetAudioOutput(
					  BOOL aud			/* [IN] Specifies whether audio output */
					  );

/*
 * Name: plySetMute
 * Sets the mute setting.
 * Return: Zero indicates success.
 */
int plySetMute(
			   BOOL mute				/* [IN] Specifies whether mute */
			   );

/*
 * Name: plySetVolume
 * Sets the volume of audio output device.
 * Return: Zero indicates success.
 */
int plySetVolume(
				 int vol				/* [IN] Specifies the volume */
				 );

/*
 * Name: plySetAspect
 * Sets the aspect setting.
 * Return: Zero indicates success.
 */
int plySetAspect(
				 int aspect				/* [IN] Specifies the aspect */
#define ASPECT_AUTO						0
#define ASPECT_SQUARE					1
#define ASPECT_4_3_SCREEN				2
#define ASPECT_4_3_NTSC					3
#define ASPECT_4_3_PAL					4
#define ASPECT_16_9_SCREEN				5
#define ASPECT_16_9_NTSC				6
#define ASPECT_16_9_PAL					7
				 );

/*
 * Name: plySetStereo
 * Sets the stereo setting.
 * Return: Zero indicates success.
 */
int plySetStereo(
				 int stereo				/* [IN] Specifies the stereo */
#define CTL_STEREO_NORMAL				0
#define CTL_STEREO_SWAPPED				1
#define CTL_STEREO_JOINED				2
#define CTL_STEREO_LEFT					3
#define CTL_STEREO_RIGHT				4
				 );

/*
 * Name: plyCaptureBmp
 * Captures a bitmap.
 * Return: Size, in bytes, of the captured bitmap file.
 */
int plyCaptureBmp(
				  LPTSTR bmpFile		/* [IN] */				  
				  );

/*
 * Name: plySetSubtitle
 * 
 * Return: Zero indicates success.
 */
int plySubtitleText(
					int x,				/* [IN] X */
					int y,				/* [IN] Y */
					LPCTSTR strText,		/* [IN] text */
					DWORD color			/* [IN] color RGB565 */
					);

int plyGetRect();

#ifdef __cplusplus
}
#endif

#endif /* _CSPLAYER_INC_ */
/* End of file */
