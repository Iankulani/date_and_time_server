#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define MAX_BUFFER_SIZE 1024

void start_server() {
    char host[50];
    int port;
    
    // Prompt for host and port
    printf("Enter the server's host:");
    scanf("%s", host);
    printf("Enter the port number:");
    scanf("%d", &port);
    
    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }
    
    // Set up the server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(host);
    
    // Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        close(server_socket);
        exit(1);
    }
    
    // Listen for incoming connections
    if (listen(server_socket, 1) == -1) {
        perror("Listen failed");
        close(server_socket);
        exit(1);
    }
    
    printf("Server started on %s:%d, waiting for a connection...\n", host, port);
    
    while (1) {
        // Accept a client connection
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }
        
        printf("Connection established with client\n");
        
        // Get current date and time
        time_t t;
        struct tm *tm_info;
        char time_buffer[26];
        
        time(&t);
        tm_info = localtime(&t);
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", tm_info);
        
        // Send the current date and time to the client
        send(client_socket, time_buffer, strlen(time_buffer), 0);
        
        // Close the connection
        close(client_socket);
    }
    
    // Close the server socket
    close(server_socket);
}

int main() {
    start_server();
    return 0;
}
