#include <vector>
#include "LatLon.h" 
#include "OSMID.h"
#include "m1.h"
#include "m2.h"
#include <string>
#pragma once

extern int selectedMap;

struct map{
    std::string name;
    std::string path;
    double maxLat;
    double minLat;
    double maxLon;
    double minLon;
    double avgLat;
};

const std::vector<map> mapInfo = {
        {"Beijing, China",
                "/cad2/ece297s/public/maps/beijing_china.streets.bin",
                40.18,
                39.68,
                116.77,
                116.08,
                39.93
        },
        {"Cairo, Egypt",
                "/cad2/ece297s/public/maps/cairo_egypt.streets.bin",
                30.15,
                29.93,
                31.41,
                31.13,
                30.04
        },
        {"Cape Town, South Africa",
                "/cad2/ece297s/public/maps/cape-town_south-africa.streets.bin",
                -33.471,
                -34.363,
                19.004,
                18.302,
                -33.917
        },
        {"Golden Horseshoe, Canada",
         "/cad2/ece297s/public/maps/golden-horseshoe_canada.streets.bin",
                44.182,
                43.201,
                -78.717,
                -80.161,
                43.6915

        },
        {"Hamilton, Canada",
                "/cad2/ece297s/public/maps/hamilton_canada.streets.bin",
                43.317,
                43.155,
                -79.614,
                -80.029,
                43.236
        },
        {"Hong Kong, China",
                "/cad2/ece297s/public/maps/hong-kong_china.streets.bin",
                23.488,
                21.591,
                115.125,
                112.78,
                22.5395

        },
        {"Iceland",
                "/cad2/ece297s/public/maps/iceland.streets.bin",
                67.416,
                63.015,
                -12.085,
                -25.291,
                65.2155
        },
        {"Interlaken, Switzerland",
                "/cad2/ece297s/public/maps/interlaken_switzerland.streets.bin",
                46.7139,
                46.6491,
                7.9189,
                7.8003,
                46.6815
        },
        {"London, England",
                "/cad2/ece297s/public/maps/london_england.streets.bin",
                51.7,
                51.28,
                0.419997,
                -0.6,
                51.49
        },
        {"Moscow, Russia",
                "/cad2/ece297s/public/maps/moscow_russia.streets.bin",
                55.92,
                55.57,
                37.88,
                37.32,
                55.745
        },
        {"New Delhi, India",
                "/cad2/ece297s/public/maps/new-delhi_india.streets.bin",
                28.74,
                28.44,
                77.5,
                76.98,
                28.59
        },
        {"New York, USA",
                "/cad2/ece297s/public/maps/new-york_usa.streets.bin",
                40.96,
                40.48,
                -73.67,
                -74.36,
                40.72
        },
        {"Rio de Janeiro, Brazil",
                "/cad2/ece297s/public/maps/rio-de-janeiro_brazil.streets.bin",
                -22.37,
                -23.216,
                -42.751,
                -43.86,
                -22.793
        },
        {"Saint Helena",
                "/cad2/ece297s/public/maps/saint-helena.streets.bin",
                -15.8887,
                -16.0481,
                -5.5637,
                -5.8332,
                -15.9684
        },
        {"Singapore",
                "/cad2/ece297s/public/maps/singapore.streets.bin",
                1.48,
                1.24,
                104.03,
                103.6,
                1.36
        },
        {"Sydney, Australia",
                "/cad2/ece297s/public/maps/sydney_australia.streets.bin",
                -33.68,
                -34.06,
                151.33,
                150.83,
                -33.87
        },
        {"Tehran, Iran",
                "/cad2/ece297s/public/maps/tehran_iran.streets.bin",
                35.92,
                35.38,
                51.65,
                50.87,
                35.65
        },
        {"Tokyo, Japan",
                "/cad2/ece297s/public/maps/tokyo_japan.streets.bin",
                36.558,
                34.867,
                141.152,
                138.779,
                35.7125
        },
        {"Toronto, Canada",
                "/cad2/ece297s/public/maps/toronto_canada.streets.bin",
                43.92,
                43.48,
                -79,
                -79.79,
                43.7
        }
};

struct streetInfo{
    std::string name;
    std::vector<LatLon> positions;
};
extern std::vector<std::vector<LatLon>> points_on_segments;
extern std::vector<streetInfo> streetPositions;
extern std::vector<std::vector<std::pair<double, double>>> xy_points_segments;
extern double aspVar;
extern double avg_lat;
extern double avg_lon;
extern std::string userInput;
std::vector<LatLon> vectorOfCoord(StreetIdx street_id);
double x_from_lon(double lon);
double y_from_lat(double lat);



