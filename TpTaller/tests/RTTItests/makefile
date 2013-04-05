
INCLUDES=../../includes

CC= g++
CCFLAGS= -g -Wall -pedantic -I$(INCLUDES) 


LIBS=

LIBS_LOCAL =

SRC=./

LIB= 

#Directorio base del proyecto relativo a este makefile
PROYECT_DIR = ../../

VFLAGS= valgrind --leak-check=full --show-reachable=yes --track-origins=yes

EXEC=reflectionTests

all: $(SRC) $(EXEC)
	
$(LIB).o: 
	$(foreach lib,$(LIB),\
		echo -n $(dir);\
		$(CC) $(CCFLAGS) -c $(SRC)$(lib).cpp; \
		echo Listo"\n";)
	
	
$(EXEC): $(EXEC).cpp
	$(CC) $(CCFLAGS) $(LIBS_LOCAL) $(EXEC).cpp -o $(EXEC) $(LIBS)
	
run: $(EXEC)
	./$(EXEC)
	
test: 
	$(VFLAGS) ./$(EXEC)

val:
	valgrind --leak-check=full --show-reachable=yes ./$(EXEC)	
clean:
	rm $(EXEC) *.o;\
	cd $(PROYECT_DIR);\
	rm $(EXEC)
	
