

main:
	g++ $$(sdl2-config --cflags) -o $@ main.cpp display.cpp texturemanager.cpp tilemanager.cpp $$(sdl2-config --libs) -lSDL2_image

clean:
	rm main