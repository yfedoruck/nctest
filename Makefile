IDIR =./include
CFLAGS=-I.
ncur: ncur.cpp
	#gcc -o out ncur.cpp -D_GNU_SOURCE -I/usr/include/ncursesw -lncursesw -ltinfo -I. -lform -lstdc++ -lcurl -lpanel -std=c++14
	gcc -o out  ncur.cpp helpers.cpp -I. -lncursesw -lformw -lstdc++ -lcurl -lpanelw -std=c++14
