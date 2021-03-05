#include "m2.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"

void drawMap(){
    ezgl::application::settings settings;
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";
    
    ezgl::application application(settings);
    
    
    
    
    
    
    
    
    application.run(nullptr, nullptr, nullptr, nullptr);
}