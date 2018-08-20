#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "encrypt.h"

char *BKEY = "A quick brown fox jumps over the lazy dog.";
char CKEY[50];
char CKEY_[50];
char *PKEY = "Let's go to school!";
char *BKEY_M = "Z";
char *PKEY_M = "9";

static void DoFileXOR(FILE *fin,FILE *fout,char *mima){
    int i=0;
    char ch;
	char p;
   
    fread(&ch,1,1,fin);
    while(!feof(fin)){
        if(mima[i]==0)i=0;
		p = ch^mima[i];
		fwrite(&p,1,1,fout);
        i++;
        fread(&ch,1,1,fin);
    }
    fclose(fin);
    fclose(fout);
}   
static void DoMemXOR(char *inMem,char *outMem,char *mima){
	int i=0;
	int j=0;
	char ch;

	ch=inMem[j];
	while(inMem[j]){
		if(mima[i]==0)i=0;
		outMem[j] = ch^mima[i];
		i++;
		j++;
		ch=inMem[j];
	}
} 

int DoEncryptFile_(char *inFile, char* key,int trustMe)
{
	FILE *fout;
	int i = 0;
	int j = 0;
	char stringValue[32] = {0}; 
	
	if((fout=fopen(inFile,"wb"))==NULL){
		return -1;
	}
	
	memset(CKEY,0,50);
	DoMemXOR(BKEY,CKEY,PKEY);
	fwrite(CKEY,1,50,fout);
	memset(CKEY,0,50);
	if (!trustMe)
	{
		DoMemXOR(key,CKEY,PKEY);
		j = strlen(key);
		sprintf(stringValue,"%d",j);
		memset(CKEY_,0,50);
		DoMemXOR(stringValue,CKEY_,PKEY);
		fwrite(CKEY_,1,32,fout);
		fwrite(CKEY,1,j,fout);
	}
	else
	{
		j = 0;
		sprintf(stringValue,"%d",j);
		memset(CKEY_,0,50);
		DoMemXOR(stringValue,CKEY_,PKEY);
		fwrite(CKEY_,1,32,fout);
	}
	fclose(fout);
	return 0;
}

int DoEncryptFile(char *inFile, char *outFile,char* key,int trustMe)
{
	FILE *fin,*fout;
	int i = 0;
	int j = 0;
	char stringValue[32] = {0}; 

	if((fin=fopen(inFile,"rb"))==NULL){
		return -1;
	}
	fout=fopen(outFile,"wb");
	memset(CKEY,0,50);
	DoMemXOR(BKEY,CKEY,PKEY);
	fwrite(CKEY,1,50,fout);
	memset(CKEY,0,50);
	if (!trustMe)
	{
		DoMemXOR(key,CKEY,PKEY);
		j = strlen(key);
		sprintf(stringValue,"%d",j);
		memset(CKEY_,0,50);
		DoMemXOR(stringValue,CKEY_,PKEY);
		fwrite(CKEY_,1,32,fout);
		fwrite(CKEY,1,j,fout);
	}
	else
	{
		j = 0;
		sprintf(stringValue,"%d",j);
		memset(CKEY_,0,50);
		DoMemXOR(stringValue,CKEY_,PKEY);
		fwrite(CKEY_,1,32,fout);
	}
	DoFileXOR(fin,fout,PKEY);
	return 0;
}
int DoDecryptFile(char *inFile, char*outFile,char *key,int trustMe)
{
	FILE *fin,*fout;
	int i=0;
	int j=0;
	char stringValue[32] = {0}; 
	
	if((fin=fopen(inFile,"rb"))==NULL){
     #ifdef _DEBUG
     #endif
		return -1;
	}
	fread(CKEY_,1,50,fin);
	memset(CKEY,0,50);
	DoMemXOR(BKEY,CKEY,PKEY);
	if (strcmp(CKEY,CKEY_) != 0)
	{ 
#ifdef _DEBUG
		printf("error input file!\n");
#endif
		fclose(fin);
		return -1;
	}

	memset(CKEY_,0,50);
	memset(stringValue,0,32);
	fread(stringValue,1,32,fin);
	memset(CKEY_,0,50);
	DoMemXOR(stringValue,CKEY_,PKEY);
#ifdef _WIN32_WCE
	j = _wtoi(CKEY_);
#else
	j = atoi(CKEY_);
#endif

	if (!j)
	{
		fout=fopen(outFile,"wb");
		DoFileXOR(fin,fout,PKEY);
	}
	else
	{
		memset(CKEY_,0,50);
		fread(CKEY_,1,j,fin);
		memset(CKEY,0,50);
		DoMemXOR(key,CKEY,PKEY);
		if (!trustMe)
		{
			if (strcmp(CKEY,CKEY_) != 0)
			{ 
#ifdef _DEBUG
#endif
				fclose(fin);
				return -1;
			}
		}

		fout=fopen(outFile,"wb");
		DoFileXOR(fin,fout,PKEY);
		
	}
	return 0;
}

//lxz 20101230
EncryInfo DoDecryptFile_(char *inFile, char *key,int trustMe)
{
	EncryInfo info;
	FILE *fin;
	
	int i=0;
	int j=0;
	char stringValue[32] = {0}; 
	info.flag = -1;
	info.file = NULL;
	info.pKey = NULL;
	info.passwordlen = 0;
	if((fin=fopen(inFile,"rb"))==NULL){
#ifdef _DEBUG
#endif
		info.flag = -1;
		return info;
	}
	fread(CKEY_,1,50,fin);
	memset(CKEY,0,50);
	DoMemXOR(BKEY,CKEY,PKEY);
	if (strcmp(CKEY,CKEY_) != 0)
	{ 
#ifdef _DEBUG
		printf("error input file!\n");
#endif
	//	fclose(fin);

		info.flag = -1;
		info.file = inFile;
		info.pKey = PKEY;
		return info;
	}
	
	memset(CKEY_,0,50);
	memset(stringValue,0,32);
	fread(stringValue,1,32,fin);
	memset(CKEY_,0,50);
	DoMemXOR(stringValue,CKEY_,PKEY);
#ifdef _WIN32_WCE
	j = _wtoi(CKEY_);
#else
	j = atoi(CKEY_);
#endif
	
	if (!j)
	{
		info.file = fin;
		info.pKey = PKEY;
	}
	else
	{
		memset(CKEY_,0,50);
		fread(CKEY_,1,j,fin);
		memset(CKEY,0,50);
		DoMemXOR(key,CKEY,PKEY);
		info.passwordlen = j;
		if (!trustMe)
		{
			if (strcmp(CKEY,CKEY_) != 0)
			{ 
#ifdef _DEBUG
#endif
				fclose(fin);
				info.flag = -1;
				return info;
			}
		}
		
		info.file = fin;
		info.pKey = PKEY;
	}
	info.flag = 0;
	return info;
}

char * DoEncryptMem(char *inMem)
{
	size_t inPutLength;
	char *out;
	
	if(!inMem){
#ifdef _DEBUG
		printf("no input string.\n");
#endif
		return -1;
	}
	memset(CKEY,0,50);
	DoMemXOR(BKEY_M,CKEY,PKEY_M);
	inPutLength = strlen(inMem);
	out = malloc(inPutLength+1);
	if (!out)
	{  
#ifdef _DEBUG
		printf("can not malloc.\n");
#endif
		return -2;
	}	
	memset(out,0,inPutLength+1);
	DoMemXOR(inMem,out,CKEY);
	//DoMemXOR(inMem,out,"12");

	return out;
}
char * DoDecryptMem(char *inMem, char*outMem)
{
	size_t inPutLength;
	char *out;

	if(!inMem){
#ifdef _DEBUG
		printf("no input string.\n");
#endif
		return NULL;
	}
	memset(CKEY,0,50);
	DoMemXOR(BKEY_M,CKEY,PKEY_M);
	inPutLength = strlen(inMem);
	out = malloc(inPutLength+1);
	if (!out)
	{
#ifdef _DEBUG
		printf("can not malloc.\n");
#endif
		return NULL;
	}
	memset(out,0,inPutLength+1);
	DoMemXOR(inMem,out,CKEY);

	return out;	
}