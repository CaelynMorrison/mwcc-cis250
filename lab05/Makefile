filecopy: filecopy.c
	gcc -Wall -Wextra -o filecopy filecopy.c
debug: 
	gdb --args ./filecopy test.txt output.txt
memcheck:
	valgrind --leak-check=full --show-leak-kinds=all ./filecopy test.txt output.txt
clean:
	rm -f filecopy output.txt
