/*
Copyright (C) 2012, 2016, International Business Machines Corporation
All Rights Reserved
*/

#ifndef __SUPPORT_MUTEX_H__
#define __SUPPORT_MUTEX_H__

#include <pthread.h>

namespace dpssupport
{
  namespace mutex
  {
    class Mutex
    {
    public:
      Mutex( );
      ~Mutex(void);
      void lock(void);
      void unlock(void);

    protected:
      pthread_mutex_t _mutex;
    } ;

    class SpinLock
    {
    public:
      SpinLock( );
      ~SpinLock(void);
      void lock(void);
      void unlock(void);

    protected:
      pthread_spinlock_t _splock ;
    } ;
    
    template<typename _T> 
    class AutoLock
    {
    public:
      
      typedef _T lock_t ;
      
    private:
      lock_t& _lock;
      bool _released ;
      
    public:
      
      AutoLock(lock_t& lock) : _lock(lock), _released(false)
      {
        _lock.lock();
      }

      ~AutoLock(void)
      {
        if (!_released)
        {
          release( ) ;
        }
      }

      void release( )
      {
        _lock.unlock( ) ;
        _released = true ;
      }
    } ;
  }
}

#endif

// Local Variables: ***
// mode:c++ ***
// End: ***
