CC = g++
CFLAGS = -I ../includes
DEPS = trees_lock.h

all:	main.cpp	trees_lock.cpp
	$(CC) $(CFLAGS) main.cpp trees_lock.cpp -o contree -lpthread

clean:
	rm contree
