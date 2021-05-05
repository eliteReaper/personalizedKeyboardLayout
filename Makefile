SHELL := /bin/bash
f=main
run: compile
	time ./${f}.out
compile:
	g++ -I /mnt/d/OpenSource/boost -std=c++17 -pthread Key.cpp KeyBoardLayout.cpp GAInitiator.cpp ${f}.cpp -g -o ${f}.out
san: compileSanitize
	time ./${f}.out
compileSanitize:
	g++ -std=c++17 -Wshadow -Wall ${f}.cpp -g -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG -o ${f}.out 
clean:
	rm *.out