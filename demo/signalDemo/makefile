
TARGET = signalMonitorDemo

SRC = main.cpp

CXX = g++

CXXFLAGS = -Wall -pthread -g -std=c++11 

LIBDIR = ../../
LIBS = $(LIBDIR)/libractor.a


INCLUDES = -I../../include/event -I../../include/timer -I../../include/plugin  -I../../include/signalMonitor -I../../include/engine -I../../include/comApi 


all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -L$(LIBDIR) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
