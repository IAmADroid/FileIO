main: assign7.c
	gcc -g assign7.c -o main

clean:
	rm main

refresh:
	cp ./data/courses.dat ./courses.dat
