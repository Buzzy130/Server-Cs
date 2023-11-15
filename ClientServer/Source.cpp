#include <iostream>
#include <winsock2.h>
#include <filesystem>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 102400
using namespace std;
void listFilesAndSubdirectories(const std::string& directoryPath, std::string& listing) {

    
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) 
    {
        if (std::filesystem::is_directory(entry)) {
            listing += "Directory: " + entry.path().filename().string() + "\n";
        }
        else if (std::filesystem::is_regular_file(entry)) {
            listing += "File: " + entry.path().filename().string() + "\n";
        }
    }
}


/* listFilesAndSubdirectories(const std::string& directoryPath, std::string& listing) {//рекурсивный обход выводит все папки и вайлы директории и подкаталога, как исправить хз
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (std::filesystem::is_directory(entry)) {
            listing += "Directory: " + entry.path().filename().string() + "\n";
            cout << listing << endl;
            listFilesAndSubdirectories(entry.path().string(), listing);  // Recursive call
        }
        else if (std::filesystem::is_regular_file(entry)) {
            listing += "File: " + entry.path().filename().string() + "\n";
        }
    }
}*/


int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return 1;
    }

    // Create socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        return 1;
    }

    // Bind socket to address
    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2007);  // Change port if needed
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen for connections" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening for connections..." << std::endl;

    // Accept connection from client
    SOCKADDR_IN clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to accept connection" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connected" << std::endl;

    // Receive directory name from client
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    std::string directoryName(buffer, bytesRead);

    // Get directory listing
    std::string listing;
    listFilesAndSubdirectories(directoryName, listing);//

    // Send directory listing to client
    send(clientSocket, listing.c_str(), listing.length(), 0);

    // Close client socket
    closesocket(clientSocket);

    // Close server socket and cleanup
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}

