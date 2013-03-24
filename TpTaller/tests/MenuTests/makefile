
INCLUDES=../../includes

CC= g++
CCFLAGS= -g -Wall -pedantic -I$(INCLUDES) 
LIBS=`sdl-config --cflags --libs` -L/usr/local/lib -lSDL -lSDL_image -lSDL_gfx -lSDL_mixer
SRC=../../src/
LIB=Menu

#Directorio base del proyecto relativo a este makefile
PROYECT_DIR = ../../

VFLAGS= valgrind --leak-check=full --show-reachable=yes --track-origins=yes

EXEC=menuTests

all: $(SRC) $(EXEC)
	
$(LIB).o: 
	$(CC) $(CCFLAGS) -c $(SRC)$(LIB).cpp
	
$(EXEC): $(EXEC).cpp $(LIB).o
	$(CC) $(CCFLAGS) $(LIB).o $(EXEC).cpp -o $(EXEC) $(LIBS)
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
	
