#include "m2.h"
#include "m1.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
void drawMainCanvas(ezgl::renderer *g);

struct intersection_data {
  LatLon position;
  std::string name;
}typedef intersection_data;


void drawMap () {
    

    ezgl::application::settings settings;
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";
    
    ezgl::application application(settings);
    
    
    ezgl::rectangle initial_world({0,0}, {1000,1000});
    application.add_canvas("MainCanvas", drawMainCanvas, initial_world);
    
    
    
    
    
    application.run(nullptr, nullptr, nullptr, nullptr);
}

void drawMainCanvas(ezgl::renderer *g) { 
  g->draw_rectangle({0, 0}, {1000, 1000});
 
  std::vector<intersection_data> intersections;


  intersections.resize(getNumIntersections());
  
  for (int id = 0; id < getNumIntersections(); ++id) {
    intersections[id].position = getIntersectionPosition(id);   
    intersections[id].name = getIntersectionName(id);
  }
  
  for (int i = 0; i < intersections.size(); ++i) {
    float x = intersections[i].position.longitude();
    float y = intersections[i].position.latitude();

    float width = 10;
    float height = width;

    g->fill_rectangle({x, y}, {x + width, y + height});
  }
}