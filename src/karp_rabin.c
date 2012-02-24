#include <string.h>

#include <list.h>

#include "search.h"



/*
 * This file contains functions based on karp-rabin algorythm
 * -- karp-rabin (srch_kr*)
 */



/* karp-rabin algorithm */

/*
 * rehash for karp-rabin search function
 */
static inline int rehash (	unsigned char first, unsigned char new,
				unsigned hash, unsigned shift)
{
	return (((hash - first*shift) << 1) + new);
}


/*
 * karp-rabin algorithm searching in buffer
 */
int srch_kr_buf (char *ptr, unsigned lp, char *txt, unsigned lt)
{
	unsigned i, shift, diff, txt_hash;
	int ptr_hash;

	if (lp > 31) {
		fprintf(stderr,
			"length of search pattern should be max 31 chars");
		srch_ret(SRCH_KR_TOO_LEN, -1);
	}

	if (lp > lt) srch_ret(SRCH_OK, -1);

	shift = 1 << (lp-1);

	for (ptr_hash=0, txt_hash=0, i=0; i<lp && i<lt; i++) {
		ptr_hash = (ptr_hash<<1) + ptr[i];
		txt_hash = (txt_hash<<1) + txt[i];
	}

	diff = lt-lp;
	i = 0;
	while (i<diff) {
		if (ptr_hash == txt_hash && memcmp(ptr, txt+i, lp) == 0)
			srch_ret(SRCH_OK, i);
		txt_hash = rehash(txt[i], txt[i+lp], txt_hash, shift);
		i++;
	}

	srch_ret(SRCH_OK, -1);
}


/*
 * karp-rabin algorithm searching in buffer
 */
void srch_kr_buf_all (char *ptr, unsigned lp, char *txt, unsigned lt)
{
	unsigned i, diff;
	int shift, ptr_hash, txt_hash;

	if (lp > 31) {
		fprintf(stderr,
			"length of search pattern should be only 31 chars");
		return;
	}

	if (lp > lt) return;

	shift = 1 << (lp-1);

	for (ptr_hash=0, txt_hash=0, i=0; i<lp && i<lt; i++) {
		ptr_hash = (ptr_hash<<1) + ptr[i];
		txt_hash = (txt_hash<<1) + txt[i];
	}

	diff = lt-lp;
	i = 0;
	while (i<diff) {
		if (ptr_hash == txt_hash && memcmp(ptr, txt+i, lp) == 0)
			printf("%u\n", i);
		txt_hash = rehash(txt[i], txt[i+lp], txt_hash, shift);
		i++;
	}
}

