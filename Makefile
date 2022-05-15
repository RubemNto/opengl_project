all:
	clear
	g++ src/main.cpp -o program.exe -std=c++11 -lGL -lGLU -lGLEW -lglfw -lX11 -lpthread -lXrandr -lXi -ldl
	./program.exe