build:
	gcc -Wall -Wextra inventory_fixed.c -o inventory

debug:
	gcc -g inventory_fixed.c -o inventory

memcheck:
	valgrind --leak-check=full --show-leak-kinds=all ./inventory

clean:
	rm -f inventory