all: sort counter

.PHONY : all

sort: mysort.cpp
	g++ -pthread mysort.cpp -o mysort

counter: counter.cpp
	g++ -pthread counter.cpp -o counter


clean:
	rm mysort counter
