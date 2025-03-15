#include <iostream> // std::cout, etc
#include <unordered_map> // std::unordered_map
#include <string> // std::string, etc...
#include <random> // std::random_device, std::mt19937, std::uniform_int_distribution
#include <sstream> // std::stringstream
#include <iomanip> // std::setw, std::setfill
#include <cstring> //memset

#include <strings.h> // bzero
#include <sys/socket.h> // sockaddr_in on Linux
#include <arpa/inet.h> // Also needed for sockaddr works...
#include <errno.h> // For errno

#include <nlohmann/json.hpp>

#include "Player.hpp"

using json = nlohmann::json;

struct Client {
    struct sockaddr_in destination;
    std::string uuid;

    Player player;

    Client()
        : player(Player(100, Location(0, 0, 0)))
    {}
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

    // Clients IP + Port, Client struct
    std::unordered_map<std::string, Client> clients;
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

/**
 * @brief Wrapper to the "sendto" function. This allows us to communicate to the Client.
 * 
 * @param serverFd The file descriptor of the socket which the server is going to be sending this data from.
 * @param client The client information which we need to send this message to.
 * @param message The message which we want to send.
 */
void sendToClient(int serverFd, const Client& client, const std::string& message) {
    sendto(serverFd, message.c_str(), message.size(), 0, (struct sockaddr *)&(client.destination), sizeof(client.destination));
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

    // Infinite loop to listen for connections and the sorts...
    while(true) {
        struct sockaddr_in destination;
        socklen_t clientSrcLen = sizeof(destination);

        // The assignment specifies that the max we are going to receive is 1000
        // chars, but hey, let's make it a power of two!
        char buf[1024];
        memset(&buf, 0, sizeof(buf));

        // If I'm not mistaken, this is a blocking function and will only proceed once it receives
        // a connection
        const int rlen = recvfrom(
            thisServer.serverFd, buf, sizeof(buf) - 1, 0, 
            (struct sockaddr*)&destination, &clientSrcLen
        );

        // Always check the return value!
        if(rlen == -1) {
            std::cerr << "Something went wrong: " << strerror(errno) << "...\n";
        }

        std::string receivedMsg(buf);

        // Determine who sent the message, get their IP Address and Port #
        const std::string asciiIp(inet_ntoa(destination.sin_addr));
        const int fixedPort = ntohs(destination.sin_port);
        const std::string senderIp(asciiIp + ":" + std::to_string(fixedPort));

        // If we have not seen this client before, add them to the list...
        bool newClient = false;
        if(thisServer.clients.find(senderIp) == thisServer.clients.end()) {
            newClient = true;
            Client newClient;
            newClient.destination = destination;
            thisServer.clients[senderIp] = newClient;
        }
        Client& client = thisServer.clients[senderIp];

        std::cout << "Server: We received the following from the client => '" 
            << receivedMsg << "'\n";

        if(newClient) {
            // We want to create them a UUID.
            client.uuid = generateUUID();

            json initalJoin;
            initalJoin["UUID"] = client.uuid;
            initalJoin["Health"] = client.player.getPlayerHealth();
            initalJoin["Location"] = {};
            initalJoin["Location"]["x"] = client.player.getPlayerLocation().x;
            initalJoin["Location"]["y"] = client.player.getPlayerLocation().y;
            initalJoin["Location"]["z"] = client.player.getPlayerLocation().z;

            sendToClient(thisServer.serverFd, client, initalJoin.dump());
        } else {
            std::cout << "To be implemented...\n";
        }
    }
}
