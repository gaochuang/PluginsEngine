#include "signalMonitor.hpp"

#include <sys/signalfd.h>
#include <signal.h>
#include <iostream>

#include <cstring>
#include <stddef.h>
#include <sys/types.h>

namespace reactorFramework
{

SignalMonitor::SignalMonitor(EventLoop* eventLoop):loop(eventLoop),signalFd(createSignalFd()),event(new Event(loop, signalFd))
{
    loop->addEvent(event);
    event->setReadCallback(std::bind(&SignalMonitor::handleSignal, this));
    event->enableReading(true);
}

SignalMonitor::~SignalMonitor()
{
     if (signalFd != -1) 
     {
        ::close(signalFd);
    }
}

int SignalMonitor::createSignalFd()
{
    //清空信号集
    ::sigemptyset(&mask);
    const int ret(::signalfd(-1, &mask, SFD_NONBLOCK | SFD_CLOEXEC));
    if (ret == -1)
    {
        std::cout << "signalfd error" << ::strerror(errno)<<std::endl;
    }
    return ret;
}

void SignalMonitor::add(int signal, const SignalHandler& sh)
{
    addImpl(signal, sh);
}

void SignalMonitor::del(int signal)
{  
    const auto it = map.find(signal);
    if (it == map.end())
    {
        std::cerr << "signal not found" << std::endl;
        return;
    }

    map.erase(it);
    sigdelset(&mask, signal);
    ::signalfd(signalFd, &mask, 0);
    //删除这个序号
    unblock(signal);
}

void SignalMonitor::block(int signal)
{
    sigset_t sset;
    sigemptyset(&sset);
    sigaddset(&sset, signal);

    const int ret(::pthread_sigmask(SIG_BLOCK, &sset, nullptr));
    if(ret < 0)
    {
        std::cout << "pthread_sigmask error" << ::strerror(errno)<<std::endl;
    }   
}

void SignalMonitor::unblock(int signal)
{
    sigset_t sset;
    sigemptyset(&sset);
    sigaddset(&sset, signal);

    const int ret(::pthread_sigmask(SIG_UNBLOCK, &sset, nullptr));
    if(ret < 0)
    {
        std::cout << "pthread_sigmask error" << ::strerror(errno)<<std::endl;
    }   
}

void SignalMonitor::addImpl(int signal, SignalHandler sh)
{
    if(map.find(signal) != map.end())
    {
        std::cout << "signal already exists" << std::endl;
        return;
    }

   //设置进程屏蔽字，让signalfd接管这个信号
    block(signal);
    ::sigaddset(&mask, signal);
    ::signalfd(signalFd, &mask, 0);

    map.insert(std::make_pair(signal, sh));
}

void SignalMonitor::handleSignal()
{
    signalfd_siginfo si;
    ::read(signalFd, &si, sizeof(si));
    //获取这个信号
    const auto it  = map.find(si.ssi_signo);
    if (it != map.end())
    {
        it->second();
    }
}

}
