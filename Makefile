all: testsdl.html testsdl testsdl.exe

clean:
	rm -f testsdl.html testsdl testsdl.exe

SDL_VERSION=2.28.5
SDL2 SDL2.dll:
	curl -L https://github.com/libsdl-org/SDL/releases/download/release-${SDL_VERSION}/SDL2-devel-${SDL_VERSION}-mingw.tar.gz | tar xz
	ln -fs SDL2-${SDL_VERSION} SDL2
	ln -fs SDL2/x86_64-w64-mingw32/bin/SDL2.dll SDL2.dll

testsdl.html: testsdl.c
	emcc -Wall -o $@ testsdl.c -s USE_SDL=2 -s SINGLE_FILE -lm
testsdl: testsdl.c
	gcc -Wall -o $@ -I/usr/include/SDL2 testsdl.c -lSDL2 -lm
testsdl.exe: testsdl.c SDL2.dll
	x86_64-w64-mingw32-gcc -Wall -o $@ testsdl.c `./SDL2/x86_64-w64-mingw32/bin/sdl2-config --cflags --libs` -lm
