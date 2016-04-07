/*
 * # Licensed Materials - Property of IBM
 * # Copyright IBM Corp. 2012
 * # US Government Users Restricted Rights - Use, duplication or
 * # disclosure restricted by GSA ADP Schedule Contract with
 * # IBM Corp.
 */

#ifndef __SUPPORT_DIE_H__
#define __SUPPORT_DIE_H__

#include <stdio.h>
#define DIEV(fmt,...) do { fprintf(stderr, " *** ERROR(%s:%d): " fmt " ***\n", __FILE__, __LINE__, ##__VA_ARGS__) ; abort( ) ; } while(0)

#ifdef __cplusplus
#include <iostream>
#define DIE(stmt) do { std::cerr << " *** ERROR(" << __FILE__ << ":" << __LINE__ << "): " << stmt << " ***" << endl ; abort( ) ; } while(0)
#else
#include <stdio.h>
#define DIE DIEV
#endif

#endif
