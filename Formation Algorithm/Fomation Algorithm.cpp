#include <math.h>
#include <stdlib.h>
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

struct Coordinate//a basic struct to store coordinates
{
	int x;
	int y;
};

struct Index {//for point assignment
	int robot;
	int point;
	int val;
	bool pointUsed;
};


struct Robot {//struct with char. of all robots
	Coordinate robotPoint;
	int targetPt;
	bool havePoint;
	int distToTarget;
	int distToPtWanted;
};

/*struct OptimalSquare {
	Coordinate a;
	Coordinate b;
	Coordinate c;
	Coordinate d;
	int error;
};*/
int dist(Coordinate a, Coordinate b) {//returns distance between two points
	float tempX = (a.x - b.x)*(a.x - b.x);
	float tempY = (a.y - b.y)*(a.y - b.y);
	float test = tempX + tempY;
	int temp = sqrt(test);
	return temp;
}
int compare(const void *ptr1, const void *ptr2) {//for the qsort function
	Index *p1 = (Index*)ptr1;
	Index *p2 = (Index*)ptr2;
	if (p1->val < p2->val) return -1;
	if (p1->val == p2->val)return 0;
	if (p1->val > p2->val)return 1;
}
int compare1(const void *ptr1, const void *ptr2) {
	Robot *p1 = (Robot*)ptr1;
	Robot *p2 = (Robot*)ptr2;
	if (p1->distToTarget < p2->distToTarget) return -1;
	if (p1->distToTarget == p2->distToTarget)return 0;
	if (p1->distToTarget > p2->distToTarget)return 1;
}
void initArray(Coordinate arr[], int numOfElements) {
	memset(arr, 0, numOfElements);
}
//Calculate the best formation for the shape of the obstacle and # of robots

Coordinate centroid;
Coordinate square[4];
Coordinate centArr[4];
Index distIndex[16];
Robot robots[4];
int orient(Coordinate a, Coordinate b, Coordinate c) {
	/*int q = (a.y-b.y)/(a.x-b.x);
	int p = (b.y-c.y)/(b.x-c.x);*/
	int temp = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
	if (temp == 0) {
		return 0;//collinear
	}
	else if (temp<0) {
		return 1;//counterclockwise
	}
	else {
		return 2;//clockwise
	}
}

Coordinate hullCentroidCalc(Coordinate points[]) {
	int tempRes[4];
	for (int i = 0; i<4; i++) {
		tempRes[i] = -1;
	}

	int leftPoint = 0;
	for (int i = 1; i<4; i++) {
		if (points[i].x<points[leftPoint].x)
			leftPoint = i;
	}

	int a = leftPoint;
	int b = 0;
	int ct = 1;
	while (b != leftPoint || ct == 1)//changed to the ct method instead of the do-while loop
	{
		b = (a + 1) % 4;
		for (int w = 0; w<4; w++) {
			if (orient(points[a], points[w], points[b]) == 1) {
				b = w;
			}
		}
		tempRes[a] = b;
		a = b;
		ct++;
	}
	int hullNum = 0;

	initArray(centArr, sizeof(centArr) / sizeof(Coordinate));
	/*for (int i = 0; i<4; i++){ //combine with next for loop
	centArr[i].x = 0;
	centArr[i].y = 0;
	*/
	for (int z = 0; z<4; z++) {
		if (tempRes[z] != -1) {
			printf("(%d, %d)\n", points[z].x, points[z].y);
			centArr[z].x = points[z].x;
			centArr[z].y = points[z].y;
			hullNum++;
		}
	}
	if (hullNum == 4) { //shouldnt need to decrement by 1
		centroid.x = (centArr[0].x + centArr[1].x + centArr[2].x + centArr[3].x) / 4;
		centroid.y = (centArr[0].y + centArr[1].y + centArr[2].y + centArr[3].y) / 4;
	}
	else {
		centroid.x = (centArr[0].x + centArr[1].x + centArr[2].x + centArr[3].x) / 3; //divide by hullNum
		centroid.y = (centArr[0].y + centArr[1].y + centArr[2].y + centArr[3].y) / 3;
	}
	printf("%d\n", hullNum);
	centroid.x = -5;
	centroid.y = 58.3;
	printf("Centroid: (%d, %d)", centroid.x, centroid.y);
	return centroid;
}
/*Procedure for Square Optimization v1
for each degree interval (36 intervals of 10 degrees each)
1.rotate
recalculate dists
reassign points
calculate shortestDist for each robot
calculate error
if(error<currError){currError=err;}
*/
/*Procedure for Translation
	calculate new square every one unit on the coordinate grid
	calculate the new distances
	if all values fall between the standard deviation of the mean (remember to divide by n-1 not n), then put it in a buffer array and calculate the error
	at the end, choose the one with the least error*/
void squareCalc(Coordinate centroid, int size, Coordinate arr[4]) {//integration with point assignment code required, change a to centroid
	int apothem = size / 2; //change var name-done
	arr[0].x = centroid.x + apothem;
	arr[0].y = centroid.y + apothem;
	arr[1].x = centroid.x - apothem;
	arr[1].y = centroid.y - apothem;
	arr[2].x = centroid.x + apothem;
	arr[2].y = centroid.y - apothem;
	arr[3].x = centroid.x - apothem;
	arr[3].y = centroid.y + apothem;
}
void polygonCalc(Coordinate centroid, int size, Coordinate arr[4]){//given centroid and side length, calculate all the coordinates of the polygon
	
}
void checkTranslation(int currDistWanted, int squareSize) {
	Coordinate tempSquare[4];
	Coordinate buffer[20];
	int errors[25];
	currDistWanted = 250;
	int ct = 0;
	int sum = 0;
	Coordinate tempCentroid;
	tempCentroid = centroid;
	for (int i = 0; i < 4; i++) {
		tempSquare[i] = square[i];
	}
	qsort(robots, 4, sizeof(Robot), compare1);
	int initialLongestDistance = robots[3].distToTarget;
	int currLongestDistance = robots[3].distToTarget;
	printf("Longest Distance:%d\n", currLongestDistance);
	for (int i = 0; i < initialLongestDistance; i++) {
		tempCentroid.x--;
		tempCentroid.y--;
		squareCalc(tempCentroid, 30, tempSquare);
		for (int m = 0; m < 4; m++) {
			robots[m].distToTarget = dist(tempSquare[robots[m].targetPt], robots[m].robotPoint);
		}
		qsort(robots, 4, sizeof(Robot), compare1);
		if (robots[3].distToTarget < currLongestDistance) {
			currLongestDistance = robots[3].distToTarget;
			for (int i = 0; i < 4; i++)
			{
				printf("Robot %d's Point is (%d,%d) DistToPoint=%d\n", i, tempSquare[robots[i].targetPt].x, tempSquare[robots[i].targetPt].y, robots[i].distToTarget);
			}
		}
	}
	printf("The New Longest Distance: %d", currLongestDistance);
	//checkSquare();
	//errors[q - 1] = (robots[0].distToTarget - robots[0].distToPtWanted) + (robots[0].distToTarget - robots[0].distToPtWanted)
	//+ (robots[0].distToTarget - robots[0].distToPtWanted) + (robots[0].distToTarget - robots[0].distToPtWanted);

}
/*void checkSquare() {
	/*for (int i = 0; i < 4; i++) {
		robots[i].distToPtWanted = 0;
		robots[i].distToTarget = 0;
		robots[i].ptWanted = 0;
	}
	Coordinate tempSquare[4];
	int squareErrors[10];
	int currDistWanted=300;
	for (int q = 1; q <= 10; q++) {
		for (int m = 0; m < 4; m++) {
			tempSquare[m].x = centroid.x + (square[q].x - centroid.x)*cos(q * 36) -
				(square[m].y - centroid.y)*sin(q * 36);
			tempSquare[m].y = centroid.y + (square[q].x + centroid.x)*sin(q * 36) -
				(square[m].y - centroid.y)*cos(q * 36);
		}
		int tempDist[4];
		for (int i = 0; i < 4; i++) {//recalculate distances
			tempDist[i] = dist(robots[i].robotPoint, tempSquare[robots[i].targetPt]);
		}
		for (int i = 0; i < 4; i++) {//figure out the preferred distances for each robot
			for (int m = 0; m < 4; m++) {
				int temp = dist(robots[i].robotPoint, tempSquare[m]);
				if (temp < currDistWanted) {
					currDistWanted = temp;
				}
			}
			robots[i].ptWanted = currDistWanted;
		}
		for (int m = 0; m <= 15; m++) {//init the robot number for the distIndex array
			distIndex[m].robot = (m % 4);
			distIndex[m].point = (m / 4);
			distIndex[m].val = dist(robots[distIndex[m].point].robotPoint, tempSquare[m % 4]);
		}
		qsort(distIndex, 16, sizeof(Index), compare);
		for (int i = 0; i <= 15; i++) {
			if (robots[distIndex[i].robot].havePoint == false) {
				robots[distIndex[i].robot].targetPt = distIndex[i].point;
				robots[distIndex[i].robot].havePoint = true;
			}
		}
		for (int i = 0; i < 4; i++) {
			robots[i].distToTarget = dist(robots[i].robotPoint, tempSquare[robots[i].targetPt]);
			robots[i].distToPtWanted = dist(robots[i].robotPoint, tempSquare[robots[i].ptWanted]);
		}
		squareErrors[q - 1] = (robots[0].distToTarget - robots[0].distToPtWanted) + (robots[0].distToTarget - robots[0].distToPtWanted)
			+ (robots[0].distToTarget - robots[0].distToPtWanted) + (robots[0].distToTarget - robots[0].distToPtWanted);
	}
	qsort(squareErrors, 10, sizeof(squareErrors), compare);
	printf("Best Error: %d", squareErrors[0]);
}*/

/*Progression of the Code
1.Determine the convex hull of the coordinates
2.Find the centroid of the convex hull.
3.Determine the coordinates of the square with the center as the centroid .
4.Assign each point on the square to one of the robots.
5.Optimize the square.*/
int main()
{
	clock_t begin, end;
	double time_spent;

	begin = clock();
	/* here, do your time-consuming job */
	
	int p[4][2] = { {-5,15},{-17,-10},{40,23},{10,8} };
	
	Robot robots[4] = { { { -5,15 }, 2, false, 0 },
	{ { -17,-10 }, 2, false, 0 },
	{ { 40,23 }, 2, false, 0 },
	{ { 10,8}, 2, false, 0 }
	};
	int squareSize = 30;
	Coordinate robotPoints[4];
	int theta = 0;//declarations for the square optimization part of the code
	for (int i = 0; i < 4; i++) {
		robotPoints[i].x = robots[i].robotPoint.x;
		robotPoints[i].y = robots[i].robotPoint.y;
	}
	//struct Coordinate points[] = { { 34, 26 }, { 4, 7 }, { 3, 8 }, { 8, 24 } };
	centroid = hullCentroidCalc(robotPoints);
	squareCalc(centroid, squareSize, square);
	for (int i = 0; i<4; i++) {
		printf("(%d,%d)\n", square[i].x, square[i].y);
	}
	for (int i = 0; i < 4; i++) {
		robots[i].targetPt = -1;
	}
	for (int m = 0; m <= 15; m++) {//init the distIndex array
		distIndex[m].robot = (m % 4);
		distIndex[m].point = (m / 4);
		distIndex[m].pointUsed = false;
		distIndex[m].val = dist(robots[distIndex[m].point].robotPoint, square[m % 4]);
	}
	qsort(distIndex, 16, sizeof(Index), compare);
	for (int i = 0; i <= 15; i++) {//point assignment
		int temp = distIndex[i].point;
		if (robots[distIndex[i].robot].havePoint == false && distIndex[i].pointUsed == false) {
			robots[distIndex[i].robot].targetPt = temp;
			robots[distIndex[i].robot].havePoint = true;
			for (int m = 0; m <= 15; m++) {
				if (temp == distIndex[m].point) {
					distIndex[m].pointUsed = true;
				}
			}
		}
	}
	int currDistWanted = 250;
	for (int i = 0; i < 4; i++) {//figure out the preferred distances for each robot
		for (int m = 0; m < 4; m++) {
			int temp = dist(robots[i].robotPoint, square[m]);
			if (temp < currDistWanted) {
				currDistWanted = temp;
			}
		}
		robots[i].distToPtWanted= currDistWanted;
	}
	for (int i = 0; i < 4; i++) {
		printf("Robot %d's Assigned Point on the Square is:(%d,%d) and it's distance from that point is %d\n", i + 1, square[robots[i].targetPt].x, square[robots[i].targetPt].y, dist(square[robots[i].targetPt], robots[i].robotPoint));
		robots[i].distToTarget = dist(square[robots[i].targetPt], robots[i].robotPoint);
		int temp = abs((robots[0].distToPtWanted - robots[0].distToTarget));
		int error = (temp + abs((robots[1].distToTarget - robots[1].distToPtWanted))
			+ abs((robots[2].distToTarget - robots[2].distToPtWanted)) + abs((robots[3].distToTarget - robots[3].distToPtWanted)))/4;
		printf("Error is:%d\n", error);
	}
	//Coordinate test[4];
	/*for (int i = 0; i <= 10; i++) {

		for (int m = 0; m < 4; m++) {
			int angle = i * 36;
			angle = (angle * 3.14) / 180;
			test[m].x = centroid.x + (square[m].x - centroid.x)*cos(angle) -
				(square[m].y - centroid.y)*sin(angle);
			printf("%d\n", test[m].x);
			test[m].y = centroid.y + (square[m].x + centroid.x)*sin(angle) -
				(square[m].y - centroid.y)*cos(angle);
			printf("%d\n", test[m].y);
		}
	}*/
	Coordinate tempSquare[4];
	Coordinate buffer[20];
	int errors[25];
	currDistWanted = 250;
	int ct = 0;
	int sum = 0;
	Coordinate tempCentroid;
	tempCentroid = centroid;
	for (int i = 0; i < 4; i++) {
		tempSquare[i] = square[i];
	}
	printf("New TempSquare");
	for (int i = 0; i < 4; i++) {
		printf("(%d,%d)", tempSquare[i].x, tempSquare[i].y);
	}
	qsort(robots, 4, sizeof(Robot), compare1);
	int initialLongestDistance = robots[3].distToTarget;
	printf("%d", robots[3].distToTarget);
	int currLongestDistance = robots[3].distToTarget;
	printf("Initial Longest Distance:%d\n", currLongestDistance);
	for (int i = 0; i < initialLongestDistance; i++) {
		tempCentroid.x++;
		tempCentroid.y++;
		squareCalc(tempCentroid, squareSize, tempSquare);
		for (int m = 0; m < 4; m++) {
			robots[m].distToTarget = dist(tempSquare[robots[m].targetPt], robots[m].robotPoint);
		}
		qsort(robots, 4, sizeof(Robot), compare1);
		if (robots[3].distToTarget < currLongestDistance) {
			currLongestDistance = robots[3].distToTarget;
			for (int i = 0; i < 4; i++)
			{
					p[i][0] = tempSquare[robots[i].targetPt].x;
					p[i][1] = tempSquare[robots[i].targetPt].y;
				printf("Robot %d's Point is (%d,%d) DistToPoint=%d\n",i, tempSquare[robots[i].targetPt].x, tempSquare[robots[i].targetPt].y, robots[i].distToTarget);
			}
		}
	}
	/*for (int i = 0; i < initialLongestDistance; i++) {
		tempCentroid.x++;
		squareCalc(tempCentroid, squareSize, tempSquare);
		for (int m = 0; m < 4; m++) {
			robots[m].distToTarget = dist(tempSquare[robots[m].targetPt], robots[m].robotPoint);
		}
		qsort(robots, 4, sizeof(Robot), compare1);
		if (robots[3].distToTarget < currLongestDistance) {
			currLongestDistance = robots[3].distToTarget;
			for (int i = 0; i < 4; i++)
			{
				p[i][0] = tempSquare[robots[i].targetPt].x;
				p[i][1] = tempSquare[robots[i].targetPt].y;
				printf("Robot %d's Point is (%d,%d) DistToPoint=%d\n", i, tempSquare[robots[i].targetPt].x, tempSquare[robots[i].targetPt].y, robots[i].distToTarget);
			}
		}
	}
	for (int i = 0; i < initialLongestDistance; i++) {
		tempCentroid.y++; 
			squareCalc(tempCentroid, squareSize, tempSquare);
		for (int m = 0; m < 4; m++) {
			robots[m].distToTarget = dist(tempSquare[robots[m].targetPt], robots[m].robotPoint);
		}
		qsort(robots, 4, sizeof(Robot), compare1);
		if (robots[3].distToTarget < currLongestDistance) {
			currLongestDistance = robots[3].distToTarget;
			for (int i = 0; i < 4; i++)
			{
				p[i][0] = tempSquare[robots[i].targetPt].x;
				p[i][1] = tempSquare[robots[i].targetPt].y;
				printf("Robot %d's Point is (%d,%d) DistToPoint=%d\n", i, tempSquare[robots[i].targetPt].x, tempSquare[robots[i].targetPt].y, robots[i].distToTarget);
			}
		}
	}
	printf("The New Longest Distance: %d", currLongestDistance);
	for (int i = 0; i < 4; i++) {
			printf("(%d,%d)\n", p[i][0],p[i][1]);
	}
	//checkSquare();
	return 0;*/
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("%f", time_spent);
}
