#include "stdafx.h"

//#include <wtypes.h> //type define
//#define AFXAPI __stdcall
#include "xmlParser.h"

namespace Util {
    CString g_XMLShowType[] = {_T("no"), _T("lefttoright")};
    CString g_XMLFont[] = {_T("song"), _T("hei")};
    CString g_XMLSize[] = {_T("10"), _T("12"), _T("14"), _T("16"), _T("18"), _T("20"), _T("22"), _T("24"), _T("26"), _T("28"), _T("30")};
    CString g_XMLWeights[] = {_T("normal"), _T("bold")};
    CString g_XMLItalic[] = {_T("no"), _T("yes")};
    CString g_XMLUnderLine[] = {_T("no"), _T("yes")};
    CString g_XMLColor[] = {_T("black"), _T("white"), _T("red"), _T("blue"), _T("green"), _T("purple")};
    CString g_XMLHeight[] = {_T("1"), _T("2"), _T("4"), _T("6"), _T("8"), _T("10"), _T("12"), _T("14"), _T("16"), _T("18"), _T("20"), _T("22"), _T("24"), _T("26"), _T("28"), _T("30")};
    CString g_XMLAlign[] = {_T("left_top"), _T("left_center"), _T("left_bottom"), _T("middle_top"), _T("middle_center"), _T("middle_bottom"), _T("right_top"), _T("right_center"), _T("right_bottom")};
    CString g_XMLActive[] = {_T("no"), _T("link"), _T("click"), _T("dial"), _T("outdial"), _T("pagedown"), _T("pageup"), _T("audio"), _T("vedio")};
    CString g_Advtype[] = {_T("Õº∆¨"), _T(" ”∆µ")};
    CString g_CtrlType[] = {_T("unit"), _T("edit"), _T("checkbutton"), _T("radiobutton"), _T("commbox")}; 
    CString g_YesNo[] = {_T("no"), _T("yes")};
}
