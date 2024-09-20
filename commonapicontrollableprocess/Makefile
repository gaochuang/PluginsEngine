CXX = g++
CXXFLAGS = -Wall -Wextra -pthread -O2 -fPIC -std=c++17

LIBNAME = libcommonpicontrollableprocess

INCLUDES = -Iinclude/comapi/controllableProcess -I../include

SRCS = src/controllableProcess.cpp

OBJS = $(SRCS:.cpp=.o)

SHARED_LIB = $(LIBNAME).so

all: $(SHARED_LIB)

$(SHARED_LIB): $(OBJS)
	@echo "Creating shared library $@"
	$(CXX) -shared -o $@ $(OBJS)

src/%.o: src/%.cpp
	@echo "Compiling $< into $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "Cleaning up"
	rm -f $(OBJS) $(SHARED_LIB)

.PHONY: all clean
