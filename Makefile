# Location of where hello.c is stored in
SRCDIR=./src

# Name of the compiler, GNU GCC in this case
CC=gcc-12

# Any include directories
INCLUDE = /opt/homebrew/include/SDL2

# Any flags for the compiler
CFLAGS = -g -I$(INCLUDE) -c

# Any libraries to link with
LIBS = /opt/homebrew/lib

# Extra flags to give to compilers when they are supposed to invoke the linker
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf 

SRCS = $(SRCDIR)/main.c $(SRCDIR)/zombie.c $(SRCDIR)/spelare.c $(SRCDIR)/bullet.c $(SRCDIR)/text.c

main: main.o zombie.o music.o spelare.o bullet.o text.o
	@echo "Building mai./n"
	$(CC) main.o zombie.o music.o spelare.o bullet.o text.o -o main $(LDFLAGS) -L $(LIBS)

main.o:
	$(CC) $(CFLAGS) $(SRCDIR)/main.c 

zombie.o:
	$(CC) $(CFLAGS) $(SRCDIR)/zombie.c 

spelare.o:
	$(CC) $(CFLAGS) $(SRCDIR)/spelare.c 

music.o:
	$(CC) $(CFLAGS) $(SRCDIR)/music.c

bullet.o:
	$(CC) $(CFLAGS) $(SRCDIR)/bullet.c

text.o:
	$(CC) $(CFLAGS) $(SRCDIR)/text.c

clean:
	rm -f *.o
	rm -f main
