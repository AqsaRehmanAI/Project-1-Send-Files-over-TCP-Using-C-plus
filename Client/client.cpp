#include <iostream>
#include <fstream>
#include <winsock2.h>

#define PORT 8080
#define SIZE 8192  // Increased buffer size to handle larger files (e.g., 100 MB)

// Inside the send_file function
void send_file(SOCKET sockfd, const char* originalFilename) {
    int n;
    char data[SIZE] = {0};

    std::ifstream inputFile(originalFilename, std::ios::in | std::ios::binary);
    if (!inputFile.is_open()) {
        perror("[-]Error opening file.");
        exit(1);
    }

    // Send the filename to the server
    if (send(sockfd, originalFilename, strlen(originalFilename), 0) == SOCKET_ERROR) {
        perror("[-]Error in sending filename.");
        exit(1);
    }

    while (inputFile.good()) {
        inputFile.read(data, SIZE);
        n = static_cast<int>(inputFile.gcount());

        if (n > 0) {
            if (send(sockfd, data, n, 0) == SOCKET_ERROR) {
                perror("[-]Error in sending file.");
                exit(1);
            }
        }
    }

    inputFile.close();
    std::cout << "[+]File '" << originalFilename << "' sent successfully.\n";
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("[-]Error initializing Winsock.");
        exit(1);
    }

    const char* ip = "127.0.0.1";
    int port = PORT;
    int e;

    SOCKET sockfd;
    struct sockaddr_in serverAddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        perror("[-]Error in socket");
        WSACleanup();
        exit(1);
    }
    std::cout << "[+]Client socket created successfully.\n";

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    e = connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (e == SOCKET_ERROR) {
        perror("[-]Error in connect");
        closesocket(sockfd);
        WSACleanup();
        exit(1);
    }
    std::cout << "[+]Connected to Server.\n";

    char filename[256];  // Buffer to store the filename
    std::cout << "Enter the filename (e.g., myfile.txt): ";
    std::cin.getline(filename, sizeof(filename));

    send_file(sockfd, filename);

    std::cout << "[+]Closing the connection.\n";
    closesocket(sockfd);
    WSACleanup();

    return 0;
}

