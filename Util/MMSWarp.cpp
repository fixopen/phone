#include "stdafx.h"
#include "MMSWarp.h"
//#include "../SMS/SMSWarp.h"


using namespace MMS;

MMSWarp::MMSWarp()
{

}
MMSWarp::~MMSWarp()
{

}
MMSWarp::MMSWarp(const MMSWarp& mms)
{
}
MMSWarp* MMSWarp::GetMMSWarp()
{
	static MMSWarp* mms = 0;
	if (mms == 0)
	{
		mms = new MMSWarp();
	}
	return mms;
}

int MMSWarp::EncodeSubmitRequest(const MMS_SubmitRequest* pSrc, char* pDst)
{
	int length = 0;
	pDst[length++] = 0x8C;//X-Mms-Message-Type被编码成8Ch，m-send-req被编码成80h
	pDst[length++] = 0x80;
	pDst[length++] = 0x98;//98h代表Transaction-ID
	memcpy(pDst + length, pSrc->TransactionID.c_str(), pSrc->TransactionID.size());
	length += pSrc->TransactionID.size();
	pDst[length++] = 0x00;
	pDst[length++] = 0x8D;//8Dh是MMS-Version，而90h相对应的值为1.0
	pDst[length++] = 0x90;
	pDst[length++] = 0x89;//89h表示源（From）
	string from = pSrc->SenderAddress;
	if (from.substr(0, 3) != "+86")
	{
		if (from.substr(0, 2) != "86")
		{
			from = "+86" + from;
		}
		else if (from.substr(0, 1) != "+")
		{
			from = "+" + from;
		}
	}
	from += "/TYPE=PLMN";
	pDst[length++] = from.size() + 2;
	pDst[length++] = 0x80;
	memcpy(pDst + length, from.c_str(), from.size());
	length += from.size();
	pDst[length++] = 0x00;
//	pDst[length++] = 0x01;
//	pDst[length++] = 0x81;
	pDst[length++] = 0x97;//，97h表示目的地（To）
	string to = pSrc->RecipientAddress;
	if (to.substr(0, 3) != "+86")
	{
		if (to.substr(0, 2) != "86")
		{
			to = "+86" + to;
		}
		else if (to.substr(0, 1) != "+")
		{
			to = "+" + to;
		}
	}
	to += "/TYPE=PLMN";
	memcpy(pDst + length, to.c_str(), to.size());
	length += to.size();
	pDst[length++] = 0x00;
	pDst[length++] = 0x96;//96h表示主题
	memcpy(pDst + length, pSrc->Subject.c_str(), pSrc->Subject.size());
	length += pSrc->Subject.size();
	pDst[length++] = 0x00;
	CTimeSpan ts;
	CTime now = CTime::GetCurrentTime();
	CTime org(1970, 1, 1, 8, 0, 0);
	ts = now - org;
	char psTime[32] = {0};
	LONGLONG seconds = ts.GetTotalSeconds();
	sprintf(psTime, "%x", seconds);
	pDst[length++] = 0x85;//Date
	unsigned char timeLen = strlen(psTime)/2;
	pDst[length++] = timeLen;
	for (int i = timeLen - 1; i >= 0; --i)
	{
		pDst[length++] = (seconds >> (i * 8)) & 0xFF;
	}
	pDst[length++] = 0x86;//
	pDst[length++] = 0x80;
	pDst[length++] = 0x94;//
	pDst[length++] = 0x81;
	pDst[length++] = 0x90;//
	pDst[length++] = 0x81;
	pDst[length++] = 0x8A;//
	pDst[length++] = 0x80;
	pDst[length++] = 0x8F;//
	pDst[length++] = 0x80;
	pDst[length++] = 0x84;//84h表示Content-Type
	pDst[length++] = 0x1C;
	pDst[length++] = 0xB3;//B3h表示application/vnd.wap.multipart.related  A3—表示:application/vnd.wap.multipart.mixed
	pDst[length++] = 0x8A;//8Ah表示起始参数
	memcpy(pDst + length, "<start>", strlen("<start>"));
	length += strlen("<start>");
	pDst[length++] = 0x00;
	pDst[length++] = 0x89;//89h表示类型参数，其值是以纯文本—“application/smil”格式跟 随其后
	memcpy(pDst + length, "application/smil", strlen("application/smil"));
	length += strlen("application/smil");
	pDst[length++] = 0x00;

	unsigned char count = 0;
	vector<MMS_PAR> pars = pSrc->pars;
	vector<MMS_PAR>::iterator iter;
	for (/*vector<MMS_PAR>::iterator */iter = pars.begin(); iter != pars.end(); ++iter)
	{
		for (vector<MMS_SRC>::iterator i = (*iter).srcs.begin(); i != (*iter).srcs.end(); ++i)
		{
			++count;
		}
	}

	pDst[length++] = count + 1;//消息内容由几部分组成 smil、图片、文字、声音

	char* buf = new char[1024*1024];      //64K
	unsigned short bufLen = 0;
	buf[bufLen++] = 0x13;
	memcpy(&buf[bufLen], "application/smil", strlen("application/smil"));
	bufLen += strlen("application/smil");
	buf[bufLen++] = 0x00;
	//memcpy(&buf[bufLen], "Content-ID", strlen("Content-ID"));
	//bufLen += strlen("Content-ID");
	//buf[bufLen++] = 0x00;
	buf[bufLen++] = 0x81;
	buf[bufLen++] = 0x83;
	buf[bufLen++] = 0xC0;
	buf[bufLen++] = 0x22;
	memcpy(&buf[bufLen], "<start>", strlen("<start>"));
	bufLen += strlen("<start>");
	buf[bufLen++] = 0x00;

	pDst[length++] = (unsigned char)bufLen;//内容类型＋其他可能标题 长度

	//smil
	std::string smil = "<smil xmlns=\"http://www.w3.org/2001/SMIL20/Language\">\r\n";
	smil += "  <head>\r\n";
	smil += "    <layout>\r\n";
	smil += "      <root-layout ";
	smil += "width=\"";
	char width[8] = {0};
	_itoa(pSrc->layout.root_layout.width, width, 10);
	smil += width;
	smil += "\" ";
	smil += "height=\"";
	char height[8] = {0};
	_itoa(pSrc->layout.root_layout.height, height, 10);
	smil += height;
	smil += "\"/>\r\n";
	smil += "      <region id=\"Image\" ";
	smil += "width=\"";
	memset(width, 0, 8);
	map<string, MMS_REGION> regions = pSrc->layout.regions;
	MMS_REGION region = regions[string("Image")];
	_itoa(region.width, width, 10);
	smil += width;
	smil += "\" ";
	smil += "height=\"";
	memset(height, 0, 8);
	_itoa(region.height, height, 10);
	smil += height;
	smil += "\" ";
	smil += "left=\"";
	char left[8] = {0};
	_itoa(region.left, left, 10);
	smil += left;
	smil += "\" ";
	smil += "top=\"";
	char top[8] = {0};
	_itoa(region.top, top, 10);
	smil += top;
	smil += "\"/>\r\n";

	smil += "      <region id=\"Text\" ";
	smil += "width=\"";
	memset(width, 0, 8);
	region = regions[string("Text")];
	_itoa(region.width, width, 10);
	smil += width;
	smil += "\" ";
	smil += "height=\"";
	memset(height, 0, 8);
	_itoa(region.height, height, 10);
	smil += height;
	smil += "\" ";
	smil += "left=\"";
	memset(left, 0, 8);
	_itoa(region.left, left, 10);
	smil += left;
	smil += "\" ";
	smil += "top=\"";
	memset(top, 0, 8);
	_itoa(region.top, top, 10);
	smil += top;
	smil += "\"/>\r\n";
	smil += "    </layout>\r\n";
	smil += "  </head>\r\n";
	smil += "\r\n";
	smil += "  <body>\r\n";

	for (/*vector<MMS_PAR>::iterator */iter = pars.begin(); iter != pars.end(); ++iter)
	{
		smil += "    <par dur=\"";
		char duration[8] = {0};
		_itoa((*iter).duration, duration, 10);
		smil += duration;
		smil += "s\">\r\n";

		for (vector<MMS_SRC>::iterator i = (*iter).srcs.begin(); i != (*iter).srcs.end(); ++i)
		{
			std::string name = (*i).name;
			if (name.rfind("/") != string::npos)
			{
				name = name.substr(name.rfind("/") + 1);
			}

			if ((*i).type == stImge)
			{
				smil += "      <img src=\"";
				smil += name;
				smil += "\" region=\"Image\" />\r\n";
			}
			else if ((*i).type == stText)
			{
				smil += "      <text str=\"";
				smil += name;
				smil += "\" region=\"Text\" />\r\n";
			}
			else if ((*i).type == stAudio)
			{
				smil += "      <audio src=\"";
				smil += name;
				smil += "\" />\r\n";
			}		
		}
		smil += "    <par/>\r\n";		
	}

	smil += "  </body>\r\n";
	smil += "</smil>\r\n";

	if (smil.size() >= 0x4FFF)//下面保存数据长度
	{
		unsigned int len = smil.size();

		unsigned char t = len & 0x7F;
		unsigned char t1 = (len >> 7) & 0x7F;
		t1 = t1 | 0x80;
		unsigned char t2 = (len >> 14) & 0x7F;
		t2 = t2 | 0x80;

		pDst[length++] = t2;
		pDst[length++] = t1;
		pDst[length++] = t;
	}
	else if (smil.size() > 0x7F)
	{
		unsigned short len = smil.size();

		unsigned char t = len & 0x7F;
		unsigned char t1 = (len >> 7) & 0x7F;
		t1 = t1 | 0x80;

		pDst[length++] = t1;
		pDst[length++] = t;
	}
	else
	{
		unsigned char len = smil.size();
		pDst[length++] = len;
	}
	memcpy(pDst + length, buf, bufLen);
	length += bufLen;
	memcpy(pDst + length, smil.c_str(), smil.size());
	length += smil.size();

	//content
	for (/*vector<MMS_PAR>::iterator */iter = pars.begin(); iter != pars.end(); ++iter)
	{
		for (vector<MMS_SRC>::iterator i = (*iter).srcs.begin(); i != (*iter).srcs.end(); ++i)
		{
			bufLen = 0;
			string filename = (*i).name;
			string name = filename;
			if (filename.rfind("/") != string::npos)
			{
				name = filename.substr(filename.rfind("/") + 1);
			}

			if ((*i).type == stImge)
			{
				buf[bufLen++] = 4 + (name.size() + 1) * 2;//len 9D 85 8E
			}
			else if ((*i).type == stText)
			{
				buf[bufLen++] = 5 + name.size() + 1;//len 83 81 EA 8E
			}
			else if ((*i).type == stAudio)
			{
				buf[bufLen++] = strlen("audio/") + 3 + 3 + (name.size() + 1) * 2;//len 85 8E
			}

			FILE* f = fopen(filename.c_str(), "rb");
			if (f != NULL)
			{
				fseek(f, 0, SEEK_END);
				unsigned int len = ftell(f);
				fseek(f, 0, SEEK_SET);
				if (len >= 0x4FFF)
				{
					unsigned char t = len & 0x7F;
					unsigned char t1 = (len >> 7) & 0x7F;
					t1 = t1 | 0x80;
					unsigned char t2 = (len >> 14) & 0x7F;
					t2 = t2 | 0x80;

					buf[bufLen++] = t2;
					buf[bufLen++] = t1;
					buf[bufLen++] = t;

				}
				else if (len > 0x7F)
				{
					unsigned char t = len & 0x7F;
					unsigned char t1 = (len >> 7) & 0x7F;
					t1 = t1 | 0x80;

					buf[bufLen++] = t1;
					buf[bufLen++] = t;
				}
				else
				{
					buf[bufLen++] = (unsigned char)len;
				}

				if ((*i).type == stImge)
				{
					string type = name.substr(name.size() - 3, 3);
					unsigned char len = 2 + name.size() + 1;
					buf[bufLen++] = len;
					if (type == "gif")
					{
						buf[bufLen++] = 0x9D;//gif
					}
					else if (type == "jpg")//9E—表示:image/jpeg
					{
						buf[bufLen++] = 0x9E;
					}
					else //wbmp
					{
						buf[bufLen++] = 0xA1;
					}
					buf[bufLen++] = 0x85;
					memcpy(buf + bufLen, name.c_str(), name.size());
					bufLen += name.size();
					buf[bufLen++] = 0x00;
					buf[bufLen++] = 0x8E;
				}
				else if ((*i).type == stText)
				{
					buf[bufLen++] = 0x03;
					buf[bufLen++] = 0x83;
					buf[bufLen++] = 0x81;
					buf[bufLen++] = 0xEA;
					buf[bufLen++] = 0x8E;
				}
				else if ((*i).type == stAudio)
				{
					string audio = "audio/";
					audio += name.substr(name.size() - 3, 3);//扩展名
					unsigned char len = audio.size() + 1 + 1 + name.size() + 1;//85
					buf[bufLen++] = len;				
					memcpy(buf + bufLen, audio.c_str(), audio.size());
					bufLen += audio.size();
					buf[bufLen++] = 0x85;
					memcpy(buf + bufLen, name.c_str(), name.size());
					bufLen += name.size();
					buf[bufLen++] = 0x00;
					buf[bufLen++] = 0x8E;
				}
				memcpy(buf + bufLen, name.c_str(), name.size());
				bufLen += name.size();
				buf[bufLen++] = 0x00;
				fread(buf + bufLen, 1, len, f);
				bufLen += len;
				fclose(f);
				memcpy(pDst + length, buf, bufLen);
				length += bufLen;
			}
		}
	}
	delete[] buf;
	return length;
}

int MMSWarp::DecodeSubmitResponse(const unsigned char* pSrc, int srcLen, MMS_SubmitResponse* pDst)
{
	int length = 0;

	while (length < srcLen)
	{
		unsigned char flag = pSrc[length++];
		if (flag == 0x8C)
		{
			pDst->MessageType = pSrc[length++];
		}
		else if (flag == 0x98)
		{
			pDst->TransactionID = string((char*)pSrc + length);
			length += pDst->TransactionID.size() + 1;
		}
		else if (flag == 0x8D)
		{
			pDst->MMSVersion = pSrc[length++];
		}
		else if (flag == 0x92)
		{
			pDst->RequestStatus = pSrc[length++];
		}
		else if (flag == 0x93)
		{
			pDst->RequestStatusText = string((char*)pSrc + length);
			length += pDst->RequestStatusText.size() + 1;
		}
		else if (flag == 0x8B)
		{
			pDst->MessageID = string((char*)pSrc + length);
			length += pDst->MessageID.size() + 1;
		}
		else
		{
			pSrc[length++];
			printf("DecodeSubmitResponse error flag=%x\n", flag);
		}
	}

	return length;
}

int MMSWarp::DecodeNotificationRequest(const unsigned char* pSrc, int srcLen, MMS_NotificationRequest* pDst)
{
	int length = 0;

	while (length < srcLen)
	{
		unsigned char flag = pSrc[length++];
		if (flag == 0x8C)
		{
			pDst->MessageType = pSrc[length++];
		}
		else if (flag == 0x98)
		{
			pDst->TransactionID = string((char*)pSrc + length);
			length += pDst->TransactionID.size() + 1;
		}
		else if (flag == 0x8D)
		{
			pDst->MMSVersion = pSrc[length++];
		}
		else if (flag == 0x83)
		{
			pDst->MessageReference = string((char*)pSrc + length);
			length += pDst->MessageReference.size() + 1;
		}
		else if (flag == 0x88)
		{
			unsigned char len = pSrc[length++];//0x05 0x81  //Expiry + Length + Relative-token + Length + Delta-secs
			unsigned int n = 0;
			unsigned char token = pSrc[length];
			if (token == 0x81)
			{
				length += 1;
				unsigned char len = pSrc[length];
				length += 1;
				for (int i = 0; i < len; ++i)
				{
					unsigned char t = pSrc[length];
					n = ((n << 8) | t);
					length += 1;
				}
			}
			else
			{
				length += 1;
				unsigned char len = pSrc[length];
				length += 1;
				char buf[128] = {0};
				//unsigned int l = SMS::SMSWarp::GetSMSWarp()->gsmSerializeNumbers(pSrc + length, buf, len);
				unsigned int l = gsmSerializeNumbers((char*)pSrc + length, buf, len);
				std::string t(buf, l);
				CTime expiry(atoi(t.substr(0, 2).c_str()) + 2000, atoi(t.substr(2, 2).c_str()), atoi(t.substr(4, 2).c_str()), atoi(t.substr(6, 2).c_str()), atoi(t.substr(8, 2).c_str()), atoi(t.substr(10, 2).c_str()));
				CTime start(1970, 1, 1, 8, 0, 0);
				CTimeSpan ts = expiry - start;
				n = ts.GetTotalSeconds();
				length += len;
			}
			pDst->TimeOfExpiry = n;
		}
		else if (flag == 0x89)
		{
			unsigned char len = pSrc[length++];
			length += 1;//0x80
			pDst->SenderAddress = string((char*)pSrc + length);
			length += pDst->SenderAddress.size() + 1;
		}
		else if (flag == 0x8A)
		{
			pDst->MessageClass = pSrc[length++];
		}
		else if (flag == 0x8E)
		{
			unsigned char len = pSrc[length++];
			unsigned int n = 0;
			for (int i = 0; i < len; ++i)
			{
				unsigned char s = pSrc[length];
				n = ((n << 8) | s);
				length += 1;
			}
			pDst->MessageSize = n;
		}
	}

	return length;
}

int MMSWarp::EncodeNotificationResponse(const MMS_NotificationResponse* pSrc, char* pDst)
{
	int length = 0;
	pDst[length++] = 0x8C;//X-Mms-Message-Type被编码成8Ch，m-send-req被编码成80h
	pDst[length++] = 0x83;
	pDst[length++] = 0x98;//98h代表Transaction-ID
	memcpy(pDst + length, pSrc->TransactionID.c_str(), pSrc->TransactionID.size());//与接收的NotificationRequest中的TransactionID对应
	length += pSrc->TransactionID.size();
	pDst[length++] = 0x00;
	pDst[length++] = 0x8D;//8Dh是MMS-Version，而90h相对应的值为1.0
	pDst[length++] = 0x90;
	pDst[length++] = 0x95;//MM status
	pDst[length++] = pSrc->MMStatus;
	pDst[length++] = 0x91;
	pDst[length++] = 0x80;
	return length;
}

int MMSWarp::DecodeRetrieveResponse(const unsigned char* pSrc, int srcLen, MMS_RetrieveResponse* pDst)
{
	int length = 0;

	while (length < srcLen)
	{
		unsigned char flag = pSrc[length++];
		if (flag == 0x8C)
		{
			pDst->MessageType = pSrc[length++];
		}
		else if (flag == 0x98)
		{
			pDst->TransactionID = string((char*)pSrc + length);
			length += pDst->TransactionID.size() + 1;
		}
		else if (flag == 0x8D)
		{
			pDst->MMSVersion = pSrc[length++];
		}
		else if (flag == 0x8B)
		{
			pDst->MessageID = string((char*)pSrc + length);
			length += pDst->MessageID.size() + 1;
		}
		else if (flag == 0x89)
		{
			unsigned char len = pSrc[length++];
			length += 1;//0x80
			pDst->SenderAddress = string((char*)pSrc + length);
			length += pDst->SenderAddress.size() + 1;
			if (pDst->SenderAddress.find("/") != string::npos)
			{
				pDst->SenderAddress = pDst->SenderAddress.substr(0, pDst->SenderAddress.find("/"));
			}
		}
		else if (flag == 0x97)
		{
			pDst->RecipientAddress = string((char*)pSrc + length);
			length += pDst->RecipientAddress.size() + 1;
			if (pDst->RecipientAddress.find("/") != string::npos)
			{
				pDst->RecipientAddress = pDst->RecipientAddress.substr(0, pDst->RecipientAddress.find("/"));
			}
		}
		else if (flag == 0x8A)
		{
			pDst->MessageClass = pSrc[length++];
		}
		else if (flag == 0x8F)
		{
			pDst->Priority = pSrc[length++];
		}
		else if (flag == 0x86)
		{
			pDst->DeliveryReport = pSrc[length++];
		}
		else if (flag == 0x85)//Date
		{
			unsigned char len = pSrc[length++];
			unsigned int n = 0;
			for (int i = 0; i < len; ++i)
			{
				unsigned char t = pSrc[length];
				n = ((n << 8) | t);
				length += 1;
			}
			pDst->DateAndTime = n;
		}
		else if (flag == 0x96)
		{
			unsigned char len = pSrc[length];
			if (len == 0x1F)
			{
				length++;
				len = pSrc[length++];
				if ((pSrc[length] == 0xEA) && (pSrc[length + 1] == 0x7F))
				{
					length = length + 2;//EA 7F
					len = len - 2;
				}
				pDst->Subject = string((char*)pSrc + length);
				length += len;
			}
			else if ((pSrc[length + 1] == 0xEA) && (pSrc[length + 2] == 0x7F))
			{
				length++;
				length = length + 2;//EA 7F
				len = len - 2;
				pDst->Subject = string((char*)pSrc + length);
				length += len;
			}
			else
			{
				pDst->Subject = string((char*)pSrc + length);
				length += pDst->Subject.size() + 1;
			}
			pDst->Subject = Convert(pDst->Subject, CP_UTF8, 936);
		}
		else if (flag == 0x84)//Content type
		{
			unsigned char value = pSrc[length++];

			if (value == 0xA3)//mixed
			{
				pDst->ContentType = value;

			}
			else
			{
				unsigned char len = value;
				if (len == 0x1F)
				{
					len = pSrc[length++];
				}
				unsigned char contentType = pSrc[length];
				if (contentType == 0xB3)//related
				{
					pDst->ContentType = contentType;
					unsigned char * p = (unsigned char*)pSrc + length + 1;
					unsigned int pos = 0;
					while (pos < len - 1)
					{
						unsigned char type = p[pos];
						if (type == 0x89)//application/smil
						{
							pos++;
							pDst->MultipartRelatedType = string((char*)p + pos);
							pos += pDst->MultipartRelatedType.size() + 1;
						}
						else if (type == 0x8A)//start
						{
							pos++;
							pDst->ContentID = string((char*)p + pos);
							pos += pDst->ContentID.size() + 1;
						}
						else
						{
							pos++;
						}
					}
					length += len;
					unsigned char count = pSrc[length++];//消息内容由几部分组成  
					while ((length < srcLen)/* && (count > 0)*/)
					{
						unsigned int headLen = pSrc[length];//内容类型＋其他可能标题 长度//数据长度
						unsigned int dataLen = 0;
						unsigned char* pBuf = (unsigned char*)(pSrc + length);
						unsigned int pos = 0;
						unsigned char type = 0;

						unsigned char textimage[] = {0x61, 0x83, 0x9D, 0x9E, 0xA1};
						bool result = false;
						for (int i = 0; i < sizeof(textimage); ++i)
						{
							if ((pBuf [1] &0x80) != 0x80)
							{
								if (textimage[i] == pBuf[2])
								{
									pos = 2;
									type = textimage[i];
									result = true;
									break;
								}	
								else if (textimage[i] == pBuf[3])
								{
									pos = 3;
									type = textimage[i];
									result = true;
									break;
								}
							}
							else
							{
								if (textimage[i] == pBuf[3])
								{
									pos = 3;
									type = textimage[i];
									result = true;
									break;
								}
								else if (textimage[i] == pBuf[4])
								{
									pos = 4;
									type = textimage[i];
									result = true;
									break;
								}
								else if (textimage[i] == pBuf[5])
								{
									pos = 5;
									type = textimage[i];
									result = true;
									break;
								}
							}	
						}

						if (result)
						{
							if (((pBuf[pos - 1] & 0x80) != 0x80) && ((pBuf[pos - 2] & 0x80) != 0x80))//have content type length
							{
								pos --;
							}
							pos--;//dataLen length
						}

						string head((char*)pBuf + pos + 1, headLen);

						dataLen = pBuf[pos];
						if (pos == 2)
						{
							unsigned int t = pBuf[pos - 1] & 0x7F;
							t = t << 7;
							dataLen = dataLen | t;
						}
						else if (pos == 3)
						{
							unsigned int t1 = pBuf[pos - 1] & 0x7F;
							unsigned int t2 = pBuf[pos - 2] & 0x7F;
							unsigned int t = (t2 << 7) | t1; 

							t = t << 7;
							dataLen = dataLen | t;
						}

						pBuf += 1;//headLen
						pBuf += pos;//dataLen
						pBuf += headLen;
						string name;
						for (int j = 1; j < headLen; ++j)
						{
							if (*(pBuf - j) == 0x8E)
							{
								name = string((char*)(pBuf - j + 1));
								break;
							}
							else if (*(pBuf - j) == '<')//((*(pBuf - j - 1) == 0xC0) && (*(pBuf - j) == 0x22))
							{
								name = string((char*)(pBuf - j + 1));
								name = name.substr(0, name.size() - 1);//remove < and >
								break;
							}
						}

						if (name.empty())
						{
							static int serial = 0;
							char buf[32] = {0};
							//itoa(0, buf, 10);
							sprintf(buf, "%d", serial);
							name = string(buf);
							serial++;
						}
						
						string suffix;
						if (name.find(".") == string::npos)
						{
							switch (type)
							{
							case 0x61://audio/* or application/smil
								if (head.find("audio") != string::npos)
								{
									suffix = head.substr(head.find("audio") + strlen("audio/"), 3);
								}
								else if (head.find("application/smil") != string::npos)
								{
									suffix = head.substr(head.find("application/smil") + strlen("application/"), 3);
								}
								break;
							case 0x83:
								suffix = "txt";
								break;
							case 0x9D:
								suffix = "gif";
								break;
							case 0x9E:
								suffix = "jpg";
								break;
							case 0xA1:
								suffix = "bmp";
								break;
							}
							name = name + "." + suffix;
						}

						if ((type == 0x61) && (head.find("application/smil") != string::npos))//application/smil
						{
							string smil((char*)pBuf, dataLen);
							std::transform(smil.begin(), smil.end(), smil.begin(), tolower);

							unsigned int pos1 = 0;
							unsigned int pos2 = 0;
							pos1 = smil.find("<layout>");
							pos2 = smil.find("</layout>");
							if ((pos1 != string::npos ) && (pos2 != string::npos))
							{
								pos1 += strlen("<layout>");
								string layout = smil.substr(pos1, pos2 - pos1);
								pDst->layout.root_layout.width = 0;
								pDst->layout.root_layout.height = 0;
								pos1 = layout.find("<root-layout");
								if (pos1 != string::npos )
								{
									pos2 = layout.find(">", pos1);
									string root = layout.substr(pos1, pos2 - pos1);
									layout = layout.substr(pos2 + strlen(">"));
									pos1 = root.find("width");
									if (pos1 != string::npos)
									{
										string width = root.substr(pos1);
										pos1 = width.find("\"");
										width = width.substr(pos1 + 1);
										pDst->layout.root_layout.width = atoi(width.c_str());
									}
									pos1 = root.find("height");
									if (pos1 != string::npos)
									{
										string height = root.substr(pos1);
										pos1 = height.find("\"");
										height = height.substr(pos1 + 1);
										pDst->layout.root_layout.height = atoi(height.c_str());
									}
								}

								while(layout.find("<region ") != string::npos) 
								{
									pos1 = layout.find("<region ");
									pos2 = layout.find(">", pos1);

									string region = layout.substr(pos1, pos2 - pos1);
									layout = layout.substr(pos2 + strlen(">"));
									pos1 = region.find("id");
									if (pos1 != string::npos)
									{
										pos1 = region.find("\"", pos1 + 1);
										pos2 = region.find("\"", pos1 + 1);
										if ((pos1 != string::npos ) && (pos2 != string::npos))
										{
											string id = region.substr(pos1 + 1, pos2 - pos1 - 1);

											MMS_REGION mms_region;
											mms_region.top = 0;
											mms_region.left = 0;
											mms_region.width = 0;
											mms_region.height = 0;

											pos1 = region.find("width");
											if (pos1 != string::npos)
											{
												string width = region.substr(pos1);
												pos1 = width.find("\"");
												width = width.substr(pos1 + 1);
												mms_region.width = atoi(width.c_str());
											}
											pos1 = region.find("height");
											if (pos1 != string::npos)
											{
												string height = region.substr(pos1);
												pos1 = height.find("\"");
												height = height.substr(pos1 + 1);
												mms_region.height = atoi(height.c_str());
											}
											pos1 = region.find("left");
											if (pos1 != string::npos)
											{
												string left = region.substr(pos1);
												pos1 = left.find("\"");
												left = left.substr(pos1 + 1);
												mms_region.left = atoi(left.c_str());
											}
											pos1 = region.find("top");
											if (pos1 != string::npos)
											{
												string top = region.substr(pos1);
												pos1 = top.find("\"");
												top = top.substr(pos1 + 1);
												mms_region.top = atoi(top.c_str());
											}
											pos1 = region.find("fit");
											if (pos1 != string::npos)
											{
												string fit = region.substr(pos1);
												pos1 = fit.find("\"");
												fit = fit.substr(pos1 + 1);
												mms_region.fit = fit;
											}

											pDst->layout.regions[id] = mms_region;
										}
									}
								}
							}

							pos1 = smil.find("<body>");
							pos2 = smil.find("</body>");
							if ((pos1 != string::npos ) && (pos2 != string::npos))
							{
								pos1 += sizeof("<body>");
								string body = smil.substr(pos1, pos2 - pos1);
								while ((body.find("<par") != string::npos) && (body.find("</par>") != string::npos))
								{
									MMS_PAR mms_par;
									pos1 = body.find("<par");
									pos2 = body.find("</par>");
									string par = body.substr(pos1, pos2 - pos1);
									body = body.substr(pos2 + strlen("</par>"));
									par.substr(strlen("<par"));

									pos1 = par.find("dur");
									pos2 = par.find("s\">");
									if ((pos1 < pos2) && (pos1 != string::npos ) && (pos2 != string::npos))
									{
										pos1 = par.find("\"");
										if (pos1 != string::npos)
										{
											string dur = par.substr(pos1 + 1);
											mms_par.duration = atoi(dur.c_str());
										}
										par = par.substr(pos2 + strlen("s\">"));
									}

									while ((par.find("<") != string::npos) && (par.find(">") != string::npos))
									{
										pos1 = par.find("<");
										pos2 = par.find(">");
										string src = par.substr(pos1, pos2 - pos1);
										par = par.substr(pos2 + strlen(">"));

										MMS_SRC mms_src;
										if (src.find("<img ") != string::npos)
										{
											mms_src.type = stImge;
										}
										else if (src.find("<text ") != string::npos)
										{
											mms_src.type = stText;
										}
										else if (src.find("<audio ") != string::npos)
										{
											mms_src.type = stAudio;
										}

										pos1 = src.find("src");
										if (pos1 != string::npos)
										{
											pos1 = src.find("\"", pos1 + 1);
											pos2 = src.find("\"", pos1 + 1);
											if ((pos1 != string::npos ) && (pos2 != string::npos))
											{
												string name = src.substr(pos1 + 1, pos2 - pos1 - 1);
												if (name.substr(0, 4) == "cid:")
												{
													name = name.substr(4);
												}
												mms_src.name = name;
											}
										}
										pos1 = src.find("region");
										if (pos1 != string::npos)
										{
											pos1 = src.find("\"", pos1 + 1);
											pos2 = src.find("\"", pos1 + 1);
											if ((pos1 != string::npos ) && (pos2 != string::npos))
											{
												string region = src.substr(pos1 + 1, pos2 - pos1 - 1);
												mms_src.region = region;
											}
										}
										pos1 = src.find("begin");
										if (pos1 != string::npos)
										{
											pos1 = src.find("\"", pos1 + 1);
											pos2 = src.find("\"", pos1 + 1);
											if ((pos1 != string::npos ) && (pos2 != string::npos))
											{
												string begin = src.substr(pos1 + 1, pos2 - pos1 - 1);
												mms_src.begin = atoi(begin.c_str());
											}
										}
										pos1 = src.find("end");
										if (pos1 != string::npos)
										{
											pos1 = src.find("\"", pos1 + 1);
											pos2 = src.find("\"", pos1 + 1);
											if ((pos1 != string::npos ) && (pos2 != string::npos))
											{
												string end = src.substr(pos1 + 1, pos2 - pos1 - 1);
												mms_src.end = atoi(end.c_str());
											}
										}
										pos1 = src.find("dur");
										if (pos1 != string::npos)
										{
											pos1 = src.find("\"", pos1 + 1);
											pos2 = src.find("\"", pos1 + 1);
											if ((pos1 != string::npos ) && (pos2 != string::npos))
											{
												string dur = src.substr(pos1 + 1, pos2 - pos1 - 1);
												mms_src.dur = atoi(dur.c_str());
											}
										}
										pos1 = src.find("alt");
										if (pos1 != string::npos)
										{
											pos1 = src.find("\"", pos1 + 1);
											pos2 = src.find("\"", pos1 + 1);
											if ((pos1 != string::npos ) && (pos2 != string::npos))
											{
												string alt = src.substr(pos1 + 1, pos2 - pos1 - 1);
												mms_src.alt = alt;
											}
										}
										mms_par.srcs.push_back(mms_src);
									}

									pDst->pars.push_back(mms_par);
								}
							}
						}

						FILE* f = fopen(name.c_str(), "wb");
						fwrite(pBuf, 1, dataLen, f);
						fclose(f);
						pDst->content.push_back(name);

						length += 1;
						length += pos;
						length += headLen;
						length += dataLen;
						count--;
					}
				}
			}
		}
		else
		{
			pSrc[length++];
			printf("DecodeRetrieveResponse error flag=%x\n", flag);
		}
	}

	return length;
}

int MMSWarp::EncodeAcknowledgementRequest(const MMS_AcknowledgementRequest* pSrc, char* pDst)
{
	int length = 0;
	pDst[length++] = 0x8C;//X-Mms-Message-Type被编码成8Ch，m-send-req被编码成80h
	pDst[length++] = 0x85;
	pDst[length++] = 0x98;//98h代表Transaction-ID
	memcpy(pDst + length, pSrc->TransactionID.c_str(), pSrc->TransactionID.size());//与接收的NotificationRequest中的TransactionID对应
	length += pSrc->TransactionID.size();
	pDst[length++] = 0x00;
	pDst[length++] = 0x8D;//8Dh是MMS-Version，而90h相对应的值为1.0
	pDst[length++] = 0x90;
	pDst[length++] = 0x91;
	pDst[length++] = 0x80;
	return length;
}

int MMSWarp::DecodeDeliveryReportingRequest(const unsigned char* pSrc, int srcLen, MMS_DeliveryReportingRequest* pDst)
{
	int length = 0;

	while (length < srcLen)
	{
		unsigned char flag = pSrc[length++];
		if (flag == 0x8C)
		{
			pDst->MessageType = pSrc[length++];
		}
		else if (flag == 0x8D)
		{
			pDst->MMSVersion = pSrc[length++];
		}
		else if (flag == 0x8B)
		{
			pDst->MessageID = string((char*)pSrc + length);
			length += pDst->MessageID.size() + 1;
		}
		else if (flag == 0x97)
		{
			pDst->RecipientAddress = string((char*)pSrc + length);
			length += pDst->RecipientAddress.size() + 1;
		}
		else if (flag == 0x85)//Date
		{
			unsigned char len = pSrc[length++];
			unsigned int n = 0;
			for (int i = 0; i < len; ++i)
			{
				unsigned char t = pSrc[length];
				n = ((n << 8) | t);
				length += 1;
			}	
			pDst->DateAndTime = n;
		}
		else if (flag == 0x92)
		{
			pDst->MMStatus = pSrc[length++];
		}
		else
		{
			pSrc[length++];
			printf("DecodeDeliveryReportingRequest error flag=%x\n", flag);
		}
	}

	return length;
}

Data::MMSData* MMSWarp::DecodeMessage(const unsigned char* pSrc, int srcLen)
{
	MMS_RetrieveResponse rep;
	DecodeRetrieveResponse(pSrc, srcLen, &rep);
	Data::MMSData* msg = new Data::MMSData();
	msg->MessageType = Util::StringOp::FromInt(rep.MessageType);
	msg->TransactionId = rep.TransactionID;
	msg->MMSVersion = Util::StringOp::FromInt(rep.MMSVersion);
	msg->MessageId = rep.MessageID;
	msg->SenderAddress = rep.SenderAddress;
	msg->ContentType = Util::StringOp::FromInt(rep.ContentType);
	msg->RecipientAddress = rep.RecipientAddress;
	msg->MessageClass = Util::StringOp::FromInt(rep.MessageClass);
	msg->DateAndTime = rep.DateAndTime;
	msg->Subject = rep.Subject;
	msg->RequestStatus = Util::StringOp::FromInt(rep.RequestStatus);
	msg->isRead = 0;
	msg->type = Data::MMSData::tpReceive;
	
	CTime t = CTime::GetCurrentTime();
	char buf[128] = {0};
	sprintf(buf, "/Flashdrv/3G/MMS/%d%02d%02d%02d%02d%02d/", t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	msg->SavePath = buf;
	CreateDirectory(_T("/Flashdrv/3G/MMS/"), NULL);
	CreateDirectory(ToUnicode(msg->SavePath).c_str(), NULL);
	for (vector<string>::iterator iter = rep.content.begin(); iter != rep.content.end(); ++iter)
	{
		CopyFile(ToUnicode(*iter).c_str(), ToUnicode(msg->SavePath + *iter).c_str(), FALSE);
		DeleteFile(ToUnicode(*iter).c_str());
	}
	return msg;
}

unsigned int MMSWarp::DecodeSubmitResponse(const unsigned char* pSrc, int srcLen)
{
	MMS_SubmitResponse res;
	DecodeSubmitResponse(pSrc, srcLen, &res);
	return res.RequestStatus - 0x80;
}

unsigned int MMSWarp::EncodeNotificationResponse(std::string transactionId, unsigned char status, char* pDst)
{
	MMS_NotificationResponse res;
	res.TransactionID = transactionId;
	res.MMStatus = status;
	return EncodeNotificationResponse(&res, pDst);
}

void MMSWarp::DecodeSmil(std::string file, vector<MMS_PAR>& vpars, MMS_LAYOUT& vlayout)
{
	std::string path;
	if (file.rfind("/") != string::npos)
	{
		path = file.substr(0, file.rfind("/") + 1);
	}

	FILE* f = fopen(file.c_str(), "rb");
	fseek(f, 0, SEEK_END);
	unsigned int dataLen = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* pBuf = new char[dataLen + 1];
	fread(pBuf, 1, dataLen, f);
	fclose(f);
	string smil((char*)pBuf, dataLen);
	std::transform(smil.begin(), smil.end(), smil.begin(), tolower);

	unsigned int pos1 = 0;
	unsigned int pos2 = 0;
	pos1 = smil.find("<layout>");
	pos2 = smil.find("</layout>");
	if ((pos1 != string::npos ) && (pos2 != string::npos))
	{
		pos1 += strlen("<layout>");
		string layout = smil.substr(pos1, pos2 - pos1);
		vlayout.root_layout.width = 0;
		vlayout.root_layout.height = 0;
		pos1 = layout.find("<root-layout");
		if (pos1 != string::npos )
		{
			pos2 = layout.find(">", pos1);
			string root = layout.substr(pos1, pos2 - pos1);
			layout = layout.substr(pos2 + strlen(">"));
			pos1 = root.find("width");
			if (pos1 != string::npos)
			{
				string width = root.substr(pos1);
				pos1 = width.find("\"");
				width = width.substr(pos1 + 1);
				vlayout.root_layout.width = atoi(width.c_str());
			}
			pos1 = root.find("height");
			if (pos1 != string::npos)
			{
				string height = root.substr(pos1);
				pos1 = height.find("\"");
				height = height.substr(pos1 + 1);
				vlayout.root_layout.height = atoi(height.c_str());
			}
		}
		while(layout.find("<region ") != string::npos) 
		{
			pos1 = layout.find("<region ");
			pos2 = layout.find(">", pos1);

			string region = layout.substr(pos1, pos2 - pos1);
			layout = layout.substr(pos2 + strlen(">"));
			pos1 = region.find("id");
			if (pos1 != string::npos)
			{
				pos1 = region.find("\"", pos1 + 1);
				pos2 = region.find("\"", pos1 + 1);
				if ((pos1 != string::npos ) && (pos2 != string::npos))
				{
					string id = region.substr(pos1 + 1, pos2 - pos1 - 1);

					MMS_REGION mms_region;
					mms_region.top = 0;
					mms_region.left = 0;
					mms_region.width = 0;
					mms_region.height = 0;

					pos1 = region.find("width");
					if (pos1 != string::npos)
					{
						string width = region.substr(pos1);
						pos1 = width.find("\"");
						width = width.substr(pos1 + 1);
						mms_region.width = atoi(width.c_str());
					}
					pos1 = region.find("height");
					if (pos1 != string::npos)
					{
						string height = region.substr(pos1);
						pos1 = height.find("\"");
						height = height.substr(pos1 + 1);
						mms_region.height = atoi(height.c_str());
					}
					pos1 = region.find("left");
					if (pos1 != string::npos)
					{
						string left = region.substr(pos1);
						pos1 = left.find("\"");
						left = left.substr(pos1 + 1);
						mms_region.left = atoi(left.c_str());
					}
					pos1 = region.find("top");
					if (pos1 != string::npos)
					{
						string top = region.substr(pos1);
						pos1 = top.find("\"");
						top = top.substr(pos1 + 1);
						mms_region.top = atoi(top.c_str());
					}
					pos1 = region.find("fit");
					if (pos1 != string::npos)
					{
						string fit = region.substr(pos1);
						pos1 = fit.find("\"");
						fit = fit.substr(pos1 + 1);
						mms_region.fit = fit;
					}

					vlayout.regions[id] = mms_region;
				}
			}
		}
	}

	pos1 = smil.find("<body>");
	pos2 = smil.find("</body>");
	if ((pos1 != string::npos ) && (pos2 != string::npos))
	{
		pos1 += sizeof("<body>");
		string body = smil.substr(pos1, pos2 - pos1);
		while ((body.find("<par") != string::npos) && (body.find("</par>") != string::npos))
		{
			MMS_PAR mms_par;
			pos1 = body.find("<par");
			pos2 = body.find("</par>");
			string par = body.substr(pos1, pos2 - pos1);
			body = body.substr(pos2 + strlen("</par>"));
			par.substr(strlen("<par"));

			pos1 = par.find("dur");
			pos2 = par.find("s\">");
			if ((pos1 < pos2) && (pos1 != string::npos ) && (pos2 != string::npos))
			{
				pos1 = par.find("\"");
				if (pos1 != string::npos)
				{
					string dur = par.substr(pos1 + 1);
					mms_par.duration = atoi(dur.c_str());
					if (par[pos2 - 1] == 'm')
					{
						mms_par.duration = mms_par.duration / 1000;
					}
				}
				par = par.substr(pos2 + strlen("s\">"));
			}

			while ((par.find("<") != string::npos) && (par.find(">") != string::npos))
			{
				pos1 = par.find("<");
				pos2 = par.find(">");
				string src = par.substr(pos1, pos2 - pos1);
				par = par.substr(pos2 + strlen(">"));

				MMS_SRC mms_src;
				if (src.find("<img ") != string::npos)
				{
					mms_src.type = stImge;
				}
				else if (src.find("<text ") != string::npos)
				{
					mms_src.type = stText;
				}
				else if (src.find("<audio ") != string::npos)
				{
					mms_src.type = stAudio;
				}

				pos1 = src.find("src");
				if (pos1 != string::npos)
				{
					pos1 = src.find("\"", pos1 + 1);
					pos2 = src.find("\"", pos1 + 1);
					if ((pos1 != string::npos ) && (pos2 != string::npos))
					{
						string name = src.substr(pos1 + 1, pos2 - pos1 - 1);
						if (name.substr(0, 4) == "cid:")
						{
							name = name.substr(4);
						}
						name = path + name;
						if ((name.size() < 4 ) || (name.substr(name.size() - 4, 1) != "."))//无扩展名
						{
							WIN32_FIND_DATA fd; 
							HANDLE hFind = FindFirstFile(ToUnicode(name+".*").c_str(), &fd); 
							if ((hFind != INVALID_HANDLE_VALUE)) 
							{ 
								while(true)
								{
									if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
									{
										std::string file = FromUnicode(fd.cFileName);
										std::string suffix = file.substr(file.rfind(".") + 1);
										if (mms_src.type == stImge)
										{
											if ((suffix == "jpg")
												|| (suffix == "gif")
												|| (suffix == "bmp"))
											{
												name = path + file;
												break;
											}
										}
										else if (mms_src.type == stText)
										{
											if (suffix == "txt")
											{
												name = path + file;
												break;
											}
										}
										else if (mms_src.type == stAudio)
										{
											if ((suffix == "amr")
												|| (suffix == "mid")
												|| (suffix == "wav"))
											{
												name = path + file;
												break;
											}
										}
									}	
									if (!FindNextFile(hFind, &fd))
									{
										break;
									}
								}
							} 
							FindClose(hFind);
						}
						mms_src.name = name;
					}
				}
				pos1 = src.find("region");
				if (pos1 != string::npos)
				{
					pos1 = src.find("\"", pos1 + 1);
					pos2 = src.find("\"", pos1 + 1);
					if ((pos1 != string::npos ) && (pos2 != string::npos))
					{
						string region = src.substr(pos1 + 1, pos2 - pos1 - 1);
						mms_src.region = region;
					}
				}
				pos1 = src.find("begin");
				if (pos1 != string::npos)
				{
					pos1 = src.find("\"", pos1 + 1);
					pos2 = src.find("\"", pos1 + 1);
					if ((pos1 != string::npos ) && (pos2 != string::npos))
					{
						string begin = src.substr(pos1 + 1, pos2 - pos1 - 1);
						mms_src.begin = atoi(begin.c_str());
					}
				}
				pos1 = src.find("end");
				if (pos1 != string::npos)
				{
					pos1 = src.find("\"", pos1 + 1);
					pos2 = src.find("\"", pos1 + 1);
					if ((pos1 != string::npos ) && (pos2 != string::npos))
					{
						string end = src.substr(pos1 + 1, pos2 - pos1 - 1);
						mms_src.end = atoi(end.c_str());
					}
				}
				pos1 = src.find("dur");
				if (pos1 != string::npos)
				{
					pos1 = src.find("\"", pos1 + 1);
					pos2 = src.find("\"", pos1 + 1);
					if ((pos1 != string::npos ) && (pos2 != string::npos))
					{
						string dur = src.substr(pos1 + 1, pos2 - pos1 - 1);
						mms_src.dur = atoi(dur.c_str());
					}
				}
				pos1 = src.find("alt");
				if (pos1 != string::npos)
				{
					pos1 = src.find("\"", pos1 + 1);
					pos2 = src.find("\"", pos1 + 1);
					if ((pos1 != string::npos ) && (pos2 != string::npos))
					{
						string alt = src.substr(pos1 + 1, pos2 - pos1 - 1);
						mms_src.alt = alt;
					}
				}
				mms_par.srcs.push_back(mms_src);
			}

			vpars.push_back(mms_par);
		}
	}
	delete[] pBuf;
}

void MMSWarp::EncodeSmil(vector<MMS_PAR> vpars, MMS_LAYOUT layout, std::string path)
{
	//smil
	std::string smil = "<smil xmlns=\"http://www.w3.org/2001/SMIL20/Language\">\r\n";
	smil += "  <head>\r\n";
	smil += "    <layout>\r\n";
	smil += "      <root-layout ";
	smil += "width=\"";
	char width[8] = {0};
	_itoa(layout.root_layout.width, width, 10);
	smil += width;
	smil += "\" ";
	smil += "height=\"";
	char height[8] = {0};
	_itoa(layout.root_layout.height, height, 10);
	smil += height;
	smil += "\"/>\r\n";
	smil += "      <region id=\"Image\" ";
	smil += "width=\"";
	memset(width, 0, 8);
	map<string, MMS_REGION> regions = layout.regions;
	MMS_REGION region = regions[string("Image")];
	_itoa(region.width, width, 10);
	smil += width;
	smil += "\" ";
	smil += "height=\"";
	memset(height, 0, 8);
	_itoa(region.height, height, 10);
	smil += height;
	smil += "\" ";
	smil += "left=\"";
	char left[8] = {0};
	_itoa(region.left, left, 10);
	smil += left;
	smil += "\" ";
	smil += "top=\"";
	char top[8] = {0};
	_itoa(region.top, top, 10);
	smil += top;
	smil += "\"/>\r\n";
	
	smil += "      <region id=\"Text\" ";
	smil += "width=\"";
	memset(width, 0, 8);
	region = regions[string("Text")];
	_itoa(region.width, width, 10);
	smil += width;
	smil += "\" ";
	smil += "height=\"";
	memset(height, 0, 8);
	_itoa(region.height, height, 10);
	smil += height;
	smil += "\" ";
	smil += "left=\"";
	memset(left, 0, 8);
	_itoa(region.left, left, 10);
	smil += left;
	smil += "\" ";
	smil += "top=\"";
	memset(top, 0, 8);
	_itoa(region.top, top, 10);
	smil += top;
	smil += "\"/>\r\n";
	smil += "    </layout>\r\n";
	smil += "  </head>\r\n";
	smil += "\r\n";
	smil += "  <body>\r\n";
	
	for (vector<MMS_PAR>::iterator iter = vpars.begin(); iter != vpars.end(); ++iter)
	{
		smil += "    <par dur=\"";
		char duration[8] = {0};
		_itoa((*iter).duration, duration, 10);
		smil += duration;
		smil += "s\">\r\n";
		
		for (vector<MMS_SRC>::iterator i = (*iter).srcs.begin(); i != (*iter).srcs.end(); ++i)
		{
			std::string name = (*i).name;			
			if (name.rfind("/") != string::npos)
			{
				name = name.substr(name.rfind("/") + 1);
			}
			
			CopyFile(ToUnicode((*i).name).c_str(), ToUnicode(path + name).c_str(), FALSE);
			DeleteFile(ToUnicode((*i).name).c_str());

			if ((*i).type == stImge)
			{
				smil += "      <img src=\"";
				smil += name;
				smil += "\" region=\"Image\" />\r\n";
			}
			else if ((*i).type == stText)
			{
				smil += "      <text str=\"";
				smil += name;
				smil += "\" region=\"Text\" />\r\n";
			}
			else if ((*i).type == stAudio)
			{
				smil += "      <audio src=\"";
				smil += name;
				smil += "\" />\r\n";
			}		
		}
		smil += "    <par/>\r\n";		
	}
	
	smil += "  </body>\r\n";
	smil += "</smil>\r\n";

	FILE* f= fopen("start.smil", "wb");
	fwrite(smil.c_str(), 1, smil.size(), f);
	fclose(f);
}

void MMSWarp::AddRootLayout(MMS_SubmitRequest* req, unsigned int width,	unsigned int height)
{
	req->layout.root_layout.width = width;
	req->layout.root_layout.height = height;
}

void MMSWarp::AddRegion(MMS_SubmitRequest* req, MMS_REGION_TYPE id, unsigned int left, unsigned int top, unsigned int width, unsigned int height, MMS_REGION_FIT fit)
{
	char* types[] = {"Image", "Text", "Video"};
	char* fits[] = {"hidden", "fill", "meet", "scroll", "slice"};
	
	MMS_REGION region;
	region.id = types[id];
	region.left = left;
	region.top = top;
	region.width = width;
	region.height = height;
	region.fit = fits[fit];      	
	
	req->layout.regions[region.id] = region;
}

void MMSWarp::AddPar(MMS_SubmitRequest* req, string imageFile, string textFile, string audioFile, unsigned int dur)
{	
	MMS_PAR par;
	par.duration = dur;
	
	if (!imageFile.empty())
	{
		MMS_SRC image;
		image.type = MMS::MMSWarp::MMS_SRCTYPE::stImge;
		image.name = imageFile;
		image.region = "Image";	
		par.srcs.push_back(image);
	}
	
	if (!textFile.empty())
	{
		MMS_SRC text;
		text.type = MMS::MMSWarp::MMS_SRCTYPE::stText;
		text.name = textFile;
		text.region = "Text";	
		par.srcs.push_back(text);
	}
	
	if (!audioFile.empty())
	{
		MMS_SRC audio;
		audio.type = MMS::MMSWarp::MMS_SRCTYPE::stAudio;
		audio.name = audioFile;
		par.srcs.push_back(audio);
	}							
	req->pars.push_back(par);
}

// 两两颠倒的字符串转换为正常顺序的字符串
// 如："683158812764F8" --> "8613851872468"   	
// 输入: pSrc - 源字符串指针    
//       nSrcLength - 源字符串长度    
// 输出: pDst - 目标字符串指针    
// 返回: 目标字符串长度    
int MMSWarp::gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength; // 目标字符串长度
	char ch; // 用于保存一个字符
	// 复制串长度
	nDstLength = nSrcLength;
	// 两两颠倒
	for(int i=0; i<nSrcLength;i+=2) {
		ch = *pSrc++; // 保存先出现的字符
		*pDst++ = *pSrc++; // 复制后出现的字符
		*pDst++ = ch; // 复制先出现的字符
	}
	// 最后的字符是'F'吗？
	if(*(pDst-1) == 'F') {
		pDst--;
		nDstLength--; // 目标字符串长度减1
	}
	*pDst = '\0'; // 输出字符串加个结束符
	return nDstLength; // 返回目标字符串长度
}

std::wstring MMSWarp::ToUnicode(std::string content)
{
	size_t wideContentLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), -1, 0, 0);
	wchar_t* wideContent = new wchar_t[wideContentLength + 1];
	memset(wideContent, 0, sizeof(wchar_t) * (wideContentLength + 1));
	size_t convLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), content.length(), wideContent, wideContentLength);
	std::wstring result(wideContent);
	delete[] wideContent;
	return result;
}
std::string MMSWarp::FromUnicode(std::wstring content)
{
	int length = WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)content.c_str(), -1, 0, 0, 0, NULL);
	char* ansiContent = new char[length + 1];
	memset(ansiContent, 0, length + 1);

	length = WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)content.c_str(), -1, ansiContent, length + 1, 0, NULL);

	std::string result = ansiContent;
	delete[] ansiContent;
	return result;
}

string MMSWarp::Convert(string str, int sourceCodepage, int targetCodepage)
{
	int len=str.size();
	
	int unicodeLen=MultiByteToWideChar(sourceCodepage,0,str.c_str(),-1,NULL,0);
	
	wchar_t* pUnicode;
	pUnicode=new wchar_t[unicodeLen+1];
	
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));
	
	MultiByteToWideChar(sourceCodepage,0,str.c_str(),-1,(LPWSTR)pUnicode,unicodeLen);
	
	BYTE * pTargetData = NULL;
	int targetLen=WideCharToMultiByte(targetCodepage,0,(LPWSTR)pUnicode,-1,(char *)pTargetData,0,NULL,NULL);
	
	pTargetData=new BYTE[targetLen+1];
	memset(pTargetData,0,targetLen+1);
	
	WideCharToMultiByte(targetCodepage,0,(LPWSTR)pUnicode,-1,(char *)pTargetData,targetLen,NULL,NULL);
	
	string rt((char*)pTargetData/*, targetLen*/);
	
	delete[] pUnicode;
	delete[] pTargetData;
	
	return rt;
}