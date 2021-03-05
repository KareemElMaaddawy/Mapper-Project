#include "m2.h"
#include "m1.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
//function declarations
void drawMainCanvas(ezgl::renderer *g);
double x_from_lon(double lon);
double y_from_lat(double lat);
double lon_from_x(double x);
double lat_from_y(double lat);
void act_on_mouse_click(ezgl::application* app,   
                        GdkEventButton* event,
                        double x, double y); 
//variable declarations
struct intersection_data {
    LatLon position;
    std::string name;
    bool highlight = false;
}typedef intersection_data;

std::vector<intersection_data> intersections;
double avg_lat = 0;

//implementations
double x_from_lon(double lon){
    return lon * kDegreeToRadian * kEarthRadiusInMeters * std::cos(avg_lat * kDegreeToRadian);
}

double y_from_lat(double lat){
    return lat * kDegreeToRadian * kEarthRadiusInMeters;
}

double lon_from_x(double x){
    return x/(kDegreeToRadian * kEarthRadiusInMeters * std::cos(avg_lat * kDegreeToRadian));
}

double lat_from_y(double lon){
    return lon/(kDegreeToRadian * kEarthRadiusInMeters);
}



void act_on_mouse_click(ezgl::application* app,   
                        GdkEventButton* event,
                        double x, double y) {
  std::cout << "Mouse clicked at (" << x << "," << y << ")\n";
  std::cout << "Button clicked is " << event->button << std::endl;
  LatLon pos = LatLon(lat_from_y(y), lon_from_x(x)); 
  int id = findClosestIntersection(pos); 

  std::cout << "Closest Intersection: "
            << intersections[id].name << "\n"; 
  intersections[id].highlight = true;
  app->refresh_drawing();
}

void drawMap () {
    double max_lat = getIntersectionPosition(0).latitude(); 
    double min_lat = max_lat;
    double max_lon = getIntersectionPosition(0).longitude();
    double min_lon = max_lon;
    intersections.resize(getNumIntersections());

    for (int id = 0; id < getNumIntersections(); ++id) {  
      intersections[id].position = getIntersectionPosition(id);
      intersections[id].name = getIntersectionName(id);

      max_lat = std::max(max_lat, intersections[id].position.latitude());
      min_lat = std::min(min_lat, intersections[id].position.latitude());
      max_lon = std::max(max_lon, intersections[id].position.longitude());
      min_lon = std::min(min_lon, intersections[id].position.longitude());
    }
    
    avg_lat = (min_lat + max_lat)/2;

    ezgl::application::settings settings;
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";
    
    ezgl::application application(settings);
    
   
    
    ezgl::rectangle initial_world({x_from_lon(min_lon), y_from_lat(min_lat)},
                              {x_from_lon(max_lon), y_from_lat(max_lat)});
    application.add_canvas("MainCanvas", drawMainCanvas, initial_world);
    
    
    
    
    
    application.run(nullptr, nullptr, act_on_mouse_click, nullptr);
}

void drawMainCanvas(ezgl::renderer *g) { 
 
    
    for (int id = 0; id < getNumIntersections(); ++id) {
        intersections[id].position = getIntersectionPosition(id);   
        intersections[id].name = getIntersectionName(id);
    }
  
    for (int i = 0; i < intersections.size(); ++i) {
        float x = x_from_lon(intersections[i].position.longitude());
        float y = y_from_lat(intersections[i].position.latitude());
        
        if (intersections[i].highlight) {
            g->set_color(ezgl::RED);
          } else {
            g->set_color(ezgl::GREY_55);
          }
        
        float width = 100;
        float height = width;

        g->fill_rectangle({x, y}, {x + width, y + height});
    }
}