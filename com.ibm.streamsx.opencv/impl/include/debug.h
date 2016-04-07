/*
 * # Licensed Materials - Property of IBM
 * # Copyright IBM Corp. 2012
 * # US Government Users Restricted Rights - Use, duplication or
 * # disclosure restricted by GSA ADP Schedule Contract with
 * # IBM Corp.
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
