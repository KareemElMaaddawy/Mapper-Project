#include "m2.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"

void drawMainCanvas (ezgl::renderer *g);

void drawMap(){
    ezgl::application::settings settings;
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";
    
    ezgl::application application(settings);
    
    
    ezgl::rectangle initial_world({0,0}, {1000,1000});
    application.add_canvas("MainCanvas", drawMainCanvas, initial_world);
    
    
    
    
    
    application.run(nullptr, nullptr, nullptr, nullptr);
}

void drawMainCanvas (ezgl::renderer *g){
    g -> draw_rectangle ({0,0},{1000,1000});
}