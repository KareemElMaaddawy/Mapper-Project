/* 
 * Copyright 2021 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <iostream>
#include <cmath> // gain access to math constants and functions such as M_PI
#include <math.h>
#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "LatLon.h" // required to use the Latlon parameters (Latitude and longitdue)
#include <algorithm>
#include <utility>
#include "TrieNode.h"
<<<<<<< HEAD
#include <vector>

#define rOfEarth 6372797.560856
=======
#define RADIUS_OF_EARTH 6371000
>>>>>>> bc0c2921345166ea949e16c0de95d86d536c24a9

// loadMap will be called with the name of the file that stores the "layer-2"
// map data accessed through StreetsDatabaseAPI: the street and intersection 
// data that is higher-level than the raw OSM data). 
// This file name will always end in ".streets.bin" and you 
// can call loadStreetsDatabaseBIN with this filename to initialize the
// layer 2 (StreetsDatabase) API.
// If you need data from the lower level, layer 1, API that provides raw OSM
// data (nodes, ways, etc.) you will also need to initialize the layer 1 
// OSMDatabaseAPI by calling loadOSMDatabaseBIN. That function needs the 
// name of the ".osm.bin" file that matches your map -- just change 
// ".streets" to ".osm" in the map_streets_database_filename to get the proper
// name.

int numOfStreets;
std::string *streetNames;
struct TrieNode *root; //root for streetnames trie

double degToRad(double degree);//helper to convert degrees to radians

std::vector<std::vector<std::string>> street_names_of_intersection; //stores the street names for each intersection
                                                                    //Includes repetition!!
std::vector<std::vector<IntersectionIdx>> intersections_of_a_street; 

std::vector<std::vector<StreetSegmentIdx>> segments_of_an_intersection;

std::vector<std::vector<IntersectionIdx>> adjacent_intersections;

bool loadMap(std::string map_streets_database_filename) {
    bool load_successful = loadStreetsDatabaseBIN(map_streets_database_filename); //Indicates whether the map has loaded successfully
   
    std::cout << "loadMap: " << map_streets_database_filename << std::endl; 
    
    if(load_successful){
        numOfStreets = getNumStreets();
        streetNames = new std::string[numOfStreets]; //container to store streetnames

        for(int i = 0; i < numOfStreets; i++){ //formatting and storing street names w/o spaces and all lowercase
            streetNames[i] = getStreetName(i);
            streetNames[i].erase(std::remove(streetNames[i].begin(), streetNames[i].end(), ' '), streetNames[i].end());
            std::transform(streetNames[i].begin(),streetNames[i].end(), streetNames[i].begin(), [] (unsigned char c){return std::tolower(c);});
        }

        root = makeNode(); //creating root for trie

        for(int i = 0; i < numOfStreets; i++){ //inputs all streetnames and indices into the trie
            insertToTrie(root, streetNames[i], i);
        }

        segments_of_an_intersection.resize(getNumIntersections());

        street_names_of_intersection.resize(getNumIntersections());
        
        intersections_of_a_street.resize(getNumStreets());
        
        adjacent_intersections.resize(getNumIntersections());

        for (int intersection = 0; intersection < getNumIntersections(); intersection++){
            for (int segment = 0; segment < getNumIntersectionStreetSegment(intersection); segment++){
                int streetSeg_id = getIntersectionStreetSegment(intersection, segment);
                segments_of_an_intersection[intersection].push_back(streetSeg_id);
                  
                /*FINDING THE STREET NAMES THAT PASS THROUGH AN INTERSECTION*/
                //gets the street id of a specific segment
                StreetIdx street_ID_of_segment = getStreetSegmentInfo(streetSeg_id).streetID;
                //stores the name at specified street id in th street names function
                street_names_of_intersection[intersection].push_back(getStreetName(street_ID_of_segment));

                 /*FINDING INTERSECTIONS OF A STREET*/
                ///if the intersection does not exists on the list of intersections of a street, then add it
                if (!(std::find(intersections_of_a_street[street_ID_of_segment].begin(), intersections_of_a_street[street_ID_of_segment].end(), intersection) != intersections_of_a_street[street_ID_of_segment].end())){  
                    intersections_of_a_street[street_ID_of_segment].push_back(intersection);
                }
                
                 /*FINDING ADJACENT INTERSECTIONS*/
                 /*logic: since each segment has one "from" and one "to" intersection ids,
                  * then as we loop through the segments we can check if the current intersection 
                  * is the "from" or the "to" of the segment
                  * if it is the "from", then we add the "to" of the segment to it's adjacent intersections
                  * if it's the to, we move on to the next segment
                  * This will allow each intersection to be added only once to another intersection's adjacent list
                  * while only going in a direction that fits one way streets */
                
                 //check if the current intersection  is the "from" intersection
                if(intersection == getStreetSegmentInfo(segment).from){
                    //add the "to" intersection to the adjacent intersections of the current intersection
                    adjacent_intersections[intersection].push_back(getStreetSegmentInfo(segment).to);
                }  
            }
        }
    }
    return load_successful;
    
}

void closeMap() {
    //Clean-up your map related data structures here
    closeStreetDatabase();
    
    delete[] streetNames;
    destroyTrie(root);//dealloc trie
}
<<<<<<< HEAD
double degToRad(double );
double degToRad(double degree){
    double pi = M_PI;
    return(degree * (pi/180));
}

=======

double degToRad(double degree){//convert degrees to radians
    return (((M_PI)/180)*degree);
}
>>>>>>> bc0c2921345166ea949e16c0de95d86d536c24a9

// Returns the nearest point of interest of the given name to the given position
// Speed Requirement --> none 
POIIdx findClosestPOI(LatLon my_position, std::string POIname){
    double smallestDistance = 99999999999;
    double distanceToCurrentPOI = 0;
    int totalNumOfPOI = getNumPointsOfInterest();
    POIIdx closestPOIIdx = -1;
    
    for(int i = 0; i < totalNumOfPOI; i++){
        if(getPOIName(i) == POIname){
            distanceToCurrentPOI = findDistanceBetweenTwoPoints(std::make_pair(my_position,getPOIPosition(i)));
            
            if(distanceToCurrentPOI < smallestDistance){ //stores value of smallest distance and the POI index for it
                smallestDistance = distanceToCurrentPOI;
                closestPOIIdx = i;
            }
        }
    }
    return closestPOIIdx;
}

// Returns the area of the given closed feature in square meters
// Assume a non self-intersecting polygon (i.e. no holes)
// Return 0 if this feature is not a closed polygon.
// Speed Requirement --> moderate
double findFeatureArea(FeatureIdx feature_id){
    double area = 0;
    double yMax = -999999;
    double avgLat = 0;
    
    int numOfPoints = getNumFeaturePoints(feature_id);//finds number of points on the feature
    
    LatLon *featurePoints = new LatLon[numOfPoints];
    
    for(int i = 0; i < numOfPoints; i++){
        featurePoints[i] = getFeaturePoint(feature_id, i);//creates array of featurePoints
        avgLat += degToRad(featurePoints[i].latitude());
    }
    
    avgLat = avgLat/numOfPoints;
    
    double *x = new double[numOfPoints];
    double *y= new double[numOfPoints];
    double radLat, radLong;
    
    for(int i = 0; i < numOfPoints; i++){
        radLat = degToRad(featurePoints[i].latitude());
        radLong = degToRad(featurePoints[i].longitude());
        
        x[i] = RADIUS_OF_EARTH*radLong*cos(avgLat);
        y[i] = RADIUS_OF_EARTH*radLat;
        
        
        
        
        if(y[i] > yMax) yMax = y[i];
    }
    if(x[0] == x[numOfPoints - 1] && y[0] == y[numOfPoints - 1]){
        for(int i = 0; i < numOfPoints; i++){
            if(i == numOfPoints - 1){
                area += x[i]*y[0] - y[i]*x[0];
            }else{
                area+= x[i]*y[i+1] - y[i]*x[i+1];
            }
        }
        area = abs(area/2);
        
        delete[] x;
        delete[] y;
        delete[] featurePoints;
        
        return area;
    }else{
        delete[] x;
        delete[] y;
        delete[] featurePoints;
        
        return 0;
    }
}

std::vector<StreetIdx> findStreetIdsFromPartialStreetName(std::string street_prefix){
    street_prefix.erase(std::remove(street_prefix.begin(), street_prefix.end(), ' '), street_prefix.end());
    std::transform(street_prefix.begin(), street_prefix.end(), street_prefix.begin(), [] (unsigned char c){return std::tolower(c);});
    
    return findStreetName(root, street_prefix);
}

double findDistanceBetweenTwoPoints(std::pair<LatLon, LatLon> points){
    // Converting latitude and longitudes from degrees to radians
   double lat1 =  kDegreeToRadian*(points.first.latitude());
   double long1 = kDegreeToRadian*(points.first.longitude());
   double lat2 =  kDegreeToRadian*(points.second.latitude());
   double long2 = kDegreeToRadian*(points.second.longitude());
   
   double latAvg = (lat2 - lat1)/2;
   double x1 = long1*(cos(latAvg));
   double x2 = long2*(cos(latAvg));
   double y1 = lat1;
   double y2 = lat2;
   double diffinY = y2 - y1;
   double diffinX = x2-x1;
   
   double power1 = pow(diffinY,2);
   double power2 = pow(diffinX,2);
   double distance = kEarthRadiusInMeters * (sqrt(power1+power2));
   return distance;
}

LatLonBounds findStreetBoundingBox(StreetIdx street_id){
    LatLonBounds stub;
    return stub;
}

double findStreetLength(StreetIdx street_id){
    double stub;
    return stub;
}

double findStreetSegmentLength(StreetSegmentIdx street_segment_id){
// case if no curve points exist
    if(getStreetSegmentInfo(street_segment_id).numCurvePoints == 0){
        double interSec1 = double(getStreetSegmentInfo(street_segment_id).from);
        double interSec2 = double(getStreetSegmentInfo(street_segment_id).to);
        std::pair<LatLon, LatLon> point;
        point = std::make_pair(getIntersectionPosition(interSec1), getIntersectionPosition(interSec2));
        double length = findDistanceBetweenTwoPoints(point);
        return length;
    } 
    // case if there is only one curve point
    else if(getStreetSegmentInfo(street_segment_id).numCurvePoints == 1){
        double interSec1 = double(getStreetSegmentInfo(street_segment_id).from);
        double interSec2 = double(getStreetSegmentInfo(street_segment_id).to);
        std::pair<LatLon, LatLon> pair1;
        std::pair<LatLon, LatLon> pair2;
        pair1 = std::make_pair(getIntersectionPosition(interSec1), getStreetSegmentCurvePoint(street_segment_id, 0));
        pair2 = std::make_pair(getStreetSegmentCurvePoint(street_segment_id,0), getIntersectionPosition(interSec2));
        double length = findDistanceBetweenTwoPoints(pair1) + findDistanceBetweenTwoPoints(pair2);
        return length;
    }
    else{
        double length = 0;
        for(int i = 0; i < getStreetSegmentInfo(street_segment_id).numCurvePoints; i++){
            double tempLength = 0;
            
            double interSec1 = double(getStreetSegmentInfo(street_segment_id).from);
            double interSec2 = double(getStreetSegmentInfo(street_segment_id).to);
            double lastCp = double((getStreetSegmentInfo(street_segment_id).numCurvePoints) -1);
            //calculate distances between each successive curve point first then add the from and two towards the end
            std::pair<LatLon, LatLon> curvePoints;
            std::pair<LatLon, LatLon> pair1;
            std::pair<LatLon, LatLon> pair2;
            pair1 = std::make_pair(getIntersectionPosition(interSec1), getStreetSegmentCurvePoint(street_segment_id, 0));
            pair2 = std::make_pair(getStreetSegmentCurvePoint(street_segment_id, lastCp), getIntersectionPosition(interSec2));
            curvePoints = std::make_pair(getStreetSegmentCurvePoint(street_segment_id,i),getStreetSegmentCurvePoint(street_segment_id,i+1));
            tempLength += findDistanceBetweenTwoPoints(curvePoints);
            length = tempLength + findDistanceBetweenTwoPoints(pair1) + findDistanceBetweenTwoPoints(pair2);
            
        }
        return length;
    }
}

double findStreetSegmentTravelTime(StreetSegmentIdx street_segment_id){
    // fetch the speed limit of the segment in question
    float speedLimit = getStreetSegmentInfo(street_segment_id).speedLimit;
    // return the speed which is length/time
    return findStreetSegmentLength(street_segment_id)/speedLimit;
    
}




//GHAMR'S FUNCTIONS////////////////////////////////////////////////////////////////////



// Returns the nearest intersection to the given position
// Speed Requirement --> none
IntersectionIdx findClosestIntersection(LatLon my_position){
    IntersectionIdx stub;
    return stub;
}






// Return all intersection ids at which the two given streets intersect
// This function will typically return one intersection id for streets
// that intersect and a length 0 vector for streets that do not. For unusual 
// curved streets it is possible to have more than one intersection at which 
// two streets cross.
// There should be no duplicate intersections in the returned vector.
// Speed Requirement --> high
std::vector<IntersectionIdx> findIntersectionsOfTwoStreets(std::pair<StreetIdx, StreetIdx> street_ids){
    std::vector<IntersectionIdx> stub;
    return stub;
}



    




//GHAMR: DONE////////////////////////////////////////////////////////////////////////

// Returns the street names at the given intersection (includes duplicate 
// street names in the returned vector)
// Speed Requirement --> high 
std::vector<std::string> findStreetNamesOfIntersection(IntersectionIdx intersection_id){
    
    return street_names_of_intersection[intersection_id];
}

// Returns the street segments that connect to the given intersection 
// Speed Requirement --> high
std::vector<StreetSegmentIdx> findStreetSegmentsOfIntersection(IntersectionIdx intersection_id){
   
    return segments_of_an_intersection[intersection_id];
}

// Returns all intersections along the a given street.
// There should be no duplicate intersections in the returned vector.
// Speed Requirement --> high
std::vector<IntersectionIdx> findIntersectionsOfStreet(StreetIdx street_id){
    
   
    return intersections_of_a_street[street_id];
    
}

// Returns all intersections reachable by traveling down one street segment 
// from the given intersection (hint: you can't travel the wrong way on a 
// 1-way street)
// the returned vector should NOT contain duplicate intersections
// Speed Requirement --> high 
std::vector<IntersectionIdx> findAdjacentIntersections(IntersectionIdx intersection_id){
    
    return adjacent_intersections[intersection_id];
}