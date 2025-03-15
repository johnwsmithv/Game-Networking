#include "ClientHelpers.hpp"

#include <limits>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string formattedMessageToServer(const std::string& userInput) {
    json toServer;
    if(userInput.starts_with("/newPlayer")) {
        toServer["Event"] = "New_Player";

        return toServer.dump();
    } else if(userInput.starts_with("/moveX")) {
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
            // The user might have tried to overflow the input!
            return "";
        }

        toServer["Event"] = "Move_X";
        toServer["Location"] = {};
        toServer["Location"]["x"] = moveAmt;

        return toServer.dump();
    } else {
        // The user entered something that is not what we were expecting, so return an empty string
        return "";
    }
}