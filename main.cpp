#include <allegro.h>
#include "game.h"

#define MOUSE_MAX 2

volatile int close_button_pressed = FALSE;
volatile int __FRAMES_COUNTER = 0;

void __close_button_handler() {
    close_button_pressed = TRUE;
}
END_OF_FUNCTION(__close_button_handler)
      
void __fps_handler() {
     ++__FRAMES_COUNTER;
}
END_OF_FUNCTION(__fps_handler)

void __abort_on_error(const char *message) {
     allegro_message(message);
     exit(-1);
}

int main() {
    
    /*
     Initialize System Variables
     */
    int _CURFPS = 0, _MAXFPS = 0;
    bool _KEY_STATES[KEY_MAX];
    bool _MOUSE_STATES[MOUSE_MAX];
    
    
    /*
      Read from config file
     */
     
    // [System]
    set_config_file("__bouncer/bouncer.cfg");  
            
    install_allegro(get_config_int("system", "system_id", SYSTEM_AUTODETECT), 
                    get_config_int("system", "errno", 1) == 1 ? &errno : NULL, 
                    get_config_int("system", "atexit", 1) == 1 ? atexit : NULL); 
                    
    set_window_title(get_config_string("system", "window_title", ""));
     
    if (get_config_int("system", "set_close_button", 1)) {
          LOCK_FUNCTION(__close_button_handler);
          set_close_button_callback(__close_button_handler);
    }
      
    // [Graphics]
     if (get_config_int("graphics", "install", 1)) {
        _CURFPS = get_config_int("graphics", "fps", 60);
        _MAXFPS = get_config_int("graphics", "maxfps", 60);
         
        install_int_ex(__fps_handler, MSEC_TO_TIMER(_CURFPS));
        LOCK_VARIABLE(__FPS_COUNTER);
        LOCK_FUNCTION(__fps_handler);
      
        set_color_depth(get_config_int("graphics", "color_depth", 32));
        request_refresh_rate(get_config_int("graphics", "refresh_rate", 60));
        
        if (set_gfx_mode(get_config_int("graphics", "gfxmode", 0), 
                         get_config_int("graphics", "reswidth", 800), 
                         get_config_int("graphics", "resheight", 600), 0, 0) != 0) {
           __abort_on_error("Couldn't set a 32 bit color resolution");
        }
        if (get_refresh_rate() != 60) {
           __abort_on_error("Couldn't set refresh rate to 60Hz!");
        }      
     }
         
    // [KEYBOARD]
    if (get_config_int("keyboard", "install", 1)) {
        install_keyboard();
    }
            
    // [MOUSE]
    if (get_config_int("mouse", "install", 1)) {
       install_mouse();
    }   
    
    // [TIMER]
    if (get_config_int("timer", "install", 1)) {
       install_timer();
    }
        
    // [SOUND]
    if (get_config_int("sound", "install", 1)) {}
    
    
    /*
     Initialize Game Variables
     */
    BITMAP *g2d = create_bitmap(SCREEN_W, SCREEN_H); 
    
    Game::init();
    
    // Main loop
    while (!close_button_pressed) {
        
        if (__FRAMES_COUNTER > 0) {
            int lastTickCount = __FRAMES_COUNTER;
            __FRAMES_COUNTER = 0;
              
            clear_keybuf();
            // Call Key/Mouse functions
            for (int keyCount = 0; keyCount < KEY_MAX; ++keyCount) {
                if (key[keyCount]) {
                    if (_KEY_STATES[keyCount]) {
                        Game::keyDown(key[keyCount]); 
                    } else {
                        Game::keyDownPressed(key[keyCount]); 
                        _KEY_STATES[keyCount] = true;
                    }
                } else {
                    if (!_KEY_STATES[keyCount]) {
                        Game::keyUp(key[keyCount]);   
                    } else {
                        Game::keyUpPressed(key[keyCount]);   
                        _KEY_STATES[keyCount] = false;
                    }
                }
            }
              
            if (mouse_b & 1) {
                if (_MOUSE_STATES[0]) {
                    Game::mouseDown(mouse_b & 1, mouse_x, mouse_y);
                } else {
                    Game::mouseDownPressed(mouse_b & 1, mouse_x, mouse_y);
                    _MOUSE_STATES[0] = true;
                }
            } else { 
                if (_MOUSE_STATES[0]) {
                    Game::mouseUp(mouse_b & 1, mouse_x, mouse_y);
                } else {
                    Game::mouseUpPressed(mouse_b & 1, mouse_x, mouse_y);
                    _MOUSE_STATES[0] = false;
                }   
            }
              
            if (!(mouse_b & 2)) {
                if (_MOUSE_STATES[1]) {
                    Game::mouseUp(mouse_b & 2, mouse_x, mouse_y);
                } else {
                    Game::mouseUpPressed(mouse_b & 2, mouse_x, mouse_y);
                    _MOUSE_STATES[1] = false;
                }
            } else { 
                if (_MOUSE_STATES[1]) {
                    Game::mouseDown(mouse_b & 2, mouse_x, mouse_y);
                } else {
                    Game::mouseDownPressed(mouse_b & 2, mouse_x, mouse_y);
                    _MOUSE_STATES[1] = true;
                }   
            }
              
            Game::update(lastTickCount * ((double)1000 / _CURFPS)); 
            Game::render(g2d);
            draw_sprite(screen, g2d, 0, 0);
            
            // Recalculate FPS
            if (lastTickCount == __FRAMES_COUNTER) {
                if (_CURFPS + lastTickCount < _MAXFPS) {
                    _CURFPS += lastTickCount;      
                } else {
                    _CURFPS = _MAXFPS;  
                }
            } else {
                if (_CURFPS - lastTickCount <= 0) {
                    _CURFPS = 1;
                } else {
                    _CURFPS -= lastTickCount;
                } 
            }
            install_int_ex(__fps_handler, MSEC_TO_TIMER(_CURFPS)); 
               
        } else {
            rest(1);
        } 
                         
    }
    
    Game::deinit();
    
    allegro_exit();
     
    return 0;
}
END_OF_MAIN()
