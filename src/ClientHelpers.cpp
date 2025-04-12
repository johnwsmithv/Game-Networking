#include "ClientHelpers.hpp"

#include <limits> // std::numeric_limits, etc
#include <cctype> // std::tolower, std::isaslum
#include <iostream> // std::cout, std::cerr

#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * @brief Checks to make sure that the input string has only alpha-numeric characters
 * 
 * @param str The string which we are checking
 * @return true 
 * @return false 
 */
bool isAlphanumeric(const std::string& str) {
    for (char c : str) {
        if (!std::isalnum(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

void trim(std::string& str) {
    while(!str.empty() && str.front() == ' ') {
        str.erase(str.begin());
    }

    while(!str.empty() && str.back() == ' ') {
        str.erase(str.end() - 1);
    }

    return;
}

std::string formattedMessageToServer(const std::string& userInput) {
    json toServer;
    if(userInput.starts_with("/newPlayer")) {
        toServer["Event"] = "New_Player";

        return toServer.dump();
    } else if(userInput.starts_with("/moveX") || userInput.starts_with("/moveY") || userInput.starts_with("/moveZ")) {
        const size_t idx = userInput.find_first_of(' ');

        if(idx == std::string::npos) {
            return "";
        }

        const std::string moveAmtStr = userInput.substr(idx + 1);
        const int moveAmt = std::atoi(moveAmtStr.c_str());

        if(moveAmt > std::numeric_limits<int>::max()) {
            return "";
        }

        if(moveAmt < std::numeric_limits<int>::min()) {
            return "";
        }

        if(std::isnan(moveAmt) || std::isinf(moveAmt)) {
            return "";
        }

        if(std::to_string(moveAmt).size() != moveAmtStr.size()) {
            // The user might have tried to overflow the input or gave it some garbage.
            return "";
        }

        // Since we know the input command is correct, we can just pull the direction out...
        assert(userInput.size() >= 6);
        const unsigned char upperDir = userInput[5];
        const unsigned char lowerDir = std::tolower(upperDir);

        // Not sure if I'm doing characters to std::string wrong, but this is the 
        // only way to not have their decimal representation shown in the string
        // and for a bunch of nulls to be shown in the Location key.
        std::string event = "Move_";
        event.push_back(upperDir);
        toServer["Event"] = event;
        toServer["Location"] = {};

        std::string lowerDirStr = "";
        lowerDirStr.push_back(lowerDir);
        toServer["Location"][lowerDirStr] = moveAmt;

        return toServer.dump();
    } else if(userInput.starts_with("/quit")) {
        toServer["Event"] = "Quit";

        return toServer.dump();
    } else if (userInput.starts_with("/changeUsername")) {
        const size_t idx = userInput.find_first_of(' ');

        if(idx == std::string::npos) {
            return "";
        }

        std::string username = userInput.substr(idx + 1);

        if(username.empty()) {
            return "";
        }

        trim(username);

        if(username.empty()) {
            return "";
        }

        if(isAlphanumeric(username)) {
            toServer["Event"] = "Change_Username";
            toServer["Username"] = username;

            return toServer.dump();
        } else {
            std::cerr << "Client Error: Username can only have alpha-numeric characters.\n";
            return "";
        }
    }  else if (userInput.starts_with("/createGame")) {
        const size_t idx = userInput.find_first_of(' ');

        if(idx == std::string::npos) {
            return "";
        }

        std::string gameName = userInput.substr(idx + 1);

        if(gameName.empty()) {
            return "";
        }

        trim(gameName);

        if(gameName.empty()) {
            return "";
        }

        if(isAlphanumeric(gameName)) {
            toServer["Event"] = "Create_Game";
            toServer["Game_Name"] = gameName;

            return toServer.dump();
        } else {
            std::cerr << "Client Error: Your gamename alpha-numeric characters.\n";
            return "";
        }
    } else if (userInput.starts_with("/listGames")) {
        toServer["Event"] = "List_Games";

        return toServer.dump();
    } else {
        // The user entered something that is not what we were expecting, so return an empty string
        return "";
    }
}