ncur: ncur.cpp
	gcc -o ncur ncur.cpp -I. -lncurses -lform -lstdc++ -lcurl -lpanel -std=c++14
