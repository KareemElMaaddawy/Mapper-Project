#include "m2.h"
#include "m1.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "globalHeader.h"
#include "string"

//function declarations

std::string MAP_PATHS[] = {
        "/cad2/ece297s/public/maps/beijing_china.streets.bin",
        "/cad2/ece297s/public/maps/cairo_egypt.streets.bin",
        "/cad2/ece297s/public/maps/cape-town_south-africa.streets.bin",
        "/cad2/ece297s/public/maps/golden-horseshoe_canada.streets.bin",
        "/cad2/ece297s/public/maps/hamilton_canada.streets.bin",
        "/cad2/ece297s/public/maps/hong-kong_china.streets.bin",
        "/cad2/ece297s/public/maps/iceland.streets.bin",
        "/cad2/ece297s/public/maps/interlaken_switzerland.streets.bin",
        "/cad2/ece297s/public/maps/london_england.streets.bin",
        "/cad2/ece297s/public/maps/moscow_russia.streets.bin",
        "/cad2/ece297s/public/maps/new-delhi_india.streets.bin",
        "/cad2/ece297s/public/maps/new-york_usa.streets.bin",
        "/cad2/ece297s/public/maps/rio-de-janeiro_brazil.streets.bin",
        "/cad2/ece297s/public/maps/saint-helena.streets.bin",
        "/cad2/ece297s/public/maps/singapore.streets.bin",
        "/cad2/ece297s/public/maps/sydney_australia.streets.bin",
        "/cad2/ece297s/public/maps/tehran_iran.streets.bin",
        "/cad2/ece297s/public/maps/tokyo_japan.streets.bin",
        "/cad2/ece297s/public/maps/toronto_canada.streets.bin"
        
};

void drawMainCanvas(ezgl::renderer *g);
void initial_setup(ezgl::application *application, bool new_window);



double lon_from_x(double x);

double lat_from_y(double lat);

double xFromLonPoi(double lon);

double yFromLatPoi(double lat);

void act_on_mouse_click(ezgl::application *app,
                        GdkEventButton *event,
                        double x, double y);

void findButton(GtkWidget *widget, ezgl::application *application);
void loadFeatures();
void drawFeatures(ezgl::renderer *g);
void loadPoi();
void drawPoi(ezgl::renderer *g);
void drawPoiLabel(ezgl::renderer *g);

//variable declarations
struct intersection_data {
    LatLon position;
    std::string name;
    bool highlight = false;
} typedef intersection_data;

struct poiData {
    LatLon position;
    std::string name;
    bool highlight = false;
};

struct featureData {
    std::string name;
    FeatureType type;
    bool closed = false;
    int numOfPoints = 0;
    std::vector<ezgl::point2d> positionalPoints;
};

std::vector<featureData> features;
std::vector<intersection_data> intersections;
std::vector<poiData> poi;

double avgPoiLat = 0;

//implementations


double lon_from_x(double x) {
    return x / (kDegreeToRadian * kEarthRadiusInMeters * std::cos(avg_lat * kDegreeToRadian));
}

double lat_from_y(double lon) {
    return lon / (kDegreeToRadian * kEarthRadiusInMeters);
}

double xFromLonPoi(double lon) {
    return lon * kDegreeToRadian * kEarthRadiusInMeters * std::cos(avgPoiLat * kDegreeToRadian);
}

double yFromLatPoi(double lat) {
    return lat * kDegreeToRadian * kEarthRadiusInMeters;
}


void act_on_mouse_click(ezgl::application *app,
                        GdkEventButton *event,
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

void drawMap() {
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

    avg_lat = (min_lat + max_lat) / 2;
    
    loadPoi();
    loadFeatures();

    ezgl::application::settings settings;
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";

    ezgl::application application(settings);


    ezgl::rectangle initial_world({x_from_lon(min_lon), y_from_lat(min_lat)},
                                  {x_from_lon(max_lon), y_from_lat(max_lat)});
    application.add_canvas("MainCanvas", drawMainCanvas, initial_world);


    application.run(initial_setup, act_on_mouse_click, nullptr, nullptr);
}



void drawMainCanvas(ezgl::renderer *g) {

    drawFeatures(g);
    drawPoi(g);
    drawPoiLabel(g);

    for (int i = 0; i < intersections.size(); ++i) {
        float x = x_from_lon(intersections[i].position.longitude());
        float y = y_from_lat(intersections[i].position.latitude());

        if (intersections[i].highlight) {
            g->set_color(ezgl::YELLOW);
        } else {
            g->set_color(ezgl::GREY_55);
        }

        float width = 10;
        float height = width;

        g->fill_rectangle({x - width / 2, y - height / 2}, {x + width / 2, y + height / 2});
        
    }
    /*DRAWS THE SEGMENTS*/
    for (StreetSegmentIdx segment = 0; segment < points_on_segments.size(); ++segment) {
        for(int point = 0; point < xy_points_segments[segment].size(); ++point){
            if(point < xy_points_segments[segment].size() - 1){ 
                g->set_line_width(0);
                std::pair<float, float> first_point = {xy_points_segments[segment][point].first, xy_points_segments[segment][point].second};
                std::pair<float, float> second_point = {xy_points_segments[segment][point + 1].first, xy_points_segments[segment][point + 1].second};
                g->draw_line({first_point.first, first_point.second}, {second_point.first, second_point.second});
            }
        }
    }
}

void loadFeatures(){
    features.resize(getNumFeatures());

    for (int i = 0; i < getNumFeatures(); ++i) {
        features[i].name = getFeatureName(i);
        features[i].type = getFeatureType(i);
        features[i].numOfPoints = getNumFeaturePoints(i);
        for (int j = 0; j < features[i].numOfPoints; ++j) {
            features[i].positionalPoints.push_back(ezgl::point2d(x_from_lon(getFeaturePoint(i, j).longitude()),
                                                                 y_from_lat(getFeaturePoint(i, j).latitude())));
        }
        if (features[i].positionalPoints[0] == features[i].positionalPoints[features[i].numOfPoints-1]) {
            features[i].closed = true;
        }
    }
}

void drawFeatures(ezgl::renderer *g){
    g->set_line_width(1);

    for (int i = 0; i < getNumFeatures(); ++i) {
        if (features[i].type == 1) {
            g->set_color(ezgl::GREEN);
        } else if (features[i].type == 2) {
            g->set_color(ezgl::KHAKI);
        } else if (features[i].type == 3) {
            g->set_color(ezgl::LIGHT_SKY_BLUE);
        } else if (features[i].type == 4) {
            g->set_color(ezgl::BLUE);
        } else if (features[i].type == 5) {
            g->set_color(ezgl::GREEN);
        } else if (features[i].type == 6) {
            g->set_color(ezgl::GREY_55);
        } else if (features[i].type == 7) {
            g->set_color(ezgl::GREEN);
        } else if (features[i].type == 8) {
            g->set_color(ezgl::DARK_GREEN);
        } else if (features[i].type == 9) {
            g->set_color(ezgl::LIGHT_SKY_BLUE);
        } else {
            g->set_color(ezgl::PINK);
        }
        if (features[i].closed) {
            if(features[i].positionalPoints.size() != 1) {
                g->fill_poly(features[i].positionalPoints);
            }
        }else{
            for(int j = 0; j < features[i].numOfPoints-1; ++j){
                g->draw_line(features[i].positionalPoints[j],features[i].positionalPoints[j+1]);
            }
        }
    }
}

void loadPoi(){
    poi.resize(getNumPointsOfInterest());
    double maxPoiLat = getPOIPosition(0).latitude();
    double minPoiLat = maxPoiLat;
    double maxPoiLong = getPOIPosition(0).longitude();
    double minPoiLong = maxPoiLong;
    
    
    
    for (int i = 0; i < getNumPointsOfInterest(); i++) {
        poi[i].position = getPOIPosition(i);
        poi[i].name = getPOIName(i);

        maxPoiLat = std::max(maxPoiLat, poi[i].position.latitude());
        minPoiLat = std::min(minPoiLat, poi[i].position.latitude());
        maxPoiLong = std::max(maxPoiLong, poi[i].position.longitude());
        minPoiLong = std::min(minPoiLong, poi[i].position.longitude());
    }
    avgPoiLat = (minPoiLat + maxPoiLat) / 2;
}

void drawPoi(ezgl::renderer *g){
    for (int i = 0; i < poi.size(); i++) {
        double x = xFromLonPoi(poi[i].position.longitude());
        double y = yFromLatPoi(poi[i].position.latitude());
        
        if (poi[i].highlight == false) {
            g->set_color(ezgl::LIGHT_SKY_BLUE);
        }

        double width = 15;
        float height = width;

        g->fill_arc({x - width / 2, y - height / 2}, 3.5, 0, 360);
    }
}

void drawPoiLabel(ezgl::renderer *g){
    if(aspVar >= 16.666){
    g -> set_font_size(20);
    g -> set_color(ezgl::BLACK);
    for(int i = 0; i < poi.size(); i++){
        std::string poiName = poi[i].name;
        double x = xFromLonPoi(poi[i].position.longitude());
        double y = yFromLatPoi(poi[i].position.latitude());
        double width = 15;
        double height = width;
        ezgl::point2d center(x-width/2,y-height/2);
        
        float poiLen = 100;
        g -> draw_text(center, poiName, poiLen, poiLen);
    }
    }
}

void selectButtonClk(GtkWidget *, ezgl::application *application){

}

void initial_setup(ezgl::application *application, bool){
    application -> create_button("Find",8,findButton);
    mapBox = (GtkComboBox*) application->get_object("MapSelectBox");
    g_signal_connect(
            application->get_object("mapSelectBtn"),
            "clicked",
            G_CALLBACK(selectButtonClk(, application)),
            application
            );

}

void findButton(GtkWidget *, ezgl::application *application){
    //Tells status bar that find button was pressed
    application -> update_message("Find Button Pressed");
    //Redraw Main Canvas
    application -> refresh_drawing();
}