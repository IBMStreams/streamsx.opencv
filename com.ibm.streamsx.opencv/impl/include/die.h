/*
Copyright (C) 2012, 2016, International Business Machines Corporation
All Rights Reserved
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
