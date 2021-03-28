/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "globalHeader.h"
#include "string"

void hilightPath(std::vector<std::vector<std::pair<std::pair<double, double>, bool>>> points_segments, 
        std::vector<StreetSegmentIdx> path){
    for (int segment = 0; segment <= path.size(); ++segment){
        for (int point = 0; point <= points_segments[path[segment]].size( ); ++point){//path[segment] produces segment id
            points_segments[path[segment]][point].second = true;
        } 
    }
}