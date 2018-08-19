#ifndef _VideoSubSysFor6410_
#define _VideoSubSysFor6410_

#include "Cameras/LocalCamera6410.h"

#include "Displays/Display.h"

#include "VideoCodecs/H263CodecHard.h"
#include "VideoCodecs/H264CodecHard.h"
#include "VideoCodecs/MP4CodecHard.h"

class VideoSubSysFor6410
{
public:
	VideoSubSysFor6410();
	~VideoSubSysFor6410();


private:
	LocalCamera6410	*pMyCamera;

	Display	*pMyLocalDisplay;
	Display	*pMyRemoteDisplay;

	H263CoderHard	*pMyH263Coder;
	H264CoderHard	*pMyH264Coder;
	MP4CoderHard	*pMyMp4Coder;
};

#endif