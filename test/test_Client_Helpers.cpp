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

TEST(FormatMessageToServer, moveYGoodInput) {
    const auto output = formattedMessageToServer("/moveY 25");

    EXPECT_EQ(output, "{\"Event\":\"Move_Y\",\"Location\":{\"y\":25}}");
}

TEST(FormatMessageToServer, moveZGoodInput) {
    const auto output = formattedMessageToServer("/moveZ 25");

    EXPECT_EQ(output, "{\"Event\":\"Move_Z\",\"Location\":{\"z\":25}}");
}

TEST(FormatMessageToServer, sneakyInputs) {
    const auto outputx = formattedMessageToServer("/movex 25");
    const auto outputy = formattedMessageToServer("/movey 25");
    const auto outputz = formattedMessageToServer("/movez 25");

    EXPECT_EQ(outputx, "");
    EXPECT_EQ(outputy, "");
    EXPECT_EQ(outputz, "");
}

TEST(Trim, Empty_Input) {
    std::string in = "";
    trim(in);
    EXPECT_EQ(in, "");
}

TEST(Trim, Spaces_Input) {
    std::string in = "    ";
    trim(in);
    EXPECT_EQ(in, "");
}

TEST(Trim, Front_Spaces_Input) {
    std::string in = "    abcd";
    trim(in);
    EXPECT_EQ(in, "abcd");
}

TEST(Trim, Back_Spaces_Input) {
    std::string in = "abcd    ";
    trim(in);
    EXPECT_EQ(in, "abcd");
}

TEST(FormatMessageToServer, NonASCII_Username) {
    ::testing::internal::CaptureStderr();

    const auto output = formattedMessageToServer("/changeUsername 人口");

    std::string errorMessage = ::testing::internal::GetCapturedStderr();

    EXPECT_EQ(output, "");
    EXPECT_EQ(errorMessage, "Client Error: Username can only have alpha-numeric characters.\n");
}

TEST(FormatMessageToServer, Username_With_Spaces) {
    const auto output = formattedMessageToServer("/changeUsername   helloThere  ");

    EXPECT_EQ(output, "{\"Event\":\"Change_Username\",\"Username\":\"helloThere\"}");
}


TEST(FormatMessageToServer, Username_With_Spaces_Between_Valid_Characters) {
    ::testing::internal::CaptureStderr();

    const auto output = formattedMessageToServer("/changeUsername   hello There  ");

    std::string errorMessage = ::testing::internal::GetCapturedStderr();

    EXPECT_EQ(output, "");
    EXPECT_EQ(errorMessage, "Client Error: Username can only have alpha-numeric characters.\n");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}