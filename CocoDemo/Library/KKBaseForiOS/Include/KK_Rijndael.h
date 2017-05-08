//
//  KK_Rijndael.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//



#ifndef _RIJNDAEL_H_
#define _RIJNDAEL_H_

#include "KK_Includes.h"

//
// File : rijndael.h
// Creation date : Sun Nov 5 2000 03:21:05 CEST
// Author : Szymon Stefanek (stefanek@tin.it)
//
// Another implementation of the KK_Rijndael cipher.
// This is intended to be an easily usable library file.
// This code is public domain.
// Based on the Vincent Rijmen and K.U.Leuven implementation 2.4.
//

//
// Original Copyright notice:
//
//    rijndael-alg-fst.c   v2.4   April '2000
//    rijndael-alg-fst.h
//    rijndael-api-fst.c
//    rijndael-api-fst.h
//
//    Optimised ANSI C code
//
//    authors: v1.0: Antoon Bosselaers
//             v2.0: Vincent Rijmen, K.U.Leuven
//             v2.3: Paulo Barreto
//             v2.4: Vincent Rijmen, K.U.Leuven
//
//    This code is placed in the public domain.
//

//
// This implementation works on 128 , 192 , 256 bit keys
// and on 128 bit blocks
//

//
// Example of usage:
//
//  // Input data
//  unsigned char key[32];                       // The key
//  initializeYour256BitKey();                   // Obviously initialized with sth
//  const unsigned char * plainText = getYourPlainText(); // Your plain text
//  int plainTextLen = strlen(plainText);        // Plain text length
//
//  // Encrypting
//  KK_Rijndael rin;
//  unsigned char output[plainTextLen + 16];
//
//  rin.init(KK_Rijndael::CBC,KK_Rijndael::Encrypt,key,KK_Rijndael::Key32Bytes);
//  // It is a good idea to check the error code
//  int len = rin.padEncrypt(plainText,len,output);
//  if(len >= 0)useYourEncryptedText();
//  else encryptError(len);
//
//  // Decrypting: we can reuse the same object
//  unsigned char output2[len];
//  rin.init(KK_Rijndael::CBC,KK_Rijndael::Decrypt,key,KK_Rijndael::Key32Bytes));
//  len = rin.padDecrypt(output,len,output2);
//  if(len >= 0)useYourDecryptedText();
//  else decryptError(len);
//

#define RIJNDAEL_MAX_KEY_COLUMNS (256/32)
#define RIJNDAEL_MAX_ROUNDS      14
#define RIJNDAEL_MAX_IV_SIZE      16

// We assume that unsigned int is 32 bits long.... 
typedef unsigned char  UINT8;
typedef unsigned int   UINT32;
typedef unsigned short UINT16;

// Error codes
#define RIJNDAEL_SUCCESS 0
#define RIJNDAEL_UNSUPPORTED_MODE -1
#define RIJNDAEL_UNSUPPORTED_DIRECTION -2
#define RIJNDAEL_UNSUPPORTED_KEY_LENGTH -3
#define RIJNDAEL_BAD_KEY -4
#define RIJNDAEL_NOT_INITIALIZED -5
#define RIJNDAEL_BAD_DIRECTION -6
#define RIJNDAEL_CORRUPTED_DATA -7

// 注：DES 算法将消息块置换成密码块。如果每一块都单独加密，那么加密方式叫做电子代码书 (Electronic Code Book) (ECB) 方式。
//     DES 加密有另外两种方式，分别叫链块编码 (Chain Block Coding) (CBC) 和密码反馈 (Cipher Feedback) (CFB)，
//     它们使每个密码块都依赖于初始 XOR 操作中所有以前的消息块。由于政府／银行业都使用这三种方式，所以需要 AES 如何处理信息方面的兼容性。 

class KK_Rijndael
{	
public:
	enum Direction { Encrypt, Decrypt };
	enum Mode { ECB, CBC, CFB1 };
	enum KeyLength { Key16Bytes, Key24Bytes, Key32Bytes };
	//
	// Creates a KK_Rijndael cipher object
	// You have to call init() before you can encrypt or decrypt stuff
	//
	KK_Rijndael();
	~KK_Rijndael();
protected:
	// Internal stuff
	enum State { Valid, Invalid };
    
public:
	//////////////////////////////////////////////////////////////////////////////////////////
	// API
	//////////////////////////////////////////////////////////////////////////////////////////
    
	// init(): Initializes the crypt session
	// Returns RIJNDAEL_SUCCESS or an error code
	// mode      : KK_Rijndael::ECB, KK_Rijndael::CBC or KK_Rijndael::CFB1
	//             You have to use the same mode for encrypting and decrypting
	// dir       : KK_Rijndael::Encrypt or KK_Rijndael::Decrypt
	//             A cipher instance works only in one direction
	//             (Well , it could be easily modified to work in both
	//             directions with a single init() call, but it looks
	//             useless to me...anyway , it is a matter of generating
	//             two expanded keys)
	// key       : array of unsigned octets , it can be 16 , 24 or 32 bytes long
	//             this CAN be binary data (it is not expected to be null terminated)
	// keyLen    : KK_Rijndael::Key16Bytes , KK_Rijndael::Key24Bytes or KK_Rijndael::Key32Bytes
	// initVector: initialization vector, you will usually use 0 here
	int init(Mode mode, Direction dir, const UINT8* key, KeyLength keyLen, UINT8* initVector = 0);
	// Encrypts the input array (can be binary data)
	// The input array length must be a multiple of 16 bytes, the remaining part
	// is DISCARDED.
	// so it actually encrypts inputLen / 128 blocks of input and puts it in outBuffer
	// Input len is in BITS!
	// outBuffer must be at least inputLen / 8 bytes long.
	// Returns the encrypted buffer length in BITS or an error code < 0 in case of error
	int blockEncrypt(const UINT8* input, int inputLen, UINT8* outBuffer);
	// Encrypts the input array (can be binary data)
	// The input array can be any length , it is automatically padded on a 16 byte boundary.
	// Input len is in BYTES!
	// outBuffer must be at least (inputLen + 16) bytes long
	// Returns the encrypted buffer length in BYTES or an error code < 0 in case of error
	int padEncrypt(const UINT8* input, int inputOctets, UINT8* outBuffer);
	// Decrypts the input vector
	// Input len is in BITS!
	// outBuffer must be at least inputLen / 8 bytes long
	// Returns the decrypted buffer length in BITS and an error code < 0 in case of error
	int blockDecrypt(const UINT8* input, int inputLen, UINT8* outBuffer);
	// Decrypts the input vector
	// Input len is in BYTES!
	// outBuffer must be at least inputLen bytes long
	// Returns the decrypted buffer length in BYTES and an error code < 0 in case of error
	int padDecrypt(const UINT8* input, int inputOctets, UINT8* outBuffer);
    
protected:
	void keySched(UINT8 key[RIJNDAEL_MAX_KEY_COLUMNS][4]);
	void keyEncToDec();
	void encrypt(const UINT8 a[16], UINT8 b[16]);
	void decrypt(const UINT8 a[16], UINT8 b[16]);
    
protected:	
	State     m_state;
	Mode      m_mode;
	Direction m_direction;
	UINT8     m_initVector[RIJNDAEL_MAX_IV_SIZE];
	UINT32    m_uRounds;
	UINT8     m_expandedKey[RIJNDAEL_MAX_ROUNDS+1][4][4];
};

#endif // _RIJNDAEL_H_
