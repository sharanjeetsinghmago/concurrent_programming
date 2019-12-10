CC = g++
CFLAGS = -I ../includes
DEPS = trees_lock.h

all:	main.cpp	trees_lock.cpp
	$(CC) $(CFLAGS) main.cpp trees_lock.cpp -o contree -lpthread

test:	unit_test.cpp	trees_lock.cpp
	$(CC) $(CFLAGS) unit_test.cpp trees_lock.cpp -o utest

clean:
	rm contree utest	
