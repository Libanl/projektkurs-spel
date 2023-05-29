INCLUDE = C:\msys64\mingw64\include
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net -mwindows -lm

spelareClient: main.o zombie.o spelare.o music.o text.o powerup.o
	gcc -o spelareClient main.o zombie.o spelare.o music.o text.o powerup.o $(LDFLAGS)

main.o: ./src/main.c
	gcc -c ./src/main.c

zombie.o: ../lib/src/zombie.c ../lib/include/zombie.h
	gcc -c ../lib/src/zombie.c

spelare.o: ../lib/src/spelare.c ../lib/include/spelare.h
	gcc -c ../lib/src/spelare.c

music.o: ../lib/src/music.c ../lib/include/music.h
	gcc -c ../lib/src/music.c

text.o: ../lib/src/text.c ../lib/include/text.h
	gcc -c ../lib/src/text.c

powerup.o: ../lib/src/powerup.c ../lib/include/powerup.h
	gcc -c ../lib/src/powerup.c

clean:
	rm *.o
	rm *.exe