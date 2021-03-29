#include "pathFinding.h"

//helper to check if a left turn occurs, takes the previous street and the current street as parameters, and returns true if a left turn occurs
std::string movementDirection(StreetSegmentIdx sourceStreet, StreetSegmentIdx destStreet){
    LatLon sourceStreetStartPosition = getIntersectionPosition(getStreetSegmentInfo(sourceStreet).from);
    LatLon sourceStreetEndPosition = getIntersectionPosition(getStreetSegmentInfo(sourceStreet).to);
    LatLon destStreetStartPosition = getIntersectionPosition(getStreetSegmentInfo(destStreet).from);
    LatLon destStreetEndPosition = getIntersectionPosition(getStreetSegmentInfo(destStreet).to);

	double vectorOneX = y_from_lat(sourceStreetEndPosition.latitude()) - y_from_lat(sourceStreetStartPosition.latitude());
	double vectorOneY = x_from_lon(sourceStreetEndPosition.longitude()) - x_from_lon(sourceStreetStartPosition.longitude());
	double vectorTwoX = y_from_lat(destStreetEndPosition.latitude()) - y_from_lat(destStreetStartPosition.latitude());
	double vectorTwoY = x_from_lon(destStreetEndPosition.longitude()) - x_from_lon(destStreetStartPosition.longitude());

	double result = (vectorOneX * vectorTwoY) - (vectorOneY * vectorTwoX);

	if(result > 0){//if cross product greater than zero, left turn occurs
		return "L";
	}else if (result < 0){
		return "R";
	}else{
	    return "S";
	}
}

double calcTurnPenalty(std::vector<StreetSegmentIdx> path){
    return 0.0;
}

double computePathTravelTime(const std::vector <StreetSegmentIdx>& path, const double turn_penalty){
    double travelTime = 0;
	std::string direction;
	for(int i = 0; i < path.size(); ++i){
		if(i == 0){//no need to check for left turn for the initial street segment
			travelTime += findStreetSegmentTravelTime(path[i]);
		}else{
			direction = movementDirection(path[i - 1], path[i]);
			if(direction == "L"){
				travelTime += findStreetSegmentTravelTime(path[i]) + turn_penalty;
			}else{
				travelTime += findStreetSegmentTravelTime(path[i]);
			}
		}
	}
	return travelTime;
}

StreetSegmentIdx findSegmentBetweenIntersections(const IntersectionIdx from, const IntersectionIdx to){
	int numOfStreetSegments = getNumIntersectionStreetSegment(from);
	for(int i = 0; i < numOfStreetSegments; ++i){
		StreetSegmentIdx streetSeg = getIntersectionStreetSegment(from, i);
		StreetSegmentInfo streetSegInfo = getStreetSegmentInfo(streetSeg);
		if(streetSegInfo.to == to || streetSegInfo.from == to){
			return streetSeg;
		}
	}
	return -1;
}

double calculateCost(const double turnPenalty,const IntersectionIdx soruceIntersection,const IntersectionIdx destinationIntersection){
	double score = 0;
	std::vector<StreetSegmentIdx> path = {findSegmentBetweenIntersections(soruceIntersection, destinationIntersection)};
	score += computePathTravelTime(path, turnPenalty);
	return score;
}

double calculateHeuristic(const IntersectionIdx currentIntersection, const IntersectionIdx destinationIntersection){
	return findDistanceBetweenTwoPoints(std::make_pair(getIntersectionPosition(currentIntersection), getIntersectionPosition(destinationIntersection)));
}

std::vector<StreetSegmentIdx> reconstructPath(std::unordered_map<IntersectionIdx, IntersectionIdx> pathOrigin, const IntersectionIdx intersect_id_destination, const IntersectionIdx intersect_id_start){
	std::vector<StreetSegmentIdx> path;
	IntersectionIdx currentIntersection = intersect_id_destination;
	while(currentIntersection != intersect_id_start){
		IntersectionIdx previousIntersection = pathOrigin[currentIntersection];
		path.insert(path.begin(), findSegmentBetweenIntersections(previousIntersection, currentIntersection));
	}
	return path;
}

std::vector<StreetSegmentIdx> findPathBetweenIntersections(
	const IntersectionIdx intersect_id_start,
	const IntersectionIdx intersect_id_destination,
	const double turn_penalty
	){
	std::unordered_map<IntersectionIdx, double> costSoFar;
	std::unordered_map<IntersectionIdx, IntersectionIdx> pathOrigin;
	std::priority_queue<prioElem, std::vector<prioElem>, compare> queueOfIntersections; //mini heap to store queue for streets that need to be expanded upon

	queueOfIntersections.push(prioElem{intersect_id_start, 0});
	pathOrigin[intersect_id_start] = intersect_id_start;
	costSoFar[intersect_id_start] = 0;

	while(!queueOfIntersections.empty()){
		prioElem current = queueOfIntersections.top(); //get value from the top of the queue

		queueOfIntersections.pop(); //remove entry from queue

		if(current.intersection == intersect_id_destination){ //allow for early exit if destination reached
			break;
		}

		std::vector<IntersectionIdx> neighbors = findAdjacentIntersections(current.intersection);

		for(int i = 0; i < neighbors.size(); ++i){
			double newCost = costSoFar[current.intersection] + calculateCost(turn_penalty, current.intersection, neighbors[i]);
			if(costSoFar.find(neighbors[i]) == costSoFar.end() || newCost < costSoFar[neighbors[i]]){
				costSoFar[neighbors[i]] = newCost;
				double priority = newCost + calculateHeuristic(neighbors[i], intersect_id_destination);
				queueOfIntersections.push(prioElem{neighbors[i], priority});
                pathOrigin[neighbors[i]] = current.intersection;
			}
		}
	}
	return reconstructPath(pathOrigin,intersect_id_destination, intersect_id_start);
}