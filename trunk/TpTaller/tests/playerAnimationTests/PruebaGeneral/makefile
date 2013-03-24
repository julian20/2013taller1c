
INCLUDES=../../../includes

CC= g++
CCFLAGS= -g -Wall -pedantic -I$(INCLUDES) 


LIBS=`sdl-config --cflags --libs` -L/usr/local/lib -lSDL -lSDL_image -lSDL_gfx -lSDL_mixer 

LIBS_LOCAL = PersonajeVista.o PersonajeControlador.o Personaje.o

SRC=../../../src/

LIB= view/personaje/PersonajeVista controller/personaje/PersonajeControlador model/personaje/Personaje

#Directorio base del proyecto relativo a este makefile
PROYECT_DIR = ../../../

VFLAGS= valgrind --leak-check=full --show-reachable=yes --track-origins=yes

EXEC=animationTests

all: $(SRC) $(EXEC)
	
$(LIB).o: 
	$(foreach lib,$(LIB),\
		echo -n $(dir)..."\n" ;\
		$(CC) $(CCFLAGS) -c $(SRC)$(lib).cpp; \
		echo Listo"\n";)
	
	
$(EXEC): $(EXEC).cpp $(LIB).o
	$(CC) $(CCFLAGS) $(LIBS_LOCAL) $(EXEC).cpp -o $(EXEC) $(LIBS)
	cp $(EXEC) ../../../
	
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
	
