#include "ClientHelpers.hpp"

#include <limits>
#include <cctype> // std::tolower

#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string formattedMessageToServer(const std::string& userInput) {
    json toServer;
    if(userInput.starts_with("/newPlayer")) {
        toServer["Event"] = "New_Player";

        return toServer.dump();
    } else if(userInput.starts_with("/moveX") || userInput.starts_with("/moveY") || userInput.starts_with("/moveZ")) {
        const int idx = userInput.find_first_of(' ');

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
    } else {
        // The user entered something that is not what we were expecting, so return an empty string
        return "";
    }
}