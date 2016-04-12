/*
Copyright (C) 2012, 2016, International Business Machines Corporation
All Rights Reserved
*/

#ifndef __SUPPORT_DEBUG_H__
#define __SUPPORT_DEBUG_H__

#include <iostream>

#ifdef SUPPORT_DEBUG
#define DEBUG(stmt) do { std::cerr << "DEBUG(" << __FILE__ << ":" << __LINE__ << "): " << stmt << endl ; } while(0)
#else
#define DEBUG(stmt)
#endif

#ifndef QT
#define QT(v) "\"" << v << "\""
#endif

#endif
