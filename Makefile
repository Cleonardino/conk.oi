main:
	g++ $$(sdl2-config --cflags) -o $@ display/main.cpp display/display.cpp display/texturemanager.cpp display/tilemanager.cpp display/numbermanager.cpp model/game.cpp model/map.cpp model/parser.cpp model/province.cpp model/tiledisplayinfo.cpp model/game_utils.cpp model/gamestamp.cpp $$(sdl2-config --libs) -lSDL2_image

clean:
	rm main