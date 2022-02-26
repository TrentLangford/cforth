
all: cforth
	bin/main.out code grammar

cforth: cls main.o gram.o token.o
	gcc bin/main.o bin/gram.o bin/token.o -o bin/main.out

cls:
	clear

main.o: src/main.c
	gcc -c src/main.c -o bin/main.o

gram.o: src/gram.c
	gcc -c src/gram.c -o bin/gram.o

token.o: src/token.c
	gcc -c src/token.c -o bin/token.o

clean:
	rm main.o
	rm main.out
	clear