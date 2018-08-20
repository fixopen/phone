#include "StdAfx.h"
#include "CParseFile.h"
#include "PSerialPort.h"
#include "..\MultimediaPhoneDlg.h"
CCParseFile::CCParseFile(void)
{
}
CCParseFile::~CCParseFile(void)
{
}

FILE * writefp=NULL;
int packetTotalcount=0;
int packetcount=0;
FILE * Readfp=NULL;
int packectId=0;
long totalCount=0;
int readFileCount=0;
char sendBuffdata[READWRITEBUFF];//用来存放发送数据
char ErrorBuffdata[READWRITEBUFF];
char readBuffdata[MAXBLOCK];
PROTOCOLTFILE sendFileType;
long FileLength=0;
BOOL ProgramIsContinue=FALSE;
BOOL m_bIsFinishDownLoad=FALSE;
extern BOOL m_bInsertUSB;

void CCParseFile::SendORReciveData(CPSerialPort *serialport)
{
	char recivedata[READWRITEBUFF];

	BOOL fReadState=TRUE;

	int readCount=0;
   
	fReadState=serialport->ReadPort(recivedata,READWRITEBUFF,readCount);

	if (!fReadState)
	{ 
		return;

	}else{

		if (parsePacket(serialport,recivedata,readCount))
		{
			PTRBASEPROTOCOL  preadbuff=(PTRBASEPROTOCOL)recivedata;

			switch(preadbuff->trasctTionID)
			{
			case AUTHENTICATIONTRAN:
				{   
					switch(preadbuff->cmdType)
					{
					case AUTHCMDREQ:
						{  
							AUTHENTI authenti;

							memset(authenti.productType,'\0',PRODUCTTYPE);
							
							memset(authenti.productSertalNumber,'\0',PRODUCTSERIALNUMBER);
							
							memset(authenti.productTionTime,'\0',PRODUCTIONTIME);
							
							memset(authenti.softwaveVersion,'\0',SOFTWAREVERSION);
							
							memset(authenti.handwaveVersion,'\0',HARDWAREVERSION);
							
		                    memset(authenti.PasswordDauthenti,'\0',PASSWORDAUTHENTI);

							memset(sendBuffdata,'\0',READWRITEBUFF);

							if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_chbSetAdminPassword.GetCheck() == BST_CHECKED )
							{       
								std::string password="yes";
								memcpy(authenti.PasswordDauthenti,password.c_str(),password.length());
								PackageAuthPacket((char *)&authenti,1,\
									AUTHCMDRSP,AUTHCMDRSP,FALSE,sendBuffdata);
                                 
							}else{

								std::string password="No";								
								memcpy(authenti.PasswordDauthenti,password.c_str(),password.length());                          
							    PackageAuthPacket((char *)&authenti,1,\
									AUTHCMDRSP,AUTHCMDRSP,FALSE,sendBuffdata);

							}

							serialport->WritePort(sendBuffdata,sizeof(PROTOCOLAU));

						}
						break;
					case AUTHCMDRSP:
						{ 
							/////显示版本号，其它内容

						}
						break;
					case RSPRETRYDATA:
						{ 
							switch(preadbuff->preCmdType)
							{
							case AUTHCMDREQ:
							  
								serialport->WritePort(sendBuffdata,sizeof(PROTOCOLAU));
                             
							    break;

							case AUTHCMDRSP:

							    serialport->WritePort(sendBuffdata,sizeof(PROTOCOLAU));

                               break;
							}
						}
						break;
					case REQRETRYDATA:
						{
                           packageErrorPacket(serialport,recivedata,RSPRETRYDATA,ErrorBuffdata);
						}
						break;
					}
				}
				break;
			case CHECKPASSWORDTRAN:
				{
					switch(preadbuff->cmdType)
					{
					case CHECKPASSREQ:
						{
							PASSWOR pwd;

							memset(pwd.userName,'\0',USERNAME);
							
							memset(pwd.userPassword,'\0',USERPASSWORD);
							
		                    memset(pwd.testResult,'\0',TESTRESULT);

							memset(sendBuffdata,'\0',READWRITEBUFF);

							std::string password=((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->adminPassword();

							memcpy(pwd.userPassword,password.c_str(),password.length());

							PackagePasswordPacket((char *)&pwd,1,CHECKPASSRSP,CHECKPASSRSP,FALSE,sendBuffdata);

							serialport->WritePort(sendBuffdata,sizeof(PROTOCOLCHECK));
						}
						break;
					case CHECKPASSRSP:
						{
							////////////密码验证//////////////////
						}
						break;
					case RSPRETRYDATA:
						{
                          switch(preadbuff->preCmdType)
						  {
						  case CHECKPASSREQ:

							  serialport->WritePort(sendBuffdata,sizeof(PROTOCOLCHECK));

							  break;

						  case CHECKPASSRSP:

							  serialport->WritePort(sendBuffdata,sizeof(PROTOCOLCHECK));

							  break;
						  }
						}
					   break;
					case REQRETRYDATA:
						{
							packageErrorPacket(serialport,recivedata,RSPRETRYDATA,ErrorBuffdata);
						}
						break;
					}
				}
				break;
			case TRANSFERFILETRAN:
				{   
					switch(preadbuff->operatorType)
					{
					case UPLOAD:
						SendFileContent(serialport,recivedata,readCount);
						break;
					case DOWNLOAD:
						ReciveFileContent(serialport,recivedata,readCount);
						break;
					}	
				}
				break;
			}
		}
	}
}

void CCParseFile::ReciveFileContent(CPSerialPort *serialport,std::string filePath,char *reciveData,int reciveCount)
{
	  BASEPROTOCOL presult;

	  memset(presult.reserved,'\0',RESERVED);

	  memcpy((char * )&presult,reciveData,sizeof(BASEPROTOCOL));

	  switch(presult.cmdType)
	   {
	    case TRANSFERFILERSP:
		   {  

			   PROTOCOLTFILE protocolFile;

			   memset(protocolFile.FileStruct.FileName,'\0',FILENMAELEN);
			   
			   memset(protocolFile.FileStruct.FileContent,'\0',PACKETCONTENT);

			   memcpy((char *)&protocolFile,reciveData,sizeof(PROTOCOLTFILE));

			   packetTotalcount=protocolFile.FileStruct.packetcount;

			   memset(sendBuffdata,'\0',READWRITEBUFF);

			   PackageFilePacket((char *)&protocolFile.FileStruct,1,REQDATATRANSFER,REQDATATRANSFER,sendBuffdata,DOWNLOAD);

			   serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE));
		   }

		   break;

	   case RSPDATATRANSFER:
		{   
			PROTOCOLTFILE protocolFile;

			memset(protocolFile.FileStruct.FileName,'\0',FILENMAELEN);

			memset(protocolFile.FileStruct.FileContent,'\0',PACKETCONTENT);

			memcpy((char *)&protocolFile,reciveData,sizeof(PROTOCOLTFILE));

			memset(sendBuffdata,'\0',READWRITEBUFF);

			if (writefp==NULL)
			{
				writefp=fopen(filePath.c_str(),"wb+");

			}
			if (writefp==NULL)
			{
				return;
			}
			if (packetcount >= packetTotalcount)
			{   
				memset(sendBuffdata,'\0',READWRITEBUFF);

				PackageFilePacket((char *)&protocolFile.FileStruct,1,REQDATATRANSEND,REQDATATRANSEND,sendBuffdata,DOWNLOAD);

				serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE));

				packetcount=0;

				packetTotalcount=0;

				if (writefp!=NULL)
				{
					fclose(writefp);

					m_bIsFinishDownLoad=TRUE;

					writefp=NULL;

					if (protocolFile.FileStruct.FileType == CONTACTDB && m_bInsertUSB )
					{  
						PostMessage(theApp.m_pMainWnd->m_hWnd,WM_UPATADATA,0,0);
					}
				}

			}else{
                
				packetcount++;

				fwrite(protocolFile.FileStruct.FileContent,sizeof(char),protocolFile.FileStruct.FileSize,writefp);

				fflush(writefp);
			    
				PackageFilePacket((char *)&protocolFile.FileStruct,1,REQDATATRANSFER,REQDATATRANSFER,sendBuffdata,DOWNLOAD);

				serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE));
			}
		}
		break;
	case RSPDATATRANSEND:
		{
			PROTOCOLTFILE protocolFile;

			memset(protocolFile.FileStruct.FileName,'\0',FILENMAELEN);

			memset(protocolFile.FileStruct.FileContent,'\0',PACKETCONTENT);

			memcpy((char *)&protocolFile,reciveData,sizeof(PROTOCOLTFILE));

			memset(sendBuffdata,'\0',READWRITEBUFF);

			PackageFilePacket((char*)&protocolFile.FileStruct,1,REQDATATRANSEND,REQDATATRANSEND,sendBuffdata,DOWNLOAD);

			serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE));

			memset(sendBuffdata,'\0',READWRITEBUFF);

			if (writefp!=NULL)
			{
				fclose(writefp);

				m_bIsFinishDownLoad=TRUE;

				writefp=NULL;

				if (protocolFile.FileStruct.FileType == CONTACTDB && m_bInsertUSB )
					{  
						PostMessage(theApp.m_pMainWnd->m_hWnd,WM_UPATADATA,0,0);
					}

			}

			packetTotalcount=0;

			packetcount=0;
		}
	   break;
	case RSPRETRYDATA:
		{
			switch(presult.cmdType)
			{

			case TRANSFERFILERSP:
				{ 

					PROTOCOLTFILE protocolFile;

					memset(protocolFile.FileStruct.FileName,'\0',FILENMAELEN);

					memset(protocolFile.FileStruct.FileContent,'\0',PACKETCONTENT);

					memcpy((char *)&protocolFile,reciveData,sizeof(PROTOCOLTFILE));

					packetTotalcount=protocolFile.FileStruct.packetcount;

					memset(sendBuffdata,'\0',READWRITEBUFF);

					PackageFilePacket((char *)&protocolFile.FileStruct,1,REQDATATRANSFER,REQDATATRANSFER,sendBuffdata,DOWNLOAD);

					serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE));

				}

				break;

			case RSPDATATRANSFER:
				{   
					PROTOCOLTFILE protocolFile;

					memset(protocolFile.FileStruct.FileName,'\0',FILENMAELEN);

					memset(protocolFile.FileStruct.FileContent,'\0',PACKETCONTENT);

					memcpy((char *)&protocolFile,reciveData,sizeof(PROTOCOLTFILE));

					memset(sendBuffdata,'\0',READWRITEBUFF);

					if (writefp==NULL)
					{  
						writefp=fopen(filePath.c_str(),"wb+");

					}
					if (writefp==NULL)
					{
						return;
					}
					if (packetcount>=packetTotalcount)
					{   
						PackageFilePacket((char *)&protocolFile.FileStruct,1,REQDATATRANSEND,REQDATATRANSEND,sendBuffdata,DOWNLOAD);

						serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE));

						packetcount=0;

						packetTotalcount=0;

						if (writefp!=NULL)
						{
							fclose(writefp);

							m_bIsFinishDownLoad=TRUE;

							writefp=NULL;

							if (protocolFile.FileStruct.FileType == CONTACTDB && m_bInsertUSB )
							{  
					        	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_UPATADATA,0,0);
							}

						}

					}else{

						packetcount++;

						fwrite(protocolFile.FileStruct.FileContent,sizeof(char),protocolFile.FileStruct.FileSize,writefp);

						fflush(writefp);

						PackageFilePacket((char *)&protocolFile.FileStruct,1,REQDATATRANSFER,REQDATATRANSFER,sendBuffdata,DOWNLOAD);

						serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE));
					}
				}
				break;
			case RSPDATATRANSEND:
				{
					PROTOCOLTFILE protocolFile;

					memset(protocolFile.FileStruct.FileName,'\0',FILENMAELEN);

					memset(protocolFile.FileStruct.FileContent,'\0',PACKETCONTENT);

					memcpy((char *)&protocolFile,reciveData,sizeof(PROTOCOLTFILE));

					memset(sendBuffdata,'\0',READWRITEBUFF);

					PackageFilePacket((char*)&protocolFile.FileStruct,1,REQDATATRANSEND,REQDATATRANSEND,sendBuffdata,DOWNLOAD);

					serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE));

					memset(sendBuffdata,'\0',READWRITEBUFF);

					if (writefp!=NULL)
					{
						fclose(writefp);

						m_bIsFinishDownLoad=TRUE;

						writefp=NULL;

						if (protocolFile.FileStruct.FileType == CONTACTDB && m_bInsertUSB )
						{  

						  PostMessage(theApp.m_pMainWnd->m_hWnd,WM_UPATADATA,0,0);
						}

					}

					packetTotalcount=0;

					packetcount=0;

				}

				break;
		   
            }
		}
		break;
	case REQRETRYDATA:
		  packageErrorPacket(serialport,reciveData,RSPRETRYDATA,ErrorBuffdata);
		break;
	}	
}
void CCParseFile::ReciveFileContent(CPSerialPort *serialport,char *reciveData,int reciveCount)
{
  PTRPROTOCOLTRANSFILE protocolFile=(PTRPROTOCOLTRANSFILE)reciveData;

  std::string FileName=protocolFile->FileStruct.FileName;

  int pos=FileName.rfind('\\');

  std::string Filepath;

  Filepath=protocolFile->FileStruct.FileName;

  if (pos>0)
  {
	  FileName=FileName.substr(pos+1,FileName.length()-pos);
  }

  switch (protocolFile->FileStruct.FileType)
  {
   case CONTACTINFO:
	   Filepath=Util::StringOp::FromCString(BackupPath);
	   ReciveFileContent(serialport,Filepath,reciveData,reciveCount);
	   break;
   case RECORDFILE:
	   //change by qi 20110707
	   //Filepath=Util::StringOp::FromCString(csFlashRecordPath)+FileName;
	   Filepath=Util::StringOp::FromCString(csFlashRecordPath)+Filepath;
	   ReciveFileContent(serialport,Filepath,reciveData,reciveCount);
	   break;
   case MIDIFILELIST:
	   Filepath=Util::StringOp::FromCString(ssFlashPath)+FileName;
	   ReciveFileContent(serialport,Filepath,reciveData,reciveCount);
	   break;
   case MIDIFILE:
	   Filepath=Util::StringOp::FromCString(csFlashRingPath)+FileName;
	   ReciveFileContent(serialport,Filepath,reciveData,reciveCount);
	   break;
   case CONTACTDB:
	   m_bIsFinishDownLoad=FALSE;
	   Filepath=Util::StringOp::FromCString(BackupPath);
	   ReciveFileContent(serialport,Filepath,reciveData,reciveCount);
	   break;
   case UPDATEINFO:
	   ReciveFileContent(serialport,protocolFile->FileStruct.FileName,reciveData,reciveCount);
	   break;
  }

}
void CCParseFile::SendFileContent(CPSerialPort *serialport,char * reviceData,int readCount)
{   
	PTRPROTOCOLTRANSFILE protocolFile=(PTRPROTOCOLTRANSFILE)reviceData;

	switch(protocolFile->FileStruct.FileType)
	{
	case CONTACTINFO:
		SendFileContent(serialport,Util::StringOp::FromCString(ssFlashPath),reviceData,readCount);
		break;
	case RECORDFILE:
	    SendFileContent(serialport,Util::StringOp::FromCString(csFlashRecordPath),reviceData,readCount);
		break;
	case UPLOADSTOAGE:
		SendFileContent(serialport,Util::StringOp::FromCString(csStorageCardRecordPath),reviceData,readCount);
         break;
	case MIDIFILELIST:
		ProgramIsContinue=FALSE;
		CreatRingListFile(csFlashRingPath);
		while (!ProgramIsContinue)
		{
		}
		SendFileContent(serialport,Util::StringOp::FromCString(ssFlashPath),reviceData,readCount);
		break;
	case MIDIFILE:
		SendFileContent(serialport,Util::StringOp::FromCString(csFlashRingPath),reviceData,readCount);
		break;
	case CONTACTDB:
		SendFileContent(serialport,Util::StringOp::FromCString(ssFlashPath),reviceData,readCount);
		break;
	}
}

void CCParseFile::SendFileContent(CPSerialPort *serialport,std::string filename,char * recivedata,int readCount)
{      
	BASEPROTOCOL protocl;

	memset(protocl.reserved,'\0',RESERVED);

	memcpy((char *)&protocl,recivedata,sizeof(BASEPROTOCOL));

	switch (protocl.cmdType)
	{
	case TRANSFERFILEREQ:
		{
			PROTOCOLTFILE protocolFile;

			memset(protocolFile.FileStruct.FileName,'\0',FILENMAELEN);

			memset(protocolFile.FileStruct.FileContent,'\0',PACKETCONTENT);

			memcpy((char *)&protocolFile,recivedata,sizeof(PROTOCOLTFILE));

			std::string FileName=filename+protocolFile.FileStruct.FileName;

			protocolFile.FileStruct.packetcount=parsePacketCount((char *)FileName.c_str());

			protocolFile.FileStruct.FileSize=PACKETCONTENT;

			memset(sendBuffdata,'\0',READWRITEBUFF);

			PackageFilePacket((char *)&protocolFile.FileStruct,1,TRANSFERFILERSP,TRANSFERFILERSP,sendBuffdata,UPLOAD);

			serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE));
		}
		break;
	case REQDATATRANSFER:
		{   

			PROTOCOLTFILE protocolFile;

			memset(protocolFile.FileStruct.FileName,'\0',FILENMAELEN);

			memset(protocolFile.FileStruct.FileContent,'\0',PACKETCONTENT);

			memcpy((char *)&protocolFile,recivedata,sizeof(PROTOCOLTFILE));

			if (Readfp==NULL)
			{   
				std::string FileName=filename+protocolFile.FileStruct.FileName;

				Readfp=fopen(FileName.c_str(),"rb");

				if (Readfp!=NULL)
				{
					fseek(Readfp,0,SEEK_END);
					
					FileLength=ftell(Readfp);
					
			     	fseek(Readfp,0,SEEK_SET);
					
				}else{

					if (Readfp!=NULL)
					{
						fclose(Readfp);
						
						Readfp=NULL;
						
					}

					memset(protocolFile.FileStruct.FileName,'\0',FILENMAELEN);
					
					memset(protocolFile.FileStruct.FileContent,'\0',PACKETCONTENT);
					
					memcpy((char *)&protocolFile,recivedata,sizeof(PROTOCOLTFILE));
					
					memcpy(protocolFile.FileStruct.FileContent,readBuffdata,PACKETCONTENT);
					
					protocolFile.FileStruct.FileSize=readFileCount;
					
					PackageFilePacket((char *)&protocolFile.FileStruct,packectId,RSPDATATRANSEND,RSPDATATRANSEND,sendBuffdata,UPLOAD);
					
				    serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE)); 

					break;
				}
	
			}

			if (totalCount<=FileLength&&!feof(Readfp))
			{
				memset(sendBuffdata,'\0',READWRITEBUFF);

				memset(readBuffdata,'\0',PACKETCONTENT);

				ReadFileTopacket(Readfp,readBuffdata);

				totalCount=totalCount+readFileCount;

				packectId++;
				
				memcpy(protocolFile.FileStruct.FileContent,readBuffdata,readFileCount);

				protocolFile.FileStruct.FileSize=readFileCount;

				PackageFilePacket((char *)&protocolFile.FileStruct,packectId,RSPDATATRANSFER,RSPDATATRANSFER,sendBuffdata,UPLOAD);

				serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE));

			}else{   

				if (Readfp!=NULL)
				{
					fclose(Readfp);

					Readfp=NULL;

				}

				PROTOCOLTFILE protocolFile;

				memset(protocolFile.FileStruct.FileName,'\0',FILENMAELEN);

				memset(protocolFile.FileStruct.FileContent,'\0',PACKETCONTENT);

				memcpy((char *)&protocolFile,recivedata,sizeof(PROTOCOLTFILE));

				memcpy(protocolFile.FileStruct.FileContent,readBuffdata,PACKETCONTENT);

				protocolFile.FileStruct.FileSize=readFileCount;

				PackageFilePacket((char *)&protocolFile.FileStruct,packectId,RSPDATATRANSEND,RSPDATATRANSEND,sendBuffdata,UPLOAD);

				serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE));  
			}
		}

		break;

	case RSPRETRYDATA:
		{  

			PROTOCOLTFILE protocolFile;
			
			memset(protocolFile.FileStruct.FileName,'\0',FILENMAELEN);
			
			memset(protocolFile.FileStruct.FileContent,'\0',PACKETCONTENT);
			
			memcpy((char *)&protocolFile,recivedata,sizeof(PROTOCOLTFILE));

           switch(protocl.preCmdType)
		   {
		   case TRANSFERFILEREQ:
			   {
				   std::string FileName=filename+protocolFile.FileStruct.FileName;
				   
			       protocolFile.FileStruct.packetcount=parsePacketCount((char *)FileName.c_str());

				   memset(sendBuffdata,'\0',READWRITEBUFF);

				   PackageFilePacket((char *)&protocolFile.FileStruct,1,TRANSFERFILERSP,TRANSFERFILERSP,sendBuffdata,UPLOAD);

				   serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE));
			   }
			   break;
		   case REQDATATRANSFER:
			   {   
				   if (Readfp==NULL)
				   {   
					   std::string FileName=filename+protocolFile.FileStruct.FileName;

					   Readfp=fopen(FileName.c_str(),"rb");

					   if (Readfp!=NULL)
					   {
						   fseek(Readfp,0,SEEK_END);
						   
						   FileLength=ftell(Readfp);
						   
					       fseek(Readfp,0,SEEK_SET); 

					   }else{
                            
						   if (Readfp!=NULL)
						   {
							   fclose(Readfp);
							   
							   Readfp=NULL;
							   
						   }
						   
						   memset(protocolFile.FileStruct.FileName,'\0',FILENMAELEN);
						   
						   memset(protocolFile.FileStruct.FileContent,'\0',PACKETCONTENT);
						   
						   memcpy((char *)&protocolFile,recivedata,sizeof(PROTOCOLTFILE));
						   
						   memcpy(protocolFile.FileStruct.FileContent,readBuffdata,PACKETCONTENT);
						   
						   protocolFile.FileStruct.FileSize=readFileCount;
						   
						   PackageFilePacket((char *)&protocolFile.FileStruct,packectId,RSPDATATRANSEND,RSPDATATRANSEND,sendBuffdata,UPLOAD);
						   
				           serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE)); 

                           break;
					   }   
				   }
				
				   if (totalCount<=FileLength&&!feof(Readfp))
				   {
					   memset(sendBuffdata,'\0',READWRITEBUFF);

					   memset(readBuffdata,'\0',MAXBLOCK);

					   ReadFileTopacket(Readfp,readBuffdata);

					   totalCount=totalCount+readFileCount;

					   packectId++;

					   PROTOCOLTFILE protocolFile;

					   memset(protocolFile.FileStruct.FileName,'\0',FILENMAELEN);

					   memset(protocolFile.FileStruct.FileContent,'\0',PACKETCONTENT);

					   memcpy((char *)&protocolFile,recivedata,sizeof(PROTOCOLTFILE));

					   memcpy(protocolFile.FileStruct.FileContent,readBuffdata,readFileCount);

					   protocolFile.FileStruct.FileSize=readFileCount;

					   PackageFilePacket((char *)&protocolFile.FileStruct,packectId,RSPDATATRANSFER,RSPDATATRANSFER,sendBuffdata,UPLOAD);

					   serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE));

				   }else
				   {   
					   if (Readfp!=NULL)
					   {
						   fclose(Readfp);

						   Readfp=NULL;
					   }

					   PROTOCOLTFILE protocolFile;

					   memset(protocolFile.FileStruct.FileName,'\0',FILENMAELEN);

					   memset(protocolFile.FileStruct.FileContent,'\0',PACKETCONTENT);

					   memcpy((char *)&protocolFile,recivedata,sizeof(PROTOCOLTFILE));

					   memcpy(protocolFile.FileStruct.FileContent,'\0',PACKETCONTENT);

					   protocolFile.FileStruct.FileSize=PACKETCONTENT;

					   PackageFilePacket((char *)&protocolFile.FileStruct,packectId,RSPDATATRANSEND,RSPDATATRANSEND,sendBuffdata,UPLOAD);

					   serialport->WritePort(sendBuffdata,sizeof(PROTOCOLTFILE));  
				   }
			   } 
			   break;
		   case REQDATATRANSEND:

			   if (Readfp!=NULL)
			   {
				   fclose(Readfp);

				   Readfp=NULL;
			   }
			   serialport->ClearPort();

			   packectId=0;

			   totalCount=0;

			   readCount=0;

			   break;
		   }
		}
		break;
	case REQDATATRANSEND:
		if (Readfp!=NULL)
		{
			fclose(Readfp);

			Readfp=NULL;
		}
		serialport->ClearPort();
		packectId=0;
		totalCount=0;
		readCount=0;
		break;
	case REQRETRYDATA:
		{
		   packageErrorPacket(serialport,recivedata,RSPRETRYDATA,ErrorBuffdata);
		}
		break;
	}
}


void CCParseFile::PackageFilePacket(char *senddata,int packetid,int preCmdType,int cmdType,char *returndata,int operatorType)
{  

		PROTOCOLTFILE protocol;

		PFILESTRUT  filestruts;

		memset(filestruts.FileName,'\0',FILENMAELEN);

		memset(filestruts.FileContent,'\0',PACKETCONTENT);

		memcpy((char *)&filestruts,senddata,sizeof(PFILESTRUT));

		protocol.baseprotocl.magicNumber=4;

		protocol.baseprotocl.trasctTionID=TRANSFERFILETRAN;

		protocol.baseprotocl.preCmdType=preCmdType;

		protocol.baseprotocl.cmdType=cmdType;

		protocol.baseprotocl.packetID=packetid;

		protocol.baseprotocl.operatorType=operatorType;

		protocol.baseprotocl.packetLength=strlen(filestruts.FileContent);

		memset(protocol.baseprotocl.reserved,'\0',RESERVED);

		memset(protocol.FileStruct.FileName,'\0',FILENMAELEN);

		memset(protocol.FileStruct.FileContent,'\0',PACKETCONTENT);

		memcpy(protocol.FileStruct.FileName,filestruts.FileName,strlen(filestruts.FileName));

		memcpy(protocol.FileStruct.FileContent,filestruts.FileContent,filestruts.FileSize);

		protocol.FileStruct.FileSize=filestruts.FileSize;

		protocol.FileStruct.FileType=filestruts.FileType;

		protocol.FileStruct.packetcount=filestruts.packetcount;

		int packetLength=sizeof(PROTOCOLTFILE);

		protocol.baseprotocl.checksum=packetLength;

		memcpy(returndata,(char *)&protocol,sizeof(PROTOCOLTFILE));
 
}

void CCParseFile::PackagePasswordPacket(char *senddata,int packetid,int preCmdType,int cmdType,BOOL isRequest,char *returndata)
{   
	if (isRequest)
	{   
		PROTOCOLCHECK protocl;

		protocl.baseprotocl.magicNumber=4;

		protocl.baseprotocl.trasctTionID=CHECKPASSWORDTRAN;

		protocl.baseprotocl.preCmdType=preCmdType;

		protocl.baseprotocl.cmdType=cmdType;

		protocl.baseprotocl.packetID=packetid;

		protocl.baseprotocl.operatorType=UPLOAD;

		protocl.baseprotocl.packetLength=sizeof(PASSWOR);

		memset(protocl.password.userName,'\0',USERNAME);

		memset(protocl.password.userPassword,'\0',USERPASSWORD);

		memset(protocl.password.testResult,'\0',TESTRESULT);

		memset(protocl.baseprotocl.reserved,'\0',RESERVED);

		int packetLenth=sizeof(PROTOCOLCHECK);

		protocl.baseprotocl.checksum=packetLenth;

		memcpy(returndata,(char *)&protocl,sizeof(PROTOCOLCHECK));

	}else{

		PROTOCOLCHECK protocl;

		PASSWOR pwd;

		memset(pwd.userName,'\0',USERNAME);

		memset(pwd.userPassword,'\0',USERPASSWORD);

		memset(pwd.testResult,'\0',TESTRESULT);

		memcpy((char *)&pwd,senddata,sizeof(PASSWOR));

		protocl.baseprotocl.magicNumber=4;

		protocl.baseprotocl.trasctTionID=CHECKPASSWORDTRAN;

		protocl.baseprotocl.preCmdType=preCmdType;

		protocl.baseprotocl.cmdType=cmdType;

		protocl.baseprotocl.packetID=packetid;

		protocl.baseprotocl.operatorType=UPLOAD;

		protocl.baseprotocl.packetLength=sizeof(PASSWOR);

		memset(protocl.password.userName,'\0',USERNAME);

		memset(protocl.password.userPassword,'\0',USERPASSWORD);

		memset(protocl.password.testResult,'\0',TESTRESULT);

		memcpy(protocl.password.userName,pwd.userName,strlen(pwd.userName));

		memcpy(protocl.password.userPassword,pwd.userPassword,strlen(pwd.userPassword));

		memcpy(protocl.password.testResult,pwd.testResult,strlen(pwd.testResult));

		int packetLenth=sizeof(PROTOCOLCHECK);

		protocl.baseprotocl.checksum=packetLenth;

		memcpy(returndata,(char *)&protocl,sizeof(PROTOCOLCHECK));
	}
}

void CCParseFile::PackageAuthPacket(char *senddata,int packetid,int preCmdType,int cmdType,BOOL isRequest,char * returndata)
{  
	if (isRequest)
	{   
		PROTOCOLAU protocol;

		protocol.baseprotocl.magicNumber=4;

		protocol.baseprotocl.trasctTionID=AUTHENTICATIONTRAN;

		protocol.baseprotocl.preCmdType=preCmdType;

		protocol.baseprotocl.cmdType=cmdType;

		protocol.baseprotocl.packetID=packetid;

		protocol.baseprotocl.operatorType=UPLOAD;

		protocol.baseprotocl.packetLength=sizeof(PROTOCOLAU);

		memset(protocol.baseprotocl.reserved,'\0',RESERVED);

		memset(protocol.authenti.productType,'\0',PRODUCTTYPE);

		memset(protocol.authenti.productSertalNumber,'\0',PRODUCTSERIALNUMBER);

		memset(protocol.authenti.productTionTime,'\0',PRODUCTIONTIME);

		memset(protocol.authenti.softwaveVersion,'\0',SOFTWAREVERSION);

		memset(protocol.authenti.handwaveVersion,'\0',HARDWAREVERSION);

		memset(protocol.authenti.PasswordDauthenti,'\0',PASSWORDAUTHENTI);

		int packetLen=sizeof(PROTOCOLAU);

		protocol.baseprotocl.checksum=packetLen;

		memcpy(returndata,(char *)&protocol,sizeof(PROTOCOLAU));

	}else{

		PROTOCOLAU protocl;

		AUTHENTI authenti;

		memset(authenti.productType,'\0',PRODUCTTYPE);

		memset(authenti.productSertalNumber,'\0',PRODUCTSERIALNUMBER);

		memset(authenti.productTionTime,'\0',PRODUCTIONTIME);

		memset(authenti.softwaveVersion,'\0',SOFTWAREVERSION);

		memset(authenti.handwaveVersion,'\0',HARDWAREVERSION);

		memset(authenti.PasswordDauthenti,'\0',PASSWORDAUTHENTI);

		memcpy((char *)&authenti,senddata,sizeof(AUTHENTI));

		protocl.baseprotocl.magicNumber=4;

		protocl.baseprotocl.trasctTionID=AUTHENTICATIONTRAN;

		protocl.baseprotocl.preCmdType=preCmdType;

		protocl.baseprotocl.cmdType=cmdType;

		protocl.baseprotocl.packetID=packetid;

		protocl.baseprotocl.operatorType=UPLOAD;

		protocl.baseprotocl.packetLength=sizeof(AUTHENTI);

		memset(protocl.authenti.productType,'\0',PRODUCTTYPE);

		memset(protocl.authenti.productSertalNumber,'\0',PRODUCTSERIALNUMBER);

		memset(protocl.authenti.productTionTime,'\0',PRODUCTIONTIME);

		memset(protocl.authenti.softwaveVersion,'\0',SOFTWAREVERSION);

		memset(protocl.authenti.handwaveVersion,'\0',HARDWAREVERSION);

		memset(protocl.authenti.PasswordDauthenti,'\0',PASSWORDAUTHENTI);

		memcpy(protocl.authenti.productType,authenti.productType,strlen(authenti.productType));

		memcpy(protocl.authenti.productSertalNumber,authenti.productSertalNumber,strlen(authenti.productSertalNumber));

		memcpy(protocl.authenti.productTionTime,authenti.productTionTime,strlen(authenti.productTionTime));

		memcpy(protocl.authenti.softwaveVersion,authenti.softwaveVersion,strlen(authenti.softwaveVersion));

		memcpy(protocl.authenti.handwaveVersion,authenti.handwaveVersion,strlen(authenti.handwaveVersion));

		memcpy(protocl.authenti.PasswordDauthenti,authenti.PasswordDauthenti,strlen(authenti.PasswordDauthenti));

		int packetLen=sizeof(PROTOCOLAU);

		protocl.baseprotocl.checksum=packetLen;

		memcpy(returndata,(char *)&protocl,sizeof(PROTOCOLAU));
	}
}


int CCParseFile::parsePacketCount(char *FileName)
{
	FILE *fp=fopen(FileName,"rb");

	if (fp==NULL)
	{
		return -1;
	}
	fseek(fp,0,SEEK_END);

	int len=ftell(fp);

	fclose(fp);

	int packetcount=len/PACKETCONTENT;

	if (len%PACKETCONTENT!=0)
	{
		packetcount++;
	}

	return packetcount;
}
void CCParseFile::ReadFileTopacket(FILE * fp,char *senddata)
{
  if (fp==NULL)
  {
	  return;
  }

  fseek(fp,0,SEEK_CUR);
 
  readFileCount=fread(senddata,sizeof(char),PACKETCONTENT,fp);

}

BOOL CCParseFile::parsePacket(CPSerialPort *serialport,char * data,DWORD dwLength)
{
   BASEPROTOCOL presult;

   memcpy((char *)&presult,data,sizeof(BASEPROTOCOL));

   if (presult.checksum==(int)dwLength)
   {
      return TRUE;

   }else{
       
	   serialport->ClearPort();

	   packageErrorPacket(serialport,data,REQRETRYDATA,ErrorBuffdata);

	  return FALSE;
   }
}


void CCParseFile::packageErrorPacket(CPSerialPort *serialport,char *recivedata,int cmdType,char *senddata)
{
	BASEPROTOCOL presult;

	memcpy((char *)&presult,recivedata,sizeof(BASEPROTOCOL));

	memset(ErrorBuffdata,'\0',READWRITEBUFF);

	switch(presult.trasctTionID)
	{
	case AUTHENTICATIONTRAN:
		{  
			PackageAuthPacket(recivedata,1,presult.preCmdType,cmdType,TRUE,ErrorBuffdata);

			serialport->WritePort(ErrorBuffdata,sizeof(PROTOCOLAU));	   
		}
		break;
	case CHECKPASSWORDTRAN:
		{
			PackagePasswordPacket(recivedata,1,presult.preCmdType,cmdType,TRUE,ErrorBuffdata);
           
		    serialport->WritePort(ErrorBuffdata,sizeof(PROTOCOLCHECK));  
		}
		break;
	case TRANSFERFILETRAN:
		{  
			PROTOCOLTFILE protocol;
			
			memset(protocol.FileStruct.FileName,'\0',FILENMAELEN);
			
			memset(protocol.FileStruct.FileContent,'\0',PACKETCONTENT);
			
			memcpy((char *)&protocol,senddata,sizeof(PROTOCOLTFILE));

			switch(protocol.baseprotocl.operatorType)
			{
			case UPLOAD:
				 PackageFilePacket((char *)&protocol.FileStruct,presult.packetID,presult.preCmdType,cmdType,ErrorBuffdata,UPLOAD);
				 break;
			case DOWNLOAD:
				 PackageFilePacket((char *)&protocol.FileStruct,presult.packetID,presult.preCmdType,cmdType,ErrorBuffdata,DOWNLOAD);
				break;

			}
		    serialport->WritePort(ErrorBuffdata,sizeof(PROTOCOLTFILE));
		}
		break;

	}
}
BOOL CCParseFile::IsRoot(CString filePath)
{
    if (filePath.Right(1).CompareNoCase(_T("\\"))==0)
    {
		return TRUE;
    }
	return FALSE;
}

void CCParseFile::SerchRecordFile(CPSerialPort *serialport,CString filePath,char *data,int readCount)
{ 	
	CString FindPath=filePath;
	
	if (!IsRoot(FindPath)) 
	{
		FindPath+=_T("\\*.*");
		
	}else {
		
		FindPath+=_T("*.*");
	}
	
	WIN32_FIND_DATA finder; 
	
	HANDLE hFind = FindFirstFile(LPCTSTR(FindPath), &finder); 
	
	if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败 
	{
		return; 
	}
	do 
	{ 
		if (finder.cFileName[0] == '.') 
		{
			continue; // 过滤这两个目录 
		}
		if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{ 
			if(IsRoot(filePath))
			{
				filePath=filePath+CString(finder.cFileName);
			}else {
				filePath=filePath+_T("\\")+CString(finder.cFileName);
			}
			
			SerchRecordFile(serialport,filePath,data,readCount); // 如果找到的是目录，则进入此目录进行递归
			
		} else { 
			
			CString resultFile; 
			
			if(IsRoot(LPCTSTR(filePath)))
			{
				resultFile=filePath+CString(finder.cFileName);
			}else {
				
				resultFile=filePath+_T("\\")+CString(finder.cFileName);
			}
			
			if((resultFile.Right(4).CompareNoCase(_T(".spx")) == 0)||(resultFile.Right(4).CompareNoCase(_T(".spd")) == 0))
			{  
				SendFileContent(serialport,Util::StringOp::FromCString(resultFile),data,readCount);
			}
		} 
	} 
	while (FindNextFile(hFind, &finder)); 
	
	FindClose(hFind); // 关闭查找句柄 
}

void CCParseFile::SerchRingFile(CPSerialPort *serialport,CString filePath,char *data,int readCount)
{   
	CString FindPath=filePath;

	if (!IsRoot(FindPath)) 
	{
		FindPath+=_T("\\*.*");

	}else{
		
		FindPath+=_T("*.*");
	}

	WIN32_FIND_DATA finder; 

	HANDLE hFind = FindFirstFile(LPCTSTR(FindPath), &finder); 

	if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败 
	{
		return; 
	}
	do 
	{ 
		if (finder.cFileName[0] == '.') 
		{
			continue; // 过滤这两个目录 
		}
		if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{ 
			if(IsRoot(filePath))
			{
				filePath=filePath+CString(finder.cFileName);

			}else {

				filePath=filePath+_T("\\")+CString(finder.cFileName);
			}
			  SerchRingFile(serialport,filePath,data,readCount); // 如果找到的是目录，则进入此目录进行递归 
		} else { 

			CString resultFile; 

			if(IsRoot(LPCTSTR(filePath)))
			{
				resultFile=filePath+CString(finder.cFileName);

			}else {

				resultFile=filePath+_T("\\")+CString(finder.cFileName);
			}

			if((resultFile.Right(4).CompareNoCase(_T(".mp3")) == 0)||(resultFile.Right(4).CompareNoCase(_T(".wav")) == 0))
			{  
				SendFileContent(serialport,Util::StringOp::FromCString(resultFile),data,readCount);
			}
		} 
		
	} while (FindNextFile(hFind, &finder)); 
	
	FindClose(hFind); // 关闭查找句柄 
}


BOOL CCParseFile::CreatRingList(CString filePath,FILE *RecordFp)
{  	
	CString FindPath=filePath;
    
	if (!IsRoot(FindPath)) 
	{
		FindPath+=_T("\\*.*");
		
	}else{
		
		FindPath+=_T("*.*");
	}
	
    WIN32_FIND_DATA finder; 
	
	HANDLE hFind = FindFirstFile(LPCTSTR(FindPath), &finder); 
	
	if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败 
	{
		return FALSE; 
	}

	do 
	{ 
		if (finder.cFileName[0] == '.') 
		{
			continue; // 过滤这两个目录 
		}
		if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{ 
			if(IsRoot(filePath))
			{
				filePath=filePath+CString(finder.cFileName);
				
			}else {
				
				filePath=filePath+_T("\\")+CString(finder.cFileName);
			}

			CreatRingList(filePath,RecordFp); // 如果找到的是目录，则进入此目录进行递归 

		} else { 
			
			CString resultFile; 
			
			if(IsRoot(LPCTSTR(filePath)))
			{
				resultFile=filePath+CString(finder.cFileName);

			}else {
				
				resultFile=filePath+_T("\\")+CString(finder.cFileName);
			}
			
			if((resultFile.Right(4).CompareNoCase(_T(".mp3")) == 0)||(resultFile.Right(4).CompareNoCase(_T(".wav")) == 0))
			{  
			  
			 std::string fld=Util::StringOp::FromCString(resultFile);

			 fwrite(fld.c_str(),sizeof(char),resultFile.GetLength(),RecordFp); 
					
			 fflush(RecordFp);
			    	
			}
		} 
		
	} while (FindNextFile(hFind, &finder)); 
	
	FindClose(hFind); // 关闭查找句柄 

	return TRUE;

}

BOOL CCParseFile::CreatRingListFile()
{ 
	FILE  * RecordFp=NULL;

	if ( RecordFp == NULL)
	{
		RecordFp=fopen("\\hive\\tempList.txt","wb+");
	}

    for (int index =0;index < 3;index ++)
    {   
		switch(index)
		{
		case 0:
         CreatRingList(ssFlashPath,RecordFp);
		 break;
		case 1:
		 CreatRingList(csFlashRingPath,RecordFp);
		 break;
		case 2:
		 CreatRingList(csFlashRingPath,RecordFp);
		 break;
		}
    }

	if (RecordFp!=NULL)
	{
	  fclose(RecordFp);
	}
   
	return TRUE;
}

BOOL CCParseFile::CreatRingListFile(CString filePath)
{ 
	FILE *RecordFp=NULL;
  
	CString FindPath=filePath;

    std::string space="\r\n";

    char buff[100];

	memset(buff ,'\0',100);
    
	if (!IsRoot(FindPath)) 
	{
		FindPath+=_T("\\*.*");
		
	}else{
		
		FindPath+=_T("*.*");
	}
	
    WIN32_FIND_DATA finder; 
	
	HANDLE hFind = FindFirstFile(LPCTSTR(FindPath), &finder); 
	
	if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败 
	{  
		FindClose(hFind);
		ProgramIsContinue=TRUE;
		if(RecordFp !=NULL)
		{
			fclose(RecordFp);
		}
		
		return FALSE; 
	}
	
	do 
	{ 
		if (finder.cFileName[0] == '.') 
		{
			continue; // 过滤这两个目录 
		}
		if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{ 
			if(IsRoot(filePath))
			{
				filePath=filePath+CString(finder.cFileName);
				
			}else {
				
				filePath=filePath+_T("\\")+CString(finder.cFileName);
			}
			
			CreatRingList(filePath,RecordFp); // 如果找到的是目录，则进入此目录进行递归 
			
		} else { 
			
			CString resultFile; 
			
			if(IsRoot(LPCTSTR(filePath)))
			{
				resultFile=filePath+CString(finder.cFileName);
				
			}else {
				
				resultFile=filePath+_T("\\")+CString(finder.cFileName);
			}
			
			if((resultFile.Right(4).CompareNoCase(_T(".mp3")) == 0)||(resultFile.Right(4).CompareNoCase(_T(".wav")) == 0))
			{  
				
				if (RecordFp ==NULL)
				{   
					std::string Ringlist=Util::StringOp::FromCString(ssFlashPath)+"RingList.txt";

					RecordFp=fopen(Ringlist.c_str(),"wb+");
				}

				std::string fld=Util::StringOp::FromCString(resultFile);
				
				fwrite(fld.c_str(),sizeof(char),fld.length(),RecordFp); 

				fwrite(buff,sizeof(char),98-fld.length(),RecordFp);

				fwrite(space.c_str(),sizeof(char),space.length(),RecordFp);
				
				fflush(RecordFp);	
			}
		} 
		
	} while (FindNextFile(hFind, &finder)); 
	
	FindClose(hFind); // 关闭查找句柄 

	fclose(RecordFp);

   	ProgramIsContinue=TRUE;
	
	return TRUE;

}

void CCParseFile::resetData()
{  
	if (Readfp!=NULL)
	{
		fclose(Readfp);

		Readfp=NULL;
	}

	if (writefp!=NULL)
	{
		fclose(writefp);

		writefp=NULL;
	}

	packetTotalcount=0;

	packetcount=0;

	packectId=0;

	totalCount=0;
}

