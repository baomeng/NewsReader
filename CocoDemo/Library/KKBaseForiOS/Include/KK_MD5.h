//
//  KK_MD5.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef __KK_MD5_H__
#define __KK_MD5_H__

#include "KK_Includes.h"

///////////////////////////////////////////////////////////////
//  兼容64位
#if defined(__LP64__) && __LP64__
typedef unsigned int  md5_uint32;
#else
typedef unsigned long md5_uint32;
#endif

/* Structure to save state of computation between the single steps.  */
struct md5_ctx
{
    md5_uint32      A;
    md5_uint32      B;
    md5_uint32      C;
    md5_uint32      D;
    
    md5_uint32      total[2];
    md5_uint32      buflen;
    char            buffer[128];
};

/*
 * The following three functions are build up the low level used in
 * the functions `md5_stream' and `md5_buffer'.
 */

/* Initialize structure containing state of computation.
 (RFC 1321, 3.3: Step 3)  */
extern void     md5_init_ctx(struct md5_ctx * ctx);

/* Starting with the result of former calls of this function (or the
 initialization function update the context for the next LEN bytes
 starting at BUFFER.
 It is necessary that LEN is a multiple of 64!!! */
extern void     md5_process_block(const void *buffer, size_t len,
                                  struct md5_ctx * ctx);

/* Starting with the result of former calls of this function (or the
 initialization function update the context for the next LEN bytes
 starting at BUFFER.
 It is NOT required that LEN is a multiple of 64.  */
extern void     md5_process_bytes(const void *buffer, size_t len,
                                  struct md5_ctx * ctx);

/* Process the remaining bytes in the buffer and put result from CTX
 in first 16 bytes following RESBUF.  The result is always in little
 endian byte order, so that a byte-wise output yields to the wanted
 ASCII representation of the message digest.
 
 IMPORTANT: On some systems it is required that RESBUF is correctly
 aligned for a 32 bits value.  */
extern void    *md5_finish_ctx(struct md5_ctx * ctx, void *resbuf);


/* Put result from CTX in first 16 bytes following RESBUF.  The result is
 always in little endian byte order, so that a byte-wise output yields
 to the wanted ASCII representation of the message digest.
 
 IMPORTANT: On some systems it is required that RESBUF is correctly
 aligned for a 32 bits value.  */
extern void    *md5_read_ctx(const struct md5_ctx * ctx, void *resbuf);


/* Compute MD5 message digest for bytes read from STREAM.  The
 resulting message digest number will be written into the 16 bytes
 beginning at RESBLOCK.  */
extern int      md5_stream(FILE * stream, void *resblock);

/* Compute MD5 message digest for LEN bytes beginning at BUFFER.  The
 result is always in little endian byte order, so that a byte-wise
 output yields to the wanted ASCII representation of the message
 digest.  */
extern void    *md5_buffer(const char *buffer, size_t len, void *resblock);




extern void     md5_hex(unsigned char *src, char *dst);
extern bool     md5_bin(char *src, unsigned char *dst);
extern void     do_md5_file(FILE * src, long start, long bytes, char *hash);
extern void     do_md5_string(const char *pass, int passlen, char *hash);
extern void     do_md5_bin(const char *pass, int passlen, unsigned char *hash);


#endif // __KK_MD5_H__
