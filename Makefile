CFLAGS:=-Wall -Werror -O2 -fomit-frame-pointer
LIBS:=-lSDL2 -lSDL2_image

LINUX:=-I./include -I/usr/include/SDL2 -D_REENTRANT -L/usr/lib/x86_64-linux-gnu $(LIBS)
MINGW64:=-I./include -I./win64/SDL2_image/include -I./win64/SDL2/include -I./win64/SDL2/include/SDL2 -Dmain=SDL_main -L./win64/SDL2_image/lib -L./win64/SDL2/lib -lmingw32 -lSDL2main $(LIBS) -mwindows

SOURCES:=src/main.cpp src/World.cpp src/Assets.cpp src/Jewel.cpp src/Spark.cpp src/Star.cpp \
	src/engine/Animation.cpp src/engine/DynamicGameObject.cpp src/engine/Font.cpp \
	src/engine/GameObject.cpp src/engine/Graphics.cpp src/engine/SpriteBatcher.cpp \
	src/engine/Texture.cpp src/engine/TextureRegion.cpp src/util/Random.cpp

gcc: dobin
	g++ -std=c++11 $(CFLAGS) -o bin/jewelminer $(SOURCES) $(LINUX)

clang: dobin
	clang++ -std=c++11 -stdlib=libc++ $(CFLAGS) -o bin/jewelminer $(SOURCES) $(LINUX)

win: dobin
	g++ -std=c++11 $(CFLAGS) -o bin/jewelminer $(SOURCES) $(MINGW64)
	copy win64\SDL2_image\bin\*.dll bin
	copy win64\SDL2\bin\*.dll bin
	copy win64\mingw64\bin\*.dll bin

dobin:
	-mkdir bin

clean:
	-rm -rf bin
	-rd /s/q bin

.PHONY: gcc clang win dobin clean
