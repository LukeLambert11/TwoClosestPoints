#include <iostream>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <fstream>
#include <cstring>
#include <vector>


using namespace std;

//prototypes
struct Point;
struct PairOfPoints;
PairOfPoints ClosestPair(const vector<Point>& points);
bool compareX(const Point &a, const Point &b);
bool compareY(const Point &a, const Point &b);
PairOfPoints ClosestPair(vector<Point> xPointSorted, vector<Point> yPointSorted);
PairOfPoints ClosestSplitPair(vector<Point> xPointSorted, vector<Point> yPointSorted, float delta);
float distanceCalc(Point a, Point b);
PairOfPoints bruteForce(vector<Point> points);

struct Point {
    float x, y;
    Point(float a, float b){
        this->x = a;
        this->y = b;
    }
    Point() {
        this->x = 0;
        this->y = 0;
    }

};
struct PairOfPoints {
    Point a, b;
    float distance;

    PairOfPoints(){
        this->a = Point();
        this->b = Point();
        this->distance = numeric_limits<float>::max();
    }
    PairOfPoints(Point a, Point b){
        this->a = a;
        this->b = b;
        this->distance = distanceCalc(a, b);
    }


};


int main() {

    int numberOfPairsOfPoints = 1105;

    std::ifstream input_file;
    input_file.open(R"(C:\Users\lukel\Documents\GitHub\TwoClosestPoints\dataSet.txt)");
    float xCoordinate, yCoordinate;
    vector<Point> points;
    if (!input_file.good()) {
        cerr << "Error opening input file: " << strerror(errno) << endl;
        return 1;
    }
    else{
        for(int i=0; i<numberOfPairsOfPoints; i++){
            input_file >> xCoordinate >> yCoordinate;
            points.emplace_back(Point(xCoordinate, yCoordinate));
        }
    }

    input_file.close();

    auto start = chrono::high_resolution_clock::now();
    PairOfPoints closestPair = ClosestPair(points);
    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);


    cout << "Closest Pair of Points O(nlgn) runtime: " << duration.count() << " microseconds\n";
    cout << "Point a: (" << closestPair.a.x << ", " << closestPair.a.y << ") Point b: (" << closestPair.b.x << ", " << closestPair.b.y << ")\n";
    cout << "Distance: " << closestPair.distance << "\n";

    auto startTwo = chrono::high_resolution_clock::now();
    PairOfPoints bruteForceClosestPair = bruteForce(points);
    auto stopTwo = chrono::high_resolution_clock::now();


    auto durationTwo = chrono::duration_cast<chrono::microseconds>(stopTwo - startTwo);

    cout << "Closest Pair of Points O(n^2) runtime: " << durationTwo.count() << " microseconds\n";
    cout << "Point a: (" <<  bruteForceClosestPair.a.x << ", " <<  bruteForceClosestPair.a.y << ") Point b: (" <<  bruteForceClosestPair.b.x << ", " <<  bruteForceClosestPair.b.y << ")\n";
    cout << "Distance: " <<  bruteForceClosestPair.distance << "\n";

}

PairOfPoints ClosestPair(const vector<Point>& points){

    int n = points.size();

   vector<Point> xPointSorted = points;
   vector<Point> yPointSorted = points;


    // O(nlgn) sorting algorithm used by C++
    sort(xPointSorted.begin(), xPointSorted.end(), compareX);
    sort(yPointSorted.begin(), yPointSorted.end(), compareY);

    /*
    cout << "sorted by X\n";
    for(int i =0; i< n; i++){
        cout << xPointSorted[i].x << ", " << xPointSorted[i].y << "\n";
    }
    cout << "sorted by Y\n";
    for(int i =0; i< n; i++){
        cout << yPointSorted[i].x << ", " << yPointSorted[i].y << "\n";
    }*/

    return ClosestPair(xPointSorted, yPointSorted);
}

PairOfPoints ClosestPair(vector<Point> xPointSorted, vector<Point> yPointSorted){

    //baseCase
    if(xPointSorted.size() <= 3){
        //have to brute force for size less than or equal to 3
       return bruteForce(xPointSorted);
    }


    vector<Point> leftHalfX(xPointSorted.begin(), xPointSorted.begin() + xPointSorted.size() / 2);
    vector<Point> leftHalfY(yPointSorted.begin(), yPointSorted.begin() + yPointSorted.size() / 2);
    vector<Point> rightHalfX(xPointSorted.begin() + xPointSorted.size() / 2, xPointSorted.end());
    vector<Point> rightHalfY(yPointSorted.begin() + yPointSorted.size() / 2, yPointSorted.end());

    /*
    cout << "Left Half X\n";
    for(int i=0; i < leftHalfX.size(); i++){
        cout << leftHalfX[i].x << ", " << leftHalfX[i].y << "\n";;
    }
    cout << "Right Half X\n";
    for(int i=0; i < rightHalfX.size(); i++){
        cout << rightHalfX[i].x << ", " << rightHalfX[i].y << "\n";;
    }
    cout << "Left Half Y\n";
    for(int i=0; i < leftHalfY.size(); i++){
        cout << leftHalfY[i].x << ", " << leftHalfY[i].y << "\n";;
    }
    cout << "Right Half Y\n";
    for(int i=0; i < rightHalfY.size(); i++){
        cout << rightHalfY[i].x << ", " << rightHalfY[i].y << "\n";;
    }*/

    PairOfPoints closestLeftPair = ClosestPair(leftHalfX, leftHalfY);
    PairOfPoints closestRightPair = ClosestPair(rightHalfX, rightHalfY);
    float delta = min(closestLeftPair.distance, closestRightPair.distance);
    PairOfPoints closestSplitPair = ClosestSplitPair(xPointSorted, yPointSorted, delta);

    //return the min of closestLeftPair, closestRightPair, closestSplitPair

    float minDistance = min(closestLeftPair.distance, min(closestRightPair.distance, closestSplitPair.distance));

    if(minDistance == closestLeftPair.distance){
        return closestLeftPair;
    }
    else if(minDistance == closestRightPair.distance){
        return closestRightPair;
    }
    else{
        return closestSplitPair;
    }

}


PairOfPoints ClosestSplitPair(vector<Point> xPointSorted, vector<Point> yPointSorted, float delta){


    float medianXCord = xPointSorted[xPointSorted.size()/2].x;

    //identify points within left/right boundary
    vector<Point> withinRange;
    for(int i=0; i< yPointSorted.size(); i++){

        if(yPointSorted[i].x < medianXCord + delta && yPointSorted[i].x > medianXCord - delta){
            withinRange.push_back(yPointSorted[i]);
        }

    }

    float closestDistance = delta;
    PairOfPoints closestSplitPair;

    //check distance within boundary

    for(int i=0; i< withinRange.size(); i++){
        int tempJ = min(i + 8, static_cast<int>(withinRange.size()));
        for(int j=i+1; j < tempJ; j++){
            if(distanceCalc(withinRange[i], withinRange[j]) < closestDistance){
                closestDistance = distanceCalc(withinRange[i], withinRange[j]);
                closestSplitPair = {PairOfPoints(withinRange[i], withinRange[j])};
            }
        }
    }

    return closestSplitPair;
}

PairOfPoints bruteForce(vector<Point> points){
    float minDistance = numeric_limits<float>::max();
    PairOfPoints twoClosestPoints;

    int n = points.size();
    for (int i=0; i<n; i++)
        for (int j=i+1; j<n; j++)
            if(minDistance > distanceCalc(points[i], points[j])){
                minDistance = distanceCalc(points[i], points[j]);
                twoClosestPoints = {PairOfPoints(points[i], points[j])};
            }

    return twoClosestPoints;
}

  float distanceCalc(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}



bool compareX(const Point &a, const Point &b) {
    if (a.x != b.x)
        return a.x < b.x;
    return a.y < b.y;
}

bool compareY(const Point &a, const Point &b) {
    if (a.y != b.y)
        return a.y < b.y;
    return a.x < b.x;
}


