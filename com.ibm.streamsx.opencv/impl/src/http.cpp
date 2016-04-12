/*
Copyright (C) 2012, 2016, International Business Machines Corporation
All Rights Reserved
*/

#include <netdb.h>
#include <errno.h>

#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdarg.h>

#include <iostream>
#include <sstream>

#include "http.h"
#include "format.h"
#include "die.h"
#include "debug.h"

using namespace std ;
using namespace dpssupport::http ;
using namespace dpssupport::format ;
using namespace dpssupport::mutex ;

static int process_http_request(void * cls,
                                struct MHD_Connection * connection,
                                const char * url,
                                const char * method,
                                const char * version,
                                const char * upload_data,
                                size_t * upload_data_size,
                                void ** ptr) ;

// ====================================================================
// ====================================================================
// ====================================================================

HttpServer::HttpServer(unsigned short port) : _http_daemon(NULL), _http_port(port)
{
}

HttpServer::~HttpServer( )
{
  stop( ) ;

  AutoLock<Mutex> lck(_mutex) ;

  map<string, handler_t>::iterator it(_url_handlers.begin( )) ;
  while (it != _url_handlers.end( ))
  {
    delete it->second.obj ;
    it->second.obj = NULL ;
    it++ ;
  }
}

void HttpServer::start( )
{
  AutoLock<Mutex> lck(_mutex) ;

  _http_daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG,
                                  _http_port,
                                  NULL,
                                  NULL,
                                  &process_http_request,
                                  this,
                                  MHD_OPTION_CONNECTION_TIMEOUT, 30,
                                  MHD_OPTION_END);
  
  if (_http_daemon == NULL)
  {
    DIE("MHD_start_daemon failed") ;
  }

  const union MHD_DaemonInfo* info = MHD_get_daemon_info(_http_daemon, MHD_DAEMON_INFO_LISTEN_FD) ;
  
  if (info == NULL)
  {
    DIE("MHD_get_daemon_info failed");
  }

  struct sockaddr_in myaddr_in ;
  socklen_t addrlen = sizeof(myaddr_in);
  if (getsockname(info->listen_fd, (struct sockaddr *)&myaddr_in, &addrlen))
  {
    int e = errno ;
    DIE("getsockname failed: " << ErrnoFormat(e)) ;
  }
  
  _http_port = ntohs(myaddr_in.sin_port) ;

  DEBUG("Server started on port " << _http_port) ;
}

void HttpServer::stop( )
{
  AutoLock<Mutex> lck(_mutex) ;

  if (_http_daemon)
  {
    MHD_stop_daemon(_http_daemon) ;
    _http_daemon = NULL ;
  }
}

unsigned short HttpServer::getPort( ) const
{
  return _http_port ;
}

int HttpServer::dispatchHTTPRequest(struct MHD_Connection * connection,
                                    const char * url,
                                    const char * method)
{
  DEBUG("HTTP Request: url=[" << url << "] meth=[" << method << "]") ;
  
  if (strcmp(method, "GET"))
  {
    return MHD_NO; /* unexpected method */
  }

  UrlHandler* handler = NULL ;
  void* data = NULL ;
  
  {
    AutoLock<Mutex> lck(_mutex) ;
    
    map<string, handler_t>::const_iterator it(_url_handlers.find(url)) ;
    if (it != _url_handlers.end( ))
    {
      handler = it->second.obj ;
      data = it->second.data ;
    }
  }

  if (handler == NULL)
  {
    const char* page =
      "<html><head><title>Page Not Found</title></head>"
      "<body><h1>404 - Page Not Found</h1>"
      "<p>The page you were looking for could not be found on the server.</p>"
      "</body></html>" ;
    
    returnPage(connection, page, 404) ;
  }
  else
  {
    handler->handle(connection, url, method, data) ; // XXXXXXXXXXXXX handler should return MHD_NO or MHD_YES
  }
  
  return MHD_YES;
}

void HttpServer::addHandler(const std::string& url, UrlHandler* handler, void* data)
{
  AutoLock<Mutex> lck(_mutex) ;

  handler->setServer(this) ;

  handler_t h ;
  h.obj = handler ;
  h.data = data ;

  _url_handlers[url] = h ;
}

void HttpServer::returnPage(struct MHD_Connection * connection,
                            const string& data, int code)
{
  char* page = strdup(data.c_str( )) ;
  
  struct MHD_Response * response = MHD_create_response_from_data(strlen(page),
                                                                 (void*)page,
                                                                 MHD_YES,
                                                                 MHD_NO) ;
  
  int ret = MHD_queue_response(connection, code, response);
  
  MHD_destroy_response(response);

  if (ret != MHD_YES)
  {
    DIE("MHD_queue_response failed") ;
  }
}

void HttpServer::returnPage(struct MHD_Connection * connection,
                            const char* page, int code)
{
  struct MHD_Response * response = MHD_create_response_from_data(strlen(page),
                                                                 (void*)page,
                                                                 MHD_NO,
                                                                 MHD_NO) ;
  
  int ret = MHD_queue_response(connection, code, response);
  
  MHD_destroy_response(response);
  
  if (ret != MHD_YES)
  {
    DIE("MHD_queue_response failed") ;
  }
}

static int process_http_request(void * cls,
                                struct MHD_Connection * connection,
                                const char * url,
                                const char * method,
                                const char * version,
                                const char * upload_data,
                                size_t * upload_data_size,
                                void ** ptr)
{
  *ptr = NULL ;

  return ((HttpServer*)cls)->dispatchHTTPRequest(connection, url, method) ;
}

// ====================================================================
// ====================================================================
// ====================================================================

UrlHandler::UrlHandler( ) : _http_server(NULL)
{
}

UrlHandler::~UrlHandler( )
{
}

void UrlHandler::setServer(HttpServer* http_server)
{
  _http_server = http_server ;
}

// ====================================================================
// ====================================================================
// ====================================================================

StaticUrlHandler::StaticUrlHandler( )
{
}

StaticUrlHandler::~StaticUrlHandler( )
{
}

void StaticUrlHandler::handle(struct MHD_Connection* connection,
                              const char* url,
                              const char* method,
                              void* data)
{
  string page ;
  int code = 200 ;
  
  handle(connection, url, method, data, page, code) ;
  
  _http_server->returnPage(connection, page, code) ;
}

// ====================================================================
// ====================================================================
// ====================================================================

SimpleUrlHandler::SimpleUrlHandler( )
{
}

SimpleUrlHandler::~SimpleUrlHandler( )
{
}

void SimpleUrlHandler::handle(struct MHD_Connection* connection,
                              const char* url,
                              const char* method,
                              void* data,
                              std::string& page,
                              int& code)
{
  handle(data, page, code) ;
}

// ====================================================================
// ====================================================================
// ====================================================================
