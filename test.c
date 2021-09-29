#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include <openssl/sha.h>
#include <pthread.h>

#include "sha.h"

#define ITER 1000
#define NTHREADS 10

typedef struct hr_args_t {
  uchar_t *msg;
  uint32_t len;
  uint64_t count;
  int stop;
} hr_args_t;

int get_rand_bytes(uchar_t * buff, int size) {
  int bread = 0;
  FILE *fp = fopen("/dev/random", "rb");
  if (fp) {
    bread = fread(buff, 1, size, fp);
    fclose(fp);
  }
  return bread;
}

void *run_hr(void *args) {
  hr_args_t *h = (hr_args_t *) args;
  uchar_t digest[SHA_DIGEST_LEN];
  while (!h->stop && ++h->count)
    sha(h->msg, h->len, digest);
  return NULL;
}

int test_hash_rate(uint64_t msg_size, uint64_t nhashes) {
  pthread_t tids[NTHREADS];
  uchar_t *msg;
  clock_t start, end;
  double duration;

  if (!(msg = malloc(msg_size)))
    return -1;

  get_rand_bytes(msg, msg_size);
  hr_args_t hr_args = {.msg = msg,.len = msg_size,.count = 0,.stop = 0 };

  for (int i = 0; i < NTHREADS; ++i)
    pthread_create(tids + i, NULL, run_hr, &hr_args);

  printf("Calculating hash rate...\n");

  start = clock();
  while (hr_args.count < nhashes)
    sleep(1);
  end = clock();

  hr_args.stop = 1;

  for (int i = 0; i < NTHREADS; ++i)
    pthread_join(tids[i], NULL);

  duration = (double) (end - start) / CLOCKS_PER_SEC;
  printf("hashrate: %lu hashes in %.2lf sec (%lf hashes/sec)\n",
         hr_args.count, duration, hr_args.count / duration);

  free(msg);
  return hr_args.count;
}

int main() {
  int len;
  uchar_t *msg, d1[SHA_DIGEST_LEN], d2[SHA_DIGEST_LEN];

  srand(time(0));

  for (int i = 0; i < ITER; ++i) {
    len = rand() % (1 << 20);
    if (!(msg = malloc(len)))
      return 1;

    get_rand_bytes(msg, len);
    sha(msg, len, d1);
    SHA1(msg, len, d2);

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

  test_hash_rate(1 << 10, 1 << 20);

  return 0;
}
