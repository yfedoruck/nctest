ncur: ncur.cpp
	gcc -o ncur ncur.cpp -I. -lncursesw -lform -lstdc++ -lcurl -lpanel -std=c++14
