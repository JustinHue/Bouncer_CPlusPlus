
#ifndef __MAP_H__
#define __MAP_H__

#include <allegro.h>


class Map {
    
    private:
        
        
        
    public:
        Map();
        ~Map();
        
        void render(BITMAP *g2d);
        void update(const double delta);
        
          
};

#endif
