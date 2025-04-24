CFLAGS := -Wpointer-arith -Wall
# changer GFL2 en GFL3 pour version 3D
PFLAGS := $(incGFL3) -I./include
LFLAGS := $(libGFL3)

CC = g++
STD = -std=c++11
# LFLAGS +=++            # PROBLEME DE COMPILATION

# optim ou debug
ifeq ($(GDB),1)
	CFLAGS += -g
	# version gdb de la libgfl
	LFLAGS +=.gdb
else
	CFLAGS += -O2
endif

SRC = src/
INCLUDE = include/

ALL = Flag

DEPS = Link.o PMat.o Point.o Vect.o SpacialHashGrid.o

%.o : $(SRC)%.cpp
	@echo "module $@"
	@$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@
	@echo "------------------------"

# règle générique de création de l'executable sans dépendance
% : %.o $(DEPS)
	@echo "assemblage [$^]->$@"
	@$(CC) $^ $(LFLAGS) -o $@
	@echo "------------------------"

all : $(ALL)

.PHONY : clean cleanall ?

# informations sur les paramètres de compilation
? :
	@echo "---------compilation informations----------"
	@echo "	compilateur : $(CC)		 "
	@echo "	standard		: $(STD)	 "
	@echo "	PFLAGS			: $(PFLAGS)"
	@echo "	CFLAGS			: $(CFLAGS)"
	@echo "	LFLAGS			: $(LFLAGS)"

clean :
	@rm -f *.o
	
cleanall :
	@rm -f *.o $(ALL)
