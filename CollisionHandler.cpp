//==============================================================================
// CApp_CollisionHandler.cpp
//==============================================================================
#include "Collision_Handler.h"


CollisionHandler::CollisionHandler() {
	msgbuf = new char[128];
}

bool CollisionHandler::hasCollisions() {
	return (currentCollisions.size() > 0);
}
void CollisionHandler::broadPhaseCheck(double xOffset, double yOffset) {
	//Clear previous vector
	possibleCollisions.clear();
	//start at 1 to skip entity map
	for(int a = 1; a < CEntity::getNumEntities(); a++) {
		//other objects need to check if on screen
		int xFlag = 0;
		int yFlag = 0;
		//is the object on the screen
		//currently 200 pixel border
		if (((CEntity::getEntityAt(a))->getX() + xOffset > -200) && ((CEntity::getEntityAt(a))->getX() + xOffset < 2760)) {xFlag = 1;}
		if (((CEntity::getEntityAt(a))->getY() + yOffset > -200) && ((CEntity::getEntityAt(a))->getY() + yOffset < 2120)) {yFlag = 1;}
		//Add Pointer to object on screen to possible collision
		if((xFlag == 1) && (yFlag==1)) {
			possibleCollisions.push_back(CEntity::getEntityAt(a));
		}
	}

	if(possibleCollisions.size() > 1) {
		narrowPhaseCheck(xOffset,yOffset);
	}
}

void CollisionHandler::narrowPhaseCheck(double xOffset, double yOffset) {
	for(int b = 0; b < possibleCollisions.size()-1; b++) {
		for(int a = b + 1; a < possibleCollisions.size(); a++) {	
			CEntity* e1 = possibleCollisions.at(a);
			CEntity* e2 = possibleCollisions.at(b);
			Vector2D minDist = areColliding(e1,e2);	
			if(!((minDist.xComponent == 0) && (minDist.yComponent == 0))) {
				entityMinDistance m;	
				m.distance = minDist;
				m.entity1=e1;
				m.entity2=e2;
				collision c;				
				c.collidingEntities.push_back(e1);
				c.collidingEntities.push_back(e2);
				c.minDistances.push_back(m);
				currentCollisions.push_back(c);
			}
		}
	}	
}

//==============================================================================
bool CollisionHandler::checkRadius(CEntity* e1, CEntity* e2) {
	double e1R;
	double e2R;
	double radius;
	double xDist;
	double yDist;
	double x1,x2,y1,y2;
	//Add check to make sure not null
	//Find radii
	e1R = e1->getRadius();
	e2R = e2->getRadius();
	//Compare radii. take the larger
	if(e1R > e2R) {radius = e1R;}
	else {radius = e2R;}
	//Find distance between centers			
	x1 = e1->getX();
	y1 = e1->getY();
	x2 = e2->getX();
	y2 = e2->getY();	
	xDist = x2 - x1;
	yDist = y2 - y1;
	double dist = sqrt(pow(xDist,2) + pow(yDist,2));
	return (dist < radius);
}

//==============================================================================
Vector2D CollisionHandler::areColliding(CEntity* e1, CEntity* e2) {
	Vector2D minDist;
	minDist.xComponent = 0.0;
	minDist.yComponent = 0.0;		
	if(!checkRadius(e1,e2)) {
		return minDist;
	}
	else {		
		int aVertices = e1->numVertices;
		int bVertices = e2->numVertices;
		int maxTrials = aVertices+bVertices;
		Vertex vertex1,vertex2;
		Vector2D edge, normal, minVector;
		double normalMagnitude, minA, maxA, minB, maxB, projection, interval, minInterval;
		double  vx, vy;
		minInterval = 99999;
		//Loop through every edge of both polygons
		for(int c = 0; c < maxTrials; c++) {
			if(c < aVertices) {
				vertex1 = e1->vertices[c];
				if(c+1 < aVertices) {vertex2 = e1->vertices[c+1];}
				else {vertex2 = e1->vertices[0];}				
			}
			else {
				vertex1 = e2->vertices[c-aVertices];
				if(c+1 < maxTrials) {vertex2 = e2->vertices[c+1];}
				else {vertex2 = e2->vertices[0];}
			}

			edge.xComponent = vertex2.x - vertex1.x;
    		edge.yComponent = vertex2.y - vertex1.y;
			normal.xComponent = edge.yComponent;
			normal.yComponent = -edge.xComponent;
			normalMagnitude = sqrt(pow(normal.xComponent,2) + pow(normal.yComponent,2));
			normal.xComponent /= normalMagnitude;
			normal.yComponent /= normalMagnitude;
			//1st polygon projection of every vertex
			vx = e1->vertices[0].x + e1->getX();
			vy = e1->vertices[0].y + e1->getY();
			minA = vx * normal.xComponent + vy*normal.yComponent;
			maxA = minA;
			for(int d = 1; d < e1->numVertices; d++) {
				vx = e1->vertices[d].x + e1->getX();
				vy = e1->vertices[d].y + e1->getY();						
				projection = vx * normal.xComponent + vy*normal.yComponent;
				if (projection < minA) {minA = projection;}
				else {if(projection > maxA) {maxA = projection;}}
			}
			//2nd polygon projection of every vertex
			vx = e2->vertices[0].x + e2->getX();
			vy = e2->vertices[0].y + e2->getY();
			minB = vx * normal.xComponent + vy*normal.yComponent;
			maxB = minB;
			for(int d = 1; d < e2->numVertices; d++) {
				vx = e2->vertices[d].x + e2->getX();
				vy = e2->vertices[d].y + e2->getY();						
				projection = vx * normal.xComponent + vy*normal.yComponent;
				if (projection < minB) {minB = projection;}
				else {if(projection > maxB) {maxB = projection;}}
			}
			//try to find overlap
			if(minA < minB) {
				interval = minB - maxA;
			}
			else {
				interval = minA - maxB;
			}				
			if(interval > 0) {
				//no collision go to next collision pair
				return minDist;
			}					
			else {
				//if there is overlap on this axis store it 
				//if it is less than overlap on other axes
				if(abs(interval) < abs(minInterval)) {
					minInterval = interval;
					minVector = normal;
				}
				if(c+1 == maxTrials) {		
					//sprintf(msgbuf,"collision \n");
					//OutputDebugString(msgbuf);
					//create collision object
					minVector *= minInterval;
					minVector.xComponent = abs(minVector.xComponent);
					minVector.yComponent = abs(minVector.yComponent);					
					minDist = minVector;					
					return minDist;
				}
			}
		}
	}
	return minDist;
}

//==============================================================================
bool CollisionHandler::willCollide(CEntity* e1, CEntity* e2, Vector2D e1Offset, Vector2D e2Offset) {
	//shift values
	//call are colliding
	//return
	return false;
}

//==============================================================================
Vector2D CollisionHandler::findMinDist(CEntity* e1, CEntity* e2, int i) {
	entityMinDistance emd;
	emd.entity1 = e1;
	emd.entity2 = e2;
	std::vector<entityMinDistance>::iterator distIter;
	Vector2D minDist;
	minDist.xComponent = 0.0;
	minDist.yComponent = 0.0;
	distIter = std::find(currentCollisions.at(i).minDistances.begin(),currentCollisions.at(i).minDistances.end(), emd);
    if(distIter != currentCollisions.at(i).minDistances.end()) {
		std::vector<entityMinDistance>::iterator beginIndex = currentCollisions.at(i).minDistances.begin();				
		entityMinDistance tmp = currentCollisions.at(i).minDistances.at(distIter-beginIndex);
		minDist = tmp.distance;
	}
	else {
		//Somehow indicate there exists no such distance
	}
	return minDist;
}
//==============================================================================
void CollisionHandler::shiftMinDist(CEntity* e1, Vector2D distance, int i) {
	int hDir = -1;
	int vDir = -1;
	if(distance.xComponent > 0) {
		hDir = VECTOR_RIGHT;
	} else {
		hDir = VECTOR_LEFT;
	}
	if(distance.yComponent > 0) {
		vDir = VECTOR_DOWN;
	} else {
		vDir = VECTOR_UP;
	}
	int numDistances = currentCollisions.at(i).minDistances.size();
	for(int a = 0; a < numDistances; a++) {
		bool match = false;
		entityMinDistance toCheck = currentCollisions.at(i).minDistances.at(a);
		CEntity* toMove;
		if(toCheck.entity1 == e1) {
			match = true;
			toMove = toCheck.entity1;
		} else if(toCheck.entity2 == e1) {
			match = true;
			toMove = toCheck.entity2;
		}
		if(match) {
			//Check where the other center is compared to current object X-wise
			if(toMove->getX() > e1->getX()) {
				//Check direction we are moving the current object
				if(hDir == VECTOR_RIGHT) {
					toCheck.distance.xComponent += distance.xComponent;
				} else if(hDir == VECTOR_LEFT) {
					toCheck.distance.xComponent -= distance.xComponent;
				}					
			} else {
				if(hDir == VECTOR_RIGHT) {
					toCheck.distance.xComponent -= distance.xComponent;
				} else if(hDir == VECTOR_LEFT) {
					toCheck.distance.xComponent += distance.xComponent;
				}	
			}
			//Check where the other center is compared to current object Y-wise
			if(toMove->getY() > e1->getY()) {
				//Check direction we are moving the current object
				if(vDir == VECTOR_DOWN) {
					toCheck.distance.yComponent += distance.yComponent;
				} else if(vDir == VECTOR_UP) {
					toCheck.distance.yComponent -= distance.yComponent;
				}		
			} else {
				//Check direction we are moving the current object
				if(vDir == VECTOR_DOWN) {
					toCheck.distance.yComponent -= distance.yComponent;
				} else if(vDir == VECTOR_UP) {
					toCheck.distance.yComponent += distance.yComponent;
				}		
			}
		}
	}
}

//==============================================================================
void CollisionHandler::handleCollisions() {
	//merge collisions
	if(currentCollisions.size() > 1) {
		mergeCollisions();
	}
	//Every collision
	for(int i = 0; i < currentCollisions.size(); i++) {
		//start in the anchor have a running sum of distance moved in either direction
		//anchor = anchorpoint essentially
		//int index = -1;		
		//sort according to x
		std::sort(currentCollisions.at(i).collidingEntities.begin(), currentCollisions.at(i).collidingEntities.end(),compareCenterX);		
		int anchor;
		anchor = currentCollisions.at(i).collidingEntities.size()/2;
		for(int a = 0; a + anchor < currentCollisions.at(i).collidingEntities.size()-1; a++) {
			//find the min distance between a and a+1
			CEntity* e1 = currentCollisions.at(i).collidingEntities.at(anchor+a);
			CEntity* e2 = currentCollisions.at(i).collidingEntities.at(anchor+a+1);
			Vector2D minDist = findMinDist(e1,e2,i);
			if(minDist.xComponent > 0) {
				int currentX = e2->getX();
				int tmpX = currentX + minDist.xComponent;
				e2->setX(tmpX);
				e2->changeDisplacementX(minDist.xComponent);
				shiftMinDist(e2, minDist, i);
			}
		}
		for(int a = 0; anchor-a > 0; a++) {
			CEntity* e1 = currentCollisions.at(i).collidingEntities.at(anchor-a);
			CEntity* e2 = currentCollisions.at(i).collidingEntities.at(anchor-a-1);
			Vector2D minDist = findMinDist(e1,e2,i);
			if(minDist.xComponent > 0) {
			int currentX = e2->getX();
				minDist.xComponent = -minDist.xComponent;
				int tmpX = currentX + minDist.xComponent;
				e2->setX(tmpX);		
				e2->changeDisplacementX(minDist.xComponent);
				shiftMinDist(e2, minDist, i);
			}
		}
		std::sort(currentCollisions.at(i).collidingEntities.begin(), currentCollisions.at(i).collidingEntities.end(),compareCenterY);
		anchor = currentCollisions.at(i).collidingEntities.size()/2;
		for(int a = 0; a + anchor < currentCollisions.at(i).collidingEntities.size()-1; a++) {
			//find the min distance between a and a+1
			CEntity* e1 = currentCollisions.at(i).collidingEntities.at(anchor+a);
			CEntity* e2 = currentCollisions.at(i).collidingEntities.at(anchor+a+1);
			Vector2D minDist = findMinDist(e1,e2,i);
			if(minDist.yComponent > 0) {
				int currentY = e2->getY();
				int tmpY = currentY + minDist.yComponent;
				e2->setY(tmpY);
				e2->changeDisplacementY(minDist.yComponent);
				shiftMinDist(e2, minDist, i);
			}
		}
		for(int a = 0; anchor-a > 0; a++) {
			//find the min distance between a and a-1
			CEntity* e1 = currentCollisions.at(i).collidingEntities.at(anchor-a);
			CEntity* e2 = currentCollisions.at(i).collidingEntities.at(anchor-a-1);
			Vector2D minDist = findMinDist(e1,e2,i);
			if(minDist.yComponent > 0) {
				int currentY = e2->getY();
				minDist.yComponent = -minDist.yComponent;
				int tmpY = currentY + minDist.yComponent;
				e2->setY(tmpY);
				e2->changeDisplacementY(minDist.yComponent);
				shiftMinDist(e2, minDist, i);
			}
		}
	}
	currentCollisions.clear();
	possibleCollisions.clear();
}

//==============================================================================
void CollisionHandler::mergeCollisions() {
	//loop through every collision grouping
	for (int i = 0; i < currentCollisions.size(); i++) {
		//Compare to every other grouping
		for(int j = i+1; j < currentCollisions.size(); j++) {
			bool firstMatch = false;
			bool secondMatch = false;
			//compare every object in grouping to the next pair 
			for(int k = 0; k < currentCollisions.at(i).collidingEntities.size(); k++) {
				if(currentCollisions.at(i).collidingEntities.at(k) == currentCollisions.at(j).collidingEntities.at(0)) {
					firstMatch = true;					
				}
				//compare first entity of first index with second entity of second index
				if(currentCollisions.at(i).collidingEntities.at(k) == currentCollisions.at(j).collidingEntities.at(1)) {
					secondMatch = true;					
				}				
			}
			//merge based on comparison
			if(firstMatch && secondMatch) {				
				//make sure the distance isn't already stored 
				if(std::find(currentCollisions.at(i).minDistances.begin(), currentCollisions.at(i).minDistances.end(), currentCollisions.at(j).minDistances.at(0)) == currentCollisions.at(i).minDistances.end()) {
					currentCollisions.at(i).minDistances.push_back(currentCollisions.at(j).minDistances.at(0));					
				}
				currentCollisions.erase(currentCollisions.begin()+j);
				j--;
				
			}
			else if(firstMatch) {
				currentCollisions.at(i).collidingEntities.push_back(currentCollisions.at(j).collidingEntities.at(1));
				currentCollisions.at(i).minDistances.push_back(currentCollisions.at(j).minDistances.at(0));
				currentCollisions.erase(currentCollisions.begin() + j);
				j--;
			}
			else if(secondMatch) {
				currentCollisions.at(i).collidingEntities.push_back(currentCollisions.at(j).collidingEntities.at(0));
				currentCollisions.at(i).minDistances.push_back(currentCollisions.at(j).minDistances.at(0));
				currentCollisions.erase(currentCollisions.begin() + j);
				j--;
			}			
		}
	}
}

//==============================================================================
bool CollisionHandler::compareCenterX(CEntity* a, CEntity* b) {
	return (a->getX() < b->getX());
}

//==============================================================================
bool CollisionHandler::compareCenterY(CEntity* a, CEntity* b) {
	return (a->getY() < b->getY());
}

//==============================================================================