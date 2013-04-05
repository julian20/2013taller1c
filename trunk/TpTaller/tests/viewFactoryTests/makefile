
INCLUDES=../../includes

CC= g++
CCFLAGS= -g -Wall -pedantic -I$(INCLUDES) 
LIBS=`sdl-config --cflags --libs` -L/usr/local/lib -lSDL -lSDL_image -lSDL_gfx -lSDL_mixer
SRC=../../src/
LIB= view/ViewFactory model/entities/Entity view/entities/EntityView model/Vector2
LIBS_LOCAL = ViewFactory.o Entity.o EntityView.o Vector2.o

#Directorio base del proyecto relativo a este makefile
PROYECT_DIR = ../../

VFLAGS= valgrind --leak-check=full --show-reachable=yes --track-origins=yes

EXEC=viewFactoryTests

all: $(SRC) $(EXEC)
	
$(LIB).o: 
	$(foreach lib,$(LIB),\
		echo -n $(dir);\
		$(CC) $(CCFLAGS) -c $(SRC)$(lib).cpp; \
		echo Listo"\n";)
	
$(EXEC): $(EXEC).cpp $(LIB).o
	$(CC) $(CCFLAGS) $(LIBS_LOCAL) $(EXEC).cpp -o $(EXEC) $(LIBS)
	cp $(EXEC) ../../
	
run: $(EXEC)
	cd $(PROYECT_DIR);\
	./$(EXEC)
	
test: 
	cd $(PROYECT_DIR);\
	$(VFLAGS) ./$(EXEC)
	
clean:
	rm $(EXEC) *.o;\
	cd $(PROYECT_DIR);\
	rm $(EXEC)
	
