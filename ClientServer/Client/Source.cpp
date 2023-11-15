#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return 1;
    }

    // Create socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        return 1;
    }

    // Server address
    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2007);  // Change port if needed
    serverAddr.sin_addr.s_addr = inet_addr("26.14.233.142");  // Change IP address if needed

    // Connect to server
    if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) != 0) {
        std::cerr << "Failed to connect to server" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Send directory name to server
    std::string directoryName;
    std::cout << "Enter directory name: ";
    std::getline(std::cin, directoryName);

    send(clientSocket, directoryName.c_str(), directoryName.length(), 0);

    // Receive and print directory listing from server
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    std::string listing;

    while (true) {
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            break;
        }
        listing += buffer;
        memset(buffer, 0, sizeof(buffer));
    }

    std::cout << "Directory listing:\n" << listing << std::endl;

    // Close socket and cleanup
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}



/*#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <cstring>
#include <winsock2.h>
#pragma comment(lib, "WS2_32.lib")
using namespace std;

int main() {
    setlocale(0, "Russian");
    WORD sockVer;
    WSADATA wsaData;
    int retVal;
    sockVer = MAKEWORD(2, 2);
    WSAStartup(sockVer, &wsaData);

    SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSock == INVALID_SOCKET) {
        printf("Unable to create socket\n");
        WSACleanup();
        system("pause");
        return SOCKET_ERROR;
    }

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2003);
    addr.sin_addr.s_addr = inet_addr("26.14.233.142"); // Замените на IP-адрес сервера

    retVal = connect(clientSock, (LPSOCKADDR)&addr, sizeof(addr));
    if (retVal == SOCKET_ERROR) {
        printf("Unable to connect to the server\n");
        WSACleanup();
        system("pause");
        return SOCKET_ERROR;
    }

    char directoryName[256];
    cout << "Введите имя директории: ";
    cin.getline(directoryName, sizeof(directoryName));

    // Отправляем длину имени директории
    char dirSize[1];
    dirSize[0] = strlen(directoryName);
    retVal = send(clientSock, dirSize, 1, 0);
    if (retVal == SOCKET_ERROR) {
        printf("Unable to send data\n");
        WSACleanup();
        return SOCKET_ERROR;
    }

    // Отправляем имя директории
    retVal = send(clientSock, directoryName, strlen(directoryName), 0);
    if (retVal == SOCKET_ERROR) {
        printf("Unable to send data\n");
        WSACleanup();
        return SOCKET_ERROR;
    }

    char response[1024];
    retVal = recv(clientSock, response, sizeof(response), 0);
    if (retVal == SOCKET_ERROR) {
        printf("Unable to receive data\n");
        WSACleanup();
        return SOCKET_ERROR;
    }

    printf("Ответ от сервера: %s\n", response);

    closesocket(clientSock);
    WSACleanup();
    return 0;
}*/