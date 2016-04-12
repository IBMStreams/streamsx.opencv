# Copyright (C) 2012, 2016, International Business Machines Corporation
# All Rights Reserved

CPPFLAGS = -DNDEBUG=1 -D_GNU_SOURCE=1
CXXFLAGS = -I../include -I$(OPENCV_INSTALL_PATH)/include -Wall -Werror -Wsign-compare -Wfloat-equal -Wno-unused-label -O3 -fPIC
CFLAGS = $(CXXFLAGS) -Wmissing-prototypes
LDFLAGS = -L../lib -L$(OPENCV_INSTALL_PATH)/lib
LDLIBS = -lpthread -ldl -lrt -lmicrohttpd -lcurl

CC = gcc
CXX = g++

O_FILE = mutex.o opencv.o hpa_time.o format.o http.o
LIB_DIR = ../lib
LIB_FILE = $(LIB_DIR)/libdpsopencv.a

all: $(LIB_FILE)

$(LIB_FILE): $(O_FILE)
	mkdir -p $(LIB_DIR)
	ar r $@ $^

%.o: %.cpp 
	${CXX} ${CXXFLAGS} -c $<

%.o: %.c 
	${CC} ${CFLAGS} -c $<

clean:
	rm -vf *.o $(LIB_DIR)/*.a

