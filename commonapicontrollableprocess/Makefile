CXX = g++
CXXFLAGS = -Wall -Wextra -pthread -O2 -fPIC -std=c++17

LIBNAME = libcommonpicontrollableprocess

INCLUDES = -Iinclude/comapi/controllableProcess -Iinclude/comApi -Iinclude/plugin -Iplugins/commonapicontrollableprocess

SRCS = src/controllableProcess.cpp

OBJS = $(SRCS:.cpp=.o)

STATIC_LIB = lib$(LIBNAME).a
SHARED_LIB = lib$(LIBNAME).so

all: $(STATIC_LIB) $(SHARED_LIB)

$(STATIC_LIB): $(OBJS)
	@echo "Creating static library $@"
	ar rcs $@ $(OBJS)

$(SHARED_LIB): $(OBJS)
	@echo "Creating shared library $@"
	$(CXX) -shared -o $@ $(OBJS)

src/%.o: src/%.cpp
	@echo "Compiling $< into $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "Cleaning up"
	rm -f $(OBJS) $(STATIC_LIB) $(SHARED_LIB)

.PHONY: all clean
