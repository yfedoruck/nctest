ncur: ncur.cpp
	gcc -o ncur ncur.cpp -I. -lncurses -lform -lstdc++ -lcurl -std=c++14
