all:
	clear
	g++ src/main.cpp -o Build/program.exe -std=c++11 -lGL -lGLU -lGLEW -lglfw -lX11 -lpthread -lXrandr -lXi -ldl
	./Build/program.exe