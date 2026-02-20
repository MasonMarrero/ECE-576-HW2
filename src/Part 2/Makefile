# HW2 Part 1 Makefile (class-style)
# Edit SYSTEMC to match your install path
SYSTEMC ?= /usr/local/systemc-2.3

CC = g++

INCLUDES = -I. -I$(SYSTEMC)/include
LFLAGS   = -L. -L$(SYSTEMC)/lib-linux64
LIBS     = -lsystemc

# Executable name
MAIN = sad

# IMPORTANT:
# If your "top.cpp" includes "sad.cpp" and "memory.cpp" (like the class example),
# compile ONLY top.cpp (do NOT also compile sad.cpp/memory.cpp separately).
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
