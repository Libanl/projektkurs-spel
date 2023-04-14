# Makefile for Windows

# Location of where the source files are stored in
SRCDIR=./src

# Name of the compiler, GNU GCC in this case
CC=gcc

# Any include directories
# INCLUDE = ./lib/macos/include
INCLUDE = C:\msys64\mingw64\include

# Any flags for the compiler
#CFLAGS = -g -I$(INCLUDE) -c
CFLAGS = -g -c

# Any libraries to link with
#LIBS = C:\msys64\mingw64\lib

# Extra flags to give to compilers when they are supposed to invoke the linker
# NOTE ORDER OF THE FLAGS MATTERS!
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows

# Source files
SRCS = $(SRCDIR)/main.c $(SRCDIR)/zombie.c

# Object files
OBJS = $(SRCS:.c=.o)

# Binary file
MAIN = main

# Rule for building the main binary
$(MAIN): $(OBJS)
	@echo "Building $(MAIN)"
	$(CC) $(OBJS) -o $(MAIN) $(LDFLAGS)

# Rule for building object files
%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $@ $<

# Clean rule
clean:
	rm -f $(OBJS) $(MAIN)
