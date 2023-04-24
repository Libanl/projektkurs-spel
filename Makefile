INCLUDE = C:\msys64\mingw64\include
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -mwindows -lm

spelare: main.o zombie.o spelare.o music.o bullet.o
	gcc -o spelare main.o zombie.o spelare.o music.o bullet.o $(LDFLAGS)

main.o: ./src/main.c
	gcc -c ./src/main.c

zombie.o: ./src/zombie.c ./includes/zombie.h
	gcc -c ./src/zombie.c

spelare.o: ./src/spelare.c ./includes/spelare.h
	gcc -c ./src/spelare.c

music.o: ./src/music.c ./includes/music.h
	gcc -c ./src/music.c

bullet.o: ./src/bullet.c ./includes/bullet.h
	gcc -c ./src/bullet.c

clean:
	rm *.o
	rm *.exe