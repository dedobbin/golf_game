#include "world.hpp"

std::vector<std::shared_ptr<Entity>> World::entities = {};
float World::gravity = 0.65;
int World::w = 10000000;
int World::h = 1000;
std::string World::name = "";