#include <stdio.h>
#include <string.h>

#include "sha.h"

int main() {
  uchar_t digest[SHA_DIGEST_LEN];
  const char *msg = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";

  sha_ctx_t c;
  sha_init(&c);
  sha_update(&c, (uchar_t *) msg, strlen(msg));
  sha_final(&c, digest);

  printf("Digest: ");
  for (int i = 0; i < SHA_DIGEST_LEN; ++i)
    printf("%02x ", digest[i]);
  printf("\n");

  return 0;
}
