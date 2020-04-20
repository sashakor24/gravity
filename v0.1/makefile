rm:
	rm gravity
	rm main.o
run: gravity
	./gravity
app: main.o const.hpp Planet.hpp Point.hpp
	g++ main.o -o gravity -lsfml-system -lsfml-window -lGL
main.o: main.cpp const.hpp Planet.hpp Point.hpp
	g++ main.cpp -c -DGL_GLEXT_PROTOTYPES