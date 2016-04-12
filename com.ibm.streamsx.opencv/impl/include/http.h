/*
Copyright (C) 2012, 2016, International Business Machines Corporation
All Rights Reserved
*/

#ifndef __SUPPORT_HTTP_H__
#define __SUPPORT_HTTP_H__

#include <string>
#include <iostream>
#include <map>

#include <microhttpd.h>

#include "mutex.h"

namespace dpssupport
{

  namespace http
  {

    class UrlHandler ;
    
    class HttpServer
    {
    protected:

      typedef struct _handler_t
      {
        UrlHandler* obj ;
        void* data ;
      } handler_t ;

      struct MHD_Daemon* _http_daemon;
      unsigned short _http_port ;
      std::map<std::string, handler_t> _url_handlers ;
      mutex::Mutex _mutex ;

    public:

      HttpServer(unsigned short port = 0) ;
      ~HttpServer( ) ;

      unsigned short getPort( ) const ;
      
      int dispatchHTTPRequest(struct MHD_Connection * connection,
                              const char * url,
                              const char * method) ;
      void addHandler(const std::string& url, UrlHandler* handler, void* data = NULL) ;
      void returnPage(struct MHD_Connection* connection,
                      const char* data, int code) ;
      void returnPage(struct MHD_Connection* connection,
                      const std::string& data, int code) ;

      void start( ) ;
      void stop( ) ;

    } ;

    class UrlHandler
    {
    protected:

      HttpServer* _http_server ;

    public:

      UrlHandler( ) ;
      virtual ~UrlHandler( ) ;

      void setServer(HttpServer* http_server) ;
      virtual void handle(struct MHD_Connection* connection,
                          const char* url,
                          const char* method,
                          void* data) = 0 ;
    } ;

    class StaticUrlHandler : public UrlHandler
    {
    public:

      StaticUrlHandler( ) ;
      virtual ~StaticUrlHandler( ) ;

      virtual void handle(struct MHD_Connection* connection,
                          const char* url,
                          const char* method,
                          void* data) ;
      virtual void handle(struct MHD_Connection* connection,
                          const char* url,
                          const char* method,
                          void* data,
                          std::string& page,
                          int& code) = 0 ;
    } ;

    class SimpleUrlHandler : public StaticUrlHandler
    {
    public:

      SimpleUrlHandler( ) ;
      virtual ~SimpleUrlHandler( ) ;

      virtual void handle(struct MHD_Connection* connection,
                          const char* url,
                          const char* method,
                          void* data,
                          std::string& page,
                          int& code) ;
      virtual void handle(void* data,
                          std::string& page,
                          int& code) = 0 ;
    } ;

    
    template<typename C>
    class FctUrlHandler : public UrlHandler
    {
      
    public:
      
      typedef void (C::*fct_ptr_t)(struct MHD_Connection* connection,
                                   const char* url,
                                   const char* method,
                                   void* data) ;
      
    private:
      
      C* _obj_ptr ;
      fct_ptr_t _fct_ptr ;
      
    public:
      
      FctUrlHandler(C* obj_ptr, fct_ptr_t fct_ptr) :
        _obj_ptr(obj_ptr),
        _fct_ptr(fct_ptr)
      {
      }
      
      virtual ~FctUrlHandler( )
      {
      }
      
      virtual void handle(struct MHD_Connection* connection,
                          const char* url,
                          const char* method,
                          void* data)
      {
        (_obj_ptr->*(_fct_ptr))(connection, url, method, data) ;
      }
      
    } ;

    template<typename C>
    class FctStaticUrlHandler : public StaticUrlHandler
    {
      
    public:
      
      typedef void (C::*fct_ptr_t)(struct MHD_Connection* connection,
                                   const char* url,
                                   const char* method,
                                   void* data,
                                   std::string& page,
                                   int& code) ;
      
    private:
      
      C* _obj_ptr ;
      fct_ptr_t _fct_ptr ;
      
    public:
      
      FctStaticUrlHandler(C* obj_ptr, fct_ptr_t fct_ptr) :
        _obj_ptr(obj_ptr),
        _fct_ptr(fct_ptr)
      {
      }
      
      virtual ~FctStaticUrlHandler( )
      {
      }
      
      virtual void handle(struct MHD_Connection* connection,
                          const char* url,
                          const char* method,
                          void* data,
                          std::string& page,
                          int& code)
      {
        (_obj_ptr->*(_fct_ptr))(connection, url, method, data, page, code) ;
      }
      
    } ;

  }
}

#endif

// Local Variables: ***
// mode:c++ ***
// End: ***
