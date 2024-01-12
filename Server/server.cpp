#include <iostream>
#include <fstream>
#include <winsock2.h>

#define PORT 8080
#define SIZE 1024


// Inside the receive_file function
void receive_file(SOCKET clientSocket, const char* originalFilename) {
    int n;
    char data[SIZE] = {0};

    // Extract the file extension from the original filename
    std::string extension = "";
    size_t dotPos = std::string(originalFilename).find_last_of(".");
    if (dotPos != std::string::npos) {
        extension = std::string(originalFilename).substr(dotPos + 1);
    }

    // Construct the new filename with the 'receive' prefix and the original extension
    std::string newFilename = "receive." + extension;

    std::ofstream outputFile(newFilename, std::ios::out | std::ios::binary);

    while ((n = recv(clientSocket, data, sizeof(data), 0)) > 0) {
        outputFile.write(data, n);
        memset(data, 0, SIZE);
    }

    if (n < 0) {
        perror("[-]Error in receiving file.");
    } else {
        std::cout << "[+]File received successfully and saved as '" << newFilename << "'.\n";
    }

    outputFile.close();
    closesocket(clientSocket);
}



int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("[-]Error initializing Winsock.");
        exit(1);
    }

    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        perror("[-]Error in socket");
        WSACleanup();
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        perror("[-]Error in bind");
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }
    std::cout << "[+]Binding successful.\n";

    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        perror("[-]Error in listen");
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }
    std::cout << "[+]Listening...\n";

    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientSocket == INVALID_SOCKET) {
        perror("[-]Error in accept");
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }
    std::cout << "[+]Connection accepted from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << "\n";

    char filename[SIZE]; // Buffer to store the received filename
    int filenameLength = recv(clientSocket, filename, SIZE, 0);
    if (filenameLength <= 0) {
        perror("[-]Error receiving filename.");
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }
    filename[filenameLength] = '\0'; // Null-terminate the received filename

    receive_file(clientSocket, filename); // Pass the received filename to receive_file


    std::cout << "[+]Closing the connection.\n";
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}

