#include <string.h>

#include <list.h>

#include "search.h"



/*
 * This file contains algorithms based on boyer-moore:
 * -- horspool-boyer-moore (srch_hbm*)
 * -- quick-boyer-moore (srch_quick*)
 * -- smith-horspool-boyer-moore (srch_smith*)
 */



/* horspool-boyer-moore algorithm */

/*
 * prebuild function for horspool-boyer-moore algorithm
 * hbm must be lenght enough
 */
static void pre_hbm (char *ptr, unsigned lp, unsigned *hbm)
{
	unsigned i;

	for (i=0; i<256; i++)
		hbm[i] = lp;
	for (i=0; i < lp-1; i++)
		hbm[(unsigned)*(ptr+i)] = lp-i-1;
}


/*
 * searching in buffer: searching for first occurence of ptr in txt
 */
int srch_hbm_buf (char *ptr, unsigned lp, char *txt, unsigned lt)
{
	unsigned i=0, hbm[256];
	char c;

	pre_hbm(ptr, lp, hbm);

	while (i <= lt-lp) {
		c = txt[i+lp-1];
		if (ptr[lp-1] == c && memcmp(ptr, txt+i, lp-1) == 0)
			srch_ret(SRCH_OK, i);
		i += hbm[(unsigned)c];
	}

	srch_ret(SRCH_OK, -1);
}


/*
 * searching in buffer: searching for all occurence of ptr in txt
 */
void srch_hbm_buf_all (char *ptr, unsigned lp, char *txt, unsigned lt)
{
	unsigned i=0, hbm[256];
	char c;

	pre_hbm(ptr, lp, hbm);

	while (i <= lt-lp) {
		c = txt[i+lp-1];
		if (ptr[lp-1] == c && memcmp(ptr, txt+i, lp-1) == 0)
			printf("%u\n", i);
		i += hbm[(unsigned)c];
	}
}



/*
 * prebuild function for quick algorithm (based on pre_hbm)
 * quick must be lenght enough
 */
void pre_quick (char *ptr, unsigned lp, unsigned *quick)
{
	unsigned i;

	for (i=0; i<256; i++)
		quick[i] = lp+1;
	for (i=0; i < lp; i++)
		quick[(unsigned)*(ptr+i)] = lp-i;
}


/*
 * quick search algorithm searching in buffer (based on boyer-moore)
 */
void srch_quick_buf (char *ptr, unsigned lp, char *txt, unsigned lt)
{
	unsigned i=0, quick[256];

	pre_quick(ptr, lp, quick);

	while (i <= lt-lp) {
		if (memcmp(ptr, txt+i, lp) == 0)
			printf("%u\n", i);
		i += quick[(unsigned)txt[i+lp]];
	}
}



/*
 * smith search algorithm searching in buffer (based on boyer-moore and quick)
 */
void srch_smith_buf(char *ptr, unsigned lp, char *txt, unsigned lt)
{
	unsigned i=0, quick[256], hbm[256];

	pre_hbm(ptr, lp, hbm);
	pre_quick(ptr, lp, quick);

	while (i <= lt-lp) {
		if (memcmp(ptr, txt+i, lp) == 0)
			printf("%u\n", i);
		i += (quick[(unsigned)txt[i+lp]] > hbm[(unsigned)txt[i+lp-1]]) ?
			quick[(unsigned)txt[i+lp]] :
			hbm[(unsigned)txt[i+lp-1]];
	}
}
