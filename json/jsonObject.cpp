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

//��һ�ַ��滻����һ�ַ�
void replace_all(std::string& str,const std::string& old_value,const std::string& new_value)
{	
	std::string::size_type pos(0);
	while((pos = str.find(old_value)) != std::string::npos)
	{
		str.replace(pos,old_value.length(),new_value);
	}
}

//��������
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

//���ݽ���
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

//�����ⲿȺ��
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

	//Ⱥ��
	Data::ContactGroup contactGroup;
	
	jsonObject *jsonData = jo.properties.find(JSON_DATA)->second.objectValue ; 
	if (jsonData->properties.find(JSON_COUNT) != jsonData->properties.end())
	{	
		//�������Ⱥ
		jsonArray * ja = new jsonArray();
		int count = jsonData->properties[JSON_COUNT].numericValue;
		
		//data:{}
		//data�����Object
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
		//����
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
			//��������Ⱥ��
			contactGroup.name(jsonData->properties[JSON_NAME].stringValue);
			contactGroup.Insert();
			j->properties[JSON_ID] = jsonValue((double)contactGroup.id());

		}
		
		obj->properties[JSON_DATA] = jsonValue(j);

	}
	
	//���ɻ�Ӧ����
	return obj;
}

//�����ⲿ��Ƭ
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
		//���������Ƭ
		jsonArray * ja = new jsonArray();
		int count = jsonData->properties[JSON_COUNT].numericValue;
		
		//data:{}
		//data�����Object
		jsonObject *jDataObject = new jsonObject ;
		jDataObject->properties[JSON_COUNT]	 = jsonValue((double)count);
		
		for (int i = 0 ; i < count ;i++)
		{
			//����
			jsonObject *jArrayObject = jsonData->properties[JSON_CHILDREN].\
				arrayValue->items[i].objectValue ;

			contact.name(jArrayObject->properties[JSON_NAME].stringValue);	
			//����
			contact.alias(main->m_pContactNewDlg->GetAlias(contact.name()));
			CString temp = Util::StringOp::ToCString(contact.name());
		
			//����
		//	contact.surName(Util::StringOp::FromCString(temp.Mid(0,1)));
			//��ƴ+����
		//	std::string name = contact.alias().substr(0,1);
		//	name += contact.name();
		//	contact.initialAndName(name);
			//����
			contact.type(Data::ContactType((int)jArrayObject->properties[JSON_CARDTYPE].numericValue));
			//�ֻ�1
			contact.mobilesTelephone(jArrayObject->properties[JSON_MOBILE].stringValue);
			//�ֻ�2
			contact.mobilesTelephone2(jArrayObject->properties[JSON_MOBILE2].stringValue);
			//ֱ��
			contact.worksTelephone(jArrayObject->properties[JSON_WORKPHONE].stringValue);
			//�ֻ�
			contact.worksTelephone2(jArrayObject->properties[JSON_WORKPHONE2].stringValue);
			//��˾
			contact.company(jArrayObject->properties[JSON_COMPANY].stringValue);
			//ְ��
			contact.duty(jArrayObject->properties[JSON_DUTY].stringValue);
			//����
			contact.department(jArrayObject->properties[JSON_DEPARTMENT].stringValue);
			//�ʼ�
			contact.email(jArrayObject->properties[JSON_EMAIL].stringValue);
			//��ַ
			contact.homesite(jArrayObject->properties[JSON_HOMESITE].stringValue);
			//��ַ
			contact.companyAddress(jArrayObject->properties[JSON_ADDRESS].stringValue);
			//�ʱ�
			contact.companyPostCode(jArrayObject->properties[JSON_POSTCODE].stringValue);
			//��ע
			contact.memo(jArrayObject->properties[JSON_MEMO].stringValue);
			//����
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
		//�����ʱ������

		g_iContactCount += count;
	}
	else
	{		
		//����������Ƭ
		//����
		contact.name(jsonData->properties[JSON_NAME].stringValue);
		//����
		contact.alias(main->m_pContactNewDlg->GetAlias(contact.name()));
		CString temp = Util::StringOp::ToCString(contact.name());
		//����
//		contact.surName(Util::StringOp::FromCString(temp.Mid(0,1)));
//		//��ƴ+����
//		std::string name = contact.alias().substr(0,1);
//		name += contact.name();
//		contact.initialAndName(name);

		//����
		contact.type(Data::ContactType((int)jsonData->\
			properties[JSON_CARDTYPE].numericValue));
		//�ֻ�1
		contact.mobilesTelephone(jsonData->properties[JSON_MOBILE].stringValue);
		//�ֻ�2
		contact.mobilesTelephone2(jsonData->properties[JSON_MOBILE2].stringValue);
		//ֱ��
		contact.worksTelephone(jsonData->properties[JSON_WORKPHONE].stringValue);
		//�ֻ�
		contact.worksTelephone2(jsonData->properties[JSON_WORKPHONE2].stringValue);
		//��˾
		contact.company(jsonData->properties[JSON_COMPANY].stringValue);
		//ְ��
		contact.duty(jsonData->properties[JSON_DUTY].stringValue);
		//����
		contact.company(jsonData->properties[JSON_DEPARTMENT].stringValue);
		//�ʼ�
		contact.email(jsonData->properties[JSON_EMAIL].stringValue);
		//��ַ
		contact.homesite(jsonData->properties[JSON_HOMESITE].stringValue);
		//��ַ
		contact.companyAddress(jsonData->properties[JSON_ADDRESS].stringValue);
		//�ʱ�
		contact.companyPostCode(jsonData->properties[JSON_POSTCODE].stringValue);
		//��ע
		contact.memo(jsonData->properties[JSON_MEMO].stringValue);
		contact.Insert();	
		//
		jsonObject *j = new jsonObject();
		j->properties[JSON_ID]     = jsonValue((double)contact.id());
		obj->properties[JSON_DATA] = jsonValue(j);
		
		g_iContactCount++;
	}
	
	//���ɻ�Ӧ����
	return obj;
}


int GetOutContactGid(std::string name)
{
	//��ѯ���Ⱥ���ڲ���
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
		//����Ⱥ��
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
	int nReset = jsonData->properties[JSON_RESET].numericValue; //�Ƿ���������
	if (jsonData->properties.find(JSON_COUNT) != jsonData->properties.end())
	{
		if (1 == nReset)
		{
			//ɾ���ڲ�Ⱥ��
			Data::ContactGroup::Remove("");
			//ɾ���ڲ���Ƭ
			Data::Contact::Remove("");

			g_iContactCount = 0 ;
		}

		//�������Ⱥ
		jsonArray * ja = new jsonArray();
		int count = jsonData->properties[JSON_COUNT].numericValue;
		
		//data:{}
		//data�����Object
		jsonObject *jDataObject = new jsonObject ;
		jDataObject->properties[JSON_COUNT]	 = jsonValue((double)count);
		
		for (int i = 0 ; i < count ;i++)
		{
			//����
			jsonObject *jArrayObject = jsonData->properties[JSON_CHILDREN].\
				arrayValue->items[i].objectValue ;
			outContact.name(jArrayObject->properties[JSON_NAME].stringValue);
			//����
			outContact.alias(main->m_pContactNewDlg->GetAlias(outContact.name()));
			CString temp = Util::StringOp::ToCString(outContact.name());
			//����
//			outContact.surName(Util::StringOp::FromCString(temp.Mid(0,1)));
			//��ƴ+����
//			std::string name = outContact.alias().substr(0,1);
//			name += outContact.name();
//			outContact.initialAndName(name);
			//����
			outContact.type(Data::ContactType((int)jArrayObject->\
				properties[JSON_CARDTYPE].numericValue));
			//�ֻ�1
			outContact.mobilesTelephone(jArrayObject->properties[JSON_MOBILE].stringValue);
			//�ֻ�2
			outContact.mobilesTelephone2(jArrayObject->properties[JSON_MOBILE2].stringValue);
			//�̻�1
			outContact.worksTelephone(jArrayObject->properties[JSON_WORKPHONE].stringValue);
			//�̻�2
			outContact.worksTelephone2(jArrayObject->properties[JSON_WORKPHONE2].stringValue);
			//��˾
			outContact.company(jArrayObject->properties[JSON_COMPANY].stringValue);
			//ְ��
			outContact.duty(jArrayObject->properties[JSON_DUTY].stringValue);
			//����
			outContact.department(jArrayObject->properties[JSON_DEPARTMENT].stringValue);

			if (jArrayObject->properties.find(JSON_GROUPNAME) !=\
				jArrayObject->properties.end())
			{
				std::string gName = jArrayObject->properties[JSON_GROUPNAME].stringValue;
				outContact.groupId(GetOutContactGid(gName));
			}

			//�ʼ�
			outContact.email(jArrayObject->properties[JSON_EMAIL].stringValue);
			//��ַ
			outContact.homesite(jArrayObject->properties[JSON_HOMESITE].stringValue);
			//��ַ
			outContact.companyAddress(jArrayObject->properties[JSON_ADDRESS].stringValue);
			//�ʱ�
			outContact.companyPostCode(jArrayObject->properties[JSON_POSTCODE].stringValue);
			//��ע
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
		//����������Ƭ
		//����
		outContact.name(jsonData->properties[JSON_NAME].stringValue);
		//����
		outContact.alias(main->m_pContactNewDlg->GetAlias(outContact.name()));
		CString temp = Util::StringOp::ToCString(outContact.name());
		//����
//		outContact.surName(Util::StringOp::FromCString(temp.Mid(0,1)));
		//��ƴ+����
//		std::string name = outContact.alias().substr(0,1);
//		name += outContact.name();
//			outContact.initialAndName(name);
		//����
		outContact.type(Data::ContactType((int)jsonData->\
			properties[JSON_CARDTYPE].numericValue));
		//�ֻ�1
		outContact.mobilesTelephone(jsonData->properties[JSON_MOBILE].stringValue);
		//�ֻ�2
		outContact.mobilesTelephone2(jsonData->properties[JSON_MOBILE2].stringValue);
		//ֱ��
		outContact.worksTelephone(jsonData->properties[JSON_WORKPHONE].stringValue);
		//�ֻ�
		outContact.worksTelephone2(jsonData->properties[JSON_WORKPHONE2].stringValue);
		//��˾
		outContact.company(jsonData->properties[JSON_COMPANY].stringValue);
		//ְ��
		outContact.duty(jsonData->properties[JSON_DUTY].stringValue);
		//����
		outContact.company(jsonData->properties[JSON_DEPARTMENT].stringValue);
		//�ʼ�
		outContact.email(jsonData->properties[JSON_EMAIL].stringValue);
		//��ַ
		outContact.homesite(jsonData->properties[JSON_HOMESITE].stringValue);
		//��ַ
		outContact.companyAddress(jsonData->properties[JSON_ADDRESS].stringValue);
		//�ʱ�
		outContact.companyPostCode(jsonData->properties[JSON_POSTCODE].stringValue);
		//��ע
		outContact.memo(jsonData->properties[JSON_MEMO].stringValue);
		//����
//		outContact.section(jsonData->properties[JSON_SECTION].stringValue);
		outContact.Insert();	
		//
		jsonObject *j = new jsonObject();
		j->properties[JSON_ID]     = jsonValue((double)outContact.id());
		obj->properties[JSON_DATA] = jsonValue(j);
		
		g_iContactCount++;
	}
	
	//���ɻ�Ӧ����
	return obj;
}


extern BOOL DetectFile(TCHAR *sDir);

//�����ļ�
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
	case 47: //ͼƬ
		filePath += "MY_PHOTO/";
		filePath += Util::StringOp::ToCString(jsonData->properties[JSON_NAME].stringValue);
		break;
	case 48: //����
		filePath += "MY_RING/" ;
		filePath += Util::StringOp::ToCString(jsonData->properties[JSON_NAME].stringValue);
		break;
	}
	
	if (DetectFile(filePath.GetBuffer(256)))
	{
		obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
		obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string("�ļ�����ʧ��"));
	}
	else if (!g_File.Open(filePath,CFile::modeCreate|CFile::modeWrite))
	{
		obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
		obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string("�ļ�����ʧ��"));
	}
	filePath.ReleaseBuffer();

	jsonObject *j = new jsonObject();
	obj->properties[JSON_DATA] = jsonValue(j);

	return obj ;
	
}	

//ɾ���ⲿȺ��
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
		//�����ʱ������
		//���������Ƭ
		jsonArray * ja = new jsonArray();
		int count = jsonData->properties[JSON_COUNT].numericValue;
		
		//data:{}
		//data�����Object
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
		//ɾ��
		int id = (int)jsonData->properties[JSON_ID].numericValue ;
		std::string filter = "id = ";
		filter += Util::StringOp::FromInt(id);
		Data::ContactGroup::Remove(filter);	
		
		//����JSON_ID
		jsonObject *j = new jsonObject();
		j->properties[JSON_ID] = jsonValue((double)id);
		obj->properties[JSON_DATA] = jsonValue(j);
		
	}
	
	//���ɻ�Ӧ����
	return obj;
}



//ɾ���ⲿ��Ƭ
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
		//�����ʱ������
		//���������Ƭ
		jsonArray * ja = new jsonArray();
		int count = jsonData->properties[JSON_COUNT].numericValue;
		
		//data:{}
		//data�����Object
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
		//ɾ��
		int id = (int)jsonData->properties[JSON_ID].numericValue ;
		std::string filter = "id = ";
		filter += Util::StringOp::FromInt(id);
		Data::Contact::Remove(filter);	
		
		//����JSON_ID
		jsonObject *j = new jsonObject();
		j->properties[JSON_ID] = jsonValue((double)id);
		obj->properties[JSON_DATA] = jsonValue(j);

		g_iContactCount--;
		
	}
	
	//���ɻ�Ӧ����
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
	case 47: //ͼƬ
		filePath += "MY_PHOTO/";
		filePath += Util::StringOp::ToCString(jsonData->properties[JSON_NAME].stringValue);
		break;
	case 48: //����
		filePath += "MY_RING/" ;
		filePath += Util::StringOp::ToCString(jsonData->properties[JSON_NAME].stringValue);
		break;
	}
	
	if (!DeleteFile(filePath))
	{
		obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
		obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string("ɾ���ļ�ʧ��"));
	}
	
	jsonObject *j = new jsonObject();
	obj->properties[JSON_DATA] = jsonValue(j);
	
	return obj ;

}

//�޸��ⲿȺ��
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
		//�����ʱ������
		//���������Ƭ
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
				sprintf(msg,"Ⱥ��ID = %d������",id);
				obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string(msg));
				break;
			}
		}
		
		jsonObject *j = new jsonObject();
		obj->properties[JSON_DATA] = jsonValue(j);
	}
	else
	{	
		//�޸ĸ���
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
			sprintf(msg,"Ⱥ��ID = %d������",id);
			obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string(msg));
		}
		
		//
		jsonObject *j = new jsonObject();
		obj->properties[JSON_DATA] = jsonValue(j);
		
	}
	
	//���ɻ�Ӧ����
	return obj;
}


//�޸��ⲿ��Ƭ
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
		//�����ʱ������
		//���������Ƭ
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
				//����
				vContact[0]->alias(main->m_pContactNewDlg->GetAlias(vContact[0]->name()));
				CString temp = Util::StringOp::ToCString(vContact[0]->name());
				//����
//				vContact[0]->surName(Util::StringOp::FromCString(temp.Mid(0,1)));
				//��ƴ+����
//				std::string name = vContact[0]->alias().substr(0,1);
//				name += vContact[0]->name();
//				vContact[0]->initialAndName(name);
				//����
				vContact[0]->type(Data::ContactType((int)atoiA(jArrayObject->properties[JSON_TYPE].stringValue.c_str())));
				//�ֻ�1
				vContact[0]->mobilesTelephone(jArrayObject->properties[JSON_MOBILE].stringValue);
				//�ֻ�2
				vContact[0]->mobilesTelephone2(jArrayObject->properties[JSON_MOBILE2].stringValue);
				//ֱ��
				vContact[0]->worksTelephone(jArrayObject->properties[JSON_WORKPHONE].stringValue);
				//�ֻ�
				vContact[0]->worksTelephone2(jArrayObject->properties[JSON_WORKPHONE2].stringValue);
				//��˾
				vContact[0]->company(jArrayObject->properties[JSON_COMPANY].stringValue);
				//ְ��
				vContact[0]->duty(jArrayObject->properties[JSON_DUTY].stringValue);
				//����
				vContact[0]->company(jArrayObject->properties[JSON_DEPARTMENT].stringValue);
				//����
//				vContact[0]->section(jArrayObject->properties[JSON_SECTION].stringValue);
				
				vContact[0]->Update();
			}
			else 
			{
				obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
				char msg[30] = {0};
				sprintf(msg,"Ⱥ��ID = %d������",id);
				obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string(msg));
				break;
			}
		}
		
		jsonObject *j = new jsonObject();
		obj->properties[JSON_DATA] = jsonValue(j);
	}
	else
	{	
		//�޸ĸ���
		int id = (int)jsonData->properties[JSON_ID].numericValue ;
		std::string filter = "id = ";
		filter += Util::StringOp::FromInt(id);
		
		std::vector<boost::shared_ptr<Data::Contact> > vContact = \
			Data::Contact::\
			GetFromDatabaseByOrderOffsetLength(filter,"id",0, 1);
		
		if (!vContact.empty())
		{
			//����
			vContact[0]->name(jsonData->properties[JSON_NAME].stringValue);
			//����
			vContact[0]->alias(main->m_pContactNewDlg->GetAlias(vContact[0]->name()));
			CString temp = Util::StringOp::ToCString(vContact[0]->name());
			//����
//			vContact[0]->surName(Util::StringOp::FromCString(temp.Mid(0,1)));
			//��ƴ+����
//			std::string name = vContact[0]->alias().substr(0,1);
//			name += vContact[0]->name();
//			vContact[0]->initialAndName(name);
			//����
			vContact[0]->type(Data::ContactType((int)jsonData->\
				properties[JSON_CARDTYPE].numericValue));
			//�ֻ�1
			vContact[0]->mobilesTelephone(jsonData->properties[JSON_MOBILE].stringValue);
			//�ֻ�2
			vContact[0]->mobilesTelephone2(jsonData->properties[JSON_MOBILE2].stringValue);
			//ֱ��
			vContact[0]->worksTelephone(jsonData->properties[JSON_WORKPHONE].stringValue);
			//�ֻ�
			vContact[0]->worksTelephone2(jsonData->properties[JSON_WORKPHONE2].stringValue);
			//��˾
			vContact[0]->company(jsonData->properties[JSON_COMPANY].stringValue);
			//ְ��
			vContact[0]->duty(jsonData->properties[JSON_DUTY].stringValue);
			//����
			vContact[0]->company(jsonData->properties[JSON_DEPARTMENT].stringValue);
			//����
//			vContact[0]->section(jsonData->properties[JSON_SECTION].stringValue);
			
			vContact[0]->Update();
		}
		else 
		{
			obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
			char msg[30] = {0};
			sprintf(msg,"��ƬID = %d������",id);
			obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string(msg));
		}
		
		//
		jsonObject *j = new jsonObject();
		obj->properties[JSON_DATA] = jsonValue(j);
		
	}
	
	//���ɻ�Ӧ����
	return obj;
}


//���һ��Ŀ¼��������
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
				//�ݹ�
				CString dir = s + L"/" +fd.cFileName;
				nSize += RecursiveDirRingCount((LPTSTR)(LPCTSTR)dir);
			}

		} while (FindNextFile(hFind, &fd));

	}	
	FindClose(hFind);
	
	return nSize ;
}

//���һ��Ŀ¼��������
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
				//�ݹ�
				CString dir = s + L"/" +fd.cFileName;
				nSize += RecursiveDirRingCount((LPTSTR)(LPCTSTR)dir);
			}

		} while (FindNextFile(hFind, &fd));

	}	
	FindClose(hFind);
	
	return nSize ;
}

//��ѯ�豸����
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

	//��Ʒ���
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
	
	//������
	CString text;
	ULARGE_INTEGER freeBytes;//�û���ʹ�õ����ɿռ�
	ULARGE_INTEGER totalBytes;//�ܿռ�
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
	
	//ʣ��ռ�
	jsonData->properties[JSON_FREESPACE] = \
		jsonValue(double(f));
	
	//������
	jsonData->properties[JSON_RINGNUM] = \
		jsonValue((double)RecursiveDirRingCount(L"/flashdrv/my_ring"));
	
	//ͼƬ��
	jsonData->properties[JSON_PICNUM] = \
		jsonValue((double)RecursiveDirPicCount(L"/flashdrv/my_photo"));


	obj->properties[JSON_DATA] = jsonValue(jsonData);

	return obj ;

}

//��ѯ�ⲿȺ��
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

	//��ѯ������Ƭ
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


//��ѯ�ⲿ��Ƭ
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

	//��ѯ�����ڲ�Ⱥ��
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
	
	//��ѯ�����ڲ�Ⱥ��
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

//��ѯ�ⲿ��Ƭ����
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
	
	//��ѯ�����ڲ�Ⱥ��
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

	//��ѯ�����ڲ�Ⱥ��
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

//��ѯͨ����¼����
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
	
	//��ѯ�����ڲ�Ⱥ��
	int nCount = Data::ContactInfo::GetDataCount("");
	
	jsonObject *jsonData = new jsonObject ;
	jsonData->properties[JSON_COUNT] = jsonValue((double)nCount);
	
	obj->properties[JSON_DATA] = jsonValue(jsonData);
	return obj ;
}

//���Ŀ¼���ļ�
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

//���JPGͼ����Ŀ
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

//��wav,mp3ʱ��
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
	case 47: //ͼƬ
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
					obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string("û������ļ�"));
				}

				jsonObject *j = new jsonObject();
				obj->properties[JSON_DATA] = jsonValue(j);

			}
			else
			{
				//Ҫ�ļ��б�
				jsonObject *jd = new jsonObject();
				jsonArray  *jsonA = new jsonArray ;
				GetDirJPGFileName(L"\\flashdrv\\my_photo\\",jsonA);
				
				//���ʱ��
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
	case 48: //����
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
					obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string("û������ļ�"));
				}
				
				jsonObject *j = new jsonObject();
				obj->properties[JSON_DATA] = jsonValue(j);
			}
			else
			{
				//Ҫ�ļ��б�
				jsonObject *jd = new jsonObject();
				jsonArray  *jsonA = new jsonArray ;
				GetDirFileName(L"\\flashdrv\\my_ring\\",jsonA);
				
				//���ʱ��
				CString filename ;
				for (int i = 0 ; i < jsonA->items.size() ;i++)
				{	
					filename = L"\\flashdrv\\my_ring\\"+\
						Util::StringOp::ToCString(\
						jsonA->items[i].objectValue->properties[JSON_NAME].stringValue);
					
					//���ʱ��
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

//���¼����Ŀ¼
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

	//����
	int nType = obj->properties[JSON_ID].numericValue ;

	jsonObject *jsonData = jo.properties.find(JSON_DATA)->second.objectValue ;
	CString filePath = L"/flashdrv/";
	switch(nType)
	{
	case 1: //�����ļ�
		{
			//���ȫ·��
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

			//���ļ�
			if (!g_File.Open(filePath,CFile::modeRead))
			{
				obj->properties[JSON_RETURN_CODE]    = jsonValue((double)1);
				obj->properties[JSON_RETURN_MESSAGE] = jsonValue(std::string("û������ļ�"));
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
	case 2: //����ͨ����¼�����ļ��б�
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
				//ȡ�ļ���
				std::string name = vSoundSegment[i]->filename();
				
				//��"\\"�任��"/"
				replace_all(name,"\\","/");

			//  size_t t = name.rfind("\\");
			//  if (t != -1)
			//	{
			//		name = name.substr(t+1);
			//	}
				
				//����
				jsonObject *jsonName = new jsonObject;
				jsonName->properties[JSON_NAME] = jsonValue(name);
				jsonA->items.push_back(jsonValue(jsonName));

			}
			
			jd->properties[JSON_COUNT]    = jsonValue((double)vSoundSegment.size());
			jd->properties[JSON_CHILDREN] = jsonValue(jsonA);

			obj->properties[JSON_DATA]    = jsonValue(jd); 

		}
		break;
	case 3: //¼��Ŀ¼���ļ�ȫ·��
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

//�¼�����
void registerProcessor()
{	
	//����
	handlers_[JSON_CREATE]["41"] = CreateOutGroup;
	//�����ڲ�Ⱥ��
//	handlers_[JSON_CREATE]["42"] = CreateInGroup ;
	//�����ⲿ��Ƭ
	handlers_[JSON_CREATE]["43"] = CreateOutContact;
	//�����ڲ���Ƭ
//	handlers_[JSON_CREATE]["44"] = CreateInContact ;
	//����ͼƬ
	handlers_[JSON_CREATE]["47"] = CreateFile ;
	//��������
	handlers_[JSON_CREATE]["48"] = CreateFile ;
	//����excel�ڲ���Ƭ
//	handlers_[JSON_CREATE]["49"] = CreateInExcelContact;
	//����excel�ⲿ��Ƭ
	handlers_[JSON_CREATE]["50"] = CreateOutExcelContact;


	//ɾ���ⲿȺ��
	handlers_[JSON_DESTORY]["41"] = DestoryOutGroup ;
	//ɾ���ڲ�Ⱥ��
//	handlers_[JSON_DESTORY]["42"] = DestoryInGroup ;
	//ɾ���ⲿ��Ƭ
	handlers_[JSON_DESTORY]["43"] = DestoryOutContact ;
	//ɾ���ڲ���Ƭ
//	handlers_[JSON_DESTORY]["44"] = DestoryInContact ;
	//ɾ��ͼƬ
	handlers_[JSON_DESTORY]["47"] = DestoryFile ;
	//ɾ������
	handlers_[JSON_DESTORY]["48"] = DestoryFile ;
	
	//�޸�
	//�޸��ⲿȺ��
	handlers_[JSON_MODIFY]["41"] = ModifyOutGroup ;
	//�޸��ڲ�Ⱥ��
//	handlers_[JSON_MODIFY]["42"] = ModifyInGroup ;
	//�޸��ⲿ��Ƭ
	handlers_[JSON_MODIFY]["43"] = ModifyOutContact ;
	//�޸��ڲ���Ƭ
//	handlers_[JSON_MODIFY]["44"] = ModifyInContact ;

	//��ѯ
	//��ѯ�豸����
	handlers_[JSON_QUERY]["40"] = QueryDeviceProperty;
	//��ѯ�ⲿȺ��
	handlers_[JSON_QUERY]["41"] = QueryOutGroup;
	//��ѯ�ڲ�Ⱥ��
//	handlers_[JSON_QUERY]["42"] = QueryInGroup;
	//��ѯ�ⲿ��Ƭ
	handlers_[JSON_QUERY]["43"] = QueryOutContact;
	//��ѯ�ڲ���Ƭ
//	handlers_[JSON_QUERY]["44"] = QueryInContact;
	//��ѯͨ����¼
	handlers_[JSON_QUERY]["45"] = QueryContactInfo;
	//��ѯͨ����¼
	handlers_[JSON_QUERY]["46"] = QueryContactInfoFile;
	//��ѯͨ����¼
	handlers_[JSON_QUERY]["47"] = QueryFile;
	//��ѯͨ����¼
	handlers_[JSON_QUERY]["48"] = QueryFile;
	//��ѯ�ڲ���Ƭ
//	handlers_[JSON_QUERY]["49"] = QueryInExcelContact ;
	//��ѯ�ⲿ��Ƭ
	handlers_[JSON_QUERY]["50"] = QueryOutExcelContact ;

	//��ѯ�ڲ���Ƭ����
	handlers_[JSON_GETCOUNT]["45"] = QueryContactInfoCount;
	//��ѯ�ڲ���Ƭ����
//	handlers_[JSON_GETCOUNT]["49"] = QueryInExcelContactCount;
	//��ѯ�ⲿ��Ƭ����
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
			//��ʼ
			//����
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
			
			//����
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
	
	//����JsonObject
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
	//�����ļ�ֱ�ӷ�����
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