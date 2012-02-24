#include <stdlib.h>
#include <string.h>

#include <list.h>

#include "search.h"



/*
 * This file contains algorithms based on brute-force:
 * -- brute-force (srch_bf*)
 * -- fuzzy (srch_fuz*)
 */



/* brute force algorithm */

/*
 * searching in buffer: searching for first occurence of ptr in txt
 */
int srch_bf_buf (char *ptr, unsigned lp, char *txt, unsigned lt)
{
	char *tmp = txt;
	unsigned i;

	for (tmp=txt, i=0; (i+lp)<=lt; i++, tmp++)
		if (memcmp(ptr, tmp, lp) == 0)
			srch_ret(SRCH_OK, tmp-txt);

	srch_ret(SRCH_OK, -1);
}


/*
 * searching in buffer: searching for all occurence of ptr in txt
 */
t_lst *srch_bf_buf_all (char *ptr, unsigned lp, char *txt, unsigned lt)
{
	char *tmp = txt;
	t_fsize *pom;
	t_lst *lst = lst_new();
	unsigned i;

	if (! lst) srch_ret(SRCH_NO_MEM, NULL);
	if (! ptr || ! txt) {
		free(lst);
		srch_ret(SRCH_NULL_PARAMS, NULL);
	}
	
	for (tmp=txt, i=0; (i+lp)<=lt; tmp++, i++)
		if (memcmp(ptr, tmp, lp) == 0) {
			pom = (t_fsize*)malloc(sizeof(t_fsize));
			if (! pom) srch_ret(SRCH_NO_MEM, lst);
			*pom = tmp-txt;
			lst_add(lst, (void*)pom);
#ifdef DEBUG
			printf("%s:%d:: found %s at %lld\n",
					__FILE__, __LINE__,
					ptr, *pom);
#endif
		}

	srch_ret(SRCH_OK, lst);
}


/*
 * searching in file: pattern (ptr) size(lp) shouldn't exceed SRCH_BUFSIZE
 */
t_fsize srch_bf (FILE *f, char *ptr, unsigned lp)
{
	unsigned real=0, pos=0, ppos=0, spos=0, tmp_real=0, tmp_spos=0,
		 act_real=0;
	t_fsize rpos=0;
	unsigned char *buf=NULL, *tmp=NULL, *act=NULL;
	
	if (! ptr || ! lp) srch_ret(SRCH_NULL_PARAMS, -1);

	buf = (char*)malloc(SRCH_BUFSIZE);
	if (! buf) srch_ret(SRCH_NO_MEM, -1);
	act = buf;
	
	real = fread(buf, 1, SRCH_BUFSIZE, f);
	act_real = real;
	while (real>0) {
		/* searching for first same chars
		 * ppos must be 0 else we are somewhere in word */
		if (! ppos)
			while (pos<act_real && act[pos]!=ptr[ppos])
				++pos;
		/* testing if is it string that we are searching for */
		for (spos=pos; ppos<lp && pos<act_real && act[pos]==ptr[ppos];
			++pos, ++ppos);
		/* yes, he is the one :-) */
		if (ppos == lp) {
			t_fsize offset = rpos + ((tmp) ? tmp_spos : spos);
/*			long long offset = rpos + spos;*/

			if (tmp) free(tmp);
			if (buf) free(buf);

			srch_ret(SRCH_OK, offset);
		}
		/* no, we must search again */
		if (pos<act_real) {
			ppos = 0;
			/* switch to the first bufer */
			if (tmp && buf==act) {
				pos = ++tmp_spos;
				act = tmp;
				act_real = tmp_real;
			} else {
				pos = spos+1;
			}
		/* if end of first buffer the switch to second buffer */
		} else if (tmp && tmp==act) {
			act_real = real;
			act = buf;
			pos = 0;
			/* if we are not somewhere in pattern then
			 * we don't need more tmp buffer */
			if (! ppos) {
				free(tmp);
				tmp=NULL;
				rpos += SRCH_BUFSIZE;
			}
		} else {
		/* if we are at end of current buffer then read new one */
			/* if we are at end of tmp buf then free them */
/*			if (tmp) {
				free(tmp);
				tmp=NULL;
				rpos += SRCH_BUFSIZE;
			}*/
			/* if we found candidate for pattern but end of
			 * buffer appear first then we need manage two
			 * buffers */
			if (buf[real-1] == ptr[(ppos) ? ppos-1 : ppos]) {
				tmp_spos = spos;
				tmp_real = real;
				tmp = buf;
				buf = (char*)malloc(SRCH_BUFSIZE);
				if (! buf) srch_ret(SRCH_NO_MEM, -1);
			} else {
			/* else we can say that in SRCH_BUFSIZE characters
			 * is not pattern */
				rpos += SRCH_BUFSIZE;
			}
			/* read new buffer and reset vars */
			real = fread(buf, 1, SRCH_BUFSIZE, f);
			act = buf;
			act_real = real;
			spos=0;
			pos=0;
		}
	}

	/* sweep away buffers */
	if (buf) free(buf);
	if (tmp) free(tmp);

	srch_ret(SRCH_OK, -1);
}


/*
 * searching for all occurence of ptr in file f using srch_bf
 */
t_lst *srch_bf_all (FILE *f, char *ptr, unsigned lp)
{
	t_fsize offset=0, res=0, *pom;
	t_lst *lst = lst_new();

	if (! lst) srch_ret(SRCH_NO_MEM, NULL);
	if (! ptr) {
		free(lst);
		srch_ret(SRCH_NULL_PARAMS, NULL)
	};

	while ((res=srch_bf(f, ptr, lp)) >= 0) {
		offset += res;
		pom = (t_fsize*)malloc(sizeof(t_fsize));
		if (! pom) srch_ret(SRCH_NO_MEM, lst);
		*pom = offset;
		lst_add(lst, (void*)pom);
#ifdef DEBUG
			printf("%s:%d:: found %s at %lld\n",
					__FILE__, __LINE__,
					ptr, *pom);
#endif

		fseek(f, ++offset, SEEK_SET);
	}

	srch_ret(SRCH_OK, lst);
}



t_lst *srch_fuzbf_buf (char *ptr, unsigned lp, char *txt, unsigned lt)
{
	char *tmp = txt;
	unsigned i;
	t_lst *lst = lst_new();
	t_fsize *pom;

	if (! lst) srch_ret(SRCH_NO_MEM, NULL);
	if (! ptr || ! txt) {
		free(lst);
		srch_ret(SRCH_NULL_PARAMS, NULL);
	}

	printf("lt=%d\nlp=%d\n", lt, lp);
	for (tmp=txt, i=0; (i+lp)<=lt; i++, tmp++)
		if (fuzcmp(ptr, tmp, lp) == 0) {
/*			printf("%u\n", tmp-txt);
			srch_ret(SRCH_OK, tmp-txt);*/
			pom = (t_fsize*)malloc(sizeof(t_fsize));
			if (! pom) srch_ret(SRCH_NO_MEM, lst);
			*pom = tmp-txt;
			lst_add(lst, (void*)pom);
#ifdef DEBUG
			printf("%s:%d:: found %s at %lld\n",
					__FILE__, __LINE__,
					ptr, *pom);
#endif
		}


	srch_ret(SRCH_OK, lst);
}


