/* Charlie the Dog Challenge
 * Charles Sedgwick 
 * The functions defined below solve the "Charlie the Dog" challenge from CoderByte. 
 * This challenge involves finding the shortest path in a matrix between a dogs 
 * current position, marked by "C", and the home position, marked by "H", in which all
 * the food in the matrix, marked by "F", has been collected by Charlie. 
 * 
 * An example matrix: 
 *        F|O|O|F
 *        O|C|O|O
 *        O|O|O|H
 *        F|O|O|O
 *        Solution: 11
 *
 * This solution will pass the CoderByte tests but does not implement the restriction
 * that Charlie cannot pass over the home cell before collecting all the food. The last
 * test in main() illustrates this. 
 */        

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

struct Point{
  int x;
  int y;
  
  bool operator<(const Point& rhs){return (this->x < rhs.x) && (this->y < rhs.y);};
};

// numMoves(Point, Point)
int numMoves(Point p1, Point p2){
  return abs(p1.x - p2.x) + abs(p1.y-p2.y); 
}

void printPath(Point path[], int size){
    for(int i = 0; i < size; i++){
      std::cout<<path[i].x<<", "<<path[i].y<<"\n";
    }
    std::cout<<'\n';
}

// genPointPermsArr(): Uses Heap's algorithm to generate all permutations of vector point with given size
void genPointPermsArr(Point points[], int size, std::vector<std::vector<Point>*> &res, int numFood){

  if (size == 1){
    std::vector<Point> *perm = new std::vector<Point>(numFood);
    std::copy(points, points+numFood, perm->begin());
    res.push_back(perm);
  }else{
    genPointPermsArr(points, size-1, res, numFood);
    for(int i = 0; i < size-1; i++ ){
      if (size%2==0){ //if size is even 
        // swap currPath[i] with element at index size-1 in points
        Point temp = points[i];
        points[i] = points[size - 1];
        points[size - 1] = temp;
      } else { // size is odd
        // swap points[0] with element at index size-1 in points
        Point temp = points[0];
        points[0] = points[size - 1];
        points[size - 1] = temp;
      }
      genPointPermsArr(points, size-1, res, numFood);
    }
  }
}

// genPointPerms(): Uses Heap's algorithm to generate all permutations of vector point with given size
// Does not properly generate permutations due to: some difference between arrays and vectors (?)
void genPointPerms(std::vector<Point> points, int size, std::vector<std::vector<Point>> &res){

  if (size == 1){
    std::vector<Point> *currPath = new std::vector<Point>{points};
    res.push_back(*currPath);
    std::cout<<"Path: \n";
    for(int i = 0; i < points.size(); i++){
      std::cout<<points[i].x<<", "<<points[i].y<<"\n";
    }
  }else{
    genPointPerms(points, size-1, res);
    for(int i = 0; i < size-1; i++ ){
      if (size%2==0){ //if size is even 
        // swap points[i] with element at index size-1 in points
        std::swap(points[i], points[size-1]);
      } else { // size is odd
        // swap points[0] with element at index size-1 in points
        std::swap(points[0], points[size-1]);
      }
      genPointPerms(points, size-1, res);
    }
  }
}

// processMatrix: check every point in matrix for food, home or the dog and store locations in variables passed in
void processMatrix(std::string* mat, Point &dog, Point &home, std::vector<Point> &food){

  Point currFood;
  for(int i = 0; i < mat->length(); i++){
    for(int j = 0; j < mat[i].length(); j++){
      switch (mat[i][j]){
        case 'F':
          currFood = {j, i};
          food.push_back(currFood);
          break;
        case 'H':
          home.x = j;
          home.y = i;
          break;
        case 'C':
          dog.x =j;
          dog.y = i;
          break;
      }

    }
  }
}

int charlieDogSearch(std::string* mat){

  Point dog; 
  std::vector<Point> food;
  Point home;

  processMatrix(mat, dog, home, food);

  // determine all permutations of food locations 
  // each permutation will be the same length of the 
  // food vector and represents a possible order/path 
  // in which the food is collected that must be tested
  std::vector<std::vector<Point>*> paths;
  Point foodArr[food.size()];
  std::copy(food.begin(), food.end(), foodArr);
  genPointPermsArr(foodArr, food.size(), paths, food.size());
  
  /*
  std::vector<std::vector<Point>> paths;
  genPointPerms(food, food.size(), paths);
  */

  int minLength = mat->length() * mat->length();
  int pathNum = 0;
  for(std::vector<Point>* pathPtr : paths){
    std::vector<Point> path = *pathPtr;
    int length = 0;
    
    //std::cout<<"Path #: "<<pathNum<<"\n";
    pathNum++;
    
    // add start and finish points to paths
    path.push_back(home);
    std::vector<Point>::iterator pathIt = path.begin();
    path.insert(pathIt, dog);
    
    //printPath(path.data(), path.size());

    // determine the total number of moves required to move from point to point in path
    for(int i = 0; i <= path.size()-2; i++){
      length += numMoves(path[i], path[i+1]);
    }
    //std::cout<<"Path length: "<<length<<"\n";
    
    // update lowest path length found if current path requires least moves
    if( length < minLength){
      minLength = length;
    }
  }
  return minLength;

}

int main() {
  int minLength;
  std::string matrix[] = {"FOOF", "OCOO", "OOOH", "FOOO"};
  minLength = charlieDogSearch(matrix);
  // expected value of minLength: 11
  std::cout<<"Shortest path found: "<<minLength<<"\n";

  std::string matrix2[] = {"OOOO", "OOFF", "OCHO", "OFOO"};
  minLength = charlieDogSearch(matrix2);
  // expected value of minLength: 7
  std::cout<<"Shortest path found: "<<minLength<<"\n";
  
  std::string matrix3[] = {"FOOO", "OCOH", "OFOF", "OFOO"};
  minLength = charlieDogSearch(matrix3);
  // expected value of minLength: 10
  std::cout<<"Shortest path found: "<<minLength<<"\n";
  
  std::string matrix4[] = {"OOOO", "FCHF", "OOOO", "OOOO"};
  minLength = charlieDogSearch(matrix4);
  // expected value of minLength: 5
  std::cout<<"Shortest path found: "<<minLength<<"\n";
}