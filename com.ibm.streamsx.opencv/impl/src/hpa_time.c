/*
 * # Licensed Materials - Property of IBM
 * # Copyright IBM Corp. 2012
 * # US Government Users Restricted Rights - Use, duplication or
 * # disclosure restricted by GSA ADP Schedule Contract with
 * # IBM Corp.
 */

static const char* IBM_COPYRIGHT(void) __attribute__ ((used));
static const char* IBM_COPYRIGHT(void) { return 
"                                                               "
" # Licensed Materials - Property of IBM                        "
" # Copyright IBM Corp. 2012                                    "
" # US Government Users Restricted Rights - Use, duplication or "
" # disclosure restricted by GSA ADP Schedule Contract with     "
" # IBM Corp.                                                   "
"                                                               "
; }

#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "hpa_time.h"
#include "die.h"

double gettime(void)
{
#if 0
  
  struct timeval tv ;
  if (gettimeofday(&tv, NULL) != 0)
  {
    DIE("gettimeofday failed: %s", strerror(errno)) ;
  }
  
  return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0 ;
  
#else
  
  struct timespec ts ;

  if (clock_gettime(CLOCK_REALTIME /*_HR*/, &ts))
  {
    DIE("clock_gettime failed: %s", strerror(errno)) ;
  }

  return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0 ;

#endif
}
