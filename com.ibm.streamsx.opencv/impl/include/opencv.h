// -*- c++ -*-
/*
 * # Licensed Materials - Property of IBM
 * # Copyright IBM Corp. 2012
 * # US Government Users Restricted Rights - Use, duplication or
 * # disclosure restricted by GSA ADP Schedule Contract with
 * # IBM Corp.
 */

#ifndef __SUPPORT_OPENCV_H__
#define __SUPPORT_OPENCV_H__

#include <ostream>
#include <string>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "mutex.h"

#define IPL_PREFIX

#define COPY2TUPLE(img, tpl) COPY2TUPLE_Q(img, tpl, get_ipl_channels, get_ipl_depth, get_ipl_width, get_ipl_height, get_ipl_data)

#define COPY2TUPLE_Q(img, tpl, acc_channels, acc_depth, acc_width, acc_height, acc_data) do { \
                                                                        \
    if ((img)->depth != IPL_DEPTH_8U)                                   \
    {                                                                   \
      THROW(SPLRuntimeOperator, "Unsupported image depth: " << IPL_DEPTH2STR((img)->depth)) ; \
    }                                                                   \
                                                                        \
    (tpl).acc_channels( ) = (img)->nChannels ;                          \
    (tpl).acc_depth( ) = (img)->depth ;                                 \
    (tpl).acc_width( ) = (img)->width ;                                 \
    (tpl).acc_height( ) = (img)->height ;                               \
    list<uint8>& copy2tuple_data = (tpl).acc_data( ) ;                \
    copy2tuple_data.resize((img)->height * (img)->width * (img)->nChannels) ; \
                                                                        \
    {                                                                   \
      uint8_t* dst_ptr = (uint8_t*)&copy2tuple_data[0] ;                \
      const uint8_t* src_ptr = (const uint8_t*)((img)->imageData) ;     \
                                                                        \
      for(int y=0; y<(img)->height; y++)                                \
      {                                                                 \
        memcpy(dst_ptr, src_ptr, (img)->width * (img)->nChannels) ;     \
                                                                        \
        src_ptr += (img)->widthStep ;                                   \
        dst_ptr += (img)->width * (img)->nChannels ;                    \
      }                                                                 \
    }                                                                   \
                                                                        \
} while(0)

#define COPY2IMG(tpl, img) COPY2IMG_Q(tpl, img, get_ipl_channels, get_ipl_depth, get_ipl_width, get_ipl_height, get_ipl_data)

#define COPY2IMG_Q(tpl, img, acc_channels, acc_depth, acc_width, acc_height, acc_data) do { \
                                                                        \
    if ((tpl).acc_depth() != IPL_DEPTH_8U)                             \
    {                                                                   \
      THROW(SPLRuntimeOperator, "Unsupported image depth: " << IPL_DEPTH2STR((img)->depth)) ; \
    }                                                                   \
                                                                        \
  (img) = cvCreateImage(cvSize((tpl).acc_width(),                      \
                               (tpl).acc_height()),                    \
                      (tpl).acc_depth(),                               \
                      (tpl).acc_channels()) ;                          \
                                                                        \
  if (img)                                                              \
  {                                                                     \
    const uint8_t* copy2img_src_ptr = (const uint8_t*)&((tpl).acc_data()[0]) ; \
    uint8_t* copy2img_dst_ptr = (uint8_t*)((img)->imageData) ;          \
                                                                        \
    for(int copy2img_y=0; copy2img_y<(img)->height; copy2img_y++)       \
    {                                                                   \
      memcpy(copy2img_dst_ptr, copy2img_src_ptr,                        \
             (img)->width * (img)->nChannels) ;                         \
                                                                        \
      copy2img_src_ptr += (img)->width * (img)->nChannels ;             \
      copy2img_dst_ptr += (img)->widthStep ;                            \
    }                                                                   \
  }                                                                     \
} while(0)

#define IPL_DEPTH2STR(d) ((uint32_t)(d) == IPL_DEPTH_8U ? "8u" :        \
                          ((uint32_t)(d) == IPL_DEPTH_8S ? "8s" :       \
                           ((uint32_t)(d) == IPL_DEPTH_16U ? "16u" :    \
                            ((uint32_t)(d) == IPL_DEPTH_16S ? "16s" :   \
                             ((uint32_t)(d) == IPL_DEPTH_32S ? "32s" :  \
                              ((uint32_t)(d) == IPL_DEPTH_32F ? "32f" : \
                               ((uint32_t)(d) == IPL_DEPTH_64F ? "64f" : "???")))))))

#define IINFO(img) (img)->width << "x" << (img)->height << "/" << (img)->nChannels << ":" << IPL_DEPTH2STR((img)->depth)

namespace cv
{
  std::ostream& operator<<(std::ostream& strm, cv::Exception& exc) ;
}

namespace dpssupport
{
  namespace opencv
  {
    typedef struct _proxy_frame_t
    {
      uint32_t magic ;
      uint32_t compressed_size ;
      uint32_t real_size ;
      uint32_t width ;
      uint32_t width_step ;
      uint32_t height ;
      uint32_t depth ;
      uint32_t channels ;
    } proxy_frame_t ;

    const char* depth2string(int depth) ;
    void run_event_loop(volatile bool* shutdown) ;
    void create_window(const std::string& name) ;
    void delete_window(const std::string& name) ;
    dpssupport::mutex::Mutex& get_toolkit_mutex( ) ;
  }
}

#endif
