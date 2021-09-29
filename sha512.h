#ifndef SHA512_H
#define SHA512_H

#include <inttypes.h>
#include "rcons.h"

#define SHA512_DIGEST_LEN 64

typedef unsigned __int128 uint128_t;

typedef struct sha512_ctx_t {
  uint64_t h[8];
} sha512_ctx_t;

uchar_t *pad_msg_512(const uchar_t *, uint128_t, uint128_t *);
void sha512_init(sha512_ctx_t *);
void sha512_update(sha512_ctx_t *, const uchar_t *, size_t);
void sha512_final(sha512_ctx_t *, uchar_t *);
void sha512(const uchar_t *, uint64_t, uchar_t *);

#endif                          /* SHA512_H */
