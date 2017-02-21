CC = g++    # On lnxsrv07 or 09, replace this with g32
LIBS = -lglut -lGL -lGLU
STD = -std=c++11

# On lnxsrv07 or 09, uncomment the following line
#STD = 

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

.PHONY: default all clean


all: Bugs

%.o: %.cpp $(HEADERS)
	$(CC) -c $(STD) $< -o $@

Bugs: $(OBJECTS) 
	$(CC) $(OBJECTS) $(LIBS) -o $@

clean:
	rm -f *.o
	rm -f Bugs
