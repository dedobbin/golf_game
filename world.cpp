#include "world.hpp"

std::vector<std::shared_ptr<Entity>> World::entities = {};
float World::gravity = 0.65;