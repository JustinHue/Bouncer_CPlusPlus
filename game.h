
#ifndef __GAME_H__
#define __GAME_H__

#include <allegro.h>
#include "world.h"
#include "map.h"

namespace Game
{
    
    World *world;
    
    void init() {
        Map *firstMap = new Map();
        world = new World(firstMap);
    }
    
    void deinit() {
        
    }
    
    void keyDownPressed(const int key) {
        
    }
    
    void keyDown(const int key) {
        
    }
    
    void keyUp(const int key) {
    
    }
    
    void keyUpPressed(const int key) {
        
    }
    
    void mouseUp(const int button, const int x, const int y) {
        
    }
    
    void mouseUpPressed(const int button, const int x, const int y) {
        
    }
    
    void mouseDown(const int button, const int x, const int y) {
        
    }
    
    void mouseDownPressed(const int button, const int x, const int y) {
        
    }
        
    void render(BITMAP *buffer) {
        world->render(buffer);
    }
    
    void update(const double delta) {
        world->update(delta);
    }
    
}

#endif
