all: sort

.PHONY : all

sort: mysort.cpp
	g++ -pthread mysort.cpp -o mysort

counter: counter.cpp
	g++ -pthread counter.cpp -o counter

omp:	omp_example.cpp
	g++ omp_example.cpp	-fopenmp -o omp

clean:
	rm mysort
