#include "SpaceDemo.hpp"

int main(int /*argc*/, char** argv) {
    SpaceGame game{argv};
    game.setup();
    return game.loop();
}
