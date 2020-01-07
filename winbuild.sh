#!/bin/sh
i686-w64-mingw32-gcc -mwindows src/*.c -o logic.exe -Iinclude/ -ISDL2/include -ISDL2_image/include -ISDL2_ttf/include -L. -D__MINGW32__ -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lpthread

