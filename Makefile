output: main.c
	gcc -Wall -Werror -g -std=c99 main.c -o main

clean:
	rm -f main
