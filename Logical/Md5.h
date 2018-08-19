#ifndef __MD5_H__
#define __MD5_H__

extern "C"
{

typedef struct {

    unsigned int state[4];      

    unsigned int count[2];      

    unsigned char buffer[64];      

} MD5Context;


void MD5_Init(MD5Context * context);

void MD5_Update(MD5Context * context, unsigned char * buf, unsigned int len);

void MD5_Final(MD5Context * context, unsigned char digest[16]);

#define S11 7

#define S12 12

#define S13 17

#define S14 22

#define S21 5

#define S22 9

#define S23 14

#define S24 20

#define S31 4

#define S32 11

#define S33 16

#define S34 23

#define S41 6

#define S42 10

#define S43 15

#define S44 21


#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))

#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))

#define H(x, y, z) ((x) ^ (y) ^ (z))

#define I(x, y, z) ((y) ^ ((x) | (~z)))


#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))


#define FF(a, b, c, d, x, s, ac)          \
    {                       \
    (a) += F((b), (c), (d)) + (x) + (unsigned int)(ac);  \
    (a) = ROTATE_LEFT((a), (s));           \
    (a) += (b);                 \
    }


#define GG(a, b, c, d, x, s, ac)          \
    {                       \
    (a) += G((b), (c), (d)) + (x) + (unsigned int)(ac);  \
    (a) = ROTATE_LEFT((a), (s));           \
    (a) += (b);                 \
    }


#define HH(a, b, c, d, x, s, ac)          \
    {                       \
    (a) += H((b), (c), (d)) + (x) + (unsigned int)(ac);  \
    (a) = ROTATE_LEFT((a), (s));           \
    (a) += (b);                 \
    }


#define II(a, b, c, d, x, s, ac)          \
    {                       \
    (a) += I((b), (c), (d)) + (x) + (unsigned int)(ac);  \
    (a) = ROTATE_LEFT((a), (s));           \
    (a) += (b);                 \
    }

void MD5_Encode(unsigned char * output, unsigned int * input, int len);


void MD5_Decode(unsigned int * output, unsigned char * input, int len);


void MD5_Transform(unsigned int state[4], unsigned char block[64]);


void MD5_Init(MD5Context * context);


void MD5_Update(MD5Context * context, unsigned char * buf, unsigned int len);


void MD5_Final(MD5Context * context, unsigned char digest[16]);


bool MD5_File(LPCTSTR filename, unsigned char buff[16]);


void MD5_Data(unsigned char * data, unsigned int dataLen, unsigned char md5[16]);
}

#endif

