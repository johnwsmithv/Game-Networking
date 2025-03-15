#include <gtest/gtest.h>

#include "ClientHelpers.hpp"

TEST(FormatMessageToServer, Garbage) {
    const auto output = formattedMessageToServer("vonsdvnsdljknvl");

    EXPECT_EQ(output, "");
}

TEST(FormatMessageToServer, newPlayer) {
    const auto output = formattedMessageToServer("/newPlayer");

    EXPECT_EQ(output, "{\"Event\":\"New_Player\"}");
}

TEST(FormatMessageToServer, newPlayerMissingBackslash) {
    const auto output = formattedMessageToServer("newPlayer");

    EXPECT_EQ(output, "");
}

TEST(FormatMessageToServer, moveXWithoutPosition) {
    const auto output = formattedMessageToServer("/moveX");

    EXPECT_EQ(output, "");
}

TEST(FormatMessageToServer, moveXWithPositivePosition) {
    const auto output = formattedMessageToServer("/moveX 10");

    EXPECT_EQ(output, "{\"Event\":\"Move_X\",\"Location\":{\"x\":10}}");
}

TEST(FormatMessageToServer, moveXWithNegativePosition) {
    const auto output = formattedMessageToServer("/moveX -10");

    EXPECT_EQ(output, "{\"Event\":\"Move_X\",\"Location\":{\"x\":-10}}");
}

TEST(FormatMessageToServer, moveXWithOverflow) {
    const auto output = formattedMessageToServer("/moveX 1000000000000000000000");

    EXPECT_EQ(output, "");
}

TEST(FormatMessageToServer, moveXWithUnderflow) {
    const auto output = formattedMessageToServer("/moveX -1000000000000000000000");

    EXPECT_EQ(output, "");
}

TEST(FormatMessageToServer, moveXGarbageInput) {
    const auto output = formattedMessageToServer("/moveX Bruh");

    EXPECT_EQ(output, "");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}