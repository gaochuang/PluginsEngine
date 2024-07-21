
TARGET = tcpServer

SRC = main.cpp tcpServerTest.cpp

CXX = g++

CXXFLAGS = -Wall  -g -pthread -std=c++11

LIBDIR = ../../
LIBS = $(LIBDIR)/libractor.a


INCLUDES = -I../../include/event -I../../include/net/tcp -I../../include/timer -I../../include/thread


all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -L$(LIBDIR) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
