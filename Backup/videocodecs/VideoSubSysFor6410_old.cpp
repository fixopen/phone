#include	"stdafx.h"
#include	"VideoSubSysFor6410.h"


VideoSubSysFor6410::VideoSubSysFor6410()
{
	pMyCamera = NULL;

	pMyLocalDisplay = NULL;
	pMyRemoteDisplay = NULL;

	pMyH263Coder = NULL;
	pMyH264Coder = NULL;
	pMyMp4Coder = NULL;

}
VideoSubSysFor6410:: ~VideoSubSysFor6410()
{
	pMyCamera = NULL;

	pMyLocalDisplay = NULL;
	pMyRemoteDisplay = NULL;

	pMyH263Coder = NULL;
	pMyH264Coder = NULL;
	pMyMp4Coder = NULL;
}
