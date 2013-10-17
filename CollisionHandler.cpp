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
void CollisionHandler::broadPhaseCheck(float xOffset, float yOffset) {
	//Clear previous vector
	possibleCollisions.clear();
	//start at 1 to skip entity map
	for(int a = 1; a < CEntity::entityList.size(); a++) {
		//other objects need to check if on screen
		int xFlag = 0;
		int yFlag = 0;
		//is the object on the screen
		//currently 200 pixel border
		if (((CEntity::entityList.at(a))->x + xOffset > -200) && ((CEntity::entityList.at(a))->x + xOffset < 2760)) {xFlag = 1;}
		if (((CEntity::entityList.at(a))->y + yOffset > -200) && ((CEntity::entityList.at(a))->y + yOffset < 2120)) {yFlag = 1;}
		//Add Pointer to object on screen to possible collision
		if((xFlag == 1) && (yFlag==1)) {
			possibleCollisions.push_back(CEntity::entityList.at(a));
		}
	}

	if(possibleCollisions.size() > 1) {
		narrowPhaseCheck(xOffset,yOffset);
	}
}

void CollisionHandler::narrowPhaseCheck(float xOffset, float yOffset) {
	float e1R;
	float e2R;
	float radius;
	float xDist;
	float yDist;
	float x1,x2,y1,y2, vx, vy;

	for(int b = 0; b < possibleCollisions.size()-1; b++) {
		for(int a = b + 1; a < possibleCollisions.size(); a++) {
			//Add check to make sure not null
			//Find radii
			e1R = possibleCollisions.at(b)->getRadius();
			e2R = possibleCollisions.at(a)->getRadius();
			//Compare radii. take the larger
			if(e1R > e2R) {radius = e1R;}
			else {radius = e2R;}
			//Find distance between centers			
			x1 = possibleCollisions.at(a)->x;
			y1 = possibleCollisions.at(a)->y;
			x2 = possibleCollisions.at(b)->x;
			y2 = possibleCollisions.at(b)->y;
			
			xDist = x2 - x1;
			yDist = y2 - y1;

			float dist = sqrt(pow(xDist,2) + pow(yDist,2));
			if(dist > radius) {
				continue;// - i think
			}
			else {		
			    //perform small checks
				int aVertices = possibleCollisions.at(a)->numVertices;
				int bVertices = possibleCollisions.at(b)->numVertices;

				int maxTrials = aVertices+bVertices;
				Vertex vertex1,vertex2;
				vector2d edge, normal, minVector;
				float normalMagnitude, minA, maxA, minB, maxB, projection, interval, minInterval;
				minInterval = 99999;
				for(int c = 0; c < maxTrials; c++) {
					if(c < aVertices) {
						vertex1 = possibleCollisions.at(a)->vertices[c];
						if(c+1 < aVertices) {vertex2 = possibleCollisions.at(a)->vertices[c+1];}
						else {vertex2 = possibleCollisions.at(a)->vertices[0];}				
					}
					else {
						vertex1 = possibleCollisions.at(b)->vertices[c-aVertices];
						if(c+1 < maxTrials) {vertex2 = possibleCollisions.at(b)->vertices[c+1];}
						else {vertex2 = possibleCollisions.at(b)->vertices[0];}
					}

					edge.xComponent = vertex2.x - vertex1.x;
     				edge.yComponent = vertex2.y - vertex1.y;
					normal.xComponent = edge.yComponent;
					normal.yComponent = -edge.xComponent;
					normalMagnitude = sqrt(pow(normal.xComponent,2) + pow(normal.yComponent,2));
					normal.xComponent /= normalMagnitude;
					normal.yComponent /= normalMagnitude;
					//1st polygon projection
					vx = possibleCollisions.at(a)->vertices[0].x + possibleCollisions.at(a)->x;
					vy = possibleCollisions.at(a)->vertices[0].y + possibleCollisions.at(a)->y;
					minA = vx * normal.xComponent + vy*normal.yComponent;
					maxA = minA;
					for(int d = 1; d < possibleCollisions.at(a)->numVertices; d++) {
						vx = possibleCollisions.at(a)->vertices[d].x + possibleCollisions.at(a)->x;
						vy = possibleCollisions.at(a)->vertices[d].y + possibleCollisions.at(a)->y;						
						projection = vx * normal.xComponent + vy*normal.yComponent;
						if (projection < minA) {minA = projection;}
						else {if(projection > maxA) {maxA = projection;}}
					}
					//2nd polygon projection
					vx = possibleCollisions.at(b)->vertices[0].x + possibleCollisions.at(b)->x;
					vy = possibleCollisions.at(b)->vertices[0].y + possibleCollisions.at(b)->y;
					minB = vx * normal.xComponent + vy*normal.yComponent;
					maxB = minB;
					for(int d = 1; d < possibleCollisions.at(b)->numVertices; d++) {
						vx = possibleCollisions.at(b)->vertices[d].x + possibleCollisions.at(b)->x;
						vy = possibleCollisions.at(b)->vertices[d].y + possibleCollisions.at(b)->y;						
						projection = vx * normal.xComponent + vy*normal.yComponent;
						if (projection < minB) {minB = projection;}
						else {if(projection > maxB) {maxB = projection;}}
					}

					if(minA < minB) {
						interval = minB - maxA;
					}
					else {
						interval = minA - maxB;
					}
					if(interval > 0) {
						break;
					}					
					else {
						if(abs(interval) < abs(minInterval)) {
							minInterval = interval;
							minVector = normal;
						}
						if(c+1 == maxTrials) {		
							//sprintf(msgbuf,"collision \n");
							//OutputDebugString(msgbuf);
							//create collision object
							collision c;
							entityMinDistance m;							
							minVector *= minInterval;
							minVector.xComponent = abs(minVector.xComponent);
							minVector.yComponent = abs(minVector.yComponent);
							m.distance = minVector;
							m.entity1=possibleCollisions.at(a);
							m.entity2=possibleCollisions.at(b);								
							c.collidingEntities.push_back(possibleCollisions.at(a));
							c.collidingEntities.push_back(possibleCollisions.at(b));
							if(abs(minInterval) > 0) {
								c.minDistances.push_back(m);
								currentCollisions.push_back(c);
							}
						}
					}

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
		//start in the middle have a running sum of distance moved in either direction
		int middle = currentCollisions.at(i).collidingEntities.size()/2;
		float distSum = 0;		
		//sort according to x
		std::sort(currentCollisions.at(i).collidingEntities.begin(), currentCollisions.at(i).collidingEntities.end(),compareCenterX);		
		for(int a = 0; a + middle < currentCollisions.at(i).collidingEntities.size()-1; a++) {
			//find the min distance between a and a+1
            entityMinDistance emd;
			emd.entity1 = currentCollisions.at(i).collidingEntities.at(middle+a);
			emd.entity2 = currentCollisions.at(i).collidingEntities.at(middle+a+1);
			std::vector<entityMinDistance>::iterator distIter;
			distIter = std::find(currentCollisions.at(i).minDistances.begin(),currentCollisions.at(i).minDistances.end(), emd);
			if(distIter != currentCollisions.at(i).minDistances.end()) {
				currentCollisions.at(i).collidingEntities.at(middle+a+1)->x += distSum + currentCollisions.at(i).minDistances.at(distIter-currentCollisions.at(i).minDistances.begin()).distance.xComponent;
				distSum += currentCollisions.at(i).minDistances.at(distIter-currentCollisions.at(i).minDistances.begin()).distance.xComponent;
			}
			else {
				int g = 0;
			}
            //catch if no distance found
		}
		distSum = 0;
		for(int a = 0; middle-a > 0; a++) {
			entityMinDistance emd;
			emd.entity1 = currentCollisions.at(i).collidingEntities.at(middle-a);
			emd.entity2 = currentCollisions.at(i).collidingEntities.at(middle-a-1);
			std::vector<entityMinDistance>::iterator distIter;
			distIter = std::find(currentCollisions.at(i).minDistances.begin(),currentCollisions.at(i).minDistances.end(), emd);
			if(distIter != currentCollisions.at(i).minDistances.end()) {
				currentCollisions.at(i).collidingEntities.at(middle-a-1)->x -= (distSum + currentCollisions.at(i).minDistances.at(distIter-currentCollisions.at(i).minDistances.begin()).distance.xComponent);
				distSum += currentCollisions.at(i).minDistances.at(distIter-currentCollisions.at(i).minDistances.begin()).distance.xComponent;
			}   
			else {
				int g = 0;
			}
		}
		distSum = 0;
		//search minDistances for one with the two adjacent entities (a for loop)   
		std::sort(currentCollisions.at(i).collidingEntities.begin(), currentCollisions.at(i).collidingEntities.end(),compareCenterY);
		for(int a = 0; a + middle < currentCollisions.at(i).collidingEntities.size()-1; a++) {
			//find the min distance between a and a+1
			entityMinDistance emd;
			emd.entity1 = currentCollisions.at(i).collidingEntities.at(middle+a);
			emd.entity2 = currentCollisions.at(i).collidingEntities.at(middle+a+1);
			std::vector<entityMinDistance>::iterator distIter;
			distIter = std::find(currentCollisions.at(i).minDistances.begin(),currentCollisions.at(i).minDistances.end(), emd);
			if(distIter != currentCollisions.at(i).minDistances.end()) {
				currentCollisions.at(i).collidingEntities.at(middle+a+1)->y += distSum + currentCollisions.at(i).minDistances.at(distIter-currentCollisions.at(i).minDistances.begin()).distance.yComponent;
				distSum += currentCollisions.at(i).minDistances.at(distIter-currentCollisions.at(i).minDistances.begin()).distance.yComponent;
			}
			else {
				int g = 0;
			}
		}
		distSum = 0;
		for(int a = 0; middle-a > 0; a++) {
			//find the min distance between a and a-1
			entityMinDistance emd;
			emd.entity1 = currentCollisions.at(i).collidingEntities.at(middle-a);
			emd.entity2 = currentCollisions.at(i).collidingEntities.at(middle-a-1);
			std::vector<entityMinDistance>::iterator distIter;
			distIter = std::find(currentCollisions.at(i).minDistances.begin(),currentCollisions.at(i).minDistances.end(), emd);
			if(distIter != currentCollisions.at(i).minDistances.end()) {
				currentCollisions.at(i).collidingEntities.at(middle-a-1)->y -= (distSum + currentCollisions.at(i).minDistances.at(distIter-currentCollisions.at(i).minDistances.begin()).distance.yComponent);
				distSum += currentCollisions.at(i).minDistances.at(distIter-currentCollisions.at(i).minDistances.begin()).distance.yComponent;
			}
			else {
				int g = 0;
			}
		}
	}
	currentCollisions.clear();
	possibleCollisions.clear();
}

//==============================================================================
//something aint right
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
	return (a->x < b->x);
}

//==============================================================================
bool CollisionHandler::compareCenterY(CEntity* a, CEntity* b) {
	return (a->y < b->y);
}

//==============================================================================