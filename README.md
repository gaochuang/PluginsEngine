# ReactorFramework

`ReactorFramework` 是一个轻量级事件驱动框架，使用 C++ 实现，基于 Reactor 模式设计，支持定时任务、signalfd、TCP 服务和 HTTP 服务。

## 核心特性

1. **事件驱动架构**
   - **Reactor 模式**：通过事件循环等待和分发事件到相应的处理程序。
   - **EventLoop**：管理主事件循环，监听并分发事件。

2. **套接字管理**
   - **Socket**：封装网络套接字操作。
   - **SocketOperation**：提供 `bind`、`connect`、`accept`、`read` 和 `write` 等静态方法。

3. **缓冲区管理**
   - **Buffer**：高效管理 I/O 缓冲区，提供数据读写和模式查找方法。

4. **连接处理**
   - **TcpConnect**：管理单个 TCP 连接的创建、数据读写和关闭。

5. **信号监控**
   - **SignalMonitor**：封装信号监控和处理，集成到 `EventLoop` 中。

6. **TCP 服务**
   - **TcpServer**：实现和管理 TCP 服务器，处理连接和数据传输。

7. **HTTP 服务**
   - **HttpServer**：继承自 `TcpServer`，处理 HTTP 请求和响应。
     
