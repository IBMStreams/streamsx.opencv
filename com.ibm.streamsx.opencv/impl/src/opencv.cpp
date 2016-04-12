/*
Copyright (C) 2012, 2016, International Business Machines Corporation
All Rights Reserved
*/

#include <unistd.h>

#include <map>
#include <string>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "opencv.h"
#include "mutex.h"

using namespace std ;
using namespace dpssupport::mutex ;
using namespace dpssupport::opencv ;

std::ostream& cv::operator<<(std::ostream& strm, cv::Exception& exc)
{
  const char* errorStr = cvErrorStr(exc.code);
    
  strm << "OpenCV Error: " << errorStr
       << " (" << exc.err.c_str() << ") in "
       << (exc.func.size() > 0 ? exc.func.c_str() : "unknown function")
       << ", file " << exc.file.c_str()
       << ", line " << exc.line ;

  return strm ;
}

const char* dpssupport::opencv::depth2string(int depth)
{
  return IPL_DEPTH2STR(depth) ;
}

static Mutex event_loop_mutex ;
static Mutex toolkit_mutex ;

void dpssupport::opencv::run_event_loop(volatile bool* shutdown)
{
  AutoLock<Mutex> lck(event_loop_mutex) ;

  while(!*shutdown)
  {
    {
      AutoLock<Mutex> lck2(toolkit_mutex) ;
      
      cvWaitKey(10) ; // wait 10ms
    }
    
    usleep(5*1000) ;
  }
}

static map<string, int> window_map ;

void dpssupport::opencv::create_window(const string& name)
{
  AutoLock<Mutex> lck(toolkit_mutex) ;
  
  map<string, int>::iterator it(window_map.find(name)) ;

  if (it == window_map.end( ))
  {
    cvNamedWindow(name.c_str( ), CV_WINDOW_AUTOSIZE) ;
    window_map[name] = 1 ;
  }
  else
  {
    it->second++ ;
  }
}

void dpssupport::opencv::delete_window(const string& name)
{
  AutoLock<Mutex> lck(toolkit_mutex) ;
  
  map<string, int>::iterator it(window_map.find(name)) ;

  if (it == window_map.end( ))
  {
    // ooops ??
  }
  else
  {
    it->second-- ;

    if (it->second == 0)
    {
      cvDestroyWindow(name.c_str( )) ;

      window_map.erase(it) ;
    }
  }
}

Mutex& dpssupport::opencv::get_toolkit_mutex( )
{
  return toolkit_mutex ;
}
