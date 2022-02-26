FLAGS=-Wall -g

all: cforth
	bin/main.out code grammar

cforth: cls main.o gram.o token.o
	gcc $(FLAGS) bin/main.o bin/gram.o bin/token.o -o bin/main.out

cls:
	clear

main.o: src/main.c
	gcc $(FLAGS) -c src/main.c -o bin/main.o

gram.o: slap/src/gram.c
	gcc $(FLAGS) -c slap/src/gram.c -o bin/gram.o

token.o: slap/src/token.c
	gcc $(FLAGS) -c slap/src/token.c -o bin/token.o

clean:
	rm main.o
	rm main.out
	clear