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
//#include <iostream>
#include <UnitTest++/UnitTest++.h>

/*
 * This is the main that drives running
 * unit tests.
 */
//int main(int /*argc*/, char** /*argv*/) {
//    //Run the unit tests
//    int num_failures = UnitTest::RunAllTests();
//    
//    
//    
//    return num_failures;
//}


#include <iostream>
#include "m1.h"
#include "LatLon.h"

int testClosestPOI();

int main() {
    
    std::string map_path = "/cad2/ece297s/public/maps/saint-helena.streets.bin";
    
    bool load_success = loadMap(map_path);
    if(!load_success) {
        std::cerr << "Failed to load map '" << map_path << "'\n";
        return 0;
    }
    std::cout << "Successfully loaded map '" << map_path << "'\n";
    
    int numOfError = 0;
    
    numOfError += testClosestPOI();
    
    std::cout << "number of errors: " << numOfError << std::endl;
    return 0;
}

int testClosestPOI(){
//    LatLon *testPositions = new LatLon[3];
//    std::string *testNames = new std::string[3];
    LatLon *position = new LatLon((-15.9498389),(-5.6811358));
    std::string name = "St.Helena Golf Club";
    POIIdx found = findClosestPOI(*position, name);
    std::cout << found << std::endl;
    
    return 0;
}