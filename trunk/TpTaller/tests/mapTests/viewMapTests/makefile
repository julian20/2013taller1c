
INCLUDES=../../../includes

CC= g++
CCFLAGS= -g -Wall -pedantic -I$(INCLUDES) 


LIBS=`sdl-config --cflags --libs` -L/usr/local/lib -lSDL -lSDL_image -lSDL_gfx -lSDL_mixer -lyaml-cpp

LIBS_LOCAL = EntityViewMap.o Coordinates.o EntityView.o Entity.o Vector2.o Base.o Power.o Speed.o

SRC=../../../src/

LIB= view/map/EntityViewMap model/entityProperties/Coordinates view/entities/EntityView /model/entities/Entity model/Vector2 model/entityProperties/Base model/entityProperties/Power model/entityProperties/Speed

#Directorio base del proyecto relativo a este makefile
PROYECT_DIR = ../../

VFLAGS= valgrind --leak-check=full --show-reachable=yes --track-origins=yes

EXEC=entityViewMapTests

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
	
