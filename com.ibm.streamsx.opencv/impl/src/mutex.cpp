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

#include <unistd.h>
#include <stdlib.h>

#include <iostream>
#include <cerrno>

#include "mutex.h"
#include "die.h"

using namespace dpssupport::mutex ;
using namespace std ;

Mutex::Mutex( )
{
  if (pthread_mutex_init(&_mutex, NULL) != 0)
  {
    DIE("pthread_mutex_init failed") ;
  }
}

Mutex::~Mutex(void)
{
  int attempts = 0 ;

  for(;;)
  {
    int ret = pthread_mutex_destroy(&_mutex) ;
    
    if (ret == 0)
    {
      break ;
    }
    
    if (ret == EBUSY)
    {
      // ouch, somebody else is holding the mutex...
      cerr << "pthread_mutex_destroy() busy, waiting for mutex to be released..." << endl ;
    }

    attempts++ ;

    if (attempts > 10)
    {
      DIE("pthread_mutex_destroy() busy, unable to free mutex because it is locked") ;
    }

    sleep(1) ;
  }
}

void Mutex::lock(void)
{
  if (pthread_mutex_lock(&_mutex) != 0)
  {
    DIE("pthread_mutex_lock failed") ;
  }

}

void Mutex::unlock(void)
{
  if (pthread_mutex_unlock(&_mutex) != 0)
  {
    DIE("pthread_mutex_unlock failed") ;
  }

}


SpinLock::SpinLock( )
{
  if (pthread_spin_init(&_splock, PTHREAD_PROCESS_PRIVATE) != 0)
  {
    DIE("pthread_spin_init failed") ;
  }
}

SpinLock::~SpinLock(void)
{
  int attempts = 0 ;

  for(;;)
  {
    int ret = pthread_spin_destroy(&_splock) ;
    
    if (ret == 0)
    {
      break ;
    }
    
    if (ret == EBUSY)
    {
      // ouch, somebody else is holding the mutex...
      cerr << "pthread_spin_destroy() busy, waiting for mutex to be released..." << endl ;
    }

    attempts++ ;

    if (attempts > 10)
    {
      DIE("pthread_spin_destroy() busy, unable to free mutex because it is locked") ;
    }

    sleep(1) ;
  }
}

void SpinLock::lock(void)
{
  if (pthread_spin_lock(&_splock) != 0)
  {
    DIE("pthread_spin_lock failed") ;
  }

}

void SpinLock::unlock(void)
{
  if (pthread_spin_unlock(&_splock) != 0)
  {
    DIE("pthread_spin_unlock failed") ;
  }

}
