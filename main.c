#include <stdio.h>
#include <string.h>

#include "sha.h"
#include "sha256.h"
#include "sha512.h"

int main() {
  const char *msg = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
  uchar_t sha1sum[SHA_DIGEST_LEN], sha256sum[SHA512_DIGEST_LEN], sha512sum[SHA512_DIGEST_LEN];

  sha_ctx_t c;
  sha_init(&c);
  sha_update(&c, (uchar_t *) msg, strlen(msg));
  sha_final(&c, sha1sum);

  printf("SHA Digest: ");
  for (int i = 0; i < SHA_DIGEST_LEN; ++i)
    printf("%02x ", sha1sum[i]);
  printf("\n");

  sha256_ctx_t c256;
  sha256_init(&c256);
  sha256_update(&c256, (uchar_t *) msg, strlen(msg));
  sha256_final(&c256, sha256sum);

  printf("SHA256 Digest: ");
  for (int i = 0; i < SHA256_DIGEST_LEN; ++i)
    printf("%02x ", sha256sum[i]);
  printf("\n");

  sha512_ctx_t c512;
  sha512_init(&c512);
  sha512_update(&c512, (uchar_t *) msg, strlen(msg));
  sha512_final(&c512, sha512sum);

  printf("SHA512 Digest: ");
  for (int i = 0; i < SHA512_DIGEST_LEN; ++i)
    printf("%02x ", sha512sum[i]);
  printf("\n");

  return 0;
}
