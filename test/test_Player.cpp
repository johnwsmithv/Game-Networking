#include <gtest/gtest.h>

#include "Player.hpp"

// Silly test, but this is really to set up the test infastructure
TEST(PlayerTesting, TestClass) {
    Player player(10, Location(12, 13, 14));

    EXPECT_EQ(player.getPlayerHealth(), 10);
    EXPECT_EQ(player.getPlayerLocation().x, 12);
    EXPECT_EQ(player.getPlayerLocation().y, 13);
    EXPECT_EQ(player.getPlayerLocation().z, 14);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}