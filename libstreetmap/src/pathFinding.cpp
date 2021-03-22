#include <functional>
#include <queue>

//helper to check if a left turn occurs, takes the previous street and the current street as parameters, and returns true if a left turn occurs
bool checkForLeftTurn(StreetSegmentIdx sourceStreet, StreetSegmentIdx destStreet){
	double vectorOneX = x_from_lat(sourceStreet.To.latitude) - x_from_lat(sourceStreet.From.latitude);
	double vectorOneY = y_from_long(sourceStreet.To.longitude) - y_from_long(sourceStreet.From.longitude);
	double vectorTwoX = x_from_lat(destStreet.To.latitude) - x_from_lat(destStreet.From.latitude);
	double vectorTwoY = y_from_long(destStreet.To.longitude) - y_from_long(destStreet.From.longitude);

	if(((vectorOneX * vectorTwoY) - (vectorOneY * vectorTwoX)) > 0){//if cross product greater than zero, left turn occurs
		return True;
	}else{
		return False;
	}
}

double computePathTravelTime(const std::vector <StreetSegmentIdx>& path, const double turn_penalty){
	double travelTime = 0;
	bool leftTurn = false;
	for(int i = 0; i < path.size(); ++i){
		if(i = 0){//no need to check for left turn for the initial street segment
			travelTime += findStreetSegmentTravelTime(path[i]);
		}else{
			leftTurn = checkForLeftTurn(path[i - 1], path[i]);
			if(leftTurn){
				travelTime += findStreetSegmentTravelTime(path[i]) + turn_penalty;
			}else{
				travelTime += findStreetSegmentTravelTime(path[i]);
			}
		}
	}
	return travelTime;
}

std::<StreetSegmentIdx> findSegmentBetweenIntersections(const IntersectionIdx from, const IntersectionIdx to){
	int numOfStreetSegments = getNumIntersectionsStreetSegment(from);
	for(int i = 0; i < numOfStreetSegments; ++i){
		StreetSegmentIdx streetSeg = getIntersectionStreetSegment(from, i);
		if(streetSeg == to){
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
	return findDistanceBetweenTwoPoints(getIntersectionPosition(currentIntersection), getIntersectionPosition(destinationIntersection));
}

std::vector<StreetSegmentIdx> reconstructPath(const std::unordered_map<IntersectionIdx> pathOrigin, const IntersectionIdx intersect_id_destination, const IntersectionIdx intersect_id_start){
	std::vector<StreetSegmentIdx> path;
	IntersectionIdx currentIntersection = intersect_id_destination;
	while(currentIntersection != intersect_id_start){
		IntersectionIdx previousIntersection = pathOrigin[currentIntersection];
		path.insert(path.begin(), findSegmentBetweenIntersections(previous, current));
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
	priority_queue<double, IntersectionIdx, std::greater<double>> queueOfIntersections; //mini heap to store queue for streets that need to be expanded upon

	queueOfIntersections.put(intersect_id_start, 0);
	pathOrigin.insert(intersect_id_start, intersect_id_start);
	costSoFar.insert(intersect_id_start, 0);

	while(!pq.empty()){
		IntersectionIdx current = queueOfIntersections.top(); //get value from the top of the queue
		
		queueOfIntersections.pop(); //remove entry from queue

		if(current == intersect_id_destination){ //allow for early exit if destination reached
			break;
		}

		std::vector<IntersectionIdx> neighbors = findAdjacentIntersections(current);

		for(int i = 0; i < neighbors.size(); ++i){
			double newCost = cost[current] + calculateCost(current, neighbors[i]);
			if(costSoFar.find(neighbors[i]) == cost.end() || newCost < costSoFar[next]){
				if(costSoFar.find(neighbors[i]) == costSoFar.end()){
					costSoFar.insert(neighbors[i], newCost);
				}else{
					costSoFar[neighbors[i]] = newCost;
				}
				double priority = newCost + calculateHeuristic(next, intersect_id_destination);
				queueOfIntersections.push(next, priority);
				if(pathOrigin.find(neighbors[i]) != pathOrigin.end()){
					pathOrigin[neighbors[i]] = current;
				}else{
					pathOrigin.insert(neighbors[i], current);
				}
				
			}
		}
	}
	return recontructPath(pathOrigin,intersect_id_destination, intersect_id_start);
}