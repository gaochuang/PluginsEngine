CXX = g++
CXXFLAGS = -Wall -Wextra -pthread -O2 -fPIC -std=c++17
LIBNAME = commapiengine
INCLUDES = -Iinclude/private -Iinclude/comapi

PREFIX ?= /usr/local
LIBDIR = $(PREFIX)/lib64/comapi
INCLUDEDIR = $(PREFIX)/include/comapi

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
	   src/engine/engine.cpp \
	   src/plugin/plugin.cpp \
	   src/plugin/pluginCreator.cpp \
	   src/plugin/pluginDataStorage.cpp \
	   src/plugin/sharedLibrary.cpp \
	   src/net/tcp/buffer.cpp \
	   src/net/tcp/socketOperation.cpp \
	   src/net/tcp/socketAddr.cpp \
	   src/net/tcp/socket.cpp \
	   src/net/tcp/tcpAccept.cpp\
	   src/net/tcp/tcpConnect.cpp \
	   src/net/tcp/tcpServer.cpp \
	   src/signalMonitor/signalMonitor.cpp \
	   src/plugin/commonapi.cpp \
	   src/adapter/IOContextAdapter.cpp

OBJS = $(SRCS:.cpp=.o)

SHARED_LIB = lib$(LIBNAME).so

all: $(SHARED_LIB)

$(SHARED_LIB): $(OBJS)
	@echo "Creating shared library $@"
	$(CXX) -shared -o $@ $(OBJS)

src/%.o: src/%.cpp
	@echo "Compiling $< into $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

install: all
	mkdir -p $(LIBDIR) $(INCLUDEDIR)
	install -m 0755 $(SHARED_LIB) $(LIBDIR)
	@echo "Installing header files to $(INCLUDEDIR)"
	cp -r ./include/comapi/* $(INCLUDEDIR)

uninstall:
	rm -f $(LIBDIR)/$(SHARED_LIB)
	rm -rf $(INCLUDEDIR)/*

clean:
	@echo "Cleaning up"
	rm -f $(OBJS) $(SHARED_LIB)

.PHONY: all clean
