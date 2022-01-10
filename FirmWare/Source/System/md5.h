
#ifndef __MD5_H
#define __MD5_H

//*************************************************************
#ifndef UINT4
typedef unsigned long int UINT4;
#endif

//*************************************************************

typedef struct {
    UINT4 i[2];                   // Number of _bits_ handled mod 2^64
    UINT4 buf[4];                 // Scratch buffer
    unsigned char in[64];         // Input buffer
    unsigned char digest[16];     // Actual digest after MD5Final call
 } MD5_CTX;

//*************************************************************
void MD5Init( MD5_CTX *mdContext, unsigned long pseudoRandomNumber );
void MD5Update( MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen );
void MD5Final( MD5_CTX *mdContext );

#endif
