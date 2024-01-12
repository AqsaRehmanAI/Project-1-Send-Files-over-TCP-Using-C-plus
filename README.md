# Socket Programming in C++ for File Transfer

## Overview

This project demonstrates the implementation of socket programming in C++ to achieve file transfer between a client and a server. It was developed as part of my PhD interview, showcasing my deep understanding of networking concepts and practical application.

## Key Features

- Reliable file transfer: Ensures accurate transmission of files of various sizes.
- Cross-platform compatibility: Designed to work on Windows operating systems.
- Robust error handling: Handles potential errors gracefully for a seamless user experience.
  
## Getting Started

Prerequisites:

- C++ compiler (e.g., g++)
- Windows operating system

Compilation:

1. Open a terminal in the project directory.
2. Compile the server code:


```bash
g++ -o server.exe server.cpp -lws2_32

g++ -o client.exe client.cpp -lws2_32
```


## Code Explanation

#### Key Code Snippets:

Server (server.cpp):

```bash
// Initialize Winsock for network communication
WSADATA wsaData;
if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    // ... error handling ...
}

// Create a socket
SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
// ...

// Bind the socket to a port
bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
// ...

// Listen for incoming connections
listen(serverSocket, 10);
// ...

// Accept a connection from a client
SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
// ...

// Receive the filename
recv(clientSocket, filename, SIZE, 0);
// ...

// Receive and store the file content
receive_file(clientSocket, filename);
// ...

// Close the connection
closesocket(clientSocket);
// ...
```





Server (server.cpp):

```bash
// Initialize Winsock
WSADATA wsaData;
if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    // ... error handling ...
}

// Create a socket
SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
// ...

// Connect to the server
connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
// ...

// Prompt the user for the filename to send
std::cout << "Enter the filename (e.g., myfile.txt): ";
std::cin.getline(filename, sizeof(filename));

// Send the filename and file content
send_file(sockfd, filename);
// ...

// Close the connection
closesocket(sockfd);
// ...
```


## Effort and Dedication
This project reflects my significant investment of time and effort to grasp the intricacies of socket programming and apply them effectively for file transfer. Key areas of focus include:

- Thorough understanding of networking concepts: Socket creation, binding, listening, accepting connections, data transmission, and error handling.
- Strategic code structuring: Clear separation of concerns between server and client components for maintainability.
- Dedication to error handling: Robust mechanisms to ensure a smooth user experience.
- Commitment to documentation: Comprehensive README.md file for clarity and understanding.



## Further Development

- Write another client application on a mobile platform (android or iOS or Raspberry PI).
