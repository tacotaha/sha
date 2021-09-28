#ifndef SHA_H
#define SHA_H

#include <inttypes.h>

#define NROUNDS 80
#define BLK_SIZE 512
#define DEBUG 0
#define SHA_DIGEST_LEN 20

typedef struct sha_ctx_t {
  uint32_t h[5];
} sha_ctx_t;

typedef unsigned char uchar_t;
typedef uint32_t(*round_func_t) (uint32_t, uint32_t, uint32_t);

extern uint32_t K[4];
extern uint32_t H[5];
extern round_func_t round_funcs[4];

uchar_t *pad_msg(const uchar_t *, uint64_t, uint64_t *);
void sha_init(sha_ctx_t *);
void sha_update(sha_ctx_t *, const uchar_t *, size_t);
void sha_final(sha_ctx_t *, uchar_t *);
void sha(const uchar_t *, uint32_t, uchar_t *);


/* SHA Round functions */

static inline uint32_t ch(uint32_t x, uint32_t y, uint32_t z) {
  return (x & y) ^ (~x & z);
}

static inline uint32_t parity(uint32_t x, uint32_t y, uint32_t z) {
  return x ^ y ^ z;
}

static inline uint32_t maj(uint32_t x, uint32_t y, uint32_t z) {
  return (x & y) ^ (x & z) ^ (y & z);
}

static inline uint32_t rotr(uint32_t x, int n) {
  return (x >> n) | (x << ((sizeof(x) << 3) - n));
}

static inline uint32_t rotl(uint32_t x, int n) {
  uint32_t res = (x << n) | (x >> ((sizeof(x) << 3) - n));
  return res;
}

#endif                          /* SHA_H */
