
#ifndef __WORLD_H__
#define __WORLD_H__

#include <allegro.h>
#include <vector>
#include "map.h"

class World {
    private:
        std::vector<Map *> *maps;
        int curmap;
        
    public:
        
        World(Map *map);
        ~World();
        
        void addMap(Map *map);
        
        void render(BITMAP *g2d);
        void update(const double delta);
          
    
};

#endif
