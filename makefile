OPTS=-Wall -Werror -std=c17 -O0 -g

all: blackjack

blackjack: blackjack.o
	gcc $(OPTS) $^ -o $@

blackjack.o: blackjack.c
	gcc $(OPTS) -c $<

valgrind: blackjack
	valgrind --leak-check=full ./blackjack

clean:
	rm -rf *.o blackjack