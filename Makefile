main: main.c
	gcc -Wall -Wextra -o main main.c -L./libraries/raylib/ -l:libraylib.a -lm
