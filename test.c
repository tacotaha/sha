#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#include <openssl/sha.h>
#include "sha.h"

#define ITER 1000

int main() {
  srand(time(0));
  uchar_t *msg, d1[SHA_DIGEST_LEN], d2[SHA_DIGEST_LEN];

  for (int i = 0; i < ITER; ++i) {
    int len = rand() % (1 << 20);
    msg = malloc(len);

    for (int i = 0; i < len; ++i)
      msg[i] = rand(); 

    sha((uchar_t *) msg, len, d1);
    SHA1((uchar_t *) msg, len, d2);

    printf("d1: ");
    for (int i = 0; i < SHA_DIGEST_LEN; ++i)
      printf("%02x ", d1[i]);
    printf("\n");

    printf("d2: ");
    for (int i = 0; i < SHA_DIGEST_LEN; ++i)
      printf("%02x ", d2[i]);
    printf("\n");

    assert(memcmp(d1, d2, SHA_DIGEST_LEN) == 0);
    free(msg);
  }

  return 0;
}
