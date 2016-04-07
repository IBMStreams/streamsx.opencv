/*
 * # Licensed Materials - Property of IBM
 * # Copyright IBM Corp. 2012
 * # US Government Users Restricted Rights - Use, duplication or
 * # disclosure restricted by GSA ADP Schedule Contract with
 * # IBM Corp.
 */

#ifndef __SUPPORT_FORMAT_H__
#define __SUPPORT_FORMAT_H__

#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <inttypes.h>

#include <iostream>
#include <sstream>

#define SSTRING(s_name, s_value) string s_name ; do { stringstream s_name##_stream ; s_name##_stream<< s_value ; s_name = s_name##_stream.str( ) ; } while(0)

namespace dpssupport
{
  namespace format
  {

    class Format
    {
    protected:

      typedef enum {
        
        FMT_INT,
        FMT_U_INT,
        FMT_DOUBLE,
        FMT_C_STR,
        FMT_P_STR,
        
      } FormatType ;
      
      std::string _fmt ;
      int _type ;
      int _int_value ;
      double _double_value ;
      std::string _cstr_value ;
      const char* _pstr_value ;
      
    public:
      
      Format( ) ;
      Format(const std::string& fmt, int i) ;
      Format(const std::string& fmt, unsigned int i) ;
      Format(const std::string& fmt, double d) ;
      Format(const std::string& fmt, const char* str) ;
      Format(const std::string& fmt, const std::string& str) ;
      virtual ~Format( ) ;
      virtual void print(std::ostream& strm) const ;

      virtual std::string str( ) const ;
      
    } ;

    class VFormat : public Format
    {
    protected:

      char* _formatted ;

    public:
      
      VFormat(const char* fmt, ...) /*__attribute__ ((format (printf, 1, 2)))*/ ;
      virtual ~VFormat( ) ;
      virtual void print(std::ostream& strm) const ;

    } ;

    class CDateFormat : public Format
    {
    protected:

      time_t _ts ;

    public:
      
      CDateFormat(time_t ts);
      virtual ~CDateFormat( ) ;
      virtual void print(std::ostream& strm) const ;

    } ;

    class ErrnoFormat : public Format
    {

    protected:

      int _errno ;

    public:

      ErrnoFormat(int e) ;
      ErrnoFormat( ) ;
      virtual ~ErrnoFormat( ) ;
      virtual void print(std::ostream& strm) const ;

    } ;

    class PollEventFormat : public Format
    {
    protected:

      short _events ;

    public:

      PollEventFormat(short events) ;
      virtual ~PollEventFormat( ) ;
      virtual void print(std::ostream& strm) const ;

    } ;

    class HexaFormat : public Format
    {
    protected:

      std::string _str ;
      void doit(const uint8_t* data, uint32_t len) ;

    public:

      HexaFormat(const uint8_t* data, uint32_t len) ;
      HexaFormat(const char* data, uint32_t len) ;
      virtual ~HexaFormat( ) ;
      virtual void print(std::ostream& strm) const ;

    } ;

    std::ostream& operator<<(std::ostream& strm, const Format& fmt) ;

  }
}

#endif

// Local Variables: ***
// mode:c++ ***
// End: ***
