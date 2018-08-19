#include "stringOp.h"

namespace Util
{
    int StringOp::UnicodeToUTF_8(char* des, wchar_t* src, int size_d, int size_s) {
        // 注意 WCHAR高低字的顺序,低字节在前，高字节在后
        enum Hex {
            Hex80 = 0x80,
            Hex800 = 0x800,
            Hex10000 = 0x10000,
            Hex200000 = 0x200000,
            Hex4000000 = 0x4000000,
            Hex80000000 = 0x80000000
        };

        int s = 0, d = 0;
        int length = 0;

        while (s < size_s && d < size_d) {
            if (src[s] < Hex80) {
                //
                //length = 1;
                des[d++] = (char)src[s++];
            } else if (src[s] < Hex800) {
                //length = 2;

                if (d + 1 >= size_d) {
                    break;
                }

                des[d + 1] = ((char)(src[s] & 0x003F)) | 0x80;
                des[d] = ((char)(src[s] >> 8)) & 0x1F;
                des[d] |= 0xC0;

                d += 2;
                s++;
            } else if (src[s] < Hex10000) {
                //length = 3;
                if (d + 2 >= size_d) {
                    break;
                }

                des[d + 2] = ((char)(src[s] & 0x003F)) | 0x80;
                des[d + 1] = ((char)((src[s] >> 6) & 0x003F)) | 0x80;
                des[d] = ((char)((src[s] >> 12) & 0x000F)) | 0xE0;

                d += 3;
                s++;
            } else {
                //more than two bytes
                //length = 4 or more;
                s++; //just skip it
            }
        }

        return d;
    }

    CString StringOp::utf82Cstring(char* pBuff) {
        int length = strlen(pBuff);
        CString s;
        if (!pBuff) {
            return s;
        }
        char* pBuf = pBuff;
        while (length) {
            WCHAR wch;
            int n = utf82unicode((unsigned char*)pBuf, 0, length, wch);
            if (n > 0) {
                s += wch;
            } else {
                break;
            }
            pBuf += n;
            length = strlen(pBuf);
        }
        return s;
    }
    // UTF-8的unicode表示方法到unicode的值转换函数
    int StringOp::utf82unicode(unsigned char* byte, int index, int count, WCHAR& unicode) {
        int offset = 0;
        if (index >= count) {
            return -1;
        }
        if ((byte[index] & 0x80) == 0x0)              //  一位
        {
            unicode = byte[index];
            offset = 1;
        } else if ((byte[index] & 0xE0) == 0xC0) // 两位
        {
            if (index + 1 >= count) {
                return false;
            }
              unicode = (((byte[index] & 0x1F)) << 6) | (byte[index + 1] & 0x3F);
              offset = 2;
        } else if ((byte[index] & 0xF0) == 0xE0) // 三位
        {
            if (index + 2 >= count) {
                return false;
            }
              unicode = (((byte[index] & 0x0F)) << 12) | (((byte[index + 1] & 0x3F)) << 6) | (byte[index + 2] & 0x3F);
              offset = 3;
        } else if ((byte[index] & 0xF8) == 0xF0) // 四位
        {
            if (index + 3 >= count) {
                return false;
            }
              unicode = (((byte[index] & 0x07)) << 18) | (((byte[index + 1] & 0x3F)) << 12) | (((byte[index + 2] & 0x3F)) << 6) | (byte[index + 3] & 0x3F);
              offset = 4;
        } else if ((byte[index] & 0xFC) == 0xF8) // 五位
        {
            if (index + 4 >= count) {
                return false;
            }
              unicode = (((byte[index] & 0x03)) << 24) | (((byte[index + 1] & 0x3F)) << 18) | (((byte[index + 2] & 0x3F)) << 12) | (((byte[index + 3] & 0x3F)) << 6) | (byte[index + 4] & 0x3F);
              offset = 5;
        } else if ((byte[index] & 0xFE) == 0xFC) // 六位
        {
            if (index + 5 >= count) {
                return false;
            }
              unicode = (((byte[index] & 0x01)) << 30) | (((byte[index + 1] & 0x3F)) << 24) | (((byte[index + 2] & 0x3F)) << 18) | (((byte[index + 3] & 0x3F)) << 12) | (((byte[index + 4] & 0x3F)) << 6) | (byte[index + 5] & 0x3F);
              offset = 6;
        } else {
            return -1;
        }
        return offset;
    } 

    int StringOp::FindStringArryIndex(CString* sArry, int ncount, std::string s) {
        for (int i = 0; i < ncount; i++) {
            if (sArry[i] == s.c_str()) {
                return i;
            }
        }
        return 0;
    }

    int StringOp::ToInt(std::string& content, size_t mode) {
        char* endPos = 0;
        int result = ::strtol(content.c_str(), &endPos, mode); //atoi(content.c_str());
        return result;
    }

    void StringOp::GetIPFromCString(CString ip, int* nIp) {
        int index1 = 0;
        for (int i = 0; i < 4; i++) {
            int index = ip.Find('.', index1);
            CString ss;
            if (index > 0) {
                ss = ip.Mid(index1, index - index1);
            } else {
                ss = ip.Mid(index1);
            }
            index1 = index + 1;
            nIp[i] = Util::StringOp::ToInt(ss);
        }
    }

    void StringOp::ToRectangle(std::string& content, CRect& rect) //[(x0,y0)(x1,y1)]
    {
        rect.left = 0;
        rect.top = 0;
        rect.right = 0;
        rect.bottom = 0;
        size_t firstB = content.find('(', 0);
        if (firstB == std::string::npos) {
            return;
        } else {
            size_t firstComma = content.find(',', firstB + 1);
            if (firstComma == std::string::npos) {
                return;
            } else {
                std::string x0 = content.substr(firstB + 1, firstComma - (firstB + 1));
                  rect.left = ToInt(x0);
                size_t firstE = content.find(')', firstComma + 1);
                if (firstE == std::string::npos) {
                    return;
                } else {
                    std::string y0 = content.substr(firstComma + 1, firstE - (firstComma + 1));
                      rect.top = ToInt(y0);
                    size_t secondB = content.find('(', firstE + 1);
                    if (secondB == std::string::npos) {
                        return;
                    } else {
                        size_t secondComma = content.find(',', secondB);
                        if (secondComma == std::string::npos) {
                            return;
                        } else {
                            std::string x1 = content.substr(secondB + 1, secondComma - (secondB + 1));
                              rect.right = ToInt(x1);
                            size_t secondE = content.find(')', secondComma + 1);
                            if (secondE == std::string::npos) {
                                return;
                            } else {
                                std::string y1 = content.substr(secondComma + 1, secondE - (secondComma + 1));
                                  rect.bottom = ToInt(y1);
                            }
                        }
                    }
                }
            }
        }
        return;
    }

    int StringOp::FindStringArryIndex(CString* sArry, int ncount, CString s) {
        for (int i = 0; i < ncount; i++) {
            if (sArry[i] == s) {
                return i;
            }
        }
        return 0;
    }

    int StringOp::ToInt(CString content, size_t mode) {
        CString s;
        char* endPos = 0;
        int result = _wtoi(content);
        //int result = ::strtol(content.c_str(), &endPos, mode); //atoi(content.c_str());
        return result;
    }

    void StringOp::ToRectangle(CString& content, CRect& rect) //[(x0,y0)(x1,y1)]
    {
        rect.left = 0;
        rect.top = 0;
        rect.right = 0;
        rect.bottom = 0;
        size_t firstB = content.Find('(', 0);
        if (firstB == -1) {
            return;
        } else {
            size_t firstComma = content.Find(',', firstB + 1);
            if (firstComma == -1) {
                return;
            } else {
                CString x0 = content.Mid(firstB + 1, firstComma - (firstB + 1));
                  rect.left = ToInt(x0);
                size_t firstE = content.Find(')', firstComma + 1);
                if (firstE == -1) {
                    return;
                } else {
                    CString y0 = content.Mid(firstComma + 1, firstE - (firstComma + 1));
                      rect.top = ToInt(y0);
                    size_t secondB = content.Find('(', firstE + 1);
                    if (secondB == -1) {
                        return;
                    } else {
                        size_t secondComma = content.Find(',', secondB);
                        if (secondComma == -1) {
                            return;
                        } else {
                            CString x1 = content.Mid(secondB + 1, secondComma - (secondB + 1));
                              rect.right = ToInt(x1);
                            size_t secondE = content.Find(')', secondComma + 1);
                            if (secondE == -1) {
                                return;
                            } else {
                                CString y1 = content.Mid(secondComma + 1, secondE - (secondComma + 1));
                                  rect.bottom = ToInt(y1);
                            }
                        }
                    }
                }
            }
        }
        return;
    }

    Timestamp StringOp::ToTimestamp(std::string const & content) {
        Timestamp r;
        if (content.length() == 8) {
            int year = 0;
            int month = 0;
            int day = 0;
            int hour = 0;
            int minute = 0;
            int second = 0;

            year = ToInt(content.substr(0, 4));
            month = ToInt(content.substr(4, 2));
            day = ToInt(content.substr(6, 2));

            Timestamp result(year, month, day, hour, minute, second);
            return result;
        }
        if (content.length() != 14) {
            return r;
        }
        //YYYY:MM:DD:HH:MM:SS format
        //YYYYMMDDHHMMSS format
        enum Type {
            tNull,
            tDeli,
            tSeq,
        };
        Type type = tNull;
        size_t pos = content.find(':', 0);
        if (pos != std::string::npos) {
            type = tDeli;
        } else {
            type = tSeq;
        }

        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int minute = 0;
        int second = 0;

        switch (type) {
        case tDeli:
            year = ToInt(content.substr(0, 4));
            month = ToInt(content.substr(5, 2));
            day = ToInt(content.substr(8, 2));
            hour = ToInt(content.substr(11, 2));
            minute = ToInt(content.substr(14, 2));
            second = ToInt(content.substr(17, 2));
            break;
        case tSeq:
            year = ToInt(content.substr(0, 4));
            month = ToInt(content.substr(4, 2));
            day = ToInt(content.substr(6, 2));
            hour = ToInt(content.substr(8, 2));
            minute = ToInt(content.substr(10, 2));
            second = ToInt(content.substr(12, 2));
            break;
        case tNull:
            break;
        default:
            break;
        }
        Timestamp now = Timestamp::GetCurrentTime();
        if (year == 0) {
            year = now.GetYear();
        }
        if (month == 0) {
            month = now.GetMonth();
        }
        if (day == 0) {
            day = now.GetDay();
        }
        //if (hour == 0)
        //  hour = now.GetHour();
        //if (minute == 0)
        //  minute = now.GetMinute();
        //if (second == 0)
        //  second = now.GetSecond();
        Timestamp result(year, month, day, hour, minute, second);
        return result;
    }

    TimeSpan StringOp::ToTimeSpan(std::string& content) {
        //int year = ToInt(content.substr(0, 4));
        //int month = ToInt(content.substr(5, 2));
        //int day = ToInt(content.substr(8, 2));
        //int hour = ToInt(content.substr(11, 2));
        //int minute = ToInt(content.substr(14, 2));
        //int second = ToInt(content.substr(17, 2));
        //TimeSpan result(year * 365 + month * 31 + day, hour, minute, second);
        //return result;

        int time = Util::StringOp::ToInt(content, 10);
        TimeSpan result(time);
        return result;
    }

    CString StringOp::ToCString(std::string const & content) {
#if 0
        return CString(content.c_str());
#else
        //step1 : get converted length
        //char* nullTermContent = new char[content.length() + 1];
        //memset(nullTermContent, 0, content.length() + 1);
        //memcpy(nullTermContent, content.c_str(), content.length());
        //size_t wideContentLength = MultiByteToWideChar(CP_ACP, 0, nullTermContent, -1, 0, 0);
        //delete[] nullTermContent;
        if(content == "")
            return CString("");
        size_t convertCount = 0;
        size_t wideContentLength = mbstowcs_s(&convertCount, 0, 0, content.c_str(), content.length());

        //step2 : alloc converted space
        wchar_t* wideContent = new wchar_t[wideContentLength + 2];
        memset(wideContent, 0, sizeof(wchar_t) * (wideContentLength + 2));

        //step3 : convert
        size_t convLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), content.length(), wideContent, wideContentLength);
        //size_t convLength = mbstowcs(wideContent, nullTermContent, wideContentLength + 1);
        //assert(convLength == wideContentLength);

        //step4 : free converted space and return
        CString result(wideContent);
        delete[] wideContent;
        return result;
#endif
    }

    std::string StringOp::FromInt(int const value) {
        char stringValue[32] = {
            0
        };
        sprintf_s(stringValue, 32, "%d", value);
        return stringValue;
    }

    std::string StringOp::FromDouble(double const value) {
        char stringValue[64] = {
            0
        };
        sprintf_s(stringValue, 64, "%f", value);
        return stringValue;
    }

    std::string StringOp::FromTimestamp(Timestamp const & timestamp) {
        char buffer[256] = {
            0
        };
        sprintf_s(buffer, 256, "%04d%02d%02d%02d%02d%02d", timestamp.GetYear(), timestamp.GetMonth(), timestamp.GetDay(), timestamp.GetHour(), timestamp.GetMinute(), timestamp.GetSecond());
        std::string result = std::string((char*)buffer);
        return result;
    }

    std::string StringOp::FromTimestampFormat(Timestamp const & timestamp) {
        char buffer[256] = {
            0
        };
        sprintf_s(buffer, 256, "%04d-%02d-%02d %02d:%02d:%02d", timestamp.GetYear(), timestamp.GetMonth(), timestamp.GetDay(), timestamp.GetHour(), timestamp.GetMinute(), timestamp.GetSecond());
        std::string result = std::string((char*)buffer);
        return result;
    }

    std::string StringOp::FromTimeSpan(TimeSpan const & timeSpan) {
        //char buffer[256] = {0};
        //sprintf(buffer, "%04d:%02d:%02d:%02d:%02d:%02d", timeSpan.GetDays() / 365, timeSpan.GetDays() / 30, timeSpan.GetDays() % 30, timeSpan.GetHours(), timeSpan.GetMinutes(), timeSpan.GetSeconds());
        //std::string result = std::string((char*)buffer);
        //return result;

        return FromInt((int)timeSpan.GetTotalSeconds());
    }

    std::string StringOp::FromCString(CString const & value) {
#if 0
        CComBSTR nodeName;
        HRESULT hr = item->get_nodeName(&nodeName);
        if (hr != S_OK)
        {
            //throw "XML Parser Error!";
            printf("XML Parser Error!\n");
        }

        int length = nodeName.Length();
        char* temp = (char*)malloc(2 * length + 1);
        memset(temp, 0, length + 1);
        wcstombs(temp, nodeName.m_str, 2 * length);
        //memcpy(temp, nodeName.m_str, 2 * length);
        std::string result = temp;
        free(temp);
        return result;
#else
        //step1 : get converted length
        //LPCSTR defaultChar = 0;
        BOOL usedDefaultChar = false;
        //int convLength = WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)value, value.GetLength(), content, defaultChar, &usedDefaultChar);
        int length = WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)value, -1, 0, 0, 0, &usedDefaultChar);
        //int length = wcstombs(0, (LPCTSTR)value, value.GetLength() * 2);

        //step2 : alloc converted space
        char* content = (char*)malloc(length + 1);
        memset(content, 0, length + 1);

        //step2 : convert
        //int convLength = wcstombs(content, (LPCTSTR)value, value.GetLength() * 2);
        int convLength = WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)value, -1, content, length + 1, 0, &usedDefaultChar);

        //step4 : free converted space and return
        std::string result = content;
        free(content);
        return result;
#endif
    }

    std::string const StringOp::ToUTF8(std::string const & content) {
        std::string result;
        enum Hex {
            Hex80 = 0x80,
            Hex800 = 0x800,
            Hex10000 = 0x10000,
            Hex200000 = 0x200000,
            Hex4000000 = 0x4000000,
            Hex80000000 = 0x80000000
        };
        CString unicode = ToCString(content);
        /*std::*/size_t length = unicode.GetLength();
        char chr = 0;
        for (/*std::*/size_t i = 0; i < length; ++i) {
            if (unicode.GetAt(i) < Hex80) {
                //1byte
                chr = (char)unicode.GetAt(i);
                result += chr;
            } else if (unicode.GetAt(i) < Hex800) {
                //2byte
                chr = (char)((unicode.GetAt(i) >> 8) & 0x1F);
                chr |= 0xC0;
                result += chr;
                chr = (char)((unicode.GetAt(i) & 0x3F) | 0x80);
                result += chr;
            } else if (unicode.GetAt(i) < Hex10000) {
                //3byte
                chr = ((char)((unicode.GetAt(i) >> 12) & 0x0F)) | 0xE0;
                result += chr;
                chr = ((char)((unicode.GetAt(i) >> 6) & 0x3F)) | 0x80;
                result += chr;
                chr = ((char)(unicode.GetAt(i) & 0x3F)) | 0x80;
                result += chr;
            } else if (unicode.GetAt(i) < Hex200000) {
                //4byte
                //
            } else if (unicode.GetAt(i) < Hex4000000) {
                //5byte
                //
            } else if (unicode.GetAt(i) < Hex80000000) {
                //6byte
                //
            }
        }
        return result;
    }
    std::string const StringOp::FromUTF8(std::string const & value) {
        std::string result;
        CString t;
        TCHAR chr;
        for (size_t i = 0; i < value.length(); ++i) {
            if ((value[i] & 0x80) == 0x0) {
                //1byte
                chr = value[i];
            } else if ((value[i] & 0xE0) == 0xC0) {
                //2byte
                chr = (((value[i] & 0x1F)) << 6) | (value[i + 1] & 0x3F);
                ++i;
            } else if ((value[i] & 0xF0) == 0xE0) {
                  //3byte
                  chr = (((value[i] & 0x0F)) << 12) | (((value[i + 1] & 0x3F)) << 6) | (value[i + 2] & 0x3F);
                  i += 2;
            } else if ((value[i] & 0xF8) == 0xF0) {
                  //4byte
                  chr = (((value[i] & 0x07)) << 18) | (((value[i + 1] & 0x3F)) << 12) | (((value[i + 2] & 0x3F)) << 6) | (value[i + 3] & 0x3F);
                  i += 3;
            } else if ((value[i] & 0xFC) == 0xF8) {
                  //5byte
                  chr = (((value[i] & 0x03)) << 24) | (((value[i + 1] & 0x3F)) << 18) | (((value[i + 2] & 0x3F)) << 12) | (((value[i + 3] & 0x3F)) << 6) | (value[i + 4] & 0x3F);
                  i += 4;
            } else if ((value[i] & 0xFE) == 0xFC) {
                  //6byte
                  chr = (((value[i] & 0x01)) << 30) | (((value[i + 1] & 0x3F)) << 24) | (((value[i + 2] & 0x3F)) << 18) | (((value[i + 3] & 0x3F)) << 12) | (((value[i + 4] & 0x3F)) << 6) | (value[i + 5] & 0x3F);
                  i += 5;
            }
            t += chr;
        }
        result = FromCString(t);
        return result;
    }
}
