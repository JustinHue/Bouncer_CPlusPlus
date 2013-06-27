#include "world.h"

World::World(Map *map) {
    maps = new std::vector<Map *>(); 
    maps->push_back(map);
    curmap = 0;
}

World::~World() {
    
}
        
void World::addMap(Map *map) {
    maps->push_back(map);
}

void World::render(BITMAP *g2d) {
     
}

void World::update(const double delta) {
     
}

