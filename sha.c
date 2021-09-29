#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sha.h"

uchar_t *pad_msg(const uchar_t * msg, uint64_t len, uint64_t * padlen) {
  uchar_t *res = NULL;
  uint64_t pad, blen = len << 3;

  pad = (blen >= 448) ? 512 - ((blen - 448) % 512) : 448 - blen;
  *padlen = (blen + pad + 65) >> 3;
  res = calloc(1, *padlen);

  if (res) {
    memcpy(res, msg, len);
    res[len] = 0x80;
    for (int i = 7; i >= 0; --i)
      res[*padlen - i - 1] = blen >> (8 * i);
  }

  return res;
}

uint32_t *get_msg_sched(uchar_t * msg_blk) {
  uint32_t w, *msg_sched = calloc(1, sizeof(uint32_t) * 80);
  if (msg_sched) {
    for (int i = 0; i < 64; i += 4) {
      msg_sched[i / 4] = msg_blk[i + 3];
      msg_sched[i / 4] |= (msg_blk[i + 2] << 8);
      msg_sched[i / 4] |= (msg_blk[i + 1] << 16);
      msg_sched[i / 4] |= (msg_blk[i] << 24);
    }
    for (int i = 16; i < 80; ++i) {
      w = msg_sched[i - 3] ^ msg_sched[i - 8] ^ msg_sched[i - 14] ^ msg_sched[i - 16];
      msg_sched[i] = rotl(w, 1);
    }
  }
  return msg_sched;
}

void sha(const uchar_t * msg, uint32_t size, uchar_t * digest) {
  sha_ctx_t c;
  sha_init(&c);
  sha_update(&c, msg, size);
  sha_final(&c, digest);
}

void sha_init(sha_ctx_t * ctx) {
  memcpy(ctx->h, H, sizeof(H));
}

void sha_final(sha_ctx_t * ctx, uchar_t * digest) {
  for (int i = 0; i < SHA_DIGEST_LEN; ++i)
    digest[i] = ((char *) &ctx->h[i / 4])[3 - i % 4];
}

void sha_update(sha_ctx_t * ctx, const uchar_t * msg, size_t len) {
  uint64_t nblks = 0;
  uint32_t a, b, c, d, e, t, *msg_sched;
  uchar_t *blks = pad_msg(msg, len, &nblks);

  if (blks) {
#if DEBUG
    printf("Initial Hash Value: \n");
    printf("H[0] = %x\n", ctx->h[0]);
    printf("H[1] = %x\n", ctx->h[1]);
    printf("H[2] = %x\n", ctx->h[2]);
    printf("H[3] = %x\n", ctx->h[3]);
    printf("H[4] = %x\n", ctx->h[4]);
    printf("\n");
#endif

    for (int i = 0; i < nblks; i += 64) {
      msg_sched = get_msg_sched(blks + i);

#if DEBUG
      printf("Block contents: \n");
      printf("W[0] = %x\n", msg_sched[0]);
      printf("W[1] = %x\n", msg_sched[1]);
      printf("W[2] = %x\n", msg_sched[2]);
      printf("W[3] = %x\n", msg_sched[3]);
      printf("W[4] = %x\n", msg_sched[4]);
      printf("W[5] = %x\n", msg_sched[5]);
      printf("W[6] = %x\n", msg_sched[6]);
      printf("W[7] = %x\n", msg_sched[7]);
      printf("W[8] = %x\n", msg_sched[8]);
      printf("W[9] = %x\n", msg_sched[9]);
      printf("W[10] = %x\n", msg_sched[10]);
      printf("W[11] = %x\n", msg_sched[11]);
      printf("W[12] = %x\n", msg_sched[12]);
      printf("W[13] = %x\n", msg_sched[13]);
      printf("W[14] = %x\n", msg_sched[14]);
      printf("W[15] = %x\n", msg_sched[15]);
#endif

      a = ctx->h[0];
      b = ctx->h[1];
      c = ctx->h[2];
      d = ctx->h[3];
      e = ctx->h[4];

      for (int j = 0; j < 80; ++j) {
        t = rotl(a, 5) + round_funcs[j / 20] (b, c, d) + e + K[j / 20] + msg_sched[j];
        e = d;
        d = c;
        c = rotl(b, 30);
        b = a;
        a = t;
#if DEBUG
        printf("t = %d: %x %x %x %x %x\n", j, a, b, c, d, e);
#endif
      }

      ctx->h[0] += a;
      ctx->h[1] += b;
      ctx->h[2] += c;
      ctx->h[3] += d;
      ctx->h[4] += e;

      free(msg_sched);
    }

#if DEBUG
    printf("Final Hash Value: \n");
    printf("H[0] = %x\n", ctx->h[0]);
    printf("H[1] = %x\n", ctx->h[1]);
    printf("H[2] = %x\n", ctx->h[2]);
    printf("H[3] = %x\n", ctx->h[3]);
    printf("H[4] = %x\n", ctx->h[4]);
#endif

    free(blks);
  }
}
