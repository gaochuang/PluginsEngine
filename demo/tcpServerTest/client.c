#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 20000
#define NUM_CONNECTIONS 50000
#define MESSAGE "Hello, Server!"
#define SEND_INTERVAL_US 200 // 每隔1秒发送一次数据

typedef struct {
    int thread_num;
    unsigned long bytes_sent;      // 记录发送的字节数
    unsigned long bytes_received;  // 记录接收的字节数
} ThreadData;

// 客户端线程函数，创建TCP长连接并持续发送数据
void* client_thread(void* arg) {

    pthread_attr_t attr;
    ThreadData *data = (ThreadData *)arg;
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];
    ssize_t bytes;

    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 256 * 1024); // 设置栈大小为256
    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return NULL;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // 将IP地址从字符串转换为网络字节序
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        close(sock);
        return NULL;
    }

    // 尝试连接到服务器
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection Failed");
        close(sock);
        return NULL;
    }

    printf("Thread %d: Connected to server\n", data->thread_num);

    while (1) {
        // 向服务器发送消息
        bytes = send(sock, MESSAGE, strlen(MESSAGE), 0);
        if (bytes < 0) {
            perror("Send error");
            close(sock);
            return NULL;
        }

        data->bytes_sent += bytes; // 累加发送的字节数
        printf("Thread %d: Sent %ld bytes to server\n", data->thread_num, bytes);

        // 等待服务器响应（如果需要的话，可以去掉这部分）
        bytes = read(sock, buffer, sizeof(buffer));
        if (bytes > 0) {
            buffer[bytes] = '\0';
            data->bytes_received += bytes; // 累加接收的字节数
            printf("Thread %d: Received %ld bytes from server: %s\n", data->thread_num, bytes, buffer);
        } else if (bytes == 0) {
            printf("Thread %d: Server closed connection\n", data->thread_num);
            close(sock);
            return NULL;
        } else {
            perror("Read error");
            close(sock);
            return NULL;
        }

        // 等待一段时间再发送下一条消息
           usleep(SEND_INTERVAL_US);
    }

    // 通常情况下不会到达这里，因为这是一个无限循环
    close(sock);
    return NULL;
}

int main() {
    pthread_t threads[NUM_CONNECTIONS];
    ThreadData thread_data[NUM_CONNECTIONS];
    int i;

    // 初始化线程数据并创建多个线程
    for (i = 0; i < NUM_CONNECTIONS; ++i) {
        thread_data[i].thread_num = i;
        thread_data[i].bytes_sent = 0;
        thread_data[i].bytes_received = 0;

        if (pthread_create(&threads[i], NULL, client_thread, &thread_data[i]) != 0) {
            perror("Failed to create thread");
        }
    }

    // 等待所有线程完成（实际上是无限等待，因为线程是无限循环）
    for (i = 0; i < NUM_CONNECTIONS; ++i) {
        pthread_join(threads[i], NULL);
        printf("Thread %d: Total bytes sent: %lu, Total bytes received: %lu\n",
               thread_data[i].thread_num, thread_data[i].bytes_sent, thread_data[i].bytes_received);
    }

    printf("All threads completed.\n");
    return 0;
}
