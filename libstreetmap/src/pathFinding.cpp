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

double calculateCost(const std::vector <StreetSegmentIdx>& path, const double turnPenalty, const IntersectionIdx finalDest){
	double score = 0;
	score += computePathTravelTime(path, turnPenalty);
	score += findDistanceBetweenTwoPoints(path[path.size()-1].To, finalDest);
	return score;
}

std::vector <StreetSegmentIdx> reconstructPath(IntersectionIdx current, std::vector<StreetSegmentIdx> visited){
	std::std::vector<StreetSegmentIdx> path = {current};
	while
}

double calculateHeuristic(const IntersectionIdx currentIntersection, const IntersectionIdx destinationIntersection){
	return findDistanceBetweenTwoPoints(getIntersectionPosition(currentIntersection), getIntersectionPosition(destinationIntersection));
}

std::vector<StreetSegmentIdx> findPathBetweenIntersections(
	const IntersectionIdx intersect_id_start,
	const IntersectionIdx intersect_id_destination,
	const double turn_penalty
	){
	std::unordered_map<IntersectionIdx, double> cost;
	std::unordered_map<IntersectionIdx, IntersectionIdx> pathOrigin;
	PriorityQueue<IntersectionIdx, double> queueOfIntersections; //mini heap to store queue for streets that need to be expanded upon

	queueOfIntersections.put(intersect_id_start, 0);
	pathOrigin[intersect_id_start] = intersect_id_start;
	cost[intersect_id_start] = 0;

	while(!pq.empty()){
		IntersectionIdx current = queueOfIntersections.get();

		if(current == intersect_id_destination){ //allow for early exit if destination reached
			break;
		}

		std::vector<IntersectionIdx> neighbors = findAdjacentIntersections(current);

		for(int i = 0; i < neighbors.size(); ++i){
			double newCost = cost[current] + calculateCost(current, next);
			if(cost.find(next) == cost.end() || newCost < cost[next]){
				cost[next] = newCost;
				double priority = newCost + calculateHeuristic(next, intersect_id_destination);
				queueOfIntersections.put(next, priority);
				pathOrigin[next] = current;
			}
		}
	}
	return path;
}