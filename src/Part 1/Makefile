# HW2 Part 1 Makefile 
# Edit SYSTEMC to match your install path
SYSTEMC ?= /usr/local/systemc-2.3

CC = g++

INCLUDES = -I. -I$(SYSTEMC)/include
LFLAGS   = -L. -L$(SYSTEMC)/lib-linux64
LIBS     = -lsystemc

# Executable name
MAIN = sad

SRCS = top.cpp

OBJS = $(SRCS:.cpp=.o)

export LD_LIBRARY_PATH := $(SYSTEMC)/lib-linux64

.PHONY: all clean

all: $(MAIN)
	@echo The program has been successfully compiled

$(MAIN): $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) -o $(MAIN) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CC) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(MAIN) *.o *~
