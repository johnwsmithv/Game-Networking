#include <iostream> // std::cout, etc
#include <unordered_map> // std::unordered_map
#include <string> // std::string, etc...
#include <random> // std::random_device, std::mt19937, std::uniform_int_distribution
#include <sstream> // std::stringstream
#include <iomanip> // std::setw, std::setfill

#include <strings.h> // bzero
#include <sys/socket.h> // sockaddr_in on Linux
#include <arpa/inet.h> // Also needed for sockaddr works...

#include "Player.hpp"

struct Client {
    struct sockaddr_in destination;
};

struct Server {
    std::string forwardingAddress;
    std::string bindingAddress;

    std::string forwardingIpAddress;
    std::string bindingIpAddress;

    int forwardingPort;
    int bindingPort;

    // This is going to be used to write to when we send out 
    // packets to other clients of servers
    int serverFd;
};

/**
 * @brief This is a function which is going to create a UUID for a player.
 * 
 * @note Thanks to ChatGPT for this function.
 * 
 * @return std::string The UUID
 */
std::string generateUUID() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);

    std::stringstream ss;
    ss << std::hex << std::setfill('0')
       << std::setw(8) << dis(gen) << "-"
       << std::setw(4) << (dis(gen) & 0xFFFF) << "-"
       << std::setw(4) << ((dis(gen) & 0x0FFF) | 0x4000) << "-"
       << std::setw(4) << ((dis(gen) & 0x3FFF) | 0x8000) << "-"
       << std::setw(12) << dis(gen);

    return ss.str();
}

// The key is going to be the players id which is going to come from the client,
// or maybe we'll assign it...
std::unordered_map<std::string, Player> players;

int main() {
    const int listeningPort = 3000;

    Server thisServer;

    // Assign a socket to a file descriptor
    thisServer.serverFd = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in servAddr;
    bzero(&servAddr, sizeof(servAddr));

    // htons is imporant since it will change the host byte order to the network byte order!
    // This is really important when sending messages across the networking since
    // most modern computers are little-endian and networking is big-endian
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htons(INADDR_ANY);
    servAddr.sin_port = htons(listeningPort);
  
    std::cout << "Server: We are listening on port " 
        << std::to_string(listeningPort) 
        << " for any incoming connections....\n";
    // Actually bind the file descriptor to a local address
    bind(thisServer.serverFd, (struct sockaddr*)&servAddr, sizeof(servAddr));
}
