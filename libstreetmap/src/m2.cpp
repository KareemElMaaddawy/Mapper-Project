#include "m2.h"
#include "m1.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "globalHeader.h"
#include "string"

//function declarations

std::string openMap = "Toronto, Canada";

void selectButtonClk(GtkEntry *,ezgl::application *application);

void drawMainCanvas(ezgl::renderer *g);
void initial_setup(ezgl::application *application, bool new_window);
double lon_from_x(double x);

double lat_from_y(double lat);

double xFromLonPoi(double lon);

double yFromLatPoi(double lat);

GtkEntry* searchEntry = nullptr;
GtkEntry* firstStreetEntry = nullptr;
GtkEntry* secondStreetEntry = nullptr;
GtkComboBox* mapBox = nullptr;

void act_on_mouse_click(ezgl::application *app,
                        GdkEventButton *event,
                        double x, double y);

void findButton(GtkWidget *widget, ezgl::application *application);
void searchBar(GtkEntry *widget, ezgl::application *application);
void loadFeatures();
void drawFeatures(ezgl::renderer *g);
void loadPoi();
void drawPoi(ezgl::renderer *g);
void drawPoiLabel(ezgl::renderer *g);
//void loadStreets();
void drawStreetLabels(ezgl::renderer *g);

//variable declarations
/*struct streetData{
    std::string name;
    std::vector<int> segmentsOfStreet;
};*/

struct intersection_data {
    LatLon position;
    std::string name;
    bool highlight = false;
};

struct poiData {
    LatLon position;
    std::string name;
    bool highlight = false;
};

struct featureData {
    std::string name;
    bool closed = false;
    int numOfPoints = 0;
    std::vector<ezgl::point2d> positionalPoints;
};

//std::vector<streetData> streets;
std::vector<std::vector<featureData>> features;
std::vector<intersection_data> intersections;
std::vector<poiData> poi;

double avgPoiLat = 0;

//implementations


double lon_from_x(double x) {
    return x / (kDegreeToRadian * kEarthRadiusInMeters * std::cos(mapInfo[selectedMap].avgLat * kDegreeToRadian));
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

void loadIntersections(){
    intersections.resize(getNumIntersections());

    for (int id = 0; id < getNumIntersections(); ++id) {
        intersections[id].position = getIntersectionPosition(id);
        intersections[id].name = getIntersectionName(id);
    }
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

void drawNewMap(ezgl::application *application, int selectedMap){

    loadIntersections();
    loadFeatures();


    ezgl::rectangle new_world({x_from_lon(mapInfo[selectedMap].minLon), y_from_lat(mapInfo[selectedMap].minLat)},
                                  {x_from_lon(mapInfo[selectedMap].maxLon), y_from_lat(mapInfo[selectedMap].maxLat)});
    application->change_canvas_world_coordinates(application->get_main_canvas_id(), new_world);

}

void drawMap() {

    loadIntersections();
    loadFeatures();

    ezgl::application::settings settings;
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";

    ezgl::application application(settings);


    ezgl::rectangle initial_world({x_from_lon(mapInfo[selectedMap].minLon), y_from_lat(mapInfo[selectedMap].minLat)},
                                  {x_from_lon(mapInfo[selectedMap].maxLon), y_from_lat(mapInfo[selectedMap].maxLat)});
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
        //if(points_on_segments.size() <= )
        for(int point = 0; point < xy_points_segments[segment].size(); ++point){
            if (point == 0){
                std::pair<double, double> secondPoint = {xy_points_segments[segment][point].first, xy_points_segments[segment][point].second};
                std::pair<double, double> thirdPoint = {xy_points_segments[segment][point+1].first, xy_points_segments[segment][point+1].second};
                IntersectionIdx fromIntersection = getStreetSegmentInfo(segment).from;
                LatLon firstPoint = getIntersectionPosition(fromIntersection);
                double x = x_from_lon(firstPoint.longitude());
                double y = y_from_lat(firstPoint.latitude());
                g->set_color(ezgl::GREEN);
                g->set_line_width(0);
                g->draw_line({x, y}, {secondPoint.first, secondPoint.second});
                if(xy_points_segments[segment].size() > 1){
                    g->draw_line({secondPoint.first, secondPoint.second}, {thirdPoint.first, thirdPoint.second});
                }
                if(xy_points_segments[segment].size() == 0){
                    std::cout<<"there is a zero"<<std::endl;
                }
            }
            if((point < xy_points_segments[segment].size() - 1) || ((xy_points_segments[segment].size() < 2)&&(point == xy_points_segments[segment].size()))){
                g->set_line_width(0);
                std::pair<double, double> formerPoint = {xy_points_segments[segment][point].first, xy_points_segments[segment][point].second};
                std::pair<double, double> latterPoint = {xy_points_segments[segment][point + 1].first, xy_points_segments[segment][point + 1].second};
                g->set_color(ezgl::BLACK);
                g->draw_line({formerPoint.first, formerPoint.second}, {latterPoint.first, latterPoint.second});
            }
            else if(point == xy_points_segments[segment].size() - 1){
                std::pair<double, double> pointBeforeLast = {xy_points_segments[segment][point].first, xy_points_segments[segment][point].second};
                IntersectionIdx toIntersection = getStreetSegmentInfo(segment).to;
                LatLon lastPoint = getIntersectionPosition(toIntersection);
                double x = x_from_lon(lastPoint.longitude());
                double y = y_from_lat(lastPoint.latitude());
                g->set_color(ezgl::PURPLE);
                g->set_line_width(0);
                g->draw_line({pointBeforeLast.first, pointBeforeLast.second}, {x, y});
            }
        }
    }
    drawStreetLabels(g);
}

void loadFeatures(){
    features.resize(10);
    for (int i = 0; i < getNumFeatures(); ++i) {
        int type = getFeatureType(i);
        features[type].resize(features[type].size()+1);
        int index = features[type].size() - 1;
        features[type][index].name = getFeatureName(i);
        features[type][index].numOfPoints = getNumFeaturePoints(i);
        for (int j = 0; j < features[type][index].numOfPoints; ++j) {
            features[type][index].positionalPoints.push_back(ezgl::point2d(x_from_lon(getFeaturePoint(i, j).longitude()),
                                                                 y_from_lat(getFeaturePoint(i, j).latitude())));
        }
        if (features[type][index].positionalPoints[0] == features[type][index].positionalPoints[features[type][index].numOfPoints-1]) {
            features[type][index].closed = true;
        }
    }
}

void setColor(ezgl::renderer *g, int type){
    if (type == 1) {
        g->set_color(214, 234, 214, 255);
    } else if (type == 2) {
        g->set_color(253, 249, 235, 255);
    } else if (type == 3) {
        g->set_color(166, 191, 247, 255);
    } else if (type == 4) {
        g->set_color(166, 191, 247, 255);
    } else if (type == 5) {
        g->set_color(214, 234, 214, 255);
    } else if (type == 6) {
        g->set_color(127, 143, 154, 255);
    } else if (type == 7) {
        g->set_color(214, 234, 214, 255);
    } else if (type == 8) {
        g->set_color(184, 217, 182, 255);
    } else if (type == 9) {
        g->set_color(ezgl::LIGHT_SKY_BLUE);
    } else {
        g->set_color(ezgl::PINK);
    }
}

void drawFeatures(ezgl::renderer *g){
    int order[] = {3,5,1,2,7,8,4,9,6,0};

    g->set_line_width(1);

    for(int i = 0; i < 10; ++i){
        int type = order[i];
        setColor(g, type);
        for(int z = 0; z < features[type].size(); ++z){
            if (features[type][z].closed) {
                if (features[type][z].positionalPoints.size() != 1) {
                    g->fill_poly(features[type][z].positionalPoints);
                }
            } else {
                for (int j = 0; j < features[type][z].numOfPoints - 1; ++j) {
                    g->draw_line(features[type][z].positionalPoints[j], features[type][z].positionalPoints[j + 1]);
                }
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



void initial_setup(ezgl::application *application, bool){
//    application -> create_button("Find",8,findButton);

    searchEntry = (GtkEntry*)(application -> get_object("SearchInput"));
    firstStreetEntry = (GtkEntry*)(application -> get_object("FirstStreet"));
    secondStreetEntry = (GtkEntry*)(application -> get_object("SecondStreet"));
    mapBox = (GtkComboBox*) application->get_object("MapSelectBox");
    g_signal_connect(
            application->get_object("mapSelectBtn"),
            "clicked",
            G_CALLBACK(selectButtonClk),
            application
            );

    g_signal_connect(application -> get_object("testbutton"), "clicked", G_CALLBACK(findButton), application);
    g_signal_connect(application -> get_object("Find"), "clicked", G_CALLBACK(findButton), application);
    g_signal_connect(searchEntry, "icon_press", G_CALLBACK(searchBar), application);
}

void selectButtonClk(GtkEntry *,ezgl::application *application){
    selectedMap = gtk_combo_box_get_active(mapBox);

    if(selectedMap == -1){
        application -> update_message("Select Map");
    }else{
        std::cout << mapInfo[selectedMap].name << " selected" << std::endl;
        if(mapInfo[selectedMap].name == openMap){
            application -> update_message(mapInfo[selectedMap].name + " already open");
        }else{
            openMap = mapInfo[selectedMap].name;
            closeMap();
            features.clear();
            intersections.clear();
            poi.clear();
            streetPositions.clear();
            points_on_segments.clear();
            xy_points_segments.clear();
            bool load_success = loadMap(mapInfo[selectedMap].path);
            if(!load_success) {
                std::cerr << "Failed to load map '" << mapInfo[selectedMap].name << "'\n";
                return;
            }else{
                application -> update_message(mapInfo[selectedMap].name + " loaded");
            }
            drawNewMap(application, selectedMap);

        }
    }application->refresh_drawing();
}
void searchBar(GtkEntry *, ezgl::application *application){
 
  const char* search_term = gtk_entry_get_text(searchEntry);
  std::cout <<"searched: "<< search_term<<std::endl;
  userInput = search_term;
  gtk_entry_set_text(GTK_ENTRY(searchEntry), "");
  
    

}


void findButton(GtkWidget *, ezgl::application *application){
    //Tells status bar that find button was pressed
    application -> update_message("Find Button Pressed");
    //Redraw Main Canvas
    application -> refresh_drawing();
    
    
    std::string firstTerm = gtk_entry_get_text(firstStreetEntry);
    std::string secondTerm = gtk_entry_get_text(secondStreetEntry);
    std::cout<< "first street: " << firstTerm << " second street: " << secondTerm << std::endl;
    
    

    if (firstTerm != "" && secondTerm != ""){
        
        std::vector<StreetIdx> possibleFirstStreets = findStreetIdsFromPartialStreetName(firstTerm);
        std::vector<StreetIdx> possibleSecondStreets = findStreetIdsFromPartialStreetName(secondTerm);
        std::string correctFirstStreetName = "";
        std::string correctSecondStreetName = "";
        if((possibleFirstStreets.size() == 1) && (possibleSecondStreets.size() == 1)){
            std::string fullFirstFromPartial = getStreetName(possibleFirstStreets[0]);
            std::string fullSecondFromPartial = getStreetName(possibleSecondStreets[0]);
            std::cout << "first full street name: " << fullFirstFromPartial <<std::endl;
            std::cout << "second full street name: " << fullSecondFromPartial <<std::endl;
        }else{
            
            for (int i = 0; i < possibleFirstStreets.size(); ++i){
                if (getStreetName(possibleFirstStreets[i]) == firstTerm){
                    correctFirstStreetName = firstTerm;
                }
            }
            for (int i = 0; i < possibleSecondStreets.size(); ++i){
                if (getStreetName(possibleSecondStreets[i]) == secondTerm){
                    correctSecondStreetName = secondTerm;
                }
            }
            if(correctSecondStreetName == "" || correctFirstStreetName == ""){
                std::cout<<"more info needed"<<std::endl << "first term: " << firstTerm << std::endl<<"second term: "<< secondTerm <<std::endl;
                std::cout << "all possible names for first: " << std::endl;
                for (int i = 0; i < possibleFirstStreets.size(); ++i){
                    std::cout << getStreetName(possibleFirstStreets[i]) << std::endl;
                }
                std::cout << "all possible names for second: " << std::endl;
                for (int i = 0; i < possibleSecondStreets.size(); ++i){

                    std::cout << getStreetName(possibleSecondStreets[i]) << std::endl;
                }
            }
            else{
                std::string fullFirstFromPartial = correctFirstStreetName;
                std::string fullSecondFromPartial = correctSecondStreetName;
                std::cout << "first full street name: " << fullFirstFromPartial <<std::endl;
                std::cout << "second full street name: " << fullSecondFromPartial <<std::endl;
            }
        }
        
    }else if (firstTerm == "" && secondTerm != ""){
        std::cout << "needs a first street" << std::endl << "second entry: " << secondTerm << std::endl;
    }else if (firstTerm != "" && secondTerm == ""){
        std::cout << "needs a second street" << std::endl << "first entry: " << firstTerm << std::endl;
    }
    std::cout << "hello world"<<std::endl;
}

/*void loadStreets(){
    streets.resize(getNumStreets());
    for(int i = 0; i < getNumStreets() ; i++){
        streets[i].name = getStreetName(i);
        for(int j = 0; j < getNumStreetSegments(); j++){
            if(getStreetSegmentInfo(j).streetID == i){
                streets[i].segmentsOfStreet.push_back(j);
            }
        }
    }
}*/

/*void drawStreetLabels(ezgl:: renderer *g){
    g -> set_color(ezgl:: BLACK);
    for(int i = 0; i < streets.size(); i++){
        std::string streetName = streets[i].name;
        for(int j = 0 ; j < streets[i].segmentsOfStreet.size(); j+=2){
            double segID = streets[i].segmentsOfStreet[j];
            double intersecFrom = getStreetSegmentInfo(segID).from;
            double firstX =  x_from_lon(getIntersectionPosition(intersecFrom).longitude());
            double firstY =  y_from_lat(getIntersectionPosition(intersecFrom).latitude());
            double intersecTo = getStreetSegmentInfo(segID).to;
            double secondX = x_from_lon(getIntersectionPosition(intersecTo).longitude());
            double secondY = y_from_lat(getIntersectionPosition(intersecTo).latitude());
            
            double midPointX = (firstX+secondX)/2;
            double midPointY = (firstY+secondY)/2;
            ezgl::point2d  center(midPointX,midPointY);
            
            if(streetName != "<unknown>"){
            g -> draw_text(center, streetName, 100,100);
            }
        }
      
    }
}*/
void drawStreetLabels(ezgl:: renderer *g){
    g-> set_color(ezgl::BLACK);
    for(int i = 0; i < streetPositions.size(); i++){
        std::string streetName = streetPositions[i].name;
        for(int j = 0; j < streetPositions[i].positions.size(); j+=5){
            double firstX = x_from_lon(streetPositions[i].positions[j].longitude());
            double firstY = y_from_lat(streetPositions[i].positions[j].latitude());
            double secondX = x_from_lon(streetPositions[i].positions[j+1].longitude());
            double secondY = y_from_lat(streetPositions[i].positions[j+1].latitude());
            
            double midPointX = (firstX+secondX)/2;
            double midPointY = (firstY+secondY)/2;
            ezgl::point2d center(midPointX,midPointY);
            if(streetName != "<unknown>"){
                g->draw_text(center, streetName, 100, 100);
            }
        }
    }
            
}

