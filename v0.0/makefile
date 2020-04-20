rm:
	rm main.o
	rm sfml-app
app: main.o
	g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
main.o: main.cpp
	g++ main.cpp -c
