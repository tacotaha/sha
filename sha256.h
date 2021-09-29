#ifndef SHA256_H
#define SHA256_H

#include <inttypes.h>
#include "rcons.h"

#define SHA256_DIGEST_LEN 32

typedef struct sha256_ctx_t {
  uint32_t h[8];
} sha256_ctx_t;

uchar_t *pad_msg_256(const uchar_t *, uint64_t, uint64_t *);
void sha256_init(sha256_ctx_t *);
void sha256_update(sha256_ctx_t *, const uchar_t *, size_t);
void sha256_final(sha256_ctx_t *, uchar_t *);
void sha256(const uchar_t *, uint32_t, uchar_t *);

#endif                          /* SHA256_H */
