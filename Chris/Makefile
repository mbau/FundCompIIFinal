BIN = main
LFLAGS = -lSDL -Wall
CFLAGS = -c -Wall

$(BIN):	main.o Game.o Level.o Tower.o Enemy.o Entity.o
	g++ $(LFLAGS) $^ -o $(BIN)
main.o:	main.cpp Game.h
	g++ $(CFLAGS) main.cpp
Game.o:	Game.cpp Game.h Level.h
	g++ $(CFLAGS) Game.cpp
Level.o:	Level.cpp Level.h Tower.h Enemy.h
	g++ $(CFLAGS) Level.cpp
Tower.o:	Tower.cpp Tower.h
	g++ $(CFLAGS) Tower.cpp
Enemy.o:	Enemy.cpp Enemy.h
	g++ $(CFLAGS) Enemy.cpp
Entity.o:	Entity.cpp Entity.h
	g++ $(CFLAGS) Entity.cpp
clean:
	rm -f *.o $(BIN)
