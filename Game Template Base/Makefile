CC = gcc
source_files = src/*.c
header_files = include/*.h
CFLAGS = `sdl2-config --static-libs --cflags` -ggdb3 -O3 --std=c99 -Wall -lm -lSDL2 -lSDL2_image -lSDL2_ttf# -lSDLmain
exe = MyGame

wCC = i686-w64-mingw32-gcc
all:
	$(CC) $(source_files) $(CFLAGS) -I$(header_files) -o $(exe)
	$(wCC) -o $(exe).exe $(source_files) -I$(header_files) -L /usr/i686-w64-mingw32/lib/ -lSDL2 -lSDLmain -lm -ggdb3 --std=c99 -g -L/usr/lib -pthread -lSDL2_image -lSDL2_ttf -ldl -lmingw32
