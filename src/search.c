/*
 * Searching algorithms
 */

/*
 * Don't use something like *tmp == '\0'. Use (i+lp)<=lt instead. Otherwise it
 * will be not binary safe.
 */

#include "search.h"

int _srch_error=0;

