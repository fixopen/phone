#include "lzw.h"

ENTRY          m_encstringtab[TAB_SIZE];//编码表

CLZWEncode::CLZWEncode()
{
    m_outbuf = NULL;
}

CLZWEncode::~CLZWEncode()
{
//	ReleaseBuf(m_outbuf);
}

void CLZWEncode::ReleaseBuf(BYTE *pBuffer)
{
	if(pBuffer)
		SysFree(pBuffer);
} 

void CLZWEncode::InitData()
{ 
	 m_curtablepos = INIT_SIZE ;
	 m_inpindex = 0;
	 m_oupindex = 0 ,m_tempcode = EMPTY;
	 m_outbuf = (BYTE *)SysMalloc(INIT_ENCBUF_SIZE);
	 m_totalsize = INIT_ENCBUF_SIZE;
	 return ;
//	 m_ncount= 0;
}


void CLZWEncode::InitTable()
{
	register int i;
	for(i = 0 ; i < TAB_SIZE ;i++)
		m_encstringtab[i].used = FALSE ,m_encstringtab[i].prev  = NO_PREV, m_encstringtab[i].c= 0;
	for(i = 0 ; i< INIT_SIZE ; i++)
	{
		m_encstringtab[i].used = TRUE;
		m_encstringtab[i].c = i;
	}
}

UINT32 CLZWEncode::QueryTable(UINT32 code , BYTE c)
{
	register ENTRY *u;
	UINT32 i;
	if(code == NO_PREV)
	    return c;
    for(i = code +1 ; i < m_curtablepos ; i++) 
	{
		u = &m_encstringtab[i];
        if(u->used && u->prev==code &&u->c == c)
			return i;
	}
	return NOT_FIND;
}

void CLZWEncode::UpdateTable(UINT32 prevcode , BYTE c)
{
//   if(m_curtablepos >= TAB_SIZE)
//		return NOT_FIND;
	m_encstringtab[m_curtablepos].used = TRUE;
	m_encstringtab[m_curtablepos].prev = prevcode;
	m_encstringtab[m_curtablepos++].c = c;
//	return m_curtablepos++;
}

UINT32 CLZWEncode::GetChars(UINT32 code , BYTE *buffer , UINT32 size)
{
	register UINT32 i =0 , j ,temp = size -1;
	register ENTRY *u = m_encstringtab;
	if(!(u + code)->used || code >= TAB_SIZE)
		return 0;
    while((u + code)->prev != NO_PREV)
	{
		buffer[temp -i++] = (u + code)->c;
		code =(u + code)->prev;
	}
	buffer[temp-i++] = (u + code)->c;
	for(j= 0 ; j< i ; j++)
	   buffer[j] = buffer[size - i +j] ;
	buffer[i] = '\0';
	return i;
}// */

void CLZWEncode::putcode(UINT32 code)
{
	register UINT32  u;
	if(m_oupindex +2 > m_totalsize)
	{
		m_outbuf = (BYTE *)SysRealloc(m_outbuf, m_totalsize ,m_totalsize + INC_SIZE);
		m_totalsize += INC_SIZE;
	}
//	if(oupindex == 255)
//		printf("error1");
	if(m_tempcode == EMPTY)
	{
		m_outbuf[m_oupindex++]= code & 0x0FF;
		m_tempcode = ((code >> 4) & 0x0F0);
	}
	else 
	{
		u = m_tempcode | (code & 0x0F);
		m_outbuf[m_oupindex++] = u;
		u = (code >> 4) & 0x0FF;
		m_outbuf[m_oupindex++] = u;
		m_tempcode = EMPTY;
	}
}


UINT32 CLZWEncode::Encode(BYTE *pSrcBuf, UINT32 nBufSize , BYTE **ppDstBuf)
{
	register UINT32 prevcode , localcode;
	UINT32 c ,count = TAB_SIZE - INIT_SIZE;
    if(!pSrcBuf|| nBufSize <=0)
	{
		ppDstBuf = NULL;
		return 0;
	}
	m_pSrcPicdata = pSrcBuf;
	m_nSrcPicsize = nBufSize;
	InitData();
	InitTable();
	putcode(CC);
	c = readc();
	prevcode = QueryTable(NO_PREV , c);
	while(UEOF != (c = readc()))
	{
//		ch = c;
	//	++total;
		if(NOT_FIND!=(localcode = QueryTable(prevcode , c)))
		{
			prevcode = localcode;
			continue;
		}
		putcode(prevcode);
		if(count)
		{
		   UpdateTable(prevcode , c);
	//	   if(currentpos == 558)
		//	   printf("error possible");
		   --count;
		}
		prevcode = QueryTable(NO_PREV , c);
	}
	putcode(prevcode);
	putcode(CEND);
	if(m_tempcode!=EMPTY)
		m_outbuf[m_oupindex++] = m_tempcode &0x0F0;  //关键！！！！
	*ppDstBuf = m_outbuf;
	return m_oupindex;
}	

UINT32 CLZWEncode::readc()
{
	if(m_inpindex >= m_nSrcPicsize)
		return UEOF;
	return m_pSrcPicdata[m_inpindex++];
}


//一下是lzw解码部分
ENTRY          m_decstringtab[TAB_SIZE];//解码表
//BYTE *CLZWDecode::m_outbuf;
BYTE    *m_outbuf;
CLZWDecode::CLZWDecode()
{
	m_pSrcPicdata= NULL;
}


CLZWDecode::~CLZWDecode()
{
/*	if(m_pSrcPicdata)
	{
		delete m_pSrcPicdata ;
		m_pSrcPicdata = NULL;
	}*/
   
}




void CLZWDecode::UpdateTable(UINT32 prevcode , BYTE c)
{
//   if(m_curtablepos >= TAB_SIZE)
//		return NOT_FIND;
	m_decstringtab[m_curtablepos].used = TRUE;
	m_decstringtab[m_curtablepos].prev = prevcode;
	m_decstringtab[m_curtablepos++].c = c;
//	return m_curtablepos++;
}

UINT32 CLZWDecode::GetChars(UINT32 code , char *buffer , UINT32 size)
{
	register UINT32 i =0 ,temp = size -1;
	register ENTRY *u =m_decstringtab;
//	register ENTRY *u1 =(ENTRY *)( m_decstringtab);
	if(!(u+code)->used || code >= TAB_SIZE)
		return 0;
    while((u+code)->prev != NO_PREV)
	{
		buffer[temp -i++] = (u+code)->c;
		code = (u+code)->prev;
	}
	buffer[temp-i++] = (u+code)->c;
	temp =size-i;
	for(int j= 0 ; j< i ; j++)
	   buffer[j] = buffer[temp +j] ;
	buffer[i] = '\0';
	return i;
}// */

void AccelerateGetChars()
{
//	__asm  add d0 , d1;
	
}
UINT32 CLZWDecode::getcode()
{
     register UINT32 code;
     if(m_tempcode == EMPTY)
	 {
		 code = readc();
		 m_tempcode = readc();
	     code = code | ((m_tempcode << 4 ) & 0x0F00);
		 if(code == CEND)
			 return UEOF;
		 return code ;
	 }
	 else
	 {
		 code = readc();
		 if(code ==UEOF)
			 return UEOF;
		 code = ((code << 4 ) & 0x0FF0) | (m_tempcode & 0x0F);
		 m_tempcode = EMPTY;
		 if(code == CEND)
			 return UEOF;
		 return code;
	 }
}    

void CLZWDecode::ReleaseBuf(BYTE *pBuffer)
{
	if(pBuffer)
		SysFree(pBuffer);
}

UINT32 CLZWDecode::readc()
{
	return m_pSrcPicdata[m_inpindex++];
}


void CLZWDecode::InitTable()
{
	register int i;
	for(i = 0 ; i < TAB_SIZE ;i++)
		m_decstringtab[i].used = FALSE ,m_decstringtab[i].prev  = NO_PREV, m_decstringtab[i].c= 0;
	for(i = 0 ; i< INIT_SIZE ; i++)
	{
		m_decstringtab[i].used = TRUE;
		m_decstringtab[i].c = i;
	}
}

BOOL CLZWDecode::InitData(BYTE **ppDstBuf)
{
	 m_curtablepos = INIT_SIZE ;
	 m_inpindex = 0;
	 m_oupindex = 0 ,m_tempcode = EMPTY;
	 if((*ppDstBuf)==NULL)
		 m_outbuf = (BYTE *)SysMalloc(INIT_DECBUF_SIZE);
	 else 
		 m_outbuf = *ppDstBuf;
	 m_totalsize = INIT_DECBUF_SIZE;
	 return (m_outbuf==NULL? FALSE: TRUE);
//	 m_ncount= 0;
}

int g_flag;
UINT32 CLZWDecode::Decode(BYTE *pSrcBuf, BYTE *pDstBuf, UINT32 nBufSize)
{
   	register UINT32 prevcode , local ,temp;
	UINT32 count ,i,space = TAB_SIZE - INIT_SIZE;
	char  curstr[TAB_SIZE];
	if(pSrcBuf==NULL) 
	{
		*pDstBuf = 0;
		return 0;
	}
	m_pSrcPicdata = pSrcBuf;
	if(!InitData(&pDstBuf))
	{
		*pDstBuf = 0;
		return 0;
	}

	prevcode = 0;
    while((prevcode = getcode())!=CC)
	{
		if(prevcode == UEOF)
			return 0;
	}
    InitTable();
	prevcode = getcode();
	count = GetChars(prevcode , curstr , TAB_SIZE);
	for(i = 0 ; i< count ;i++)
	   m_outbuf[i] = curstr[i];
	m_oupindex += count;
	g_flag = 1;
    local = getcode();
	temp = 0;
	while(UEOF != local )
	{
		g_flag = 0;
        if(m_decstringtab[local].used)
		    count = GetChars(local , curstr , TAB_SIZE);
        else
		{
		    count = GetChars(prevcode , curstr , TAB_SIZE);
			curstr[count] = curstr[0];
			curstr[++count] = '\0';
		}
		g_flag = 2;
        if(space)
		{
		   UpdateTable(prevcode , curstr[0] );
		   --space;
		}
		prevcode = local;
		if(m_oupindex + count > nBufSize)
		    return m_oupindex;
		for(i = 0; i< count; i++)
		    m_outbuf[m_oupindex + i] = curstr[i] ;
		m_oupindex += count;
		g_flag = 1;
        local = getcode();
		temp++;
	}
	return m_oupindex;  
}
 
UINT32 CLZWDecode::Decode(BYTE *pSrcBuf, BYTE **ppDstBuf)
{
	register UINT32 prevcode , local ,temp;
	UINT32 count ,i,space = TAB_SIZE - INIT_SIZE;
	char  curstr[TAB_SIZE];
	if(pSrcBuf==NULL) 
	{
		ppDstBuf = NULL;
		return 0;
	}
	m_pSrcPicdata = pSrcBuf;
	*ppDstBuf = NULL;
	if(!InitData(ppDstBuf))
	{
		*ppDstBuf = 0;
		return 0;
	}

	prevcode = 0;
    while((prevcode = getcode())!=CC)
	{
		if(prevcode == UEOF)
			return 0;
	}
    InitTable();
	prevcode = getcode();
	count = GetChars(prevcode , curstr , TAB_SIZE);
	for(i = 0 ; i< count ;i++)
	   m_outbuf[i] = curstr[i];
	m_oupindex += count;
	g_flag = 1;
    local = getcode();
	temp = 0;
	while(UEOF != local )
	{
		g_flag = 0;
        if(m_decstringtab[local].used)
		    count = GetChars(local , curstr , TAB_SIZE);
        else
		{
		    count = GetChars(prevcode , curstr , TAB_SIZE);
			curstr[count] = curstr[0];
			curstr[++count] = '\0';
		}
		g_flag = 2;
        if(space)
		{
		   UpdateTable(prevcode , curstr[0] );
		   --space;
		}
		prevcode = local;
		if(m_totalsize < m_oupindex + count)
		{
            m_outbuf = (BYTE *)SysRealloc(m_outbuf , m_totalsize, m_oupindex + DEC_INC_SIZE + count);
			m_totalsize = m_oupindex + DEC_INC_SIZE + count;
		}
		for(i = 0; i< count; i++)
		    m_outbuf[m_oupindex + i] = curstr[i] ;
		m_oupindex += count;
		g_flag = 1;
        local = getcode();
		temp++;
	}
	*ppDstBuf = m_outbuf; 
    return m_oupindex;  
}