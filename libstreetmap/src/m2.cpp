#include "m2.h"
#include "m1.h"
#include "m3.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "globalHeader.h"
#include "pathFinding.h"
#include "buttons.h"
#include "string"
#include <math.h>
#include <cmath>
//function declarations
std::string openMap = "Toronto, Canada"; //holds name of map open, default is toronto

GtkWidget *userGuideWindow;
void drawMainCanvas(ezgl::renderer *g);
void initial_setup(ezgl::application *application, bool new_window);
double lon_from_x(double x);

bool test = false;

double lat_from_y(double lat);

double xFromLonPoi(double lon);

double yFromLatPoi(double lat);
GtkTextBuffer *findPathHelpBuffer = nullptr;
GtkTextBuffer *poiFilterHelpBuffer = nullptr;
GtkTextView *helpText = nullptr;
GtkEntry* firstStreetEntry = nullptr;
GtkEntry* secondStreetEntry = nullptr;

GtkEntry* FirstIntersectionEntry = nullptr;
GtkEntry* secondIntersectionEntry = nullptr;

GtkComboBox* mapBox = nullptr;



void act_on_mouse_click(ezgl::application *app,
                        GdkEventButton *event,
                        double x, double y);
void drawNewMap(ezgl::application *application);
void findButton(GtkWidget *widget, ezgl::application *application);
void findPathButton(GtkWidget *widget, ezgl::application *application);
void clearHighlightButton(GtkEntry *,ezgl::application *application);
void helpMenuItem(GtkWidget*, ezgl::application *application);
void poiFilterHelp(GtkWidget*, ezgl::application *application);
void writeInMiddleOfStreetSection(std::vector<StreetSegmentIdx> streetSectionSegments, ezgl:: renderer *g);
void FindPathHelp(GtkWidget*, ezgl::application *application);
void showPathButton(GtkEntry *,ezgl::application *application);
void hideUserManualButton(GtkEntry *,ezgl::application *application);
void loadFeatures();
void drawFeatures(ezgl::renderer *g);
void loadPoi();
void drawPoi(ezgl::renderer *g);
void drawPoiLabel(ezgl::renderer *g);
void drawSegments(ezgl::renderer *g);
void loadIntersections();
void drawStreetLabels(ezgl::renderer *g);
void drawPath(ezgl::renderer *g);
void setColor(ezgl::renderer *g, int type);
double slope(double x1, double y1, double x2, double y2);
double perpSlope(double m);
std::pair<double, double> findPointOfReference(double m, double distance, double x, double y);
//std::vector<int> tempID;
//std::vector<LatLon> curvePointVector;
//std::vector<std::pair<double,double>> xyPointsOfCurves;
//std::vector<int> pathSegmentIDs;

double highlightCount = 0;
double mouseClick = 0;
int firstID = 0;
int secondID = 0;

void directions(std::vector<int> path);


IntersectionIdx findFunction(std::string, std::string, int);



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
                        GdkEventButton *,
                        double x, double y) {  
     highlightCount += 1;  // keep track of how many highlights so far
     mouseClick +=1; // keep track of how many mouse clicks so far
     //std::cout << "coord: " << x << " " << y << std::endl;

    LatLon pos = LatLon(lat_from_y(y), lon_from_x(x)); // extracting position of mouse click
    int id = findClosestIntersection(pos);  // closest intersection related to that mouse click

    if((highlightCount <=2)& (mouseClick == 1)){
        firstID = id;  //grab ID of first intersection clicked
    intersections[id].highlight = true; // highlight the intersection
    
    std::cout << getIntersectionName(id) <<std::endl;//<< ": " << id << std::endl;
    }
    if((highlightCount <=2) & (mouseClick ==2)){
        secondID = id;  // grab ID of second intersection clicked
        intersections[id].highlight = true; // highlight the intersection
        std::cout << getIntersectionName(id)<<std::endl;// << ": " << id << std::endl;
       // pathSegmentIDs = findPathBetweenIntersections(firstID,secondID,15); // initializing pathSegmentIDs 
    }  
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
    int draw = 0;
    draw++;
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
    
    /*for (StreetSegmentIdx segment = 0; segment < points_on_segments.size(); ++segment) {
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
    }*/
    drawSegments(g);
    drawStreetLabels(g);
    drawPoiLabel(g);
    drawPath(g);
    
    ////////////////test for writeInMiddleOfStreetSection
//        //Sheppard Avenue West & Bangor Road
//   //Harlandale Avenue & Bangor Road
//    if(test){
//        int id1 = -1;
//        int id2 = -1;
//        std::cout << "enter int ids: "<<std::endl;
//        std::cin >> id1 >> id2;
//        if ((id1 != -1)&&(id2 != -1)){
//            std::vector<StreetSegmentIdx> section = findPathBetweenIntersections(id1, id2, 15);
//            writeInMiddleOfStreetSection(section, g);
//        }
//        test = false;
//    }
    
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
            g->set_color(191, 179, 42, 255);
        } else if (type == 6) {
            g->set_color(127, 143, 154, 255);
        } else if (type == 7) {
            g->set_color(253, 248, 80, 255);//
        } else if (type == 8) {
            g->set_color(235, 222, 73, 255);
        } else if (type == 9) {
            g->set_color(ezgl::LIGHT_SKY_BLUE);
        } else {
            g->set_color(ezgl::PINK);//bright colour to stand out
        }
    }else{
        if (type == 1) {//park = darker green
            g->set_color(83, 217, 181, 255);
        } else if (type == 2) {//beach = beige
            g->set_color(253, 249, 235, 255);
        } else if (type == 3) {//lake = blue
            g->set_color(166, 191, 247, 255);
        } else if (type == 4) {//river = blue
            g->set_color(166, 191, 247, 255);
        } else if (type == 5) {//island = green
            g->set_color(214, 234, 214, 255);
        } else if (type == 6) {//building = grey
            g->set_color(127, 143, 154, 255);
        } else if (type == 7) {//greenspace = light green
            g->set_color(214, 234, 214, 255);
        } else if (type == 8) {//golfcourse = dark green
            g->set_color(184, 217, 182, 255);
        } else if (type == 9) {//stream
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
    int zoomLevel;
    if(aspVar >= 13){
        zoomLevel = 3;
    }else if(aspVar >= 6.66){
        zoomLevel = 2;
    }else if(aspVar >= 3.33){
        zoomLevel = 1;
    }else{
        zoomLevel = 0;
    }

    g->set_line_width(0);

    for(int i = 0; i < featureDrawOrder[zoomLevel].size(); ++i){//looping through array of feature types
        int type = featureDrawOrder[zoomLevel][i];//fetching what the feature type is
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

bool checkPOIFilter(std::string poiType){
    if(std::find(poiFilter.begin(), poiFilter.end(), poiType) != poiFilter.end()){
        return true;
    }return false;
}

void drawPoi(ezgl::renderer *g){
    for (int i = 0; i < poi.size(); i++) {
        if(!poiFilterActive||checkPOIFilter(getPOIType(i))){
            double x = x_from_lon(poi[i].position.longitude());
            double y = y_from_lat(poi[i].position.latitude());

            if (colorBlind) {
                g->set_color(96,92,75,255);
            }else{
                g->set_color(ezgl:: RED);
            }

            double width = 15;
            float height = width;

            g->fill_arc({x - width / 2, y - height / 2}, 3.5, 0, 360);
        }
    }
}

void drawPoiLabel(ezgl::renderer *g){
    if(aspVar >= 16.666){
    g -> set_font_size(10);
    g -> set_color(ezgl::BLACK);
    for(int i = 0; i < poi.size(); i++){
        if(!poiFilterActive||checkPOIFilter(getPOIType(i))) {
            std::string poiName = poi[i].name;
            double x = x_from_lon(poi[i].position.longitude());
            double y = y_from_lat(poi[i].position.latitude());
            double width = 15;
            double height = width;
            ezgl::point2d center(x - width / 2, y - height / 2);

            float poiLen = 100;
            g->draw_text(center, poiName, poiLen, poiLen);
        }
    }
    }
}



void docButtonClk(GtkEntry *,ezgl::application *application){
    poiFilterActive = true;
    poiFilter = poiFilterHealthcare;
    application->refresh_drawing(); //refreshes
}

void bkButtonClk(GtkEntry *,ezgl::application *application){
    poiFilterActive = true;
    poiFilter = poiFilterEducation;
    application->refresh_drawing(); //refreshes
}

void carButtonClk(GtkEntry *,ezgl::application *application){
    poiFilterActive = true;
    poiFilter = poiFilterTransportation;
    application->refresh_drawing(); //refreshes
}

void artButtonClk(GtkEntry *,ezgl::application *application){
    poiFilterActive = true;
    poiFilter = poiFilterArt;
    application->refresh_drawing(); //refreshes
}

void foodButtonClk(GtkEntry *,ezgl::application *application){
    poiFilterActive = true;
    poiFilter = poiFilterFood;
    application->refresh_drawing(); //refreshes
}

void monButtonClk(GtkEntry *,ezgl::application *application){
    poiFilterActive = true;
    poiFilter = poiFilterFinance;
    application->refresh_drawing(); //refreshes
}

void filClrClk(GtkEntry *,ezgl::application *application){
    poiFilterActive = false;
    application->refresh_drawing(); //refreshes
}

void loadFilterButtons(ezgl::application *application){
    g_signal_connect(//connecting map select button to callback function
            application->get_object("doctorFilterBtn"),
            "clicked",
            G_CALLBACK(docButtonClk),
            application
    );
    g_signal_connect(//connecting map select button to callback function
            application->get_object("bookFilterBtn"),
            "clicked",
            G_CALLBACK(bkButtonClk),
            application
    );
    g_signal_connect(//connecting map select button to callback function
            application->get_object("carFilterBtn"),
            "clicked",
            G_CALLBACK(carButtonClk),
            application
    );
    g_signal_connect(//connecting map select button to callback function
            application->get_object("artFilterBtn"),
            "clicked",
            G_CALLBACK(artButtonClk),
            application
    );
    g_signal_connect(//connecting map select button to callback function
            application->get_object("foodFilterBtn"),
            "clicked",
            G_CALLBACK(foodButtonClk),
            application
    );
    g_signal_connect(//connecting map select button to callback function
            application->get_object("moneyFilterBtn"),
            "clicked",
            G_CALLBACK(monButtonClk),
            application
    );
    g_signal_connect(//connecting map select button to callback function
            application->get_object("filterClearBtn"),
            "clicked",
            G_CALLBACK(filClrClk),
            application
    );
}

void initial_setup(ezgl::application *application, bool){
    firstStreetEntry = (GtkEntry*)(application -> get_object("FirstStreet"));
    secondStreetEntry = (GtkEntry*)(application -> get_object("SecondStreet"));
    FirstIntersectionEntry = (GtkEntry*)(application -> get_object("FirstIntersection"));
    secondIntersectionEntry = (GtkEntry*)(application -> get_object("SecondIntersection"));    
    helpText = (GtkTextView *)(application -> get_object("HelpText"));
    poiFilterHelpBuffer = (GtkTextBuffer *)(application -> get_object("poiFilterHelpBuffer"));
    findPathHelpBuffer = (GtkTextBuffer *)(application -> get_object("findPathHelpBuffer"));
    userGuideWindow = (GtkWidget*)(application -> get_object("UserWindowId"));
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
    g_signal_connect(application -> get_object("findPath"), "clicked", G_CALLBACK(findPathButton), application);
    g_signal_connect(application -> get_object("clearHighlightBtn"), "clicked", G_CALLBACK(clearHighlightButton), application);
    g_signal_connect(application -> get_object("findPath"), "clicked", G_CALLBACK(clearHighlightButton), application);
    g_signal_connect(application -> get_object("showPathBtn"), "clicked", G_CALLBACK(showPathButton), application);
    g_signal_connect(application -> get_object("UserGuide"), "activate", G_CALLBACK(helpMenuItem), application);
    g_signal_connect(application -> get_object("hideUserGuide"), "clicked", G_CALLBACK(hideUserManualButton), application);
    
    g_signal_connect(application -> get_object("PoiFilterHelp"), "clicked", G_CALLBACK(poiFilterHelp), application);
    g_signal_connect(application -> get_object("FindPathHelp"), "clicked", G_CALLBACK(FindPathHelp), application);
    

    
    loadFilterButtons(application);
}

//function that sets the text to the user guide for poi filter
void poiFilterHelp(GtkWidget*, ezgl::application *application){
    gtk_text_view_set_buffer (helpText, poiFilterHelpBuffer);
}

void FindPathHelp(GtkWidget*, ezgl::application *application){
    gtk_text_view_set_buffer (helpText, findPathHelpBuffer);
}

void showPathButton(GtkEntry *,ezgl::application *application){
    
    showPath = !showPath;
    application -> refresh_drawing();
    test = true;
}



//////////////////////////////

void clearHighlightButton(GtkEntry *,ezgl::application *application){
    for(int i = 0; i < intersections.size(); i++){
        if(intersections[i].highlight){
            intersections[i].highlight = false;
        }
    }
    firstID = 0;
    secondID = 0;
   // curvePointVector.clear();
    //xyPointsOfCurves.clear();
    //std::cout << firstID << " " << secondID << std::endl;
    highlightCount = 0;
    mouseClick = 0;
    showPath = false;
   // std::cout << "button pressed \n";
    application->refresh_drawing();
}

void colorBlindToggle(GtkEntry *,ezgl::application *application){
    colorBlind = !colorBlind;
    application->refresh_drawing();
}

void selectButtonClk(GtkEntry *,ezgl::application *application){//callback function when map select button is pressed
    selectedMap = gtk_combo_box_get_active(mapBox);//fetching selection

    if(selectedMap == -1){//if nothing selected and button pressed, prompt selection
        //application -> update_message("Select Map");
    }else{
        if(mapInfo[selectedMap].name == openMap){//if selected map is already open, prompt another selection
            //application -> update_message(mapInfo[selectedMap].name + " already open");
        }else{
            //application -> update_message("Loading...");
            openMap = mapInfo[selectedMap].name;//sets open map to selected map
            highlightCount = 0;
            mouseClick = 0;
            showPath = false;
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
                //application -> update_message(mapInfo[selectedMap].name + " loaded");
            }
            drawNewMap(application);//draws new map

        }
        application->refresh_drawing(); //refreshes
    }
}

void helpMenuItem(GtkWidget*, ezgl::application *application){
    std::cout<<"help button activated"<<std::endl;
    gtk_widget_show (userGuideWindow);
}

//subwindows cannot be closed normally if they are created only once and they need to be hidden in the background
void hideUserManualButton(GtkEntry *,ezgl::application *application){
    gtk_widget_hide (userGuideWindow);
}

void findButton(GtkWidget *, ezgl::application *application){
    //Redraw Main Canvas
    application -> refresh_drawing();
    
    
    std::string firstTerm = gtk_entry_get_text(firstStreetEntry);
    std::string secondTerm = gtk_entry_get_text(secondStreetEntry);
    std::cout<< "first search term: " << firstTerm << ", " << " second search term: " << secondTerm << std::endl;
    
    //helps identify which find button has been pressed to the findFunction;
    const int noIntersection = 0;
    
    findFunction(firstTerm, secondTerm, noIntersection);
            
    application->refresh_drawing();
    

}

//function that works with the find path button
void findPathButton(GtkWidget *, ezgl::application *application){
   application -> refresh_drawing();
    
    //parse the names of the 2 intersections
    std::string firstTerm = gtk_entry_get_text(FirstIntersectionEntry);
    std::string secondTerm = gtk_entry_get_text(secondIntersectionEntry);
    
    //separate the names of the 4 streets and find them
    if(firstTerm.find("&") && secondTerm.find("&") ){
        
    //help identify which find button has been pressed to the findFunction;
        const int fromIntersection = 1;
        const int toIntersection = 2;
        
     
        std::size_t positionOfAndInFirst = firstTerm.find("&");
        std::size_t positionOfAndInSecond = secondTerm.find("&");
        
        
        std::string firstStreet = firstTerm.substr (0, positionOfAndInFirst);
        std::string secondStreet = firstTerm.substr (positionOfAndInFirst + 1);
        
        std::string thirdStreet = secondTerm.substr (0, positionOfAndInSecond);
        std::string fourthStreet = secondTerm.substr (positionOfAndInSecond + 1);
        
        //find the names of the intersections (using the same function from last milestone)
        IntersectionIdx firstIntersection = findFunction(firstStreet, secondStreet, fromIntersection);
        IntersectionIdx secondIntersection = findFunction(thirdStreet, fourthStreet, toIntersection);
        
        //find the path between the intersections
        std::vector<int> pathSegmentIDs = findPathBetweenIntersections(firstIntersection,secondIntersection,15);
        //highlight the path between the intersections
        
        showPath = true;
        application -> refresh_drawing();
        
        //print diresctions
    }
    
    
}

//function that finds the intersections of streets 2 given street names. 
//also prints some stuff about them depending on whether it's used with find intersections or find path functions
IntersectionIdx findFunction(std::string firstTerm, std::string secondTerm, int intersectionNumber){
    
    IntersectionIdx returnIntersection = -1;
    
    if (firstTerm != "" && secondTerm != ""){
        
        std::vector<StreetIdx> possibleFirstStreets = findStreetIdsFromPartialStreetName(firstTerm);
        std::vector<StreetIdx> possibleSecondStreets = findStreetIdsFromPartialStreetName(secondTerm);
        std::string correctFirstStreetName = "";
        std::string correctSecondStreetName = "";
        std::vector<std::string> possibleFirstStreetsNames;
        std::vector<std::string> possibleSecondStreetsNames;
        
        for(int i = 0; i< possibleFirstStreets.size();i++){
           possibleFirstStreetsNames.push_back( getStreetName(possibleFirstStreets[i]));
        }
        
        for(int i = 0; i< possibleSecondStreets.size();i++){
           possibleSecondStreetsNames.push_back( getStreetName(possibleSecondStreets[i]));
        }
        std::sort( possibleFirstStreetsNames.begin(), possibleFirstStreetsNames.end() );
            possibleFirstStreetsNames.erase( std::unique( possibleFirstStreetsNames.begin(), possibleFirstStreetsNames.end() ), possibleFirstStreetsNames.end() );
            
        std::sort( possibleSecondStreetsNames.begin(), possibleSecondStreetsNames.end() );
            possibleSecondStreetsNames.erase( std::unique( possibleSecondStreetsNames.begin(), possibleSecondStreetsNames.end() ), possibleSecondStreetsNames.end() );   
            
        if((possibleFirstStreetsNames.size() == 1) && (possibleSecondStreetsNames.size() == 1)){
            std::string fullFirstFromPartial = possibleFirstStreetsNames[0];
            std::string fullSecondFromPartial = possibleSecondStreetsNames[0];
            
            //this is for testing for now. Actual needs to only print if intersection number is 0
            if((intersectionNumber == 0) || (intersectionNumber == 1)){
                std::cout << "first street name: " << fullFirstFromPartial <<std::endl;
                std::cout << "second street name: " << fullSecondFromPartial <<std::endl;
            }else if(intersectionNumber == 2){
                std::cout << "third street name: " << fullFirstFromPartial <<std::endl;
                std::cout << "fourth street name: " << fullSecondFromPartial <<std::endl;
            }
            
            
            std::vector<IntersectionIdx> intersectionsOfStreets = findIntersectionsOfTwoStreets({possibleFirstStreets[0], possibleSecondStreets[0]}); 
            //print all the names of the intersections if we are finding common intersections
            if (intersectionNumber == 0){
                std::cout << "names of common intersections are:" << std::endl;
                for(int i = 0; i < intersectionsOfStreets.size(); ++i){
                   // std::cout<< "iterator: " << i << "     " << "intersecttionsOfStreetEntry" << intersectionsOfStreets[i] << std::endl;
                    std::cout << "   " << intersections[intersectionsOfStreets[i]].name;
                    intersections[intersectionsOfStreets[i]].highlight = true;
                }
                
            //if we are finding the path print the name of the FIRST intersection in the list of 
            //common intersections and then return that name    
                
            }else if((intersectionNumber == 2) || (intersectionNumber == 1)){
              std::cout << "name of  intersection #" << intersectionNumber << " is: " <<std::endl;
              returnIntersection = intersectionsOfStreets[0];
              std::cout << intersections[intersectionsOfStreets[0]].name<< std::endl;
              
              intersections[intersectionsOfStreets[0]].highlight = true;
              
            }
//            std::cout << "if "<<std::endl;
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
                for (int i = 0; i < possibleFirstStreetsNames.size(); ++i){
                    std::cout << possibleFirstStreetsNames[i] << std::endl;
                }
                std::cout << "all possible names for second: " << std::endl;
                for (int i = 0; i < possibleSecondStreetsNames.size(); ++i){

                    std::cout << possibleSecondStreetsNames[i] << std::endl;
                }
            }
            else{
                
                std::string fullFirstFromPartial = correctFirstStreetName;
                std::string fullSecondFromPartial = correctSecondStreetName;
                
                if((intersectionNumber == 0) || (intersectionNumber == 1)){
                    std::cout << "first full street name: " << fullFirstFromPartial <<std::endl;
                    std::cout << "second full street name: " << fullSecondFromPartial <<std::endl;
                }else if(intersectionNumber == 2){
                std::cout << "third full street name: " << fullFirstFromPartial <<std::endl;
                std::cout << "fourth full street name: " << fullSecondFromPartial <<std::endl;
                }
                
                std::vector<StreetIdx> firstS = findStreetIdsFromPartialStreetName(correctFirstStreetName);
                std::vector<StreetIdx> secondS = findStreetIdsFromPartialStreetName(correctSecondStreetName);
                std::vector<IntersectionIdx> intersectionsOfStreets = findIntersectionsOfTwoStreets({firstS[0], secondS[0]}); 
                //std::cout << "street ids: " << firstS[0] << "     " << secondS[0] << std::endl;
                //std::cout << "street names: " << getStreetName(firstS[0]) << "     " << getStreetName(secondS[0]) << std::endl;
               
                if(intersectionNumber == 0){
                    std::cout << "names of common intersections are:" << std::endl;
                    std::cout << intersectionsOfStreets.size() << std::endl;
                    for(int i = 0; i < intersectionsOfStreets.size(); ++i){
                     //   std::cout<< "iterator: " << i << "     " << "intersecttionsOfStreetEntry" << intersectionsOfStreets[i] << std::endl;
                        std::cout << "   " << intersections[intersectionsOfStreets[i]].name << std::endl;
                        intersections[intersectionsOfStreets[i]].highlight = true;
                    }
                }else if((intersectionNumber == 2) || (intersectionNumber == 1)){
                    std::cout << "name of  intersection #" << intersectionNumber << " is: " <<std::endl;
                    returnIntersection = intersectionsOfStreets[0];
                    std::cout << intersections[intersectionsOfStreets[0]].name<< std::endl;

                    intersections[intersectionsOfStreets[0]].highlight = true;
                    
                }    
           // std::cout << "else "<<std::endl;
            }
        }
        
    }else if (firstTerm == "" && secondTerm != ""){
        std::cout << "needs a first street" << std::endl << "first entry: " << secondTerm << std::endl;
    }else if (firstTerm != "" && secondTerm == ""){
        std::cout << "needs a second street" << std::endl << "second entry: " << firstTerm << std::endl;
    }

    return returnIntersection;
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

void drawPath(ezgl:: renderer *g){
    std::vector<int> pathSegmentIDs = findPathBetweenIntersections(firstID,secondID,15);
    if(showPath) //works when button show Path button is clicked
    {
    if(pathSegmentIDs.size() == 0){
        std::cout << "Path does not exist" << std::endl;
    } else {
    for(int i=0; i < pathSegmentIDs.size(); i++){
        int segID = pathSegmentIDs[i]; // grab ID of segment currently on
        int curvePoints = getStreetSegmentInfo(segID).numCurvePoints; // grab number of curve points of segment
        int fromPoint = getStreetSegmentInfo(segID).from; // grab the from point ID
        int toPoint = getStreetSegmentInfo(segID).to; // grab the to point ID
        LatLon fromPosition = getIntersectionPosition(fromPoint); // latlon position of from
        LatLon toPosition = getIntersectionPosition(toPoint); // latlon position of to

        double firstX = x_from_lon(fromPosition.longitude()); // grab x position
        double firstY = y_from_lat(fromPosition.latitude());  // grab y position
        double lastX = x_from_lon(toPosition.longitude()); // grab x position
        double lastY = y_from_lat(toPosition.latitude()); //grab y position
//        std::pair<double,double>  startPair = {firstX, firstY};
//        std::pair<double,double> endPair = {lastX,lastY};
        double dX = firstX - lastX;
        double dY = firstY - lastY;
        double length1 = findStreetSegmentLength(segID);
        double length2 = 50;
        double cosVal = cos(0.1);
        double sinVal= sin(0.1);
        
        double x3 = lastX + (length2/length1)*((dX*cosVal) + (dY*sinVal));
        double y3 = lastY + (length2/length1)*((dY*cosVal) - (dX*sinVal));
        double x4 = lastX + (length2/length1)*((dX*cosVal) - (dY*sinVal));
        double y4 = lastY + (length2/length1)*((dY*cosVal) + (dX*sinVal));
        
        ezgl::point2d point1(x3,y3);
        ezgl::point2d point2(x4,y4);
//        double Norm = sqrt(dX * dX + dY * dY);
//        double uDx = dX/Norm;
//        double uDy = dY/Norm;
//        double aX = uDx * sqrt(3)/2 - uDy *0.5;
//        double aY = uDx * 1/2 + uDy * sqrt(3)/2;
//        double bX = uDx * sqrt(3)/2 + uDy * 0.5;
//        double bY = - uDx * 0.5 + uDy * sqrt(3)/2;
        
//       double point1X = (firstX + 2 * aX);
//       double point1Y = (firstY + 2 * aY);
       
 //      double point2X = (firstX + 2 * bX);
  //     double point2Y = (firstY + 2 * bY);
       
       //ezgl::point2d point1(point1X, point1Y);
       //ezgl::point2d point2(point2X, point2Y);
       
        ezgl::point2d start(firstX,firstY); 
        ezgl::point2d end(lastX,lastY);
        if(curvePoints == 0){
            g->set_color(ezgl::PURPLE);
        g-> set_line_width(5);
        g->draw_line(start,end); // if no curve point just draw line from from point to to point
        //g-> draw_line(point1, start);
       // g-> draw_line(point2, start);
        }
        else if(curvePoints == 1){
            LatLon curvePointPosition = getStreetSegmentCurvePoint(segID,0);
            double curvePointX = x_from_lon(curvePointPosition.longitude());
            double curvePointY = y_from_lat(curvePointPosition.latitude());
        g->set_color(ezgl::PURPLE);
        g-> set_line_width(5);
        g->draw_line(start,{curvePointX, curvePointY});
        g->draw_line({curvePointX, curvePointY}, end);   // if there is only one curve point, then draw from start to first curve point to last point
        }
        else if(curvePoints > 1){
            std::vector<LatLon> curvePointVector;
            std::vector<std::pair<double,double>> xyPointsOfCurves;
            for(int j = 0; j < curvePoints; j++){
                LatLon curvePointPosition = getStreetSegmentCurvePoint(segID,j);
                curvePointVector.push_back(curvePointPosition);
            }
            for(int k = 0; k < curvePointVector.size();k++){
                double curvePointsX = x_from_lon(curvePointVector[k].longitude());
                double curvePointsY = y_from_lat(curvePointVector[k].latitude());
                std::pair<double,double> tempPair = {curvePointsX,curvePointsY};
                xyPointsOfCurves.push_back(tempPair);
            }
            g->set_color(ezgl::PURPLE);
            g-> set_line_width(5);
             g->draw_line(start, {xyPointsOfCurves[0].first, xyPointsOfCurves[0].second});
              for(int k = 0; k < xyPointsOfCurves.size()-1; k++){
                g->set_color(ezgl::PURPLE);
                g-> set_line_width(5);
                g-> draw_line({xyPointsOfCurves[k].first,xyPointsOfCurves[k].second},{xyPointsOfCurves[k+1].first,xyPointsOfCurves[k+1].second}); //iterate through all curve points and draw between them
            }
            //double last = xyPointsOfCurves.size();
             double lastXs = xyPointsOfCurves.back().first;
             double lastYs = xyPointsOfCurves.back().second;
            g->set_color(ezgl::PURPLE);
            g-> set_line_width(5);
            g-> draw_line({lastXs,lastYs} , end);
        }
        
        //std::cout << "Curve points: " << curvePoints << std::endl;
     }
    directions(pathSegmentIDs);
   }
 }
}

void drawSegments(ezgl::renderer *g){
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
}

void directions(std::vector<int> path){
     int lastSegID = path.back();
     int lastSegStreetID = getStreetSegmentInfo(lastSegID).streetID;
     std::string lastSegName = getStreetName(lastSegStreetID);
     int beforeLastSegID = path.back()-1;
     int beforeLastSegStreetID = getStreetSegmentInfo(beforeLastSegID).streetID;
     std::string beforeLastSegName = getStreetName(beforeLastSegStreetID);
     if(path.size() == 1){
         int ID = path[0];
         int streetID = getStreetSegmentInfo(ID).streetID;
         std::string segName = getStreetName(streetID);
         std::cout << "straight on " << segName << "for " << findStreetSegmentLength(ID) << "m" << std::endl;
         std::cout << "        " << std::endl;
     } else{
         // for the first segment only 
         int firstSegid = path[0];
         int firstSegStreetid = getStreetSegmentInfo(firstSegid).streetID;
         std:: string segidName = getStreetName(firstSegStreetid);
         std::cout << "Straight on " << segidName << " for " << findStreetSegmentLength(firstSegid) << "m" << std::endl;
         std::cout << "    " << std::endl;
    for(int i = 0; i < path.size()-1; i++){
        int firstSegID = path[i]; // fetch segment ID of current road
        int secondSegID = path[i+1]; //fetch segment ID of next segment about to go to
        int firstSegStreetID = getStreetSegmentInfo(firstSegID).streetID; //. fetch streetID of first seg
        int secondSegStreetID = getStreetSegmentInfo(secondSegID).streetID; // fetch streetID of second seg
        std::string firstSegName = getStreetName(firstSegStreetID);
        std::string secondSegName = getStreetName(secondSegStreetID);
        if(calculateDirection(firstSegID, secondSegID) == "straight"){
            std::cout << "Straight on " << firstSegName << " for " << findStreetSegmentLength(firstSegID) <<"m" << std::endl;
            std::cout<< "             "<< std::endl;
        }else if(calculateDirection(firstSegID, secondSegID) == "left"){
            std::cout << "Head left onto " << secondSegName << std::endl; 
                      std::cout<< "             "<< std::endl;  
        }else if(calculateDirection(firstSegID, secondSegID) == "right"){
            std:: cout << "Head right onto " << secondSegName << std::endl;
            std::cout<< "             "<< std::endl;
        }
        
        /*int fromID = getStreetSegmentInfo(path[i]).from;
        int toID = getStreetSegmentInfo(path[i]).to;
        
        std::cout << fromID << std::endl;
        std::cout << toID << std::endl;
        std::cout << "     " << std::endl;*/
    }
     /*if(calculateDirection(beforeLastSegID,lastSegID) == "straight"){
         std::cout << "Straight on " << beforeLastSegName << std::endl;
         std::cout<< "             "<< std::endl;
     }
      else if(calculateDirection(beforeLastSegID,lastSegID) == "right"){
         std::cout << "Head right on " << lastSegName << std::endl;
         std::cout<< "             "<< std::endl;
     }else if(calculateDirection(beforeLastSegID,lastSegID) == "left"){
         std::cout << "Head left on " << lastSegName << std::endl;
         std::cout<< "             "<< std::endl;
     }*/
 }
}


void writeInMiddleOfStreetSection(std::vector<StreetSegmentIdx> streetSectionSegments, ezgl:: renderer *g){
    int numSegmentsInSection = streetSectionSegments.size();
    LatLon coordsOfmiddleFrom;
    LatLon coordsOfmiddleTo;
    StreetSegmentIdx middleSegmentOfSectionIdx = streetSectionSegments[numSegmentsInSection/2];
    StreetSegmentInfo middleSegmentInfo = getStreetSegmentInfo(middleSegmentOfSectionIdx);
    std::string streetName = getStreetName(middleSegmentInfo.streetID);
    if (middleSegmentInfo.numCurvePoints <= 0){
        
        coordsOfmiddleFrom = getIntersectionPosition(middleSegmentInfo.from);
        coordsOfmiddleTo = getIntersectionPosition(middleSegmentInfo.to);
        
        
        
    }else{
        int numberOfCurvePoints = middleSegmentInfo.numCurvePoints;
        int middleCurvePoint = numberOfCurvePoints/2;
         if(numberOfCurvePoints > 1){  
            coordsOfmiddleFrom = getStreetSegmentCurvePoint(middleSegmentOfSectionIdx, middleCurvePoint);
            int middlePlusPoint = numberOfCurvePoints + 1;
            coordsOfmiddleTo = getStreetSegmentCurvePoint(middleSegmentOfSectionIdx, middlePlusPoint);
        }else{
           coordsOfmiddleFrom = getIntersectionPosition(middleSegmentInfo.from);
        coordsOfmiddleTo = getIntersectionPosition(middleSegmentInfo.to);
        }
        
    }
    double fromX = x_from_lon(coordsOfmiddleFrom.longitude());
    double fromY = y_from_lat(coordsOfmiddleFrom.latitude());
    
    double toX = x_from_lon(coordsOfmiddleTo.longitude());
    double toY = y_from_lat(coordsOfmiddleTo.latitude());
    
    double deltaX = toX - fromX;
    double deltaY = toY - fromY;
    
    double midPointX = (toX + fromX)/2;
    double midPointY = (toY + fromY)/2;
    
    ezgl::point2d center(midPointX,midPointY);
    
    double theta = atan(deltaY/deltaX);
    theta = M_1_PI * 180; //convert to degrees
    
    if((theta < -90) && (theta > -270)){
        theta = theta + 180;
    }else if((theta > 90) && (theta < 270)){
        theta = -180 + theta;
    }
    
    
    findDistanceBetweenTwoPoints(std::make_pair(coordsOfmiddleFrom, coordsOfmiddleTo));
    
    g->set_text_rotation(theta);
    g->draw_text(center, streetName, 100, 100);
    
}