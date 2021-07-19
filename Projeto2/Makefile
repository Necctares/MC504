LDFLAGS = -pthread

.PHONY: all clean

all: barbearia

barbearia: semaforoBarbearia.c
	gcc -o barbearia semaforoBarbearia.c $(LDFLAGS)

barbearia.o: barbearia.c
	gcc -c barbearia.c $(LDFLAGS)

clean:
	rm -f *.o