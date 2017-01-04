tldmonitor:tldmonitor.c
	gcc -c date.c tldlist.c tldmonitor.c -W -Wall
	gcc date.o tldlist.o tldmonitor.o -o AP3Ex1FINAL
	valgrind --leak-resolution=high --leak-check=full ./AP3Ex1FINAL "01/01/2000" "01/01/2006" large.txt | sort -n