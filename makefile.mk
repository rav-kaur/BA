#Do not edit the contents of this file.
warnings_BAD: *.c
	gcc -Werror -Wall -g -std=gnu99 -o 170469100_170384420_a04 *.c -lrt -lpthread
	
warnings_OK: *.c
	gcc -Wall -g -std=gnu99 -o 170469100_170384420_a04 *.c -lrt -lpthread
	
clean: *.c
	rm 170469100_170384420_a04
