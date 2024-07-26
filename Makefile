CXX = g++
CXXFLAGS = -Wall -Wextra -pthread -O2 -fPIC -std=c++11

LIBNAME = ractor

INCLUDES = -Iinclude/event -Iinclude/thread -Iinclude/timer -Iinclude/net/tcp -Iinclude/signalMonitor -Iinclude/net/http

SRCS = src/event/epoll.cpp \
       src/event/eventfd.cpp \
	   src/event/event.cpp \
	   src/event/eventCtrl.cpp \
	   src/event/eventLoop.cpp \
	   src/thread/thread.cpp \
	   src/thread/eventThreadPool.cpp \
	   src/thread/eventThread.cpp \
	   src/timer/timer.cpp \
	   src/timer/timerQueue.cpp \
	   src/net/tcp/buffer.cpp \
	   src/net/tcp/socketOperation.cpp \
	   src/net/tcp/socketAddr.cpp \
	   src/net/tcp/socket.cpp \
	   src/net/tcp/tcpAccept.cpp\
	   src/net/tcp/tcpConnect.cpp \
	   src/net/tcp/tcpServer.cpp \
	   src/signalMonitor/signalMonitor.cpp \
	   src/net/http/httpContext.cpp\
	   src/net/http/httpRequest.cpp\
	   src/net/http/httpResponse.cpp\
	   src/net/http/httpServer.cpp

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
