#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>//for memset
#include <Windows.h>

#define PI 3.14159
#define polygonType 3

struct Coordinate {
	float x;
	float y;
};

Coordinate centArr[polygonType];
Coordinate arr[100];
Coordinate centroid;

float squared(float x) {
	return x*x;
}
void polygonCalc(Coordinate centroid, int numOfSides, int r, Coordinate *arr) {
	float angle = 2 * PI / numOfSides;
	float x = 0.0, y = 0.0;
	for (int i = 0; i < numOfSides; i++)
	{
		arr[i].x = centroid.x + r * sin(i * angle);
		arr[i].y = centroid.y + r * cos(i * angle);
	}
	for (int i = 0; i < numOfSides; i++) {
		printf("Cart:(%f,%f)\n", arr[i].x, arr[i].y);
		float r = sqrt(squared(arr[i].x) + squared(arr[i].y));
		printf("Polar:(%f,%f)\n", r, atan2(arr[i].y, arr[i].x)*180/PI);
	}
}

void initArray(Coordinate arr[], int numOfElements) {
	memset(arr, 0, numOfElements);
}

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
	int tempRes[polygonType] = { 0 };
	for (int i = 0; i < polygonType; i++) {
		tempRes[i] = -1;
	}
	int leftPoint = 0;
	for (int i = 1; i<polygonType; i++) {
		if (points[i].x<points[leftPoint].x)
			leftPoint = i;
	}

	int a = leftPoint;
	int b = 0;
	int ct = 1;
	while (b != leftPoint || ct == 1)//changed to the ct method instead of the do-while loop
	{
		b = (a + 1) % polygonType;
		for (int w = 0; w<polygonType; w++) {
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
	for (int z = 0; z<polygonType; z++) {
		if (tempRes[z] != -1) {
			hullNum++;
			//printf("(%f, %f)\n", points[z].x, points[z].y);
			centArr[z].x = points[z].x;
			centArr[z].y = points[z].y;
		}
	}
	Coordinate sum = { 0 };
	for (int i = 0; i < polygonType; i++) {
		sum.x += centArr[i].x;
		sum.y += centArr[i].y;
	}
	centroid.x = sum.x / hullNum;
	centroid.y = sum.y / hullNum;
	//printf("HUllNUM:%f\n", hullNum);
	printf("Centroid: (%f, %f)", centroid.x, centroid.y);
	return centroid;
}
/*Coordinate sum = { 0 };
for (int i = 0; i < polygonType; i++) {
sum.x += hullArray[i].x;
sum.y += hullArray[i].y;
}
centroid.x = sum.x / hullNum; centroid.y = sum.y / hullNum;
//printf("%f\n", hullNum);
//printf("Centroid: (%f, %f)", centroid.x, centroid.y);
return centroid;*/
void startClock(LARGE_INTEGER start, LARGE_INTEGER frequency) {
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
}
LARGE_INTEGER endClock(LARGE_INTEGER end, LARGE_INTEGER start, LARGE_INTEGER elapsedTime, LARGE_INTEGER frequency) {
	QueryPerformanceCounter(&end);
	elapsedTime.QuadPart = end.QuadPart - start.QuadPart;

	elapsedTime.QuadPart *= 1000000;
	elapsedTime.QuadPart /= frequency.QuadPart;
	return elapsedTime;
}
int main() {
	LARGE_INTEGER StartingTime, EndingTime,
		ElapsedMicroSeconds;
	LARGE_INTEGER Frequency;

	//startClock(StartingTime, Frequency);
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	Coordinate initialPositions[] = {
		{ 0,87 },{ 23,54 },{ 43,41 },
	};
	centroid = hullCentroidCalc(initialPositions);
	polygonCalc(centroid, polygonType, 10, arr);

	//ElapsedMicroSeconds = endClock(EndingTime, StartingTime, ElapsedMicroSeconds, Frequency);

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroSeconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

	ElapsedMicroSeconds.QuadPart *= 1000000;//multiplying by 1 million to get to microseconds
	ElapsedMicroSeconds.QuadPart /= Frequency.QuadPart;//dividing by ticks per second
	printf("Runtime:%ld", ElapsedMicroSeconds);
}