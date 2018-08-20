#ifndef __XMLPARSER_H__
#define __XMLPARSER_H__

//#include "stdafx.h"

#include <objbase.h>
#include <msxml2.h>
#include <comdef.h>
#include <msxml.h>
#include <objsafe.h>
#include <objbase.h>
#include <atlbase.h>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include <assert.h>

#include "DateTime.h"
#include "config.h"
#include "stringOp.h"

namespace Util
{
    //using Util::MJPGList;
    //using Util::MJPGItem;
    //using Util::PLAYConfigList;
    //using Util::AdvPlayList;
    //using Util::VIDEOFILEItem;
    //using Util::SOUNDFILEItem;
    //using Util::KEYFILEItem;
    //using Util::ADVItem;

    extern CString g_XMLShowType[];
    extern CString g_XMLFont[];
    extern CString g_XMLSize[];
    extern CString g_XMLWeights[];
    extern CString g_XMLItalic[];
    extern CString g_XMLUnderLine[];
    extern CString g_XMLColor[];
    extern CString g_XMLHeight[];
    extern CString g_XMLAlign[];
    extern CString g_XMLActive[];
    extern CString g_Advtype[];
    extern CString g_CtrlType[];
    extern CString g_YesNo[];

	class XmlParser
	{
	public:
		typedef void (*ElementProcess)(IXMLDOMNode* element);
		static MJPGList* const ParseFileToMJPGList(std::string const& filename)
		{
			MJPGList* result = new MJPGList();
			result->bgfilename = "";
			result->showtype = no_show;

			Prepare();
			IXMLDOMDocument* parser = CreateXMLParser();
			DWORD  s = GetTickCount(); 
			LoadXMLFile(parser, filename);
			DWORD os = GetTickCount()-s; 
			TRACE(L"load %d\n", os);
			//get root element
			IXMLDOMElement* rootElement = 0;
			HRESULT hr = parser->get_documentElement(&rootElement);
			if (hr != S_OK)
			{
				//throw "Not Find Root Element!";
				//Dprintf("Not Find Root Element!\n");
				return result;
			}
			ForeachChildElements(rootElement, MJPGProcess, result);
			Final();
			return result;
		}
			
	private:
		static void MJPGProcess(IXMLDOMNode* element, void* structurePointer)
		{
			MJPGList* result = static_cast<MJPGList*>(structurePointer);
			//get nodeName
			std::string nodeName = GetNodeName(element);
			if(nodeName == "BG")
			{
				/*
				std::string s = GetText(element);
				result->bgfilename = s.c_str();
				*/
				BSTR bstrItemText;
				element->get_text(&bstrItemText);
				result->bgfilename = CString(bstrItemText);
				SysFreeString(bstrItemText);
				if(result->bgfilename == "no")
					result->bgfilename = "";
			}
			else if(nodeName == "SHOWTYPE")
			{
				std::string s = GetText(element);
				int nIndex = Util::StringOp::FindStringArryIndex(g_XMLShowType, showtype_end, s);
				result->showtype = (SHOWTYPE)nIndex;
			}
			else if(nodeName == "ITEM")
			{
				MJPGItem* item = new MJPGItem();
				item->unitparam.m_Rect = CRect(0, 0, 0, 0);
				item->unitparam.m_bgFilename = "";
				item->unitparam.m_Font = font_song;
				item->unitparam.m_FontAlign = left_top;
				item->unitparam.m_FontColor = font_black;
				item->unitparam.m_FontSize = font_12;
				item->unitparam.m_sContent = "";
				item->unitparam.m_UnitContent = "";
				item->unitparam.m_UnitType = unit_no;
				item->unitparam.m_FontWeights = font_normal;		
				item->unitparam.m_bFontUnLine = FALSE;
				item->unitparam.m_bFontItalic = FALSE;
				item->unitparam.m_bIsDownStatus = FALSE;
				item->unitparam.m_nSiererNO = -1;
				item->unitparam.m_bIsShow = TRUE;
				item->unitparam.m_bIsTranslate = FALSE;
				item->unitparam.m_bIsDisable = FALSE;

				MJPGList* result = static_cast<MJPGList*>(structurePointer);
				result->items.push_back(item);
				
				ForeachChildElements(element, MJPGItemDetailProcess, structurePointer);
			}
		}

		static void MJPGItemDetailProcess(IXMLDOMNode* element, void* structurePointer)
		{
			MJPGList* result = static_cast<MJPGList*>(structurePointer);
			MJPGItem* item = result->items[result->items.size() - 1];
			//item->
	
			//get nodeName
			std::string nodeName = GetNodeName(element);
			//compare it and process
			std::string s = GetText(element);
			if(nodeName == "RECT")
			{
				Util::StringOp::ToRectangle(s, item->unitparam.m_Rect);
                item->unitparam.m_Rect.bottom = item->unitparam.m_Rect.bottom * 125 / 100;
                item->unitparam.m_Rect.right = item->unitparam.m_Rect.right * 125 / 100;
                item->unitparam.m_Rect.top = item->unitparam.m_Rect.top * 125 / 100;
                item->unitparam.m_Rect.left = item->unitparam.m_Rect.left * 125 / 100;
			}
			else if(nodeName == "PIC")
			{
				/*
				item->unitparam.m_bgFilename = s.c_str();
				if(item->unitparam.m_bgFilename == "no")
					item->unitparam.m_bgFilename = "";
					*/
				BSTR bstrItemText;
				element->get_text(&bstrItemText);
				item->unitparam.m_bgFilename = CString(bstrItemText);
				SysFreeString(bstrItemText);
				if(item->unitparam.m_bgFilename == "no")
					item->unitparam.m_bgFilename = "";
			}
			else if(nodeName == "TEXT")
			{
				/*
				item->unitparam.m_sContent = s.c_str();
				if(item->unitparam.m_sContent == "no")
					item->unitparam.m_sContent = "";
					*/
				BSTR bstrItemText;
				element->get_text(&bstrItemText);
				item->unitparam.m_sContent = CString(bstrItemText);
				SysFreeString(bstrItemText);
				if(item->unitparam.m_sContent == "no")
					item->unitparam.m_sContent = "";
			}
			else if(nodeName == "FONT")
			{
				int nIndex = Util::StringOp::FindStringArryIndex(g_XMLFont, textfont_end, s);
				item->unitparam.m_Font = (TEXTFONT)nIndex;
			}
			else if(nodeName == "SIZE")
			{
				int nIndex = Util::StringOp::FindStringArryIndex(g_XMLSize, textsize_end, s);
				item->unitparam.m_FontSize = (TEXTSIZE)nIndex;
			}
			else if(nodeName == "WEIGHTS")
			{
				int nIndex = Util::StringOp::FindStringArryIndex(g_XMLWeights, textweights_end, s);
				item->unitparam.m_FontWeights = (TEXTWEIGHTS)nIndex;
			}
			else if(nodeName == "ITALIC")
			{
				int nIndex = Util::StringOp::FindStringArryIndex(g_XMLItalic, 2, s);
				item->unitparam.m_bFontItalic = (BOOL)nIndex;
			}
			else if(nodeName == "UNDERLINE")
			{
				int nIndex = Util::StringOp::FindStringArryIndex(g_XMLUnderLine, 2, s);
				item->unitparam.m_bFontUnLine = (BOOL)nIndex;
			}
			else if(nodeName == "COLOR")
			{
				int nIndex = Util::StringOp::FindStringArryIndex(g_XMLColor, textcolor_end, s);
				item->unitparam.m_FontColor = (TEXTCOLOR)nIndex;
			}
			else if(nodeName == "HEIGHT")
			{
				int nIndex = Util::StringOp::FindStringArryIndex(g_XMLHeight, font_h_end, s);
				item->unitparam.m_FontHeight = (TEXTHEIGHT)nIndex;
			}
			else if(nodeName == "ALIGN")
			{
				int nIndex = Util::StringOp::FindStringArryIndex(g_XMLAlign, textalign_end, s);
				item->unitparam.m_FontAlign = (TEXTALIGN)nIndex;
			}
			else if(nodeName == "ACTIVE")
			{
				int nIndex = Util::StringOp::FindStringArryIndex(g_XMLActive, unittype_end, s);
				item->unitparam.m_UnitType = (UNIT_TYPE)nIndex;
			}
			else if(nodeName == "DOWITH")
			{
				/*
				item->unitparam.m_UnitContent = s.c_str();
				if(item->unitparam.m_UnitContent == "no")
					item->unitparam.m_UnitContent = "";
				*/
				BSTR bstrItemText;
				element->get_text(&bstrItemText);
				item->unitparam.m_UnitContent = CString(bstrItemText);
				SysFreeString(bstrItemText);
				if(item->unitparam.m_UnitContent == "no")
					item->unitparam.m_UnitContent = "";
			}
		}

		static void Prepare(void)
		{
			//init com env
			HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED); 
			if (hr != S_OK)
			{
				//throw "COM Environment Initialize Failure!";
				//Dprintf("COM Environment Initialize Failure!\n");
			}
			return;
		}
		
		static void Final(void)
		{
			CoUninitialize();
			return;
		}
		
		static IXMLDOMDocument* CreateXMLParser(void)
		{
			//create parser component
			IXMLDOMDocument* result = 0;
				
			HRESULT hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&result);
			if (hr != S_OK)
			{
				//throw "MSXML Component Creation Failure!";
				//Dprintf("MSXML Component Creation Failure!\n");
			}
			return result;
		}

		static void LoadXMLFile(IXMLDOMDocument* parser, std::string filename)
		{
			//parse xml file
			int fileNameLength = filename.length();
			BSTR temp = SysAllocStringLen(0, fileNameLength);
			mbstowcs(temp, filename.c_str(), fileNameLength);
			VARIANT_BOOL isSuccessful = false;
			HRESULT hr = parser->load(CComVariant(temp), &isSuccessful);
			SysFreeString(temp);
			if (hr != S_OK)
			{
				//throw "Start Parsing Failure!";
				//Dprintf("Start Parsing Failure!\n");
			}
			return;
		}

		static tagDOMNodeType GetNodeType(IXMLDOMNode* item)
		{
			tagDOMNodeType nodeType;
			HRESULT hr = item->get_nodeType(&nodeType);
			if (hr != S_OK)
			{
				//throw "XML Parser Error!";
				//Dprintf("XML Parser Error!\n");
			}
			return nodeType;
		}

		static std::string GetText(IXMLDOMNode* item)
		{
			CComBSTR nodeContent;
			HRESULT hr = item->get_text(&nodeContent);
			if (hr != S_OK)
			{
				//throw "XML Parser Error!";
				//Dprintf("XML Parser Error!\n");
			}
			int length = nodeContent.Length();
			char* temp = (char*)malloc(2 * length + 1);
			memset(temp, 0, length + 1);
			wcstombs(temp, nodeContent.m_str, 2 * length);
			//memcpy(temp, nodeContent.m_str, 2 * length);
			std::string result = temp;
			free(temp);
			return result;
		}

		static std::string GetNodeName(IXMLDOMNode* item)
		{
			CComBSTR nodeName;
			HRESULT hr = item->get_nodeName(&nodeName);
			if (hr != S_OK)
			{
				//throw "XML Parser Error!";
				//Dprintf("XML Parser Error!\n");
			}
			int length = nodeName.Length();
			char* temp = (char*)malloc(2 * length + 1);
			memset(temp, 0, length + 1);
			wcstombs(temp, nodeName.m_str, 2 * length);
			//memcpy(temp, nodeName.m_str, 2 * length);
			std::string result = temp;
			free(temp);
			return result;
		}

		static void ForeachChildElements(IXMLDOMNode* item, void (*process)(IXMLDOMNode* element, void* structurePointer), void* structurePointer)
		{
			//get children
			IXMLDOMNodeList* children = 0;
			HRESULT hr = item->get_childNodes(&children);
			if (hr != S_OK)
			{
				//throw "XML Document Format Error!";
				//Dprintf("XML Document Format Error!\n");
			}
			long length = 0;
			hr = children->get_length(&length);
			if (hr != S_OK)
			{
				//throw "XML Parser Error!";
				//Dprintf("XML Parser Error!\n");
			}
			//walk children
			for (long i = 0; i < length; ++i)
			{
				//get an child
				IXMLDOMNode* item = 0;
				hr = children->get_item(i, &item);
				if (hr != S_OK)
				{
					//throw "XML Parser Error!";
					//Dprintf("XML Parser Error!\n");
				}
				//if is element, then do it
				if (GetNodeType(item) == NODE_ELEMENT)
				{
					process(item, structurePointer);
				}
			}
		}
public:
		static AdvPlayList* const ParseFileToAdvList_(CString sFilename)
		{
			AdvPlayList* result = NULL;
			CFile file;
			if(file.Open(sFilename, CFile::modeRead))
			{
				result = new AdvPlayList();
										
				//读取文件
				ULONGLONG mlength = file.GetLength();
                std::size_t length = (std::size_t)mlength;
				char *lBuff = new char[length+2];
				memset(lBuff, 0, length+2);
				file.Read(lBuff, length);
				char *sXmlFile = lBuff;
				//分析文件
				BOOL flag = TRUE;
				PLAYCONFIGITEMTYPE itemtype;
				CString sParentElementName = _T("");
				while(flag)
				{
					int offset;
					char *sFindName = FindElementsName(sXmlFile, offset);
					if(sFindName)
					{	
						if(!strcmp(sFindName, "SLIDEPIC"))
						{
							itemtype = type_ADVItem; 
							ADVItem* item = new ADVItem();
							item->advFilename = "";
							item->advRefre = "";
							item->advType = adv_video;
							item->advTimeOut = 60000;
							result->ADVItems.push_back(item);
							offset += strlen(sFindName)+2;
						}
						else
						{							
							if(!strcmp(sFindName, "FILENAME"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
													
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								if(sChar)
									delete sChar;
							}
							else if(!strcmp(sFindName, "PIC"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								ADVItem* item = result->ADVItems[result->ADVItems.size() - 1];
								item->advFilename = s;
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								if(sChar)
									delete sChar;
							}
							else if(!strcmp(sFindName, "TYPE"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								int nIndex = Util::StringOp::FindStringArryIndex(g_Advtype, advtype_end, s);
								ADVItem* item = result->ADVItems[result->ADVItems.size() - 1];
								item->advType = (ADV_TYPE)nIndex;
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								if(sChar)
									delete sChar;
							}
							else if(!strcmp(sFindName, "REFVIDEO"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								ADVItem* item = result->ADVItems[result->ADVItems.size() - 1];
								item->advRefre = s;
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								if(sChar)
									delete sChar;
							}
							else if(!strcmp(sFindName, "TIMEOUT") && itemtype == type_ADVItem)
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								ADVItem* item = result->ADVItems[result->ADVItems.size() - 1];
								item->advTimeOut = Util::StringOp::ToInt(s)*1000;
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								if(sChar)
									delete sChar;
							}
							else
							{
								offset += strlen(sFindName) + 2;
							}
						}
						sParentElementName = sFindName;
						if(sFindName)
							delete sFindName;
						sXmlFile = sXmlFile+offset;
					}
					else
						flag = FALSE;
				}
				delete lBuff;
			}
			return result;
		}

		static PLAYConfigList* const ParseFileToPLAYConfigList_(CString sFilename)
		{
			PLAYConfigList* result = NULL;
			CFile file;
			if(file.Open(sFilename, CFile::modeRead))
			{
				result = new PLAYConfigList();
				result->text = "";
				result->mainFile = "";
				result->outcode = "";
							
				//读取文件
				ULONGLONG mlength = file.GetLength();
                std::size_t length = (std::size_t)mlength;
				char *lBuff = new char[length+2];
				memset(lBuff, 0, length+2);
				file.Read(lBuff, length);
				char *sXmlFile = lBuff;
				//分析文件
				BOOL flag = TRUE;
				PLAYCONFIGITEMTYPE itemtype;
				CString sParentElementName = _T("");
				while(flag)
				{
					int offset;
					char *sFindName = FindElementsName(sXmlFile, offset);
					if(sFindName)
					{	
						if(!strcmp(sFindName, "SLIDEPIC"))
						{
							itemtype = type_ADVItem; 
							ADVItem* item = new ADVItem();
							item->advFilename = "";
							item->advRefre = "";
							item->advType = adv_video;
							item->advTimeOut = 60000;
							result->ADVItems.push_back(item);
							offset += strlen(sFindName)+2;
						}
						else if(!strcmp(sFindName, "SOUND"))
						{
							itemtype = type_SoundItem; 
							SOUNDFILEItem* item = new SOUNDFILEItem();
							item->filename = "";
							result->Sounditems.push_back(item);
							offset += strlen(sFindName)+2;
						}
						else if(!strcmp(sFindName, "KEY"))
						{
							itemtype = type_KeyItem; 
							KEYFILEItem* item = new KEYFILEItem();
							item->filename = "";
							item->telcode = "";
							result->Keyitems.push_back(item);
							offset += strlen(sFindName)+2;
						}
						else if(!strcmp(sFindName, "MAINKEY"))
						{
							itemtype = type_MainFile; 
							offset += strlen(sFindName) + 2;
						}
						else if(!strcmp(sFindName, "TEXT"))
						{
							char *sChar = FindElementsContent(sXmlFile, sFindName);
							CString s = Util::StringOp::utf82Cstring(sChar);
							for(int in = 0; in < s.GetLength(); in++)
							{
								if(s.GetAt(in) == L'_')
									s.SetAt(in, L' ');
								else
									break;
							}
							result->text = s;
							offset += strlen(sFindName) + 2;
							if(sChar)
								delete sChar;
						}
						else if(!strcmp(sFindName, "OUTCODE"))
						{
							char *sChar = FindElementsContent(sXmlFile, sFindName);
							CString s = Util::StringOp::utf82Cstring(sChar);
							result->outcode = s;
							offset += strlen(sFindName) + 2;
							if(sChar)
								delete sChar;
						}
						else
						{							
							if(!strcmp(sFindName, "FILENAME"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								switch(itemtype)
								{
								case type_MainFile:
									result->mainFile = s;
									break;
								case type_SoundItem:
									{
										SOUNDFILEItem* item = result->Sounditems[result->Sounditems.size() - 1];
										item->filename = s;
									}
									break;
								case type_KeyItem:
									{
										KEYFILEItem* item = result->Keyitems[result->Keyitems.size() - 1];
										item->filename = s;
									}
									break;
								default:
									break;
								}
													
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								if(sChar)
									delete sChar;
							}
							else if(!strcmp(sFindName, "PIC"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								ADVItem* item = result->ADVItems[result->ADVItems.size() - 1];
								item->advFilename = s;
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								if(sChar)
									delete sChar;
							}
							else if(!strcmp(sFindName, "TYPE"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								int nIndex = Util::StringOp::FindStringArryIndex(g_Advtype, advtype_end, s);
								ADVItem* item = result->ADVItems[result->ADVItems.size() - 1];
								item->advType = (ADV_TYPE)nIndex;
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								if(sChar)
									delete sChar;
							}
							else if(!strcmp(sFindName, "REFVIDEO"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								ADVItem* item = result->ADVItems[result->ADVItems.size() - 1];
								item->advRefre = s;
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								if(sChar)
									delete sChar;
							}
							else if(!strcmp(sFindName, "TIMEOUT") && itemtype == type_ADVItem)
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								ADVItem* item = result->ADVItems[result->ADVItems.size() - 1];
								item->advTimeOut = Util::StringOp::ToInt(s)*1000;
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								if(sChar)
									delete sChar;
							}
							else if(!strcmp(sFindName, "TELCODE"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								switch(itemtype)
								{
								case type_KeyItem:
									{
										KEYFILEItem* item = result->Keyitems[result->Keyitems.size() - 1];
										item->telcode = s;
									}
									break;
								default:
									break;
								}
								
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								if(sChar)
									delete sChar;
							}
							else
							{
								offset += strlen(sFindName) + 2;
							}
						}
						sParentElementName = sFindName;
						if(sFindName)
							delete sFindName;
						sXmlFile = sXmlFile+offset;
					}
					else
						flag = FALSE;
				}
				delete lBuff;
			}
			return result;
		}
		
		static MJPGList* const ParseFileToMJPGList_(CString sFilename)
		{
			MJPGList* result = NULL;
			CFile file;
			if(file.Open(sFilename, CFile::modeRead))
			{
				result = new MJPGList();
				result->bgfilename = "";
				result->bgfilename_down = "";
				result->showtype = no_show;
				result->m_Rect = CRect(0, 0, 480 * 125 / 100, 204);
				
				//读取文件
				ULONGLONG mlength = file.GetLength();
                std::size_t length = (std::size_t)mlength;
				char *lBuff = new char[length+2];
				memset(lBuff, 0, length+2);
				file.Read(lBuff, length);
				char *sXmlFile = lBuff;
				//分析文件
				BOOL flag = TRUE;
				while(flag)
				{
					int offset;
					char *sFindName = FindElementsName(sXmlFile, offset);
					if(sFindName)
					{	
						if(strstr(sFindName, "ITEM"))
						{
							MJPGItem* item = new MJPGItem();
							item->unitparam.m_Rect = CRect(0, 0, 0, 0);
							item->unitparam.m_bgFilename = "";
							item->unitparam.m_bgFilename_down = "";
							item->unitparam.m_Font = font_song;
							item->unitparam.m_FontAlign = left_top;
							item->unitparam.m_FontColor = font_black;
							item->unitparam.m_FontHeight = font_h_4;
							item->unitparam.m_FontSize = font_12;
							item->unitparam.m_sContent = "";
							item->unitparam.m_UnitContent = "";
							item->unitparam.m_UnitType = unit_no;
							item->unitparam.m_FontWeights = font_normal;		
							item->unitparam.m_bFontUnLine = FALSE;
							item->unitparam.m_bFontItalic = FALSE;
							item->unitparam.m_bIsDownStatus = FALSE;
							item->unitparam.m_nSiererNO = -1;
							item->unitparam.m_bIsShow = TRUE;
							item->unitparam.m_bIsTranslate = FALSE;
							item->unitparam.m_bIsDisable = FALSE;

							item->unitparam.m_Ctrltype = ctrl_unit;			
							item->unitparam.m_nIsScroll = 0;		
							item->unitparam.m_pUnitWnd = NULL;

							result->items.push_back(item);
							offset += strlen(sFindName)+2;
						}

						else if(strstr(sFindName, "BG"))
						{
							char *sChar = FindElementsContent(sXmlFile, sFindName);
							CString s = Util::StringOp::utf82Cstring(sChar);
							result->bgfilename = s;
							if(result->bgfilename == "no")
								result->bgfilename = "";
							offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
							if(sChar)
								delete sChar;
						}
						else if(strstr(sFindName, "BB_DOWN"))
						{
							char *sChar = FindElementsContent(sXmlFile, sFindName);
							CString s = Util::StringOp::utf82Cstring(sChar);
							result->bgfilename_down = s;
							if(result->bgfilename_down == "no")
								result->bgfilename_down = "";
							offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
							if(sChar)
								delete sChar;
						}
						else if(strstr(sFindName, "BRECT"))
						{
						
							char *sChar = FindElementsContent(sXmlFile, sFindName);
							CString s = Util::StringOp::utf82Cstring(sChar);
							Util::StringOp::ToRectangle(s, result->m_Rect);
                            result->m_Rect.bottom = result->m_Rect.bottom * 125 / 100;
                            result->m_Rect.right = result->m_Rect.right * 125 / 100;
                            result->m_Rect.top = result->m_Rect.top * 125 / 100;
                            result->m_Rect.left = result->m_Rect.left * 125 / 100;
							offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
							if(sChar)
								delete sChar;		
						}
						else if(strstr(sFindName, "SHOWTYPE"))
						{
							char *sChar = FindElementsContent(sXmlFile, sFindName);
							CString s = Util::StringOp::utf82Cstring(sChar);
							int nIndex = Util::StringOp::FindStringArryIndex(g_XMLShowType, showtype_end, s);
							result->showtype = (SHOWTYPE)nIndex;
							offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
							if(sChar)
								delete sChar;
						}
						else
						{
							if(result->items.size() <= 0)
							{
								offset += strlen(sFindName) + 2;
								if(sFindName)
									delete sFindName;
								sXmlFile = sXmlFile+offset;
								continue;
							}
								
							MJPGItem* item = result->items[result->items.size() - 1];

							if(strstr(sFindName, "RECT"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								Util::StringOp::ToRectangle(s, item->unitparam.m_Rect);
                                item->unitparam.m_Rect.bottom = item->unitparam.m_Rect.bottom * 125 / 100;
                                item->unitparam.m_Rect.right = item->unitparam.m_Rect.right * 125 / 100;
                                item->unitparam.m_Rect.left = item->unitparam.m_Rect.left * 125 / 100;
                                item->unitparam.m_Rect.top = item->unitparam.m_Rect.top * 125 / 100;
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								if(sChar)
									delete sChar;
							}
							else if(strstr(sFindName, "SIERERNO"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								item->unitparam.m_nSiererNO = Util::StringOp::ToInt(s);
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								if(sChar)
									delete sChar;
							}
							else if(strstr(sFindName, "PIC"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								item->unitparam.m_bgFilename = s;
								if(item->unitparam.m_bgFilename == "no")
									item->unitparam.m_bgFilename = "";
								if(sChar)
									delete sChar;
							}
							else if(strstr(sFindName, "BP_DOWN"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								item->unitparam.m_bgFilename_down = s;
								if(item->unitparam.m_bgFilename_down == "no")
									item->unitparam.m_bgFilename_down = "";
								if(sChar)
									delete sChar;
							}
							else if(strstr(sFindName, "TEXT"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								if(s != "")
								{
									for(int in = 0; in < 100; in++)
									{
										if(s.GetAt(in) == L'_')
											s.SetAt(in, L' ');
										else
											break;
									}
								}
								
								item->unitparam.m_sContent = s;
								if(item->unitparam.m_sContent == "no")
									item->unitparam.m_sContent = "";
								if(sChar)
									delete sChar;
							}
							else if(strstr(sFindName, "FONT"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								int nIndex = Util::StringOp::FindStringArryIndex(g_XMLFont, textfont_end, s);
								item->unitparam.m_Font = (TEXTFONT)nIndex;
								if(sChar)
									delete sChar;
							}
							else if(strstr(sFindName, "SIZE"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								int nIndex = Util::StringOp::FindStringArryIndex(g_XMLSize, textsize_end, s);
								item->unitparam.m_FontSize = (TEXTSIZE)nIndex;
								if(sChar)
									delete sChar;
							}
							else if(strstr(sFindName, "WEIGHTS"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								int nIndex = Util::StringOp::FindStringArryIndex(g_XMLWeights, textweights_end, s);
								item->unitparam.m_FontWeights = (TEXTWEIGHTS)nIndex;
								if(sChar)
									delete sChar;
							}
							else if(strstr(sFindName, "ITALIC"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								int nIndex = Util::StringOp::FindStringArryIndex(g_XMLItalic, 2, s);
								item->unitparam.m_bFontItalic = (BOOL)nIndex;
								if(sChar)
									delete sChar;
							}
							else if(strstr(sFindName, "UNDERLINE"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								int nIndex = Util::StringOp::FindStringArryIndex(g_XMLUnderLine, 2, s);
								item->unitparam.m_bFontUnLine = (BOOL)nIndex;
								if(sChar)
									delete sChar;
							}
							else if(strstr(sFindName, "COLOR"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								int nIndex = Util::StringOp::FindStringArryIndex(g_XMLColor, textcolor_end, s);
								item->unitparam.m_FontColor = (TEXTCOLOR)nIndex;
								if(sChar)
									delete sChar;
							}
							else if(strstr(sFindName, "HEIGHT"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								if(sChar)
								{
									CString s = Util::StringOp::utf82Cstring(sChar);
									offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
									int nIndex = Util::StringOp::FindStringArryIndex(g_XMLHeight, font_h_end, s);
									item->unitparam.m_FontHeight = (TEXTHEIGHT)nIndex;
									if(sChar)
										delete sChar;
								}
								else 
									offset += (strlen(sFindName)+2)*2 + 1;
							}
							else if(strstr(sFindName, "ALIGN"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								int nIndex = Util::StringOp::FindStringArryIndex(g_XMLAlign, textalign_end, s);
								item->unitparam.m_FontAlign = (TEXTALIGN)nIndex;
								if(sChar)
									delete sChar;
							}
							else if(strstr(sFindName, "ACTIVE"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								int nIndex = Util::StringOp::FindStringArryIndex(g_XMLActive, unittype_end, s);
								item->unitparam.m_UnitType = (UNIT_TYPE)nIndex;
								if(sChar)
									delete sChar;
							}
							else if(strstr(sFindName, "DOWITH"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								item->unitparam.m_UnitContent = s;
								if(item->unitparam.m_UnitContent == "no")
									item->unitparam.m_UnitContent = "";
								if(sChar)
									delete sChar;
							}
							////
							else if(strstr(sFindName, "CTRLTYPE"))
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								int nIndex = Util::StringOp::FindStringArryIndex(g_CtrlType, ctrtype_end, s);
								item->unitparam.m_Ctrltype = (CTRL_TYPE)nIndex;
								if(sChar)
									delete sChar;
							}
							else if(strstr(sFindName, "ISSCROLL"))			//是否滚动
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);

								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);

								item->unitparam.m_nIsScroll = Util::StringOp::ToInt(s);
								if(sChar)
									delete sChar;
							}
							else if(strstr(sFindName, "ISSHOW"))			//初始是否显示
							{
								char *sChar = FindElementsContent(sXmlFile, sFindName);
								CString s = Util::StringOp::utf82Cstring(sChar);
								
								offset += (strlen(sFindName)+2)*2 + 1 + strlen(sChar);
								
								int nIndex = Util::StringOp::FindStringArryIndex(g_YesNo, 2, s);
								item->unitparam.m_bIsShow = (BOOL)nIndex;
								if(sChar)
									delete sChar;
							}
							////

							else
							{
								offset += strlen(sFindName) + 2;
							}
						}
						if(sFindName)
							delete sFindName;
						sXmlFile = sXmlFile+offset;
					}
					else
						flag = FALSE;
				}
				delete lBuff;
			}
			return result;
		}

		static char *FindElementsName(char *sXmlFileBuff, int &offset)
		{
			char *reS = NULL;
			int index = 0;
			while(1)
			{
				char *nStart = strstr(sXmlFileBuff+index, "<");
				offset = nStart - sXmlFileBuff;
				index = (offset+1);
				if(nStart <= 0)
					break;
				if(nStart >= 0)
				{
					if(*(nStart+1) != '/')
					{
						char *nEnd = strstr(nStart+1, ">");
						if(nEnd > 0)
						{
							reS = new char[nEnd-nStart];
							memset(reS, 0, nEnd-nStart);
							memcpy(reS, nStart+1, nEnd-nStart-1);
							break;
						}
					}
				}
			}
			return reS;
		}

		static char *FindElementsContent(char *sXmlFileBuff, char *sName)
		{
			char *reS = NULL;
			char sStart[64];
			sprintf(sStart, "<%s>", sName);
			char sEnd[64];
			sprintf(sEnd, "</%s>", sName);

			char *nStart = strstr(sXmlFileBuff, sStart);
			if(nStart > 0)
			{
				char *nEnd = strstr(sXmlFileBuff, sEnd);
				if(nEnd > 0)
				{
					char *n1 = nStart + strlen(sStart);
					int n2 = nEnd - n1;
					reS = new char[n2+1];
					memset(reS, 0, n2+1);
					memcpy(reS,  n1, n2);
				}
			}
			return reS;
		}
	};
}

//2008-07-25 wwf
/*
BOOL OpenURL(char *url, int type)
{
	char cFileName[128];
	if(!httpdownload(url, type, cFileName)) return FALSE;

	MJPGList *pList = ::Util::XmlParser::ParseFileToMJPGList_(cFileName);
	
	char cSvrName[] = "192.168.1.82/";
	if (!pList->bgfilename.IsEmpty())
	{
		CString sName = pList->bgfilename;
		if (sName.GetAt(0) == _T('.'))
		{
			sName.Delete(0);
		}
		
		int nLen =sName.GetLength()+strlen(cSvrName)+1;
		char *pUrl = new char[nLen];
		strcpy(pUrl, cSvrName);
		strcat(pUrl, sName.GetBuffer(sName.GetLength()));
		sName.ReleaseBuffer();
		pUrl[nLen-1] = '\0';

		httpdownload(pUrl, type, cFileName);
		delete []pUrl;
			
	}

	if (!pList->bgfilename_down.IsEmpty())
	{
		CString sName = pList->bgfilename_down;
		if (sName.GetAt(0) == _T('.'))
		{
			sName.Delete(0);
		}
		
		int nLen =sName.GetLength()+strlen(cSvrName)+1;
		char *pUrl = new char[nLen];
		strcpy(pUrl, cSvrName);
		strcat(pUrl, sName.GetBuffer(sName.GetLength()));
		sName.ReleaseBuffer();
		pUrl[nLen-1] = '\0';
		
		httpdownload(pUrl, type, cFileName);
		delete []pUrl;
	}
	
	for (int i=0; i<pList->items.size(); i++)
	{
		CString sName = pList->items[i]->unitparam.m_bgFilename;
		if (!sName.IsEmpty())
		{
			if (sName.GetAt(0) == _T('.'))
			{
				sName.Delete(0);
			}
			
			int nLen =sName.GetLength()+strlen(cSvrName)+1;
			char *pUrl = new char[nLen];
			strcpy(pUrl, cSvrName);
			strcat(pUrl, sName.GetBuffer(sName.GetLength()));
			sName.ReleaseBuffer();
			pUrl[nLen-1] = '\0';
			
			httpdownload(pUrl, type, cFileName);
			delete []pUrl;
		}

			
		sName = pList->items[i]->unitparam.m_bgFilename_down;
		if (!sName.IsEmpty())
		{
			if (sName.GetAt(0) == _T('.'))
			{
				sName.Delete(0);
			}
			
			int nLen =sName.GetLength()+strlen(cSvrName)+1;
			char *pUrl = new char[nLen];
			strcpy(pUrl, cSvrName);
			strcat(pUrl, sName.GetBuffer(sName.GetLength()));
			sName.ReleaseBuffer();
			pUrl[nLen-1] = '\0';
			
			httpdownload(pUrl, type, cFileName);
			delete []pUrl;
			
		}
	}

}
*/
//2008-07-25 end

#endif //__XMLPARSER_H__