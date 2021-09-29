#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sha256.h"

uchar_t *pad_msg_256(const uchar_t * msg, uint64_t len, uint64_t * padlen) {
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

uint32_t *get_msg_sched_256(uchar_t * msg_blk) {
  uint32_t w, *msg_sched = calloc(1, sizeof(uint32_t) * 64);
  if (msg_sched) {
    for (int i = 0; i < 64; i += 4) {
      msg_sched[i / 4] = msg_blk[i + 3];
      msg_sched[i / 4] |= (msg_blk[i + 2] << 8);
      msg_sched[i / 4] |= (msg_blk[i + 1] << 16);
      msg_sched[i / 4] |= (msg_blk[i] << 24);
    }
    for (int i = 16; i < 64; ++i) {
      w = s1(msg_sched[i - 2]) + msg_sched[i - 7] + s0(msg_sched[i - 15]) + msg_sched[i - 16];
      msg_sched[i] = w;
    }
  }
  return msg_sched;
}

void sha256(const uchar_t * msg, uint32_t size, uchar_t * digest) {
  sha256_ctx_t c;
  sha256_init(&c);
  sha256_update(&c, msg, size);
  sha256_final(&c, digest);
}

void sha256_init(sha256_ctx_t * ctx) {
  memcpy(ctx->h, H_256, sizeof(H_256));
}

void sha256_final(sha256_ctx_t * ctx, uchar_t * digest) {
  for (int i = 0; i < SHA256_DIGEST_LEN; ++i)
    digest[i] = ((char *) &ctx->h[i / 4])[3 - i % 4];
}

void sha256_update(sha256_ctx_t * ctx, const uchar_t * msg, size_t len) {
  uint64_t nblks = 0;
  uint32_t a, b, c, d, e, f, g, h, t1, t2, *msg_sched;
  uchar_t *blks = pad_msg_256(msg, len, &nblks);

  if (blks) {
#if DEBUG
    printf("Initial Hash Value: \n");
    printf("H[0] = %x\n", ctx->h[0]);
    printf("H[1] = %x\n", ctx->h[1]);
    printf("H[2] = %x\n", ctx->h[2]);
    printf("H[3] = %x\n", ctx->h[3]);
    printf("H[4] = %x\n", ctx->h[4]);
    printf("H[5] = %x\n", ctx->h[5]);
    printf("H[6] = %x\n", ctx->h[6]);
    printf("H[7] = %x\n", ctx->h[7]);
    printf("\n");
#endif

    for (int i = 0; i < nblks; i += 64) {
      msg_sched = get_msg_sched_256(blks + i);

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
      f = ctx->h[5];
      g = ctx->h[6];
      h = ctx->h[7];

      for (int j = 0; j < 64; ++j) {
        t1 = h + S1(e) + ch(e, f, g) + K_256[j] + msg_sched[j];
        t2 = S0(a) + maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
#if DEBUG
        printf("t = %d: %x %x %x %x %x %x %x %x\n", j, a, b, c, d, e, f, g, h);
#endif
      }

      ctx->h[0] += a;
      ctx->h[1] += b;
      ctx->h[2] += c;
      ctx->h[3] += d;
      ctx->h[4] += e;
      ctx->h[5] += f;
      ctx->h[6] += g;
      ctx->h[7] += h;
      free(msg_sched);
    }

#if DEBUG
    printf("Final Hash Value: \n");
    printf("H[0] = %x\n", ctx->h[0]);
    printf("H[1] = %x\n", ctx->h[1]);
    printf("H[2] = %x\n", ctx->h[2]);
    printf("H[3] = %x\n", ctx->h[3]);
    printf("H[4] = %x\n", ctx->h[4]);
    printf("H[5] = %x\n", ctx->h[5]);
    printf("H[6] = %x\n", ctx->h[6]);
    printf("H[7] = %x\n", ctx->h[7]);
#endif

    free(blks);
  }
}
