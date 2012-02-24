#ifndef _SRCH_ALGORITHMS_
#define _SRCH_ALGORITHMS_

#ifdef __cplusplus
export "C" {
#endif

#include <stdio.h>

#include <list.h>


#define SRCH_BUFSIZE 4096
#define srch_ret(e, r) {_srch_error=(e); return (r);}

enum {SRCH_OK=0, SRCH_NO_MEM, SRCH_NULL_PARAMS, SRCH_KR_TOO_LEN};

typedef long long t_fsize;


extern int _srch_error;


/* binary safe is only brute force algorithms */


extern int fuzcmp (unsigned char *ptr1, unsigned char *ptr2, unsigned l);
extern int fuzstrip (unsigned char *ptr, unsigned lp,
		unsigned char *txt, unsigned lt, t_lst *found);


/* brute force algorithm */

/* searching in buffer: searchig for first occurence of ptr in txt */
extern int srch_bf_buf (char *ptr, unsigned lp, char *txt, unsigned lt);

/* searching in buffer using method of fuzzy compare */
extern t_lst *srch_fuzbf_buf (char *ptr, unsigned lp, char *txt, unsigned lt);

/* searching in buffer: searchich for all occurence of ptr in txt */
extern t_lst *srch_bf_buf_all (char *ptr, unsigned lp, char *txt, unsigned lt);

/* searching in file: pattern (ptr) size (lp) shouldn't exceed SRCH_BUFSIZE */
extern long long srch_bf (FILE *f, char *ptr, unsigned lp);

/* searching for all occurence of ptr in file f using srch_bf */
extern t_lst *srch_bf_all (FILE *f, char *ptr, unsigned lp);



/* karp-rabin algorithm */

/* searching in buffer: searching for first occurence ptr in txt */
extern int srch_kr_buf (char *ptr, unsigned lp, char *txt, unsigned lt);

/* searching in buffer: searching for all occurence ptr in txt */
extern void srch_kr_buf_all (char *ptr, unsigned lp, char *txt, unsigned lt);



/* horspool-boyer-moore algorithm */

/* searching in buffer: searching for first occurence ptr in txt */
extern int srch_hbm_buf (char *ptr, unsigned lp, char *txt, unsigned lt);

/* searching in buffer: searching for all occurence ptr in txt */
extern void srch_hbm_buf_all (char *ptr, unsigned lp, char *txt, unsigned lt);



/* quick search algorithm searching in buffer (based on boyer-moore */
extern void srch_quick_buf (char *ptr, unsigned lp, char *txt, unsigned lt);

/* smith search algorithm searching in buffer (based on quick and boyer-moore */
extern void srch_smith_buf (char *ptr, unsigned lp, char *txt, unsigned lt);



#ifdef __cplusplus
}
#endif

#endif
