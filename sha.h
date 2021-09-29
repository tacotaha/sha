#ifndef SHA_H
#define SHA_H

#include <inttypes.h>

#include "rcons.h"

#define SHA_DIGEST_LEN 20

typedef struct sha_ctx_t {
  uint32_t h[5];
} sha_ctx_t;


uint32_t *get_msg_sched(uchar_t *);

void sha_init(sha_ctx_t *);
void sha_update(sha_ctx_t *, const uchar_t *, size_t);
void sha_final(sha_ctx_t *, uchar_t *);
void sha(const uchar_t *, uint32_t, uchar_t *);

#endif                          /* SHA_H */
