#include "../config.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_SYS_MMAN_H
  #include <sys/mman.h>
#endif

#include <unistd.h>

#include <list.h>
#include <search.h>


void use (void)
{
	printf( "test -p <patern> -f <input file> -m <method number> -h\n"
		"\n"
		"\t-p <patern>        -- search pattern\n"
		"\t-f <input file>    -- file name or - for stdin\n"
		"\t-m <method number> -- 0: brute force\n"
		"\t                      1: karp-rabin\n"
		"\t                      2: horspool (based on boyer-moore)\n"
		"\t                      3: quick search (based on) boyer-moore\n"
		"\t                      4: smith search (based on boyer-moore and quick search\n"
		"\t                      5: fuzzy brute force search\n"
		"\t-h                 -- this help\n");
}

long fsize (FILE *f)
{
	long p, old;
	
	old = ftell(f);
	fseek(f, 0, SEEK_END);
	p = ftell(f);
	fseek(f, old, SEEK_SET);
	
	return p;
}

char *read_buf (FILE *f)
{
	long s = fsize(f);
	char *buf;

#ifndef HAVE_MMAP
	if (! (buf = (char*) malloc(s + 1)))
		return NULL;
#endif

#ifdef HAVE_MMAP
	buf = (char*) mmap(0, s, PROT_READ, MAP_SHARED, f->_fileno, 0);
	if (buf == MAP_FAILED) {
		perror("Unable map file to memory\n");
		exit(1);
	}
#else
	fread((void*)buf, s, 1, f);
	buf[s] = '\0';
#endif

	return buf;
}

#define MAX_METHOD 6

int main(int argc, char **argv) {
	FILE *fr = NULL;
	/*t_fsize flen;*/
	char *ptr=NULL, *txt=NULL, *fname=NULL, c;
	char *mstrings[] = {	"brute force",
				"karp-rabin",
				"horspool (based on boyer-moore)",
				"quick search (based on boyer-moore)",
				"smith (based on quick and boyer-moore)",
				"fuzzy brute force"};
	int method=0;
	t_lst *lst = NULL;
	t_itm *itm = NULL;
	unsigned char *pomtxt = NULL;
	unsigned lp = 0, lt = 0;
	t_fsize pompoz;

	while ((c=getopt(argc, argv, "f:p:m:h")) != -1) {
		switch (c) {
			case 'f':
				if (! optarg) {
					use();
					return 1;
				}
				if (optarg[0] == '-') {
					fname = NULL;
					fr = stdin;
					continue;
				}
				fname = strdup(optarg);
				fr = fopen(fname, "r");
				if (errno) {
					perror("");
					use();
					return 1;
				}
				break;
			case 'p':
				if (! optarg) {
					use();
					return 1;
				}
				ptr = strdup(optarg);
				break;
			case 'm':
				if (! optarg) {
					use();
					return 1;
				}
				method = atol(optarg);
				if (method >= MAX_METHOD) {
					use();
					return 1;
				}
				break;
			case 'h':
				use();
				return 0;
				break;
			default:
				use();
				return 1;
				break;
		}
	}

	if (! fr || ! ptr) {
		use();
		return 1;
	}
	lp = strlen(ptr);
	
	printf("\nUsing %s method\n", mstrings[method]);
	switch (method) {
		case 0:
			txt = read_buf(fr);
/*			lst = srch_bf_all(fr, ptr, strlen(ptr));*/
			lst = srch_bf_buf_all(ptr, lp, txt, fsize(fr));
			break;
		case 1:
			txt = read_buf(fr);
			srch_kr_buf_all(ptr, lp, txt, fsize(fr));
			break;
		case 2:
			txt = read_buf(fr);
			srch_hbm_buf_all(ptr, lp, txt, fsize(fr));
			break;
		case 3:
			txt = read_buf(fr);
			srch_quick_buf(ptr, lp, txt, fsize(fr));
			break;
		case 4:
			txt = read_buf(fr);
			srch_smith_buf(ptr, lp, txt, fsize(fr));
			break;
		case 5:
			txt = read_buf(fr);
			lst = srch_fuzbf_buf(ptr, lp, txt, fsize(fr));
			fuzstrip(ptr, strlen(ptr), txt, fsize(fr), lst);
			break;
	}

	if (lst) itm = lst->first;
	pomtxt = (unsigned char *)malloc(lp+1);
	lt = fsize(fr);
	while (itm) {
		pompoz =  *((t_fsize*)itm->data);
		if ((lt-lp) > pompoz)
			memcpy(pomtxt, txt+pompoz, lp);
		else
			pomtxt[0] = '\0';
		pomtxt[lp] = '\0';
		printf("%lld: %s\n", pompoz, pomtxt);
		itm = itm->next;
	}
	free(pomtxt);

	if (lst) lst_clean(lst, free);

	return 0;
}
