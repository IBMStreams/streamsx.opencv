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

#include <string.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <poll.h>

#include "format.h"

using namespace std ;
using namespace dpssupport::format ;

Format::Format( ) : _fmt("")
{
  // ok
}

Format::Format(const string& fmt, int i) :
  _fmt(fmt),
  _type(FMT_INT),
  _int_value(i)
{
  // ok
}

Format::Format(const string& fmt, unsigned int i) :
  _fmt(fmt),
  _type(FMT_U_INT),
  _int_value(i)
{
  // ok
}

Format::Format(const string& fmt, double d) :
  _fmt(fmt),
  _type(FMT_DOUBLE),
  _double_value(d)
{
  // ok
}

Format::Format(const string& fmt, const char* s) :
  _fmt(fmt),
  _type(FMT_P_STR),
  _pstr_value(s)
{
  // ok
}

Format::Format(const string& fmt, const string& s) :
  _fmt(fmt),
  _type(FMT_C_STR),
  _cstr_value(s)
{
  // ok
}

Format::~Format( )
{
}

void Format::print(ostream& strm) const
{
  char buffer[1024] ;

  switch(_type)
  {
  case FMT_INT:
    snprintf(buffer, 1024, _fmt.c_str( ), _int_value) ;
    break ;

  case FMT_U_INT:
    snprintf(buffer, 1024, _fmt.c_str( ), (unsigned int)_int_value) ;
    break ;

  case FMT_DOUBLE:
    snprintf(buffer, 1024, _fmt.c_str( ), _double_value) ;
    break ;

  case FMT_P_STR:
    snprintf(buffer, 1024, _fmt.c_str( ), _pstr_value) ;
    break ;

  case FMT_C_STR:
    snprintf(buffer, 1024, _fmt.c_str( ), _cstr_value.c_str( )) ;
    break ;

  default:
    abort( ) ;
  }

  strm << buffer ;
}

string Format::str( ) const
{
  stringstream strm ;
  print(strm) ;
  return strm.str( ) ;
}

ostream& dpssupport::format::operator<<(ostream& strm, const Format& fmt)
{
  fmt.print(strm) ;
  return strm ;
}

/* ======================================================================== */
/* ======================================================================== */
/* ======================================================================== */

VFormat::VFormat(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  
  _formatted = new char[4096] ;
  vsnprintf(_formatted, 4096, fmt, ap) ;
  
  va_end(ap) ;
}

VFormat::~VFormat( )
{
  delete[] _formatted ;
}

void VFormat::print(std::ostream& strm) const
{
  strm << _formatted ;
}

/* ======================================================================== */
/* ======================================================================== */
/* ======================================================================== */

CDateFormat::CDateFormat(time_t ts) : _ts(ts)
{
}

CDateFormat::~CDateFormat( )
{
}
 
void CDateFormat::print(ostream& strm) const
{
  char buffer[256] ;
  
  ctime_r(&_ts, buffer) ;
  
  buffer[strlen(buffer) - 1] = 0 ; // remove trailing \n
  
  strm << buffer ;
}

/* ======================================================================== */
/* ======================================================================== */
/* ======================================================================== */

ErrnoFormat::ErrnoFormat(int e) : _errno(e)
{
}

ErrnoFormat::ErrnoFormat( ) : _errno(errno)
{
}

ErrnoFormat::~ErrnoFormat( )
{
}

void ErrnoFormat::print(ostream& strm) const
{
  char buffer[4096] ;
  strm << strerror_r(_errno, buffer, 4096) ;
}

/* ======================================================================== */
/* ======================================================================== */
/* ======================================================================== */

PollEventFormat::PollEventFormat(short events) : _events(events)
{
}

PollEventFormat::~PollEventFormat( )
{
}

void PollEventFormat::print(std::ostream& o) const
{
  if (!_events)
  {
    o << "<none>" ;
  }
  else
  {
    short events = _events ;

    bool first = true ;

#define M(V) do {                               \
      if (events & V)                           \
      {                                         \
        if (!first)                             \
        {                                       \
          o << "," ;                            \
        }                                       \
        first = false ;                         \
        o << #V ;                               \
                                                \
        events &= ~V ;                          \
      }                                         \
    } while(0)

    M(POLLIN) ;
    M(POLLPRI) ;
    M(POLLOUT) ;
    M(POLLRDHUP) ;
    M(POLLERR) ;
    M(POLLHUP) ;
    M(POLLNVAL) ;

    if (events)
    {
      if (!first)
      {
        o << "," ;
      }
      o << "??? [" << _events << "]" ;
    }

#undef M
  }
}

/* ======================================================================== */
/* ======================================================================== */
/* ======================================================================== */

HexaFormat::HexaFormat(const uint8_t* data, uint32_t len)
{
  doit(data, len) ;
}

HexaFormat::HexaFormat(const char* data, uint32_t len)
{
  doit((const uint8_t*)data, len) ;
}

void HexaFormat::doit(const uint8_t* data, uint32_t len)
{
  stringstream strm ;

  strm << "[" ;
  for(uint32_t i=0 ;i<len; i++)
  {
    if (i > 0)
    {
      strm << " " ;

      if ((i % 4) == 0)
      {
        strm << " " ;
      }
    }
    strm << Format("%2.2x", data[i]) ;
  }
  strm << "]" ;

  _str = strm.str( ) ;
}

HexaFormat::~HexaFormat( )
{
}

void HexaFormat::print(std::ostream& o) const
{
  o << _str ;
}
