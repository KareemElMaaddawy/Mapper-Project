#include "m2.h"
#include "m1.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "globalHeader.h"
#include "string"

//function declarations
std::string openMap = "Toronto, Canada"; //holds name of map open, default is toronto

void selectButtonClk(GtkEntry *,ezgl::application *application); //widgit to hold btn for select map
void colorBlindToggle(GtkEntry *,ezgl::application *application);

void drawMainCanvas(ezgl::renderer *g);
void initial_setup(ezgl::application *application, bool new_window);
double lon_from_x(double x);

double lat_from_y(double lat);

double xFromLonPoi(double lon);

double yFromLatPoi(double lat);

GtkEntry* firstStreetEntry = nullptr;
GtkEntry* secondStreetEntry = nullptr;
GtkComboBox* mapBox = nullptr;

void act_on_mouse_click(ezgl::application *app,
                        GdkEventButton *event,
                        double x, double y);
void drawNewMap(ezgl::application *application);
void findButton(GtkWidget *widget, ezgl::application *application);
void searchBar(GtkEntry *widget, ezgl::application *application);
void loadFeatures();
void drawFeatures(ezgl::renderer *g);
void loadPoi();
void drawPoi(ezgl::renderer *g);
void drawPoiLabel(ezgl::renderer *g);
void loadIntersections();
void drawStreetLabels(ezgl::renderer *g);
void setColor(ezgl::renderer *g, int type);
double slope(double x1, double y1, double x2, double y2);
double perpSlope(double m);
std::pair<double, double> findPointOfReference(double m, double distance, double x, double y);




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
    for(int i = 0; i < intersections.size(); i++){
        if(intersections[i].highlight == true ){
            intersections[i].highlight = false;
        }
    }
    std::cout << "Mouse clicked at (" << x << "," << y << ")\n";
    std::cout << "Button clicked is " << event->button << std::endl;
    LatLon pos = LatLon(lat_from_y(y), lon_from_x(x));
    int id = findClosestIntersection(pos);
    
    std::cout << "Closest Intersection: "
              << intersections[id].name << "\n";
    intersections[id].highlight = true;
    app->refresh_drawing();
}
//loads information for when when map is selected
void drawNewMap(ezgl::application *application){
    loadPoi();
    loadIntersections();//loads data
    loadFeatures();

    ezgl::rectangle new_world({x_from_lon(mapInfo[selectedMap].minLon), y_from_lat(mapInfo[selectedMap].minLat)}, //creates new world rectangle
                                  {x_from_lon(mapInfo[selectedMap].maxLon), y_from_lat(mapInfo[selectedMap].maxLat)});
    application->change_canvas_world_coordinates(application->get_main_canvas_id(), new_world); //changes world coords to rectangle

}

//draws initial map
void drawMap() {
    loadPoi();
    loadIntersections();//loads data
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

double slope(double x1, double y1, double x2, double y2){
    return((y2-y1)/(x2-x1));
}
double perpSlope(double m){
    return (-(1/m));
}
std::pair<double, double> findPointOfReference(double m, double distance, double x, double y){
    double a = distance/(sqrt(1+(m*m)));
    double b = a*m;
    std::pair <double, double> returnPair = {x-a, y-b};
    return(returnPair);
}

void drawMainCanvas(ezgl::renderer *g) {

    drawFeatures(g);
    drawPoi(g);
    

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
                g->set_color(234,191,75,255);
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
                g->set_color(234,191,75,255);
                double m = slope (formerPoint.first, formerPoint.second, latterPoint.first, latterPoint.second);
                double perpM = perpSlope(m);
                std::pair<double, double> por;
                std::pair<double, double> posSidePoint;
                std::pair<double, double> negSidePoint;
                if(m == 0){
                    por = std::make_pair(latterPoint.first - 1, latterPoint.second); 
                    posSidePoint =  {por.first, por.second + 0.5};
                    negSidePoint = { por.first, por.second - 0.5};
                }else if(perpM == 0){
                    por = std::make_pair(latterPoint.first, latterPoint.second - 1); 
                    posSidePoint =  {por.first + 0.5, por.second};
                    negSidePoint = { por.first - 0.5, por.second};
                }else{
                    por = findPointOfReference( m, -1, latterPoint.first, latterPoint.second);
                    posSidePoint = findPointOfReference( perpM, 0.5, por.first, por.second);
                    negSidePoint = findPointOfReference( perpM, -0.5, por.first, por.second);
                }
                
                
                g->draw_line({formerPoint.first, formerPoint.second}, {latterPoint.first, latterPoint.second});
                g->fill_poly({{latterPoint.first, latterPoint.second}, {posSidePoint.first, posSidePoint.second}, {negSidePoint.first, negSidePoint.second}});
            }
            else if(point == xy_points_segments[segment].size() - 1){
                std::pair<double, double> pointBeforeLast = {xy_points_segments[segment][point].first, xy_points_segments[segment][point].second};
                IntersectionIdx toIntersection = getStreetSegmentInfo(segment).to;
                LatLon lastPoint = getIntersectionPosition(toIntersection);
                double x = x_from_lon(lastPoint.longitude());
                double y = y_from_lat(lastPoint.latitude());
                g->set_color(234,191,75,255);
                g->set_line_width(0);
                g->draw_line({pointBeforeLast.first, pointBeforeLast.second}, {x, y});
            }
        }
    }
    drawStreetLabels(g);
    drawPoiLabel(g);
}

void loadFeatures(){//loads features and their associated properties
    features.resize(10);//ten types of different features
    for (int i = 0; i < getNumFeatures(); ++i) {//loops through all features
        int type = getFeatureType(i);//gets what type of feature it is
        features[type].resize(features[type].size()+1);//increments the associated feature type vector
        int index = features[type].size() - 1;//gets next open slot in vector
        features[type][index].name = getFeatureName(i);//fetches data
        features[type][index].numOfPoints = getNumFeaturePoints(i);
        for (int j = 0; j < features[type][index].numOfPoints; ++j) {//fetches coords and converts to xy
            features[type][index].positionalPoints.push_back(ezgl::point2d(x_from_lon(getFeaturePoint(i, j).longitude()),
                                                                 y_from_lat(getFeaturePoint(i, j).latitude())));
        }
        if (features[type][index].positionalPoints[0] == features[type][index].positionalPoints[features[type][index].numOfPoints-1]) {//checks if closed or not
            features[type][index].closed = true;
        }
    }
}

void setColor(ezgl::renderer *g, int type){//sets drawing based on feature type
    if(colorBlind){
        if (type == 1) {
            g->set_color(167, 159, 66, 255);
        } else if (type == 2) {
            g->set_color(214, 200, 48, 255);
        } else if (type == 3) {
            g->set_color(49, 86, 249, 255);
        } else if (type == 4) {
            g->set_color(49, 86, 249, 255);
        } else if (type == 5) {
            g->set_color(167, 159, 66, 255);
        } else if (type == 6) {
            g->set_color(127, 143, 154, 255);
        } else if (type == 7) {
            g->set_color(167, 159, 66, 255);
        } else if (type == 8) {
            g->set_color(235, 222, 73, 255);
        } else if (type == 9) {
            g->set_color(ezgl::LIGHT_SKY_BLUE);
        } else {
            g->set_color(ezgl::PINK);//bright colour to stand out
        }
    }else{
        if (type == 1) {
            g->set_color(183, 217, 181, 255);
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
            g->set_color(ezgl::PINK);//bright colour to stand out
        }
    }
}

void loadPoi(){
    poi.resize(getNumPointsOfInterest());

    for (int i = 0; i < getNumPointsOfInterest(); i++) {
        poi[i].position = getPOIPosition(i);
        poi[i].name = getPOIName(i);
    }
}

void drawFeatures(ezgl::renderer *g){//draws features
    int order[] = {3,5,1,2,7,8,4,9,6,0};//order of feature types to draw

    g->set_line_width(0);

    for(int i = 0; i < 10; ++i){//looping through array of feature types
        int type = order[i];//fetching what the feature type is
        setColor(g, type);//setting color based on feature type
        for(int z = 0; z < features[type].size(); ++z){//looping through all the features of the specified type
            if (features[type][z].closed) {//if its closed draw as polygon
                if (features[type][z].positionalPoints.size() != 1) {
                    g->fill_poly(features[type][z].positionalPoints);
                }
            } else {//not closed draw as line
                for (int j = 0; j < features[type][z].numOfPoints - 1; ++j) {
                    g->draw_line(features[type][z].positionalPoints[j], features[type][z].positionalPoints[j + 1]);
                }
            }
        }
    }
}

void drawPoi(ezgl::renderer *g){
    for (int i = 0; i < poi.size(); i++) {
        double x = x_from_lon(poi[i].position.longitude());
        double y = y_from_lat(poi[i].position.latitude());
        
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
    g -> set_font_size(10);
    g -> set_color(ezgl::BLACK);
    for(int i = 0; i < poi.size(); i++){
        std::string poiName = poi[i].name;
        double x = x_from_lon(poi[i].position.longitude());
        double y = y_from_lat(poi[i].position.latitude());
        double width = 15;
        double height = width;
        ezgl::point2d center(x-width/2,y-height/2);
        
        float poiLen = 100;
        g -> draw_text(center, poiName, poiLen, poiLen);
    }
    }
}



void initial_setup(ezgl::application *application, bool){
    firstStreetEntry = (GtkEntry*)(application -> get_object("FirstStreet"));
    secondStreetEntry = (GtkEntry*)(application -> get_object("SecondStreet"));
    mapBox = (GtkComboBox*) application->get_object("MapSelectBox");
    g_signal_connect(//connecting map select button to callback function
            application->get_object("mapSelectBtn"),
            "clicked",
            G_CALLBACK(selectButtonClk),
            application
            );
    g_signal_connect(//connecting map select button to callback function
            application->get_object("colorToggle"),
            "clicked",
            G_CALLBACK(colorBlindToggle),
            application
    );
    g_signal_connect(application -> get_object("Find"), "clicked", G_CALLBACK(findButton), application);
}

void colorBlindToggle(GtkEntry *,ezgl::application *application){
    colorBlind = !colorBlind;
    application->refresh_drawing();
}

void selectButtonClk(GtkEntry *,ezgl::application *application){//callback function when map select button is pressed
    selectedMap = gtk_combo_box_get_active(mapBox);//fetching selection

    if(selectedMap == -1){//if nothing selected and button pressed, prompt selection
        application -> update_message("Select Map");
    }else{
        if(mapInfo[selectedMap].name == openMap){//if selected map is already open, prompt another selection
            application -> update_message(mapInfo[selectedMap].name + " already open");
        }else{
            application -> update_message("Loading...");
            openMap = mapInfo[selectedMap].name;//sets open map to selected map
            closeMap();
            features.clear();
            intersections.clear();
            poi.clear();
            streetPositions.clear();
            points_on_segments.clear();
            xy_points_segments.clear();//clears data structures
            bool load_success = loadMap(mapInfo[selectedMap].path);//loads map
            if(!load_success) {
                std::cerr << "Failed to load map '" << mapInfo[selectedMap].name << "'\n";//error catch
                return;
            }else{
                application -> update_message(mapInfo[selectedMap].name + " loaded");
            }
            drawNewMap(application);//draws new map

        }
        application->refresh_drawing(); //refreshes
    }
}


void findButton(GtkWidget *, ezgl::application *application){
    //Tells status bar that find button was pressed
    application -> update_message("Find Button Pressed");
    //Redraw Main Canvas
    application -> refresh_drawing();
    
    
    std::string firstTerm = gtk_entry_get_text(firstStreetEntry);
    std::string secondTerm = gtk_entry_get_text(secondStreetEntry);
    std::cout<< "first search term: " << firstTerm << ", " << " second search term: " << secondTerm << std::endl;
    
    

    if (firstTerm != "" && secondTerm != ""){
        
        std::vector<StreetIdx> possibleFirstStreets = findStreetIdsFromPartialStreetName(firstTerm);
        std::vector<StreetIdx> possibleSecondStreets = findStreetIdsFromPartialStreetName(secondTerm);
        std::string correctFirstStreetName = "";
        std::string correctSecondStreetName = "";
        if((possibleFirstStreets.size() == 1) && (possibleSecondStreets.size() == 1)){
            std::string fullFirstFromPartial = getStreetName(possibleFirstStreets[0]);
            std::string fullSecondFromPartial = getStreetName(possibleSecondStreets[0]);
            std::cout << "first street name: " << fullFirstFromPartial <<std::endl;
            std::cout << "second street name: " << fullSecondFromPartial <<std::endl;
            std::vector<IntersectionIdx> intersectionsOfStreets = findIntersectionsOfTwoStreets({possibleFirstStreets[0], possibleSecondStreets[0]}); 
            std::cout << "names of common intersections are:" << std::endl;
            for(int i = 0; i < intersectionsOfStreets.size(); ++i){
            //    std::cout<< "iterator: " << i << "     " << "intersecttionsOfStreetEntry" << intersectionsOfStreets[i] << std::endl;
            //    std::cout << "   " << intersections[intersectionsOfStreets[i]].name;
                intersections[intersectionsOfStreets[i]].highlight = true;
            }
            std::cout << "if "<<std::endl;
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
                std::vector<StreetIdx> firstS = findStreetIdsFromPartialStreetName(correctFirstStreetName);
                std::vector<StreetIdx> secondS = findStreetIdsFromPartialStreetName(correctSecondStreetName);
                std::vector<IntersectionIdx> intersectionsOfStreets = findIntersectionsOfTwoStreets({firstS[0], secondS[0]}); 
                std::cout << "street ids: " << firstS[0] << "     " << secondS[0] << std::endl;
                std::cout << "street names: " << getStreetName(firstS[0]) << "     " << getStreetName(secondS[0]) << std::endl;
                std::cout << "names of common intersections are:" << std::endl;
                std::cout << intersectionsOfStreets.size() << std::endl;
                for(int i = 0; i < intersectionsOfStreets.size(); ++i){
                    std::cout<< "iterator: " << i << "     " << "intersecttionsOfStreetEntry" << intersectionsOfStreets[i] << std::endl;
                    std::cout << "   " << intersections[intersectionsOfStreets[i]].name << std::endl;
                    intersections[intersectionsOfStreets[i]].highlight = true;
                }
            std::cout << "else "<<std::endl;
            }
        }
        
    }else if (firstTerm == "" && secondTerm != ""){
        std::cout << "needs a first street" << std::endl << "second entry: " << secondTerm << std::endl;
    }else if (firstTerm != "" && secondTerm == ""){
        std::cout << "needs a second street" << std::endl << "first entry: " << firstTerm << std::endl;
    }
    std::cout << "hello world"<<std::endl;
    application->refresh_drawing();
}

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

