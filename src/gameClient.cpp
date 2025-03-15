#include <iostream>
#include <memory> // std::unique_ptr
#include <ostream>

#include <strings.h>    // bzero
#include <sys/socket.h> // sockaddr_in on Linux
#include <arpa/inet.h>  // Also needed for sockaddr works...
#include <errno.h>      // For errno
#include <unistd.h>     // STDIN_FILENO
#include <cstring>      //memset

#include <nlohmann/json.hpp>

#include "Player.hpp"

using json = nlohmann::json;

struct Client {
    int fd;
    struct sockaddr_in server;

    std::string uuid;
    Player player;

    Client()
        : player(Player(0, Location(0, 0, 0)))
    {}
};

std::ostream &operator<<(std::ostream &os, const std::unique_ptr<Client> &client) {
    os << "UUID: " << client->uuid
       << "\tPlayer(Health: " << client->player.getPlayerHealth()
       << ", Location(" << client->player.getPlayerLocation().x
       << ", " << client->player.getPlayerLocation().y
       << ", " << client->player.getPlayerLocation().z
       << ")\n";
    return os;
}

// I am the client!!!
std::unique_ptr<Client> client = nullptr;

void sendToServer(std::unique_ptr<Client> &client, const std::string &message) {
    sendto(client->fd, message.c_str(), message.size(), 0, (struct sockaddr *)&(client->server), sizeof(client->server));
}

void connectToServer(std::unique_ptr<Client> &client, const std::string &ip, const int portNum) {
    client->fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (client->fd < 0) {
        std::cerr << "Cannot open socket (" + errno << ")...\r\n";
    }

    bzero(&client->server, sizeof(client->server));
    client->server.sin_family = AF_INET;
    client->server.sin_port = htons(portNum);

    inet_pton(AF_INET, ip.c_str(), &(client->server.sin_addr));
}

int main() {
    // TODO: Change to not be hard coded...
    constexpr int serverPort = 3000;
    const std::string serverIp = "127.0.0.1";

    client = std::make_unique<Client>();

    // Connecting the client to the server!
    connectToServer(client, serverIp, serverPort);

    while (true) {
        std::string clientInput = "";

        fd_set fileDescSet;
        int maxFd, ready = 0;

        FD_ZERO(&fileDescSet);
        FD_SET(STDIN_FILENO, &fileDescSet);
        FD_SET(client->fd, &fileDescSet);

        maxFd = STDIN_FILENO > client->fd ? STDIN_FILENO : client->fd;

        // TODO: Add a timeout????
        ready = select(maxFd + 1, &fileDescSet, NULL, NULL, NULL);
        if (ready == -1) {
            perror("Error with select...\n");
            exit(EXIT_FAILURE);
        } else if (ready == 0) {
            // If we implement timout...
            std::cerr << "Ready is 0...\n";
        } else {
            if (FD_ISSET(STDIN_FILENO, &fileDescSet)) {
                // We want to get the clients input
                std::getline(std::cin, clientInput);
                sendToServer(client, clientInput);

                // Receive response from Server...
                char buf[1024];
                memset(&buf, 0, sizeof(buf));
                struct sockaddr_in src;
                socklen_t srcSize = sizeof(src);
                int rlen = recvfrom(client->fd, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&src, &srcSize);

                // Always check the return value!
                if (rlen == -1) {
                    std::cerr << "Something went wrong: " << strerror(errno) << "...\n";
                }

                if (clientInput.starts_with("/newPlayer")) {
                    json initialState = json::parse(buf);

                    client->uuid = initialState["UUID"];
                    client->player.updatePlayerHealth(initialState["Health"]);
                    client->player.updatePlayerXLocation(initialState["Location"]["x"]);
                    client->player.updatePlayerYLocation(initialState["Location"]["y"]);
                    client->player.updatePlayerZLocation(initialState["Location"]["z"]);

                    std::cout << "Updated player state:\n"
                              << client << "\n";
                }

                if (std::string(buf).compare("+OK Client QUIT") == 0) {
                    std::cout << "Client: Closing connection with Server...\n";
                    break;
                }

                std::cout << buf << "\n";
            }

            if (FD_ISSET(client->fd, &fileDescSet)) {
                char buf[1024];
                struct sockaddr_in src;
                socklen_t srcSize = sizeof(src);
                int rlen = recvfrom(client->fd, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&src, &srcSize);
                buf[rlen] = 0;

                std::cout << buf << "\n";
            }
        }
    }

    close(client->fd);

    return EXIT_SUCCESS;
}