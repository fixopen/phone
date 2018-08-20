#ifndef _encrypt_h_
#define _encrypt_h_

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	FILE *file;
	char *pKey;
	int flag;
	int passwordlen;
}EncryInfo;

int DoEncryptFile(char *inFile, char*outFile,char* key,int trustMe);
int DoDecryptFile(char *inFile, char*outFile,char *key,int trustMe);

//lxz 20101230
EncryInfo DoDecryptFile_(char *inFile, char *key, int trustMe);
int DoEncryptFile_(char *inFile, char* key,int trustMe);

char * DoEncryptMem(char *inMem);
char * DoDecryptMem(char *inMem);

#ifdef __cplusplus
}
#endif

#endif