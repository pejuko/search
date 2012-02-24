#include <list.h>

#include "search.h"



/*
 * This file contains functions to support fuzzy search. Fuzzy search
 * is in these files:
 * -- brute_force.c
 */



/*
 * fuzzy compare
 */
int fuzcmp (unsigned char *ptr1, unsigned char *ptr2, unsigned l)
{
	unsigned i,j;

	if (ptr1 && !  ptr2) return 1;
	if (! ptr1 && ptr2) return -1;
	if (! ptr1 && ! ptr2) return 0;

	i=j=0;
	while ((i < l) && (j < l)) {
		if (ptr1[i] == ptr2[j]) {
			++i; ++j;
			continue;
		}
		if ((i+1) < l && (j+1) < l) {
			if (ptr1[i+1] == ptr2[j+1]) {
				i+=2; j+=2;
				continue;
			}
			if (ptr1[i] == ptr2[j+1]) {
				++i; ++j;
/*				++i; j+=2;*/
				continue;
			}
			if (ptr1[i+1] == ptr2[j]) {
				++i; ++j;
/*				i+=2; ++j;*/
				continue;
			}
		}
		if (i > 0 && j > 0) {
			if (ptr1[i] == ptr2[j-1]) {
				++i; ++j;
				continue;
			}
			if (ptr1[i-1] == ptr2[j]) {
				++i; ++j;
				continue;
			}
		}

		if (ptr1[i] < ptr2[j])
			return -1;
		else
			return 1;
	}

	return 0;
}


/*
 * Cut off offsets from found list which point to the same search patern.
 */
int fuzstrip (unsigned char *ptr, unsigned lp, unsigned char *txt, unsigned lt,
		t_lst *found)
{
	t_itm *itm=NULL, *pomitm=NULL;
	t_fsize *pom=NULL, start=0;
	int in=0;

	if (! ptr || ! txt || ! found || lp == 0 || lt == 0)
		srch_ret(SRCH_NULL_PARAMS, 0);

	itm = found->first;
	while (itm) {
		pom = (t_fsize*)itm->data;
		if (! in) {
			start = *pom;
			in = 1;
		} else if (in && ((*pom - start) <= lp)) {
			pomitm = itm;
			itm = itm->next;
			lst_cut(found, pomitm);
			continue;
		} else {
			start = *pom;
			in = 1;
		}
		itm = itm->next;
	}

	srch_ret(SRCH_OK, 1);
}


/*TODO: int fuzstrstrip ()
{
}
*/
