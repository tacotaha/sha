#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sha512.h"

uchar_t *pad_msg_512(const uchar_t * msg, uint128_t len, uint128_t * padlen) {
  uchar_t *res = NULL;
  uint128_t pad, blen = (len << 3);
  pad = (blen >= 896) ? 1024 - ((blen - 896) % 1024) : 896 - blen;
  *padlen = (blen + pad + 129) >> 3;
  res = calloc(1, *padlen);
  if (res) {
    memcpy(res, msg, len);
    res[len] = 0x80;
    for (int i = 15; i >= 0; --i)
      res[*padlen - i - 1] = (uint64_t)(blen >> (8 * i));
  }
  return res;
}

uint64_t *get_msg_sched_512(uchar_t * msg_blk) {
  uint64_t w, *msg_sched = calloc(1, sizeof(uint64_t) * 80);
  if (msg_sched) {
    for (int i = 0; i < 128; i += 8)
      for(int j = 7; j >= 0; --j)
        msg_sched[i >> 3] |= (((uint64_t)msg_blk[i + j]) << ((7 - j) << 3));
    for (int i = 16; i < 80; ++i) {
      w = s164(msg_sched[i - 2]) + msg_sched[i - 7] + s064(msg_sched[i - 15]) + msg_sched[i - 16];
      msg_sched[i] = w;
    }
  }
  return msg_sched;
}

void sha512(const uchar_t * msg, uint64_t size, uchar_t * digest) {
  sha512_ctx_t c;
  sha512_init(&c);
  sha512_update(&c, msg, size);
  sha512_final(&c, digest);
}

void sha512_init(sha512_ctx_t * ctx) {
  memcpy(ctx->h, H_512, sizeof(H_512));
}

void sha512_final(sha512_ctx_t * ctx, uchar_t * digest) {
  for (int i = 0; i < SHA512_DIGEST_LEN; ++i)
    digest[i] = ((char *) &ctx->h[i / 8])[7 - i % 8];
}

void sha512_update(sha512_ctx_t * ctx, const uchar_t * msg, size_t len) {
  uint128_t nblks = 0;
  uint64_t a, b, c, d, e, f, g, h, t1, t2, *msg_sched;
  uchar_t *blks = pad_msg_512(msg, len, &nblks);

  if (blks) {
#if DEBUG
    printf("Initial Hash Value: \n");
    printf("H[0] = %lx\n", ctx->h[0]);
    printf("H[1] = %lx\n", ctx->h[1]);
    printf("H[2] = %lx\n", ctx->h[2]);
    printf("H[3] = %lx\n", ctx->h[3]);
    printf("H[4] = %lx\n", ctx->h[4]);
    printf("H[5] = %lx\n", ctx->h[5]);
    printf("H[6] = %lx\n", ctx->h[6]);
    printf("H[7] = %lx\n", ctx->h[7]);
    printf("\n");
#endif

    for (int i = 0; i < nblks; i += 128) {
      msg_sched = get_msg_sched_512(blks + i);

#if DEBUG
      printf("Block contents: \n");
      printf("W[0] = %lx\n", msg_sched[0]);
      printf("W[1] = %lx\n", msg_sched[1]);
      printf("W[2] = %lx\n", msg_sched[2]);
      printf("W[3] = %lx\n", msg_sched[3]);
      printf("W[4] = %lx\n", msg_sched[4]);
      printf("W[5] = %lx\n", msg_sched[5]);
      printf("W[6] = %lx\n", msg_sched[6]);
      printf("W[7] = %lx\n", msg_sched[7]);
      printf("W[8] = %lx\n", msg_sched[8]);
      printf("W[9] = %lx\n", msg_sched[9]);
      printf("W[10] = %lx\n", msg_sched[10]);
      printf("W[11] = %lx\n", msg_sched[11]);
      printf("W[12] = %lx\n", msg_sched[12]);
      printf("W[13] = %lx\n", msg_sched[13]);
      printf("W[14] = %lx\n", msg_sched[14]);
      printf("W[15] = %lx\n", msg_sched[15]);
#endif

      a = ctx->h[0];
      b = ctx->h[1];
      c = ctx->h[2];
      d = ctx->h[3];
      e = ctx->h[4];
      f = ctx->h[5];
      g = ctx->h[6];
      h = ctx->h[7];

      for (int j = 0; j < 80; ++j) {
        t1 = h + S164(e) + ch64(e, f, g) + K_512[j] + msg_sched[j];
        t2 = S064(a) + maj64(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
#if DEBUG
        printf("t = %d: %lx %lx %lx %lx %lx %lx %lx %lx\n", j, a, b, c, d, e, f, g, h);
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
    printf("H[0] = %lx\n", ctx->h[0]);
    printf("H[1] = %lx\n", ctx->h[1]);
    printf("H[2] = %lx\n", ctx->h[2]);
    printf("H[3] = %lx\n", ctx->h[3]);
    printf("H[4] = %lx\n", ctx->h[4]);
    printf("H[5] = %lx\n", ctx->h[5]);
    printf("H[6] = %lx\n", ctx->h[6]);
    printf("H[7] = %lx\n", ctx->h[7]);
#endif

    free(blks);
  }
}
