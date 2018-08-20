#include "StdAfx.h"
#include "MultimediaPhone.h"
#include "MultimediaPhoneDlg.h"
#include "jsonObject.h"
#include "yajl_parse.h"
#include "../Util/stringOp.h"

#include "Data/Contact.h"
#include "Data/ContactInfo.h"
#include "Data/ContactGroup.h"
#include "assert.h"

#define JSON_METHOD				"method"
#define JSON_CREATE				"create"
#define JSON_DESTORY			"destroy"
#define JSON_MODIFY				"modify"
#define JSON_QUERY				"query.children"
#define JSON_GETCOUNT			"getcount"

#define JSON_CONTACT			"contactnum"
#define JSON_INCONTACT			"insidecontactnum" 
#define JSON_CONTACTINFO		"contactinfonum"
#define JSON_SOUND				"soundnum"
#define JSON_VERSION			"version"
#define JSON_SERIALNUM			"serialnum"
#define JSON_TOTALSPACE			"totalspace"
#define JSON_FREESPACE			"freespace"
#define JSON_RINGNUM			"ringnum"
#define JSON_PICNUM				"picnum"

#define JSON_RETURN_CODE		"return_code"
#define JSON_RETURN_MESSAGE		"return_message"
#define JSON_SUCCESS			std::string("success")
#define JSON_TYPE				"type"
#define JSON_COUNT				"count"
#define JSON_DATA				"data"
#define JSON_ID					"id"
#define JSON_PARENTID			"parent_id"
#define JSON_CHILDREN			"children"
#define JSON_RESET				"isreset"
#define JSON_CONTACTINFOID		"contactinfoid"
#define JSON_ISSENDFILE			"issendfile"
#define JSON_SESSION			"session"
#define JSON_BEGIN				"begin"


#define JSON_NAME				 "name"
#define JSON_CARDTYPE			 "cardtype"
#define JSON_MOBILE				 "mobilestelephone"
#define JSON_MOBILE2			 "mobilestelephone2"
#define JSON_WORKPHONE			 "workstelephone"
#define JSON_WORKPHONE2			 "workstelephone2"
#define JSON_COMPANY			  "company"
#define JSON_DEPARTMENT			  "department"
#define JSON_DUTY				  "duty"
#define JSON_EMAIL				  "email"
#define JSON_HOMESITE			  "homesite"
#define JSON_ADDRESS			  "address"
#define JSON_POSTCODE			  "postcode"
#define JSON_MEMO			      "memo"
#define JSON_SECTION			  "section"
#define JSON_GROUPNAME			  "groupname"
#define JSON_DEPARTMENT1		  "department1"
#define JSON_DEPARTMENT2		  "department2"
#define JSON_DEPARTMENT3		  "department3"

#define JSON_DUTATION			  "duration"
#define JSON_TEL			      "telephoneNumber"
#define JSON_PLAYED				  "played"
#define JSON_STARTTIME			  "startTime"
#define JSON_ISSOUND			  "isSound"
#define JSON_SIZE				  "size"



std::string					g_fieldName;
std::vector<jsonValue>		g_vAncestors;
CFile						g_File;

std::map<std::string ,std::map<std::string,Process> > handlers_; 
	
CString s_VerSionTitle ;
extern DWORD PreFreeSize;
extern int g_iContactCount;


void CloseFile()
{
	if (g_File.m_hFile != CFile::hFileNull)
	{
		g_File.Close();
	}
}

//把一字符替换成另一字符
void replace_all(std::string& str,const std::string& old_value,const std::string& new_value)
{	
	std::string::size_type pos(0);
	while((pos = str.find(old_value)) != std::string::npos)
	{
		str.replace(pos,old_value.length(),new_value);
	}
}

//解析函数
static int json_yajl_null(void *ctx)
{
	jsonValue* v = (jsonValue*)ctx;
	jsonValue jv;
	jv.type = vtNull;

	switch (v->type)
	{
	case vtArray:
		v->arrayValue->items.push_back(jv);
		break;
	case vtObject:
		v->objectValue->properties[g_fieldName] = jv;
		break;
	}

    return 1;
}

static int json_yajl_boolean(void * ctx, int boolVal)
{	
	jsonValue* v = (jsonValue*)ctx;
	jsonValue jv(!!boolVal);

	switch (v->type)
	{
	case vtArray:
		v->arrayValue->items.push_back(jv);
		break;
	case vtObject:
		v->objectValue->properties[g_fieldName] = jv;
		break;
	}

	return 1;
}

static int json_yajl_integer(void *ctx, long integerVal)
{	
	jsonValue* v = (jsonValue*)ctx;
	jsonValue jv((double)integerVal);
	
	switch (v->type)
	{
	case vtArray:
		v->arrayValue->items.push_back(jv);
		break;
	case vtObject:
		v->objectValue->properties[g_fieldName] = jv;
		break;
	}

    return 1;
}

static int json_yajl_double(void *ctx, double doubleVal)
{	
	jsonValue* v = (jsonValue*)ctx;
	jsonValue jv(doubleVal);
	
	switch (v->type)
	{
	case vtArray:
		v->arrayValue->items.push_back(jv);
		break;
	case vtObject:
		v->objectValue->properties[g_fieldName] = jv;
		break;
	}
    return 1;
}

static int json_yajl_string(void *ctx, const unsigned char * stringVal,
                            unsigned int stringLen)
{	
	jsonValue* v = (jsonValue*)ctx;
	jsonValue jv(std::string((char *)stringVal,stringLen));
	
	switch (v->type)
	{
	case vtArray:
		v->arrayValue->items.push_back(jv);
		break;
	case vtObject:
		v->objectValue->properties[g_fieldName] = jv;
		break;
	}
	//

    return 1;
}

static int json_yajl_map_key(void *ctx, const unsigned char * stringVal,
                             unsigned int stringLen)
{
   g_fieldName = std::string((char *)stringVal, stringLen);
   return 1;
}

static int json_yajl_start_map(void *ctx)
{	
	jsonValue* v = (jsonValue*)ctx;
	//"{"
	jsonObject* jo = new jsonObject();
	jsonValue jv(jo);

	switch (v->type)
	{
	case vtArray:
		v->arrayValue->items.push_back(jv);
		break;
	case vtObject:
		v->objectValue->properties[g_fieldName] = jv;
		break;
	}

	g_vAncestors.push_back(jv);
	*v = jv;
	
    return 1;
}

static int json_yajl_end_map(void *ctx)
{	
	jsonValue* v = (jsonValue*)ctx;
	//"}"
	g_vAncestors.pop_back();
	int nSize = g_vAncestors.size();
	if (g_vAncestors.size() > 0)
	{
		*v = g_vAncestors[g_vAncestors.size() - 1];
	}
	//

    return 1;
}

static int json_yajl_start_array(void *ctx)
{	
	jsonValue* v = (jsonValue*)ctx;
	//"["
	jsonArray* ja = new jsonArray();
	jsonValue  jv(ja);
	
	switch (v->type)
	{
	case vtArray:
		v->arrayValue->items.push_back(jv);
		break;
	case vtObject:
		v->objectValue->properties[g_fieldName] = jv;
		break;
	}

	g_vAncestors.push_back(jv);
	*v = jv;

	return 1;
}

static int json_yajl_end_array(void *ctx)
{	
	jsonValue* v = (jsonValue*)ctx;
	//"]" 
	g_vAncestors.pop_back();
	int nSize = g_vAncestors.size();
	if (g_vAncestors.size())
	{
		*v = g_vAncestors[g_vAncestors.size() - 1];
	}
    return 1;
}

static yajl_callbacks callbacks = {
    json_yajl_null,
	json_yajl_boolean,
	json_yajl_integer,
	json_yajl_double,
	NULL,
	json_yajl_string,
	json_yajl_start_map,
	json_yajl_map_key,
	json_yajl_end_map,
	json_yajl_start_array,
	json_yajl_end_array
};
//

/* memory debugging routines */
typedef struct
{
    unsigned int numFrees;
    unsigned int numMallocs;
    /* XXX: we really need a hash table here with per-allocation
     *information */
} yajlTestMemoryContext;
/* cast void * into context */

#define TEST_CTX(vptr) ((yajlTestMemoryContext *) (vptr))

void json_yajl_Free(void * ctx, void * ptr)
{
    assert(ptr != NULL);
    TEST_CTX(ctx)->numFrees++;
    free(ptr);
}

void * json_yajl_Malloc(void * ctx, unsigned int sz)
{
    assert(sz != 0);
    TEST_CTX(ctx)->numMallocs++;
    return malloc(sz);
}

void * json_yajl_Realloc(void * ctx, void * ptr, unsigned int sz)
{
	void *ptr1;
    if (ptr == NULL) {
        assert(sz != 0);
        TEST_CTX(ctx)->numMallocs++;
    } else if (sz == 0) {
        TEST_CTX(ctx)->numFrees++;
    }
	
	if(ptr)
		ptr1 =  realloc(ptr, sz);
	else
		ptr1 = malloc(sz);
	return ptr1;

}

//数据解析
#define BUF_SIZE 2048
jsonValue const parse(std::string const& str)
{	
	yajl_handle hand;
    static unsigned char * fileData = NULL;
    unsigned int bufSize = BUF_SIZE;
    yajl_status stat;
    size_t rd;
    yajl_parser_config cfg = { 0, 1};
    int done;
	jsonValue result;
	/* memory allocation debugging: allocate a structure which collects
     * statistics */
    yajlTestMemoryContext memCtx = { 0,0 };

    /* memory allocation debugging: allocate a structure which holds
     * allocation routines */
    yajl_alloc_funcs allocFuncs = {
        json_yajl_Malloc,
        json_yajl_Realloc,
        json_yajl_Free,
        (void *) NULL
    };

	allocFuncs.ctx = (void *) &memCtx;		
    hand = yajl_alloc(&callbacks, &cfg, &allocFuncs, &result);
	
   done = 0;	   
   while (!done) 
   {	   
	   if (done)
	   {
		   /* parse any remaining buffered data */
		   stat = yajl_parse_complete(hand);	
	   }
	   else
	   {
		   /* read file data, pass to parser */
		   stat = yajl_parse(hand, (uchar *)str.c_str(), str.size());		
	   }
	   
	   if (stat != yajl_status_insufficient_data &&
		   stat != yajl_status_ok)
	   {
		   unsigned char *msg = yajl_get_error(hand, 0, (uchar *)str.c_str(), rd);
		   fflush(stdout);
		   fprintf(stderr, "%s", (char *)msg);
		   yajl_free_error(hand, msg);
		   break;
	   }
	   done = 1;
   }
	   
	yajl_free(hand);
	   
	fflush(stderr);
	fflush(stdout);
	return result;
}

//创建外部群组
static jsonObject const * CreateOutGroup(jsonObject const& jo)
{	
	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	
	//
	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_RETURN_CODE]    = jsonValue((double)0);
	obj->properties[JSON_RETURN_MESSAGE] = jsonValue(JSON_SUCCESS);
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID] = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION]   = jo.properties.find(JSON_SESSION)->second;

	//群组
	Data::ContactGroup contactGroup;
	
	jsonObject *jsonData = jo.properties.find(JSON_DATA)->second.objectValue ; 
	if (jsonData->properties.find(JSON_COUNT) != jsonData->properties.end())
	{	
		//创建多个群
		jsonArray * ja = new jsonArray();
		int count = jsonData->properties[JSON_COUNT].numericValue;
		
		//data:{}
		//data后面的Object
		jsonObject *jDataObject = new jsonObject ;
		jDataObject->properties[JSON_COUNT]	 = jsonValue((double)count);

		for (int i = 0 ; i < count ;i++)
		{
			contactGroup.name(jsonData->properties[JSON_CHILDREN].\
				arrayValue->items[i].objectValue->properties[JSON_NAME].stringValue);
			
			contactGroup.Insert();
			
			//
			jsonObject *jId = new jsonObject();
			jId->properties[JSON_ID] = jsonValue((double)contactGroup.id());
			
			jsonValue jv = jsonValue(jId);
			ja->items.push_back(jv);

		}

		jDataObject->properties[JSON_CHILDREN] = jsonValue(ja);
		obj->properties[JSON_DATA] = jsonValue(jDataObject);	

	}
	else
	{	
		jsonObject *j = new jsonObject();
		//名字
		std::string name   = jsonData->properties[JSON_NAME].stringValue;
		std::string filter = "name = '";
		filter += name;
		filter += "'";
		std::vector<boost::shared_ptr<Data::ContactGroup> > vContactGroup = \
			Data::ContactGroup::GetFromDatabase(filter);
		if (vContactGroup.size() > 0)
		{
			j->properties[JSON_ID] = jsonValue((double)vContactGroup[0]->id());
		}
		else
		{
			//创建单个群组
			contactGroup.name(jsonData->properties[JSON_NAME].stringValue);
			contactGroup.Insert();
			j->properties[JSON_ID] = jsonValue((double)contactGroup.id());

		}
		
		obj->properties[JSON_DATA] = jsonValue(j);

	}
	
	//生成回应数据
	return obj;
}

//创建外部名片
static jsonObject const * CreateOutContact(jsonObject const& jo)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	
	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_RETURN_CODE]    = jsonValue((double)0);
	obj->properties[JSON_RETURN_MESSAGE] = jsonValue(JSON_SUCCESS);
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID] = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION] = jo.properties.find(JSON_SESSION)->second;

	Data::Contact contact;
	contact.groupId(jo.properties.find(JSON_ID)->second.numericValue);
	
	jsonObject *jsonData = jo.properties.find(JSON_DATA)->second.objectValue ; 
	if (jsonData->properties.find(JSON_COUNT) != jsonData->properties.end())
	{
		//创建多个名片
		jsonArray * ja = new jsonArray();
		int count = jsonData->properties[JSON_COUNT].numericValue;
		
		//data:{}
		//data后面的Object
		jsonObject *jDataObject = new jsonObject ;
		jDataObject->properties[JSON_COUNT]	 = jsonValue((double)count);
		
		for (int i = 0 ; i < count ;i++)
		{
			//姓名
			jsonObject *jArrayObject = jsonData->properties[JSON_CHILDREN].\
				arrayValue->items[i].objectValue ;

			contact.name(jArrayObject->properties[JSON_NAME].stringValue);	
			//别名
			contact.alias(main->m_pContactNewDlg->GetAlias(contact.name()));
			CString temp = Util::StringOp::ToCString(contact.name());
		
			//姓氏
		//	contact.surName(Util::StringOp::FromCString(temp.Mid(0,1)));
			//首拼+姓名
		//	std::string name = contact.alias().substr(0,1);
		//	name += contact.name();
		//	contact.initialAndName(name);
			//类型
			contact.type(Data::ContactType((int)jArrayObject->properties[JSON_CARDTYPE].numericValue));
			//手机1
			contact.mobilesTelephone(jArrayObject->properties[JSON_MOBILE].stringValue);
			//手机2
			contact.mobilesTelephone2(jArrayObject->properties[JSON_MOBILE2].stringValue);
			//直线
			contact.worksTelephone(jArrayObject->properties[JSON_WORKPHONE].stringValue);
			//分机
			contact.worksTelephone2(jArrayObject->properties[JSON_WORKPHONE2].stringValue);
			//公司
			contact.company(jArrayObject->properties[JSON_COMPANY].stringValue);
			//职务
			contact.duty(jArrayObject->properties[JSON_DUTY].stringValue);
			//部门
			contact.department(jArrayObject->properties[JSON_DEPARTMENT].stringValue);
			//邮件
			contact.email(jArrayObject->properties[JSON_EMAIL].stringValue);
			//网址
			contact.homesite(jArrayObject->properties[JSON_HOMESITE].stringValue);
			//地址
			contact.companyAddress(jArrayObject->properties[JSON_ADDRESS].stringValue);
			//邮编
			contact.companyPostCode(jArrayObject->properties[JSON_POSTCODE].stringValue);
			//备注
			contact.memo(jArrayObject->properties[JSON_MEMO].stringValue);
			//区号
//			contact.section(jArrayObject->properties[JSON_SECTION].stringValue);

			contact.Insert();
			
			//
			jsonObject *jId = new jsonObject();
			jId->properties[JSON_ID] = jsonValue((double)contact.id());
			
			jsonValue jv = jsonValue(jId);
			ja->items.push_back(jv);

		}
		jDataObject->properties[JSON_CHILDREN] = jsonValue(ja);
		obj->properties[JSON_DATA] = jsonValue(jDataObject);	
		//多个暂时不处理

		g_iContactCount += count;
	}
	else
	{		
		//创建单个名片
		//姓名
		contact.name(jsonData->properties[JSON_NAME].stringValue);
		//别名
		contact.alias(main->m_pContactNewDlg->GetAlias(contact.name()));
		CString temp = Util::StringOp::ToCString(contact.name());
		//姓氏
//		contact.surName(Util::StringOp::FromCString(temp.Mid(0,1)));
//		//首拼+姓名
//		std::string name = contact.alias().substr(0,1);
//		name += contact.name();
//		contact.initialAndName(name);

		//类型
		contact.type(Data::ContactType((int)jsonData->\
			properties[JSON_CARDTYPE].numericValue));
		//手机1
		contact.mobilesTelephone(jsonData->properties[JSON_MOBILE].stringValue);
		//手机2
		contact.mobilesTelephone2(jsonData->properties[JSON_MOBILE2].stringValue);
		//直线
		contact.worksTelephone(jsonData->properties[JSON_WORKPHONE].stringValue);
		//分机
		contact.worksTelephone2(jsonData->properties[JSON_WORKPHONE2].stringValue);
		//公司
		contact.company(jsonData->properties[JSON_COMPANY].stringValue);
		//职务
		contact.duty(jsonData->properties[JSON_DUTY].stringValue);
		//部门
		contact.company(jsonData->properties[JSON_DEPARTMENT].stringValue);
		//邮件
		contact.email(jsonData->properties[JSON_EMAIL].stringValue);
		//网址
		contact.homesite(jsonData->properties[JSON_HOMESITE].stringValue);
		//地址
		contact.companyAddress(jsonData->properties[JSON_ADDRESS].stringValue);
		//邮编
		contact.companyPostCode(jsonData->properties[JSON_POSTCODE].stringValue);
		//备注
		contact.memo(jsonData->properties[JSON_MEMO].stringValue);
		contact.Insert();	
		//
		jsonObject *j = new jsonObject();
		j->properties[JSON_ID]     = jsonValue((double)contact.id());
		obj->properties[JSON_DATA] = jsonValue(j);
		
		g_iContactCount++;
	}
	
	//生成回应数据
	return obj;
}


int GetOutContactGid(std::string name)
{
	//查询这个群组在不在
	std::string filter = "name = '";
	filter += name ;
	filter += "'" ;	
	//
	int gID = 0;
	Data::ContactGroup outGroup ;
	std::vector<boost::shared_ptr<Data::ContactGroup> > vOutContactGroup = \
		Data::ContactGroup::GetFromDatabase(filter);
	if (vOutContactGroup.size())
	{
		gID =  vOutContactGroup[0]->id();
	}
	else
	{
		//创建群组
		if (!name.empty())
		{
			outGroup.name(name);
			outGroup.Insert();
			gID = outGroup.id();
		}

	}	
	return gID;
}

static jsonObject const * CreateOutExcelContact(jsonObject const& jo)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	
	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_RETURN_CODE]    = jsonValue((double)0);
	obj->properties[JSON_RETURN_MESSAGE] = jsonValue(JSON_SUCCESS);
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID]   = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION]   = jo.properties.find(JSON_SESSION)->second;

	Data::Contact outContact;
	
	jsonObject *jsonData = jo.properties.find(JSON_DATA)->second.objectValue ; 
	int nReset = jsonData->properties[JSON_RESET].numericValue; //是否重新设置
	if (jsonData->properties.find(JSON_COUNT) != jsonData->properties.end())
	{
		if (1 == nReset)
		{
			//删除内部群组
			Data::ContactGroup::Remove("");
			//删除内部名片
			Data::Contact::Remove("");

			g_iContactCount = 0 ;
		}

		//创建多个群
		jsonArray * ja = new jsonArray();
		int count = jsonData->properties[JSON_COUNT].numericValue;
		
		//data:{}
		//data后面的Object
		jsonObject *jDataObject = new jsonObject ;
		jDataObject->properties[JSON_COUNT]	 = jsonValue((double)count);
		
		for (int i = 0 ; i < count ;i++)
		{
			//姓名
			jsonObject *jArrayObject = jsonData->properties[JSON_CHILDREN].\
				arrayValue->items[i].objectValue ;
			outContact.name(jArrayObject->properties[JSON_NAME].stringValue);
			//别名
			outContact.alias(main->m_pContactNewDlg->GetAlias(outContact.name()));
			CString temp = Util::StringOp::ToCString(outContact.name());
			//姓氏
//			outContact.surName(Util::StringOp::FromCString(temp.Mid(0,1)));
			//首拼+姓名
//			std::string name = outContact.alias().substr(0,1);
//			name += outContact.name();
//			outContact.initialAndName(name);
			//类型
			outContact.type(Data::ContactType((int)jArrayObject->\
				properties[JSON_CARDTYPE].numericValue));
			//手机1
			outContact.mobilesTelephone(jArrayObject->properties[JSON_MOBILE].stringValue);
			//手机2
			outContact.mobilesTelephone2(jArrayObject->properties[JSON_MOBILE2].stringValue);
			//固话1
			outContact.worksTelephone(jArrayObject->properties[JSON_WORKPHONE].stringValue);
			//固话2
			outContact.worksTelephone2(jArrayObject->properties[JSON_WORKPHONE2].stringValue);
			//公司
			outContact.company(jArrayObject->properties[JSON_COMPANY].stringValue);
			//职务
			outContact.duty(jArrayObject->properties[JSON_DUTY].stringValue);
			//部门
			outContact.department(jArrayObject->properties[JSON_DEPARTMENT].stringValue);

			if (jArrayObject->properties.find(JSON_GROUPNAME) !=\
				jArrayObject->properties.end())
			{
				std::string gName = jArrayObject->properties[JSON_GROUPNAME].stringValue;
				outContact.groupId(GetOutContactGid(gName));
			}

			//邮件
			outContact.email(jArrayObject->properties[JSON_EMAIL].stringValue);
			//网址
			outContact.homesite(jArrayObject->properties[JSON_HOMESITE].stringValue);
			//地址
			outContact.companyAddress(jArrayObject->properties[JSON_ADDRESS].stringValue);
			//邮编
			outContact.companyPostCode(jArrayObject->properties[JSON_POSTCODE].stringValue);
			//备注
			outContact.memo(jArrayObject->properties[JSON_MEMO].stringValue);
						
			outContact.Insert();
			
			//
			jsonObject *jId = new jsonObject();
			jId->properties[JSON_ID] = jsonValue((double)outContact.id());
			
			jsonValue jv = jsonValue(jId);
			ja->items.push_back(jv);

		}
		
		jDataObject->properties[JSON_CHILDREN] = jsonValue(ja);
		obj->properties[JSON_DATA] = jsonValue(jDataObject);	
		
		g_iContactCount += count ;
	}
	else
	{		
		//创建单个名片
		//姓名
		outContact.name(jsonData->properties[JSON_NAME].stringValue);
		//别名
		outContact.alias(main->m_pContactNewDlg->GetAlias(outContact.name()));
		CString temp = Util::StringOp::ToCString(outContact.name());
		//姓氏
//		outContact.surName(Util::StringOp::FromCString(temp.Mid(0,1)));
		//首拼+姓名
//		std::string name = outContact.alias().substr(0,1);
//		name += outContact.name();
//			outContact.initialAndName(name);
		//类型
		outContact.type(Data::ContactType((int)jsonData->\
			properties[JSON_CARDTYPE].numericValue));
		//手机1
		outContact.mobilesTelephone(jsonData->properties[JSON_MOBILE].stringValue);
		//手机2
		outContact.mobilesTelephone2(jsonData->properties[JSON_MOBILE2].stringValue);
		//直线
		outContact.worksTelephone(jsonData->properties[JSON_WORKPHONE].stringValue);
		//分机
		outContact.worksTelephone2(jsonData->properties[JSON_WORKPHONE2].stringValue);
		//公司
		outContact.company(jsonData->properties[JSON_COMPANY].stringValue);
		//职务
		outContact.duty(jsonData->properties[JSON_DUTY].stringValue);
		//部门
		outContact.company(jsonData->properties[JSON_DEPARTMENT].stringValue);
		//邮件
		outContact.email(jsonData->properties[JSON_EMAIL].stringValue);
		//网址
		outContact.homesite(jsonData->properties[JSON_HOMESITE].stringValue);
		//地址
		outContact.companyAddress(jsonData->properties[JSON_ADDRESS].stringValue);
		//邮编
		outContact.companyPostCode(jsonData->properties[JSON_POSTCODE].stringValue);
		//备注
		outContact.memo(jsonData->properties[JSON_MEMO].stringValue);
		//区号
//		outContact.section(jsonData->properties[JSON_SECTION].stringValue);
		outContact.Insert();	
		//
		jsonObject *j = new jsonObject();
		j->properties[JSON_ID]     = jsonValue((double)outContact.id());
		obj->properties[JSON_DATA] = jsonValue(j);
		
		g_iContactCount++;
	}
	
	//生成回应数据
	return obj;
}


extern BOOL DetectFile(TCHAR *sDir);

//创建文件
static jsonObject const * CreateFile(jsonObject const& jo)
{	
	
	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	CloseFile();

	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_RETURN_CODE]    = jsonValue((double)0);
	obj->properties[JSON_RETURN_MESSAGE] = jsonValue(JSON_SUCCESS);
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID] = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION] = jo.properties.find(JSON_SESSION)->second;

	jsonObject *jsonData = jo.properties.find(JSON_DATA)->second.objectValue ;
	CString filePath = L"/flashdrv/";
	switch((int)atoiA((char *)obj->properties[JSON_TYPE].stringValue.c_str()))
	{
	case 47: //图片
		filePath += "MY_PHOTO/";
		filePath += Util::StringOp::ToCString(jsonData->properties[JSON_NAME].stringValue);
		break;
	case 48: //铃声
		filePath += "MY_RING/" ;
		filePath += Util::StringOp::ToCString(jsonData->properties[JSON_NAME].stringValue);
		break;
	}
	
	if (DetectFile(filePath.GetBuffer(256)))
	{
		obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
		obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string("文件创建失败"));
	}
	else if (!g_File.Open(filePath,CFile::modeCreate|CFile::modeWrite))
	{
		obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
		obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string("文件创建失败"));
	}
	filePath.ReleaseBuffer();

	jsonObject *j = new jsonObject();
	obj->properties[JSON_DATA] = jsonValue(j);

	return obj ;
	
}	

//删除外部群组
static jsonObject const * DestoryOutGroup(jsonObject const& jo)
{
	//
	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	
	//
	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_RETURN_CODE]    = jsonValue((double)0);
	obj->properties[JSON_RETURN_MESSAGE] = jsonValue(JSON_SUCCESS);
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID]   = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION]   = jo.properties.find(JSON_SESSION)->second;

	jsonObject *jsonData = jo.properties.find(JSON_DATA)->second.objectValue ; 
	if (jsonData->properties.find(JSON_COUNT) != jsonData->properties.end())
	{
		//多个暂时不处理
		//创建多个名片
		jsonArray * ja = new jsonArray();
		int count = jsonData->properties[JSON_COUNT].numericValue;
		
		//data:{}
		//data后面的Object
		jsonObject *jDataObject = new jsonObject ;
		jDataObject->properties[JSON_COUNT]	= jsonValue((double)count);
		for (int i = 0 ; i < count ;i++)
		{
			int id = (int)jsonData->properties[JSON_CHILDREN].\
				arrayValue->items[i].objectValue->properties[JSON_ID].numericValue ;
			std::string filter = "id = ";
			filter += Util::StringOp::FromInt(id);
			Data::ContactGroup::Remove(filter);	

			jsonObject *jId = new jsonObject();
			jId->properties[JSON_ID] = jsonValue((double)id);
			
			jsonValue jv = jsonValue(jId);
			ja->items.push_back(jv);
		}

		jDataObject->properties[JSON_CHILDREN] = jsonValue(ja);
		obj->properties[JSON_DATA] = jsonValue(jDataObject);
	}
	else
	{	
		//删除
		int id = (int)jsonData->properties[JSON_ID].numericValue ;
		std::string filter = "id = ";
		filter += Util::StringOp::FromInt(id);
		Data::ContactGroup::Remove(filter);	
		
		//负责JSON_ID
		jsonObject *j = new jsonObject();
		j->properties[JSON_ID] = jsonValue((double)id);
		obj->properties[JSON_DATA] = jsonValue(j);
		
	}
	
	//生成回应数据
	return obj;
}



//删除外部名片
static jsonObject const * DestoryOutContact(jsonObject const& jo)
{
	//
	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	
	//
	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_RETURN_CODE]    = jsonValue((double)0);
	obj->properties[JSON_RETURN_MESSAGE] = jsonValue(JSON_SUCCESS);
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID]   = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION]   = jo.properties.find(JSON_SESSION)->second;

	jsonObject *jsonData = jo.properties.find(JSON_DATA)->second.objectValue ; 
	if (jsonData->properties.find(JSON_COUNT) != jsonData->properties.end())
	{
		//多个暂时不处理
		//创建多个名片
		jsonArray * ja = new jsonArray();
		int count = jsonData->properties[JSON_COUNT].numericValue;
		
		//data:{}
		//data后面的Object
		jsonObject *jDataObject = new jsonObject ;
		jDataObject->properties[JSON_COUNT]	= jsonValue((double)count);
		for (int i = 0 ; i < count ;i++)
		{
			int id = (int)jsonData->properties[JSON_CHILDREN].\
				arrayValue->items[i].objectValue->properties[JSON_ID].numericValue ;
			std::string filter = "id = ";
			filter += Util::StringOp::FromInt(id);
			Data::Contact::Remove(filter);	
			
			jsonObject *jId = new jsonObject();
			jId->properties[JSON_ID] = jsonValue((double)id);
			
			jsonValue jv = jsonValue(jId);
			ja->items.push_back(jv);
		}
		
		jDataObject->properties[JSON_CHILDREN] = jsonValue(ja);
		obj->properties[JSON_DATA] = jsonValue(jDataObject);

		g_iContactCount -= count;
	}
	else
	{	
		//删除
		int id = (int)jsonData->properties[JSON_ID].numericValue ;
		std::string filter = "id = ";
		filter += Util::StringOp::FromInt(id);
		Data::Contact::Remove(filter);	
		
		//负责JSON_ID
		jsonObject *j = new jsonObject();
		j->properties[JSON_ID] = jsonValue((double)id);
		obj->properties[JSON_DATA] = jsonValue(j);

		g_iContactCount--;
		
	}
	
	//生成回应数据
	return obj;
}


//
static jsonObject const * DestoryFile(jsonObject const& jo)
{
	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	
	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_RETURN_CODE]    = jsonValue((double)0);
	obj->properties[JSON_RETURN_MESSAGE] = jsonValue(JSON_SUCCESS);
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID] = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION] = jo.properties.find(JSON_SESSION)->second;

	jsonObject *jsonData = jo.properties.find(JSON_DATA)->second.objectValue ;
	CString filePath = L"/flashdrv/";
	switch((int)atoiA(obj->properties[JSON_TYPE].stringValue.c_str()))
	{
	case 47: //图片
		filePath += "MY_PHOTO/";
		filePath += Util::StringOp::ToCString(jsonData->properties[JSON_NAME].stringValue);
		break;
	case 48: //铃声
		filePath += "MY_RING/" ;
		filePath += Util::StringOp::ToCString(jsonData->properties[JSON_NAME].stringValue);
		break;
	}
	
	if (!DeleteFile(filePath))
	{
		obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
		obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string("删除文件失败"));
	}
	
	jsonObject *j = new jsonObject();
	obj->properties[JSON_DATA] = jsonValue(j);
	
	return obj ;

}

//修改外部群组
static jsonObject const * ModifyOutGroup(jsonObject const& jo)
{
	//
	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	
	//
	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_RETURN_CODE]    = jsonValue((double)0);
	obj->properties[JSON_RETURN_MESSAGE] = jsonValue(JSON_SUCCESS);
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID]   = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION]   = jo.properties.find(JSON_SESSION)->second;

	jsonObject *jsonData = jo.properties.find(JSON_DATA)->second.objectValue ; 
	if (jsonData->properties.find(JSON_COUNT) != jsonData->properties.end())
	{
		//多个暂时不处理
		//创建多个名片
		int count = jsonData->properties[JSON_COUNT].numericValue;
		
		for (int i = 0 ; i < count ;i++)
		{	
			int id = (int)jsonData->properties[JSON_CHILDREN].\
				arrayValue->items[i].objectValue->properties[JSON_ID].numericValue ;
			std::string filter = "id = ";
			filter += Util::StringOp::FromInt(id);
			
			std::vector<boost::shared_ptr<Data::Contact> > vContact = Data::Contact::\
				GetFromDatabaseByOrderOffsetLength(filter,"id",0, 1);
			
			if (!vContact.empty())
			{
				vContact[0]->name(jsonData->properties[JSON_NAME].stringValue);
				vContact[0]->Update();
			}
			else 
			{
				obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
				char msg[30] = {0};
				sprintf(msg,"群组ID = %d不存在",id);
				obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string(msg));
				break;
			}
		}
		
		jsonObject *j = new jsonObject();
		obj->properties[JSON_DATA] = jsonValue(j);
	}
	else
	{	
		//修改更新
		int id = (int)jsonData->properties[JSON_ID].numericValue ;
		std::string filter = "id = ";
		filter += Util::StringOp::FromInt(id);

		std::vector<boost::shared_ptr<Data::Contact> > vContact = Data::Contact::\
			GetFromDatabaseByOrderOffsetLength(filter,"id",0, 1);
		
		if (!vContact.empty())
		{
			vContact[0]->name(jsonData->properties[JSON_NAME].stringValue);
			vContact[0]->Update();
		}
		else 
		{
			obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
			char msg[30] = {0};
			sprintf(msg,"群组ID = %d不存在",id);
			obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string(msg));
		}
		
		//
		jsonObject *j = new jsonObject();
		obj->properties[JSON_DATA] = jsonValue(j);
		
	}
	
	//生成回应数据
	return obj;
}


//修改外部名片
static jsonObject const * ModifyOutContact(jsonObject const& jo)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	
	//
	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_RETURN_CODE]    = jsonValue((double)0);
	obj->properties[JSON_RETURN_MESSAGE] = jsonValue(JSON_SUCCESS);
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID]   = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION]   = jo.properties.find(JSON_SESSION)->second;

	jsonObject *jsonData = jo.properties.find(JSON_DATA)->second.objectValue ; 
	if (jsonData->properties.find(JSON_COUNT) != jsonData->properties.end())
	{
		//多个暂时不处理
		//创建多个名片
		int count = jsonData->properties[JSON_COUNT].numericValue;
		
		for (int i = 0 ; i < count ;i++)
		{	
			int id = (int)jsonData->properties[JSON_CHILDREN].\
				arrayValue->items[i].objectValue->properties[JSON_ID].numericValue ;
			std::string filter = "id = ";
			filter += Util::StringOp::FromInt(id);
			
			std::vector<boost::shared_ptr<Data::Contact> > vContact =	\
				Data::Contact::\
				GetFromDatabaseByOrderOffsetLength(filter,"id",0, 1);
			
			if (!vContact.empty())
			{	
				jsonObject *jArrayObject = jsonData->properties[JSON_CHILDREN].\
					arrayValue->items[i].objectValue ;

				vContact[0]->name(jArrayObject->properties[JSON_NAME].stringValue);
				//别名
				vContact[0]->alias(main->m_pContactNewDlg->GetAlias(vContact[0]->name()));
				CString temp = Util::StringOp::ToCString(vContact[0]->name());
				//姓氏
//				vContact[0]->surName(Util::StringOp::FromCString(temp.Mid(0,1)));
				//首拼+姓名
//				std::string name = vContact[0]->alias().substr(0,1);
//				name += vContact[0]->name();
//				vContact[0]->initialAndName(name);
				//类型
				vContact[0]->type(Data::ContactType((int)atoiA(jArrayObject->properties[JSON_TYPE].stringValue.c_str())));
				//手机1
				vContact[0]->mobilesTelephone(jArrayObject->properties[JSON_MOBILE].stringValue);
				//手机2
				vContact[0]->mobilesTelephone2(jArrayObject->properties[JSON_MOBILE2].stringValue);
				//直线
				vContact[0]->worksTelephone(jArrayObject->properties[JSON_WORKPHONE].stringValue);
				//分机
				vContact[0]->worksTelephone2(jArrayObject->properties[JSON_WORKPHONE2].stringValue);
				//公司
				vContact[0]->company(jArrayObject->properties[JSON_COMPANY].stringValue);
				//职务
				vContact[0]->duty(jArrayObject->properties[JSON_DUTY].stringValue);
				//部门
				vContact[0]->company(jArrayObject->properties[JSON_DEPARTMENT].stringValue);
				//区号
//				vContact[0]->section(jArrayObject->properties[JSON_SECTION].stringValue);
				
				vContact[0]->Update();
			}
			else 
			{
				obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
				char msg[30] = {0};
				sprintf(msg,"群组ID = %d不存在",id);
				obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string(msg));
				break;
			}
		}
		
		jsonObject *j = new jsonObject();
		obj->properties[JSON_DATA] = jsonValue(j);
	}
	else
	{	
		//修改更新
		int id = (int)jsonData->properties[JSON_ID].numericValue ;
		std::string filter = "id = ";
		filter += Util::StringOp::FromInt(id);
		
		std::vector<boost::shared_ptr<Data::Contact> > vContact = \
			Data::Contact::\
			GetFromDatabaseByOrderOffsetLength(filter,"id",0, 1);
		
		if (!vContact.empty())
		{
			//姓名
			vContact[0]->name(jsonData->properties[JSON_NAME].stringValue);
			//别名
			vContact[0]->alias(main->m_pContactNewDlg->GetAlias(vContact[0]->name()));
			CString temp = Util::StringOp::ToCString(vContact[0]->name());
			//姓氏
//			vContact[0]->surName(Util::StringOp::FromCString(temp.Mid(0,1)));
			//首拼+姓名
//			std::string name = vContact[0]->alias().substr(0,1);
//			name += vContact[0]->name();
//			vContact[0]->initialAndName(name);
			//类型
			vContact[0]->type(Data::ContactType((int)jsonData->\
				properties[JSON_CARDTYPE].numericValue));
			//手机1
			vContact[0]->mobilesTelephone(jsonData->properties[JSON_MOBILE].stringValue);
			//手机2
			vContact[0]->mobilesTelephone2(jsonData->properties[JSON_MOBILE2].stringValue);
			//直线
			vContact[0]->worksTelephone(jsonData->properties[JSON_WORKPHONE].stringValue);
			//分机
			vContact[0]->worksTelephone2(jsonData->properties[JSON_WORKPHONE2].stringValue);
			//公司
			vContact[0]->company(jsonData->properties[JSON_COMPANY].stringValue);
			//职务
			vContact[0]->duty(jsonData->properties[JSON_DUTY].stringValue);
			//部门
			vContact[0]->company(jsonData->properties[JSON_DEPARTMENT].stringValue);
			//区号
//			vContact[0]->section(jsonData->properties[JSON_SECTION].stringValue);
			
			vContact[0]->Update();
		}
		else 
		{
			obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
			char msg[30] = {0};
			sprintf(msg,"名片ID = %d不存在",id);
			obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string(msg));
		}
		
		//
		jsonObject *j = new jsonObject();
		obj->properties[JSON_DATA] = jsonValue(j);
		
	}
	
	//生成回应数据
	return obj;
}


//获得一个目录下铃声数
DWORD RecursiveDirRingCount(TCHAR *sDir)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	DWORD nSize = 0 ;
	
	CString s = sDir;
	CString csDir = s + L"/*" ;

	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(csDir, &fd); 
	if(hFind != INVALID_HANDLE_VALUE)
	{	
		do 
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{		
				CString s = fd.cFileName;
				s = s.Mid(s.GetLength()-4) ;
				if (s == L".wav" || s == L".WAV" || s == L".mp3" || s == L".MP3" )
				{
					nSize++;
				}
				
			}
			else
			{
				//递归
				CString dir = s + L"/" +fd.cFileName;
				nSize += RecursiveDirRingCount((LPTSTR)(LPCTSTR)dir);
			}

		} while (FindNextFile(hFind, &fd));

	}	
	FindClose(hFind);
	
	return nSize ;
}

//获得一个目录下铃声数
DWORD RecursiveDirPicCount(TCHAR *sDir)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	DWORD nSize = 0 ;
	
	CString s = sDir;
	CString csDir = s + L"/*" ;

	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(csDir, &fd); 
	if(hFind != INVALID_HANDLE_VALUE)
	{	
		do 
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{		
				CString s = fd.cFileName;
				s = s.Mid(s.GetLength()-4) ;
				if (s == L".jpg" || s == L".JPG")
				{
					nSize++;
				}
				
			}
			else
			{
				//递归
				CString dir = s + L"/" +fd.cFileName;
				nSize += RecursiveDirRingCount((LPTSTR)(LPCTSTR)dir);
			}

		} while (FindNextFile(hFind, &fd));

	}	
	FindClose(hFind);
	
	return nSize ;
}

//查询设备属性
extern CString s_VerSion ;
static jsonObject const * QueryDeviceProperty(jsonObject const& jo)
{
	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	
	//
	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_TYPE]   = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID]     = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION]   = jo.properties.find(JSON_SESSION)->second;

	//
	jsonObject *jsonData = new jsonObject ;
	jsonData->properties[JSON_CONTACT] = jsonValue((double)Data::Contact::GetDataCount(""));
	jsonData->properties[JSON_INCONTACT] = \
		jsonValue((double)0);
	jsonData->properties[JSON_CONTACTINFO] = \
		jsonValue((double)Data::ContactInfo::GetDataCount(""));
	jsonData->properties[JSON_SOUND] = \
		jsonValue((double)Data::SoundSegment::GetDataCount(""));
	jsonData->properties[JSON_VERSION] = \
		jsonValue(Util::StringOp::FromCString(s_VerSion));

	//产品序号
	char sSerialNo[64] = {0};
	char buf[256] = {0};
	FILE *fp = fopen("\\hive\\productcode.dat", "r");
	if(fp)
	{
		fread(buf, sizeof(char), 256, fp);
		
		char *start = NULL;
		char *end = NULL;
		if(strstr(buf, "SerialNo:") != NULL)
		{
			start = strstr(buf, "SerialNo:");
			start += strlen("SerialNo:");
			end = strstr(start, ";");
			memcpy(sSerialNo, start, end-start);
		}
		fclose(fp);
	}
	//
	jsonData->properties[JSON_SERIALNUM] = \
		jsonValue(std::string(sSerialNo));
	
	//总容量
	CString text;
	ULARGE_INTEGER freeBytes;//用户可使用的自由空间
	ULARGE_INTEGER totalBytes;//总空间
	ULARGE_INTEGER totalFreeBytes;
	GetDiskFreeSpaceEx(_T("\\Flashdrv"), &freeBytes,\
		&totalBytes, &totalFreeBytes);
	int m = totalBytes.QuadPart ;
	int f = freeBytes.QuadPart  ;
	f = f - PreFreeSize;
	if (f < 0)
	{
		f = 0 ;
	}
	jsonData->properties[JSON_TOTALSPACE] = \
		jsonValue(double(m));
	
	//剩余空间
	jsonData->properties[JSON_FREESPACE] = \
		jsonValue(double(f));
	
	//铃声数
	jsonData->properties[JSON_RINGNUM] = \
		jsonValue((double)RecursiveDirRingCount(L"/flashdrv/my_ring"));
	
	//图片数
	jsonData->properties[JSON_PICNUM] = \
		jsonValue((double)RecursiveDirPicCount(L"/flashdrv/my_photo"));


	obj->properties[JSON_DATA] = jsonValue(jsonData);

	return obj ;

}

//查询外部群组
static jsonObject const * QueryOutGroup(jsonObject const& jo)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	
	//
	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID]   = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION]   = jo.properties.find(JSON_SESSION)->second;

	//查询所有名片
	std::vector<boost::shared_ptr<Data::ContactGroup> > vContactGroup =\
		Data::ContactGroup::GetFromDatabase("");

	jsonObject *jsonData = new jsonObject ;
	jsonData->properties[JSON_COUNT] = jsonValue((double)vContactGroup.size());
	jsonData->properties[JSON_TYPE] = jsonValue((double)41);
	
	jsonArray  *jsonA = new jsonArray ;
	for (int i = 0 ;i < vContactGroup.size() ;i++)
	{
		jsonObject *jsonChild = new jsonObject ;
		jsonChild->properties[JSON_ID] = jsonValue((double)vContactGroup[i]->id());
		jsonChild->properties[JSON_NAME] = jsonValue(vContactGroup[i]->name());
		jsonA->items.push_back(jsonValue(jsonChild));
	}
	jsonData->properties[JSON_CHILDREN] = jsonValue(jsonA);
}


//查询外部名片
static jsonObject const * QueryOutContact(jsonObject const& jo)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	
	//
	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID]   = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION]   = jo.properties.find(JSON_SESSION)->second;

	//查询所有内部群组
	std::vector<boost::shared_ptr<Data::Contact> > vContact =\
		Data::Contact::GetFromDatabase("");
	
	jsonObject *jsonData = new jsonObject ;
	jsonData->properties[JSON_COUNT] = jsonValue((double)vContact.size());
	jsonData->properties[JSON_TYPE] = jsonValue((double)41);
	
	jsonArray  *jsonA = new jsonArray ;
	for (int i = 0 ;i < vContact.size() ;i++)
	{
		jsonObject *jsonChild = new jsonObject ;

		jsonChild->properties[JSON_ID]		= jsonValue((double)vContact[i]->id());
		jsonChild->properties[JSON_NAME]	= jsonValue(vContact[i]->name());
		jsonChild->properties[JSON_TYPE]	= jsonValue((double)vContact[i]->type());
		jsonChild->properties[JSON_MOBILE]	 = jsonValue(vContact[i]->mobilesTelephone().number());
		jsonChild->properties[JSON_MOBILE2]	 = jsonValue(vContact[i]->mobilesTelephone2().number());
		jsonChild->properties[JSON_WORKPHONE]	 = jsonValue(vContact[i]->worksTelephone().number());
		jsonChild->properties[JSON_WORKPHONE2]	 = jsonValue(vContact[i]->worksTelephone2().number());
		jsonChild->properties[JSON_COMPANY]	 = jsonValue(vContact[i]->company());
		jsonChild->properties[JSON_DUTY]	= jsonValue(vContact[i]->duty());
		jsonChild->properties[JSON_DEPARTMENT]	= jsonValue(vContact[i]->department());
		
		jsonChild->properties[JSON_EMAIL]	 = jsonValue(vContact[i]->email());
		jsonChild->properties[JSON_HOMESITE] = jsonValue(vContact[i]->homesite());
		jsonChild->properties[JSON_ADDRESS]  = jsonValue(vContact[i]->companyAddress());
		jsonChild->properties[JSON_POSTCODE] = jsonValue(vContact[i]->companyPostCode());
		jsonChild->properties[JSON_MEMO]     = jsonValue(vContact[i]->memo());

	//		jsonChild->properties[JSON_SECTION]	= jsonValue(vContact[i]->section());

		jsonA->items.push_back(jsonValue(jsonChild));
	}
	jsonData->properties[JSON_CHILDREN] = jsonValue(jsonA);
	
}


std::string GetGroupName(int gID)
{
	std::string filter = "id = " ;
	filter += Util::StringOp::FromInt(gID);
	std::vector<boost::shared_ptr<Data::ContactGroup> > vContactGroup = \
		Data::ContactGroup::GetFromDatabase(filter);
	if (vContactGroup.size())
	{
		return vContactGroup[0]->name();
	}
	return "";
}

static jsonObject const * QueryOutExcelContact(jsonObject const& jo)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	
	//
	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_SESSION] = jo.properties.find(JSON_SESSION)->second;
	//	obj->properties[JSON_ID]   = jo.properties.find(JSON_ID)->second;
	
	jsonObject *jd = jo.properties.find(JSON_DATA)->second.objectValue ; 
	int nBegin = jd->properties.find(JSON_BEGIN)->second.numericValue;
	int nCount = jd->properties.find(JSON_COUNT)->second.numericValue;
	
	//查询所有内部群组
	std::vector<boost::shared_ptr<Data::Contact> > vContact = Data::Contact::\
		GetFromDatabaseByOrderOffsetLength(\
		"","id",nBegin,nCount );
	
	jsonObject *jsonData = new jsonObject ;
	jsonData->properties[JSON_COUNT] = jsonValue((double)vContact.size());
	
	jsonArray  *jsonA = new jsonArray ;
	for (int i = 0 ;i < vContact.size() ;i++)
	{
		jsonObject *jsonChild = new jsonObject ;
		
		jsonChild->properties[JSON_ID]			= jsonValue((double)vContact[i]->id());
		jsonChild->properties[JSON_NAME]		= jsonValue(vContact[i]->name());
		jsonChild->properties[JSON_CARDTYPE]	= jsonValue((double)vContact[i]->type());
		jsonChild->properties[JSON_MOBILE]		= jsonValue(vContact[i]->mobilesTelephone().number());
		jsonChild->properties[JSON_MOBILE2]		= jsonValue(vContact[i]->mobilesTelephone2().number());
		jsonChild->properties[JSON_WORKPHONE]	= jsonValue(vContact[i]->worksTelephone().number());
		jsonChild->properties[JSON_WORKPHONE2]	= jsonValue(vContact[i]->worksTelephone2().number());

		jsonChild->properties[JSON_GROUPNAME]   = jsonValue(GetGroupName(vContact[i]->groupId()));
		jsonChild->properties[JSON_COMPANY]	    = 	jsonValue(vContact[i]->company());
		jsonChild->properties[JSON_DEPARTMENT]	= 	jsonValue(vContact[i]->department());
		
		jsonChild->properties[JSON_DUTY]	 = jsonValue(vContact[i]->duty());
		jsonChild->properties[JSON_EMAIL]	 = jsonValue(vContact[i]->email());
		jsonChild->properties[JSON_HOMESITE] = jsonValue(vContact[i]->homesite());
		jsonChild->properties[JSON_ADDRESS]  = jsonValue(vContact[i]->companyAddress());
		jsonChild->properties[JSON_POSTCODE] = jsonValue(vContact[i]->companyPostCode());
		jsonChild->properties[JSON_MEMO]     = jsonValue(vContact[i]->memo());

		jsonA->items.push_back(jsonValue(jsonChild));
	}
	jsonData->properties[JSON_CHILDREN] = jsonValue(jsonA);
	
	obj->properties[JSON_DATA] = jsonValue(jsonData);
	return obj ;
}

//查询外部名片数量
static jsonObject const * QueryOutExcelContactCount(jsonObject const& jo)
{
	
	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	
	//
	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID]   = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION]   = jo.properties.find(JSON_SESSION)->second;
	
	//查询所有内部群组
	int nCount = Data::Contact::GetDataCount("");
	
	jsonObject *jsonData = new jsonObject ;
	jsonData->properties[JSON_COUNT] = jsonValue((double)nCount);
	
	obj->properties[JSON_DATA] = jsonValue(jsonData);
	return obj ;
}

static jsonObject const * QueryContactInfo(jsonObject const& jo)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	
	//
	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID]   = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION]   = jo.properties.find(JSON_SESSION)->second;
	
	jsonObject *jd = jo.properties.find(JSON_DATA)->second.objectValue ; 
	int nBegin = jd->properties.find(JSON_BEGIN)->second.numericValue;
	int nCount = jd->properties.find(JSON_COUNT)->second.numericValue;

	//查询所有内部群组
	std::vector<boost::shared_ptr<Data::ContactInfo> > vContactInfo =\
		Data::ContactInfo::GetFromDatabaseByTypeOffsetLength("",nBegin,nCount);
	
	jsonObject *jsonData = new jsonObject ;
	jsonData->properties[JSON_COUNT] = jsonValue((double)vContactInfo.size());
	jsonData->properties[JSON_TYPE]  = jsonValue((double)41);
	
	jsonArray  *jsonA = new jsonArray ;
	for (int i = 0 ;i < vContactInfo.size() ;i++)
	{
		jsonObject *jsonChild = new jsonObject ;
		
		jsonChild->properties[JSON_ID]		= jsonValue((double)vContactInfo[i]->id());
		jsonChild->properties[JSON_NAME]	= jsonValue(vContactInfo[i]->name());
		jsonChild->properties[JSON_TYPE]	= jsonValue((double)vContactInfo[i]->type());
		jsonChild->properties[JSON_STARTTIME]	=\
			jsonValue(Util::StringOp::FromTimestamp(vContactInfo[i]->startTime()));
		jsonChild->properties[JSON_ISSOUND]	= jsonValue((double)vContactInfo[i]->isSound());
		jsonChild->properties[JSON_DUTATION]	= jsonValue((double)vContactInfo[i]->duration());
		jsonChild->properties[JSON_TEL]	 = jsonValue(vContactInfo[i]->telephoneNumber().number());
		jsonChild->properties[JSON_PLAYED]	 = jsonValue(vContactInfo[i]->played());
		
		jsonA->items.push_back(jsonValue(jsonChild));
	}
	
	jsonData->properties[JSON_CHILDREN] = jsonValue(jsonA);
	obj->properties[JSON_DATA] = jsonValue(jsonData);
	
	return obj ;
}

//查询通话记录数量
static jsonObject const * QueryContactInfoCount(jsonObject const& jo)
{
	
	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	
	//
	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID]   = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION]   = jo.properties.find(JSON_SESSION)->second;
	
	//查询所有内部群组
	int nCount = Data::ContactInfo::GetDataCount("");
	
	jsonObject *jsonData = new jsonObject ;
	jsonData->properties[JSON_COUNT] = jsonValue((double)nCount);
	
	obj->properties[JSON_DATA] = jsonValue(jsonData);
	return obj ;
}

//获得目录下文件
void GetDirFileName(CString path,jsonArray *ja)
{	
	CString allPath = path + L"*" ;
	
	WIN32_FIND_DATA fd;
	HANDLE hfind;
	hfind	  =	 FindFirstFile(allPath,&fd);
	if(hfind !=	 INVALID_HANDLE_VALUE)
	{
		do
		{
			if(fd.dwFileAttributes !=FILE_ATTRIBUTE_DIRECTORY)
			{ 				
				jsonObject *jo = new jsonObject ;
				jo->properties[JSON_NAME] = jsonValue(\
					Util::StringOp::FromCString(fd.cFileName));
				
				ja->items.push_back(jsonValue(jo));
			}
			else
			{
				GetDirFileName(path+fd.cFileName + L"\\",ja);
			}
			
		}
		while(FindNextFile(hfind,&fd));
	}
}

//获得JPG图的数目
void GetDirJPGFileName(CString path,jsonArray *ja)
{	
	CString allPath = path + L"*" ;
	
	WIN32_FIND_DATA fd;
	HANDLE hfind;
	hfind	  =	 FindFirstFile(allPath,&fd);
	if(hfind !=	 INVALID_HANDLE_VALUE)
	{
		do
		{
			if(fd.dwFileAttributes !=FILE_ATTRIBUTE_DIRECTORY)
			{ 	
				CString s = fd.cFileName;
				s = s.Mid(s.GetLength()-4) ;
				if (s == L".jpg" || s == L".JPG")
				{
					jsonObject *jo = new jsonObject ;
					jo->properties[JSON_NAME] = jsonValue(\
						Util::StringOp::FromCString(fd.cFileName));
					
					ja->items.push_back(jsonValue(jo));
				}
			
			}
			else
			{
				GetDirFileName(path+fd.cFileName + L"\\",ja);
			}
			
		}
		while(FindNextFile(hfind,&fd));
	}
}

//求wav,mp3时长
double   GetMediaDuration(WCHAR*   sFileName) 
{ 
	double  nDuration   ; 
	HRESULT hr; 
	IGraphBuilder *pigb; 
	IMediaPosition *pims     =   NULL; 
	
	hr   =   CoCreateInstance(CLSID_FilterGraph,   //   get   this   documents   graph   object 
		NULL, 
		CLSCTX_INPROC_SERVER, 
		IID_IGraphBuilder, 
		(void   **)   &pigb); 
	
	if   (FAILED(hr)) return   -1; 
	
	hr   =   pigb-> RenderFile(sFileName,   NULL); 
	pigb-> QueryInterface(IID_IMediaPosition,   (void   **)&pims); 
	pims-> get_Duration(&nDuration); 
	
	pims-> Release(); 
	pigb-> Release();   
	
	return   (float)nDuration   ; 
}

//
static jsonObject const * QueryFile(jsonObject const& jo)
{
	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	CloseFile();

	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_RETURN_CODE]    = jsonValue((double)0);
	obj->properties[JSON_RETURN_MESSAGE] = jsonValue(JSON_SUCCESS);
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID]   = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION] = jo.properties.find(JSON_SESSION)->second;

	jsonObject *jsonData = jo.properties.find(JSON_DATA)->second.objectValue ;
	CString filePath = L"/flashdrv/";
	switch((int)atoiA(obj->properties[JSON_TYPE].stringValue.c_str()))
	{
	case 47: //图片
		{
			if (jsonData->properties.find(JSON_NAME) != jsonData->properties.end())
			{	
				obj->properties[JSON_ISSENDFILE] = jsonValue((double)1);

				filePath += "MY_PHOTO/";
				filePath += Util::StringOp::ToCString(jsonData->properties[JSON_NAME].stringValue);
				
				if (!g_File.Open(filePath,CFile::modeRead))
				{
					obj->properties[JSON_ISSENDFILE]  = jsonValue((double)0);
					obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
					obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string("没有这个文件"));
				}

				jsonObject *j = new jsonObject();
				obj->properties[JSON_DATA] = jsonValue(j);

			}
			else
			{
				//要文件列表
				jsonObject *jd = new jsonObject();
				jsonArray  *jsonA = new jsonArray ;
				GetDirJPGFileName(L"\\flashdrv\\my_photo\\",jsonA);
				
				//添加时长
				CFile File;
				for (int i = 0 ; i < jsonA->items.size() ;i++)
				{	
					int nSize = 0 ;
					if (File.Open(L"\\flashdrv\\my_photo\\"+\
						Util::StringOp::ToCString(\
						jsonA->items[i].objectValue->properties[JSON_NAME].stringValue),\
						CFile::modeRead,NULL))
					{
						nSize = File.GetLength();
						File.Close();
					}			
					jsonA->items[i].objectValue->properties[JSON_SIZE] =\
						jsonValue((double)nSize);
				}

				jd->properties[JSON_COUNT]    = jsonValue((double)jsonA->items.size());
				jd->properties[JSON_CHILDREN] = jsonValue(jsonA);
				obj->properties[JSON_DATA]    = jsonValue(jd);
			}
		}
		break;
	case 48: //铃声
		{
			if (jsonData->properties.find(JSON_NAME) != jsonData->properties.end())
			{	
				obj->properties[JSON_ISSENDFILE] = jsonValue((double)1);

				filePath += "MY_RING/" ;
				filePath += Util::StringOp::ToCString(jsonData->properties[JSON_NAME].stringValue);	
				
				if (!g_File.Open(filePath,CFile::modeRead))
				{
					obj->properties[JSON_ISSENDFILE]  = jsonValue((double)0);
					obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
					obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string("没有这个文件"));
				}
				
				jsonObject *j = new jsonObject();
				obj->properties[JSON_DATA] = jsonValue(j);
			}
			else
			{
				//要文件列表
				jsonObject *jd = new jsonObject();
				jsonArray  *jsonA = new jsonArray ;
				GetDirFileName(L"\\flashdrv\\my_ring\\",jsonA);
				
				//添加时长
				CString filename ;
				for (int i = 0 ; i < jsonA->items.size() ;i++)
				{	
					filename = L"\\flashdrv\\my_ring\\"+\
						Util::StringOp::ToCString(\
						jsonA->items[i].objectValue->properties[JSON_NAME].stringValue);
					
					//获得时长
					int nDuration = GetMediaDuration(filename.GetBuffer(256));
					filename.ReleaseBuffer();

					jsonA->items[i].objectValue->properties[JSON_DUTATION] =\
						jsonValue((double)nDuration);
				}

				jd->properties[JSON_COUNT]    = jsonValue((double)jsonA->items.size());
				jd->properties[JSON_CHILDREN] = jsonValue(jsonA);
				obj->properties[JSON_DATA]    = jsonValue(jd); 
			}
		}
		break;
	}
	
	return obj ;

}

//获得录音下目录
void GetRecordDirList(std::vector<CString > &record)
{
	CString allPath = L"/flashdrv/MY_RECORD/*" ;
	
	WIN32_FIND_DATA fd;
	HANDLE hfind;
	hfind	  =	 FindFirstFile(allPath,&fd);
	if(hfind !=	 INVALID_HANDLE_VALUE)
	{
		do
		{
			if(fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{ 				
				record.push_back(fd.cFileName);
			}			
		}
		while(FindNextFile(hfind,&fd));
	}
}

static jsonObject const * QueryContactInfoFile(jsonObject const& jo)
{
	jsonObject *obj = new jsonObject();
	if (obj  == NULL)
	{
		return NULL;
	}
	CloseFile();

	obj->properties[JSON_METHOD] = jo.properties.find(JSON_METHOD)->second;
	obj->properties[JSON_RETURN_CODE]    = jsonValue((double)0);
	obj->properties[JSON_RETURN_MESSAGE] = jsonValue(JSON_SUCCESS);
	obj->properties[JSON_TYPE] = jo.properties.find(JSON_TYPE)->second;
	obj->properties[JSON_ID]   = jo.properties.find(JSON_ID)->second;
	obj->properties[JSON_SESSION]   = jo.properties.find(JSON_SESSION)->second;

	//类型
	int nType = obj->properties[JSON_ID].numericValue ;

	jsonObject *jsonData = jo.properties.find(JSON_DATA)->second.objectValue ;
	CString filePath = L"/flashdrv/";
	switch(nType)
	{
	case 1: //传送文件
		{
			//获得全路径
			/*std::vector<CString> rdList ;
			GetRecordDirList(rdList);
			CString dir ;
			for (int i = 0 ; i < rdList.size();i++)
			{
				filePath    += "MY_RECORD/" ;
				filePath	+= rdList[i];
				filePath	+= L"/";
				filePath	+= Util::StringOp::ToCString(\
					jsonData->properties[JSON_NAME].stringValue);
				if (FindFirstFile(filePath,NULL))
				{
					break;
				}
			}*/
			
			filePath = Util::StringOp::ToCString(jsonData->properties[JSON_NAME].stringValue);

			//打开文件
			if (!g_File.Open(filePath,CFile::modeRead))
			{
				obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
				obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string("没有这个文件"));
				obj->properties[JSON_ISSENDFILE] = jsonValue((double)0);
			}
			else
			{
				obj->properties[JSON_ISSENDFILE] = jsonValue((double)1);
			}

			jsonObject *j = new jsonObject();
			obj->properties[JSON_DATA] = jsonValue(j);

		}
		break;
	case 2: //根据通话记录请求文件列表
		{	
			jsonObject *jd = new jsonObject();
			jsonArray  *jsonA = new jsonArray ;
			//
			int contactId = jsonData->properties[JSON_CONTACTINFOID].numericValue ;
			std::string filter = "contactinfoid = ";
			filter += Util::StringOp::FromInt(contactId);
			std::vector<boost::shared_ptr<Data::SoundSegment> > vSoundSegment = \
				Data::SoundSegment::GetFromDatabase(filter);
			for (int i = 0 ; i < vSoundSegment.size() ; i++)
			{	
				//取文件名
				std::string name = vSoundSegment[i]->filename();
				
				//把"\\"变换成"/"
				replace_all(name,"\\","/");

			//  size_t t = name.rfind("\\");
			//  if (t != -1)
			//	{
			//		name = name.substr(t+1);
			//	}
				
				//名字
				jsonObject *jsonName = new jsonObject;
				jsonName->properties[JSON_NAME] = jsonValue(name);
				jsonA->items.push_back(jsonValue(jsonName));

			}
			
			jd->properties[JSON_COUNT]    = jsonValue((double)vSoundSegment.size());
			jd->properties[JSON_CHILDREN] = jsonValue(jsonA);

			obj->properties[JSON_DATA]    = jsonValue(jd); 

		}
		break;
	case 3: //录音目录下文件全路径
		{
			jsonObject *jd = new jsonObject();
			jsonArray  *jsonA = new jsonArray ;
			GetDirFileName(L"\\flashdrv\\my_record\\",jsonA);
			
			jd->properties[JSON_COUNT]    = jsonValue((double)jsonA->items.size());
			jd->properties[JSON_CHILDREN] = jsonValue(jsonA);
			obj->properties[JSON_DATA]    = jsonValue(jd); 
		}
		break;
	}
	
	return obj ;
}

//事件处理
void registerProcessor()
{	
	//创建
	handlers_[JSON_CREATE]["41"] = CreateOutGroup;
	//创建内部群组
//	handlers_[JSON_CREATE]["42"] = CreateInGroup ;
	//创建外部名片
	handlers_[JSON_CREATE]["43"] = CreateOutContact;
	//创建内部名片
//	handlers_[JSON_CREATE]["44"] = CreateInContact ;
	//创建图片
	handlers_[JSON_CREATE]["47"] = CreateFile ;
	//创建铃声
	handlers_[JSON_CREATE]["48"] = CreateFile ;
	//创建excel内部名片
//	handlers_[JSON_CREATE]["49"] = CreateInExcelContact;
	//创建excel外部名片
	handlers_[JSON_CREATE]["50"] = CreateOutExcelContact;


	//删除外部群组
	handlers_[JSON_DESTORY]["41"] = DestoryOutGroup ;
	//删除内部群组
//	handlers_[JSON_DESTORY]["42"] = DestoryInGroup ;
	//删除外部名片
	handlers_[JSON_DESTORY]["43"] = DestoryOutContact ;
	//删除内部名片
//	handlers_[JSON_DESTORY]["44"] = DestoryInContact ;
	//删除图片
	handlers_[JSON_DESTORY]["47"] = DestoryFile ;
	//删除铃声
	handlers_[JSON_DESTORY]["48"] = DestoryFile ;
	
	//修改
	//修改外部群组
	handlers_[JSON_MODIFY]["41"] = ModifyOutGroup ;
	//修改内部群组
//	handlers_[JSON_MODIFY]["42"] = ModifyInGroup ;
	//修改外部名片
	handlers_[JSON_MODIFY]["43"] = ModifyOutContact ;
	//修改内部名片
//	handlers_[JSON_MODIFY]["44"] = ModifyInContact ;

	//查询
	//查询设备属性
	handlers_[JSON_QUERY]["40"] = QueryDeviceProperty;
	//查询外部群组
	handlers_[JSON_QUERY]["41"] = QueryOutGroup;
	//查询内部群组
//	handlers_[JSON_QUERY]["42"] = QueryInGroup;
	//查询外部名片
	handlers_[JSON_QUERY]["43"] = QueryOutContact;
	//查询内部名片
//	handlers_[JSON_QUERY]["44"] = QueryInContact;
	//查询通话记录
	handlers_[JSON_QUERY]["45"] = QueryContactInfo;
	//查询通话记录
	handlers_[JSON_QUERY]["46"] = QueryContactInfoFile;
	//查询通话记录
	handlers_[JSON_QUERY]["47"] = QueryFile;
	//查询通话记录
	handlers_[JSON_QUERY]["48"] = QueryFile;
	//查询内部名片
//	handlers_[JSON_QUERY]["49"] = QueryInExcelContact ;
	//查询外部名片
	handlers_[JSON_QUERY]["50"] = QueryOutExcelContact ;

	//查询内部名片数量
	handlers_[JSON_GETCOUNT]["45"] = QueryContactInfoCount;
	//查询内部名片数量
//	handlers_[JSON_GETCOUNT]["49"] = QueryInExcelContactCount;
	//查询外部名片数量
	handlers_[JSON_GETCOUNT]["50"] = QueryOutExcelContactCount;

}

std::string const serialize(jsonObject const* obj)
{
	//
	std::string data = "{";
	if (obj)
	{
		std::map<std::string, jsonValue>::const_iterator it = obj->properties.begin() ;
		std::map<std::string, jsonValue>::const_iterator itEnd ;
		for (;it != obj->properties.end();)
		{	
			//开始
			//类型
			data += "\"";
			data += it->first;
			data += '\"';
			data += ":";
			//
			if(it->second.type == vtObject)
			{
				data += serialize(it->second.objectValue);
			}
			else if (it->second.type == vtArray)
			{
				data += "[";
				for (int i = 0 ;i < it->second.arrayValue->items.size() ;i++)
				{	
					data += serialize(it->second.arrayValue->items[i].objectValue);
					if (i < it->second.arrayValue->items.size()-1)
					{
						data += ",";
					}
					
				}
				data += "]";
			}
			else if (it->second.type == vtString)
			{
				data += '\"';
				data += it->second.stringValue;
				data += '\"';
			}
			else if (it->second.type == vtNumeric)
			{
				data += Util::StringOp::FromInt(it->second.numericValue);
			}
			else if (it->second.type == vtBool)
			{	
				std::string v = it->second.boolValue ?"\"true\"":"\"false\""; 
				data += v;
			}
			
			//结束
			it++ ;
			if(it != obj->properties.end()) 
			{
				data += ",";
			}
		}
	}
	data += "}";
	return data ;
}

std::string const jsonProcessor(std::string const& received) {
	
	//生成JsonObject
	jsonValue v = parse(received);
	
	jsonObject *result = NULL;
	jsonObject *obj = v.objectValue; 

    jsonValue method = obj->properties["method"];
    if (handlers_.find(method.stringValue) != handlers_.end()) 
	{
        jsonValue type = obj->properties["type"];
        if (handlers_[method.stringValue].find(type.stringValue) !=\
			handlers_[method.stringValue].end()) 
		{
            result = (jsonObject *)handlers_[method.stringValue][type.stringValue](*obj);
        } 
    } 
	
	std::string data = serialize(result);
	//请求文件直接发数据
	if (result)
	{
		if (result->properties.find(JSON_ISSENDFILE) != result->properties.end())
		{
			if (result->properties[JSON_ISSENDFILE].numericValue == (double)1)
			{
				data = JSON_SENDFILE;
			}
			else 
			{
				data = JSON_NOFILE ;
			}
		}
	}

	delete obj    ;
	if (result)
	{
		delete result ;
	}
	
    return data;

}

jsonObject::~jsonObject()
{	
	for (std::map<std::string, jsonValue>::iterator i = properties.begin(); \
		i != properties.end(); ++i)
	{
		if (i->second.type == vtObject)
		{	
			if (i->second.objectValue != NULL)
			{
				delete i->second.objectValue;
			}
			
		}
		else if(i->second.type == vtArray)
		{	
			if (i->second.arrayValue != NULL)
			{
				delete i->second.arrayValue;
			}

		}
	}
}

jsonArray::~jsonArray()
{	
	for (std::vector<jsonValue>::iterator i = items.begin() ; \
		i != items.end() ;i++)
	{
		if (i->type == vtObject)
		{	
			if (i->objectValue != NULL)
			{
				delete i->objectValue;
			}
			
		}
		else if(i->type == vtArray)
		{	
			if (i->arrayValue != NULL)
			{
				delete i->arrayValue;
			}
			
		}
	}
}