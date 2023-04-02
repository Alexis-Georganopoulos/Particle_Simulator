# Definitions de macros

CC = gcc
CFLAGS = -Wall -g
CFILES = vector.c generator.c black_hole.c particle.c error.c sim.c graphic.c main.cpp
OFILES = vector.o generator.o black_hole.o particle.o error.o sim.o graphic.o main.o
LIBS   = -lglui -lglut -lGL -lGLU -lm -Llib -L/usr/X11R6/lib -lX11 -lXext -lXmu -lXi -lstdc++

# Definition des regles

all: simulation.x
	mv simulation.x ..
vector.o: vector.c vector.h
	$(CC) $(CFLAGS) -c $< -o $@

error.o: error.c error.h
	$(CC) $(CFLAGS) -c $< -o $@

graphic.o: graphic.c graphic.h
	$(CC) $(CFLAGS) -c $< -o $@
	
sim.o: sim.c generator.h black_hole.h particle.h error.h
	$(CC) $(CFLAGS) -c $< -o $@
	
main.o: main.cpp sim.h vector.h
	$(CC) $(CFLAGS) -c $< -o $@

generator.o: generator.c error.h vector.h
	$(CC) $(CFLAGS) -c $< -o $@

black_hole.o: black_hole.c error.h vector.h
	$(CC) $(CFLAGS) -c $< -o $@

particle.o: particle.c generator.h black_hole.h error.h vector.h
	$(CC) $(CFLAGS) -c $< -o $@
	
simulation.x: $(OFILES)
	$(CC) $(OFILES) $(LIBS) -o $@
	@/bin/rm -f *.o *.h.gch *.c~ *.h~
	
# Definitions de cibles particulieres	
	
depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CC) -MM $(CFLAGS) $(CFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ***"
	@/bin/rm -f *.o *.c~ *.h~

# -- Regles de dependances generees automatiquement
#
# DO NOT DELETE THIS LINE
generator.o: generator.c constantes.h tolerance.h error.h
black_hole.o: black_hole.c constantes.h tolerance.h error.h
particle.o: particle.c constantes.h tolerance.h error.h
error.o: error.c error.h
sim.o: sim.c constantes.h tolerance.h error.h generator.h black_hole.h \
 particle.h
main.o: main.cpp sim.h
