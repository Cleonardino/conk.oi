

main:
	g++ $$(sdl2-config --cflags) -o $@ main.cpp display.cpp texturemanager.cpp tilemanager.cpp numbermanager.cpp ../model/game.cpp ../model/map.cpp ../model/parser.cpp ../model/province.cpp $$(sdl2-config --libs) -lSDL2_image

clean:
	rm main