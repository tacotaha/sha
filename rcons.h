#ifndef RCONS_H
#define RCONS_H 

#include <inttypes.h>

#define BLK_SIZE 512
#define DEBUG 0

typedef unsigned char uchar_t;
typedef uint32_t(*round_func_t) (uint32_t, uint32_t, uint32_t);

extern round_func_t round_funcs[4];

extern uint32_t K[4];
extern uint32_t H[5];

extern uint32_t K_256[64];
extern uint32_t H_256[8];

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

static inline uint32_t S0(uint32_t x) {
  return rotr(rotr(rotr(x, 9) ^ x, 11) ^ x, 2);
}

static inline uint32_t S1(uint32_t x) {
  return rotr(rotr(rotr(x, 14) ^ x, 5) ^ x, 6);
}

static inline uint32_t s0(uint32_t x) {
  return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

static inline uint32_t s1(uint32_t x) {
  return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}

#endif /* RCONS_H */
