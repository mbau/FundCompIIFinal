# Based somewhat on mrbook.org/tutorials/make
BIN = main
LFLAGS = -lSDL -lSDL_gfx -Wall
CFLAGS = -c -Wall
SOURCES = main.cpp Game.cpp Level.cpp Tower.cpp Enemy.cpp Surface.cpp Bullet.cpp
OBJECTS = $(SOURCES:.cpp=.o)

$(BIN):	$(OBJECTS)
	g++ $(LFLAGS) $(OBJECTS) -o $@

.cpp.o:
	g++ $(CFLAGS) $<


#$(BIN):	main.o Game.o Level.o Tower.o Enemy.o Surface.o Bullet.cpp
#	g++ $(LFLAGS) $^ -o $(BIN)
#main.o:	main.cpp Game.h
#	g++ $(CFLAGS) main.cpp
#Game.o:	Game.cpp Game.h Level.h Surface.h
#	g++ $(CFLAGS) Game.cpp
#Level.o:	Level.cpp Level.h Tower.h Enemy.h Surface.h Bullet.h
#	g++ $(CFLAGS) Level.cpp
#Tower.o:	Tower.cpp Tower.h Surface.h
#	g++ $(CFLAGS) Tower.cpp
#Enemy.o:	Enemy.cpp Enemy.h Surface.h
#	g++ $(CFLAGS) Enemy.cpp
#Entity.o:	Entity.cpp Entity.h Surface.h
#	g++ $(CFLAGS) Entity.cpp
#Surface.o:	Surface.cpp Surface.h
#	g++ $(CFLAGS) Surface.cpp
#Bullet.o:	Bullet.cpp Bullet.h Surface.h
#	g++ $(CFLAGS) Bullet.cpp
clean:
	rm -f *.o $(BIN)
