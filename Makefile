# Makefile
# 

CC=g++
CPPFLAGS=-std=c++17 -Wall -s -O6 -c
TARGET=threadworker
#DEBUGFLAGS=-DMSS
#DEBUGLIBS=-lmss
#LIBS=

objects = worker.o threadworkerdemo.o

all: app

app: $(objects)
	$(CC) -o $(TARGET) $(objects) $(LIBS) $(DEBUGLIBS)

clean:
	$(RM) $(objects) $(TARGET)

include $(objects:.o)

$(objects): %.o: %.cpp
	$(CC) $(DEBUGFLAGS) $(CPPFLAGS) $< -o $@

%.d: %.cc
	$(SHELL) -ec '$(CC) -MM $(CPPFLAGS) $< \
    	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
      	[ -s $@ ] || rm -f $@'


