#include "spacegame.h"

int main() {
    std::unique_ptr<SpaceGame> game = std::make_unique<SpaceGame>();
    if (!game->init()) {
        return -1;
    }
    game->run();
    return 0;
}
