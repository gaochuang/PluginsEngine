
TARGET = timerDemo

SRC = main.cpp

CXX = g++

CXXFLAGS = -Wall  -g -std=c++11

LIBDIR = ../../
LIBS = $(LIBDIR)/libractor.a


INCLUDES = -I../../include/event -I../../include/timer


all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -L$(LIBDIR) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
