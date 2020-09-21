#include <iostream>
#include <vector>
#include <algorithm>

struct Point{
  int x;
  int y;
  
  bool operator<(const Point& rhs){return (this->x < rhs.x) && (this->y < rhs.y);};
};

// genPointPerms(): Uses Heap's algorithm to generate all permutations of vector point with given size
void genPointPerms(std::vector<Point> points, int size, std::vector<std::vector<Point>> &res){

  std::vector<Point> currPath{points};
  if (size == 1){
    res.push_back(currPath);
  }else{
    genPointPerms(currPath, size-1, res);
    for(int i = 0; i < size-1; i++ ){
      if (size%2==0){
        Point temp = currPath[i];
        currPath[i] = currPath[size - 1];
        currPath[size - 1] = temp;
      } else { 
        Point temp = currPath[0];
        currPath[0] = currPath[size - 1];
        currPath[size - 1] = temp;
      }
      genPointPerms(currPath, size-1, res);
    }
  }
}

void charlieDogSearch(std::string* mat){

  Point dog; 
  std::vector<Point> food;
  Point home;
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

  // determine all permutations of food locations 
  // each permutation will be the same length of the 
  // food vector and represents a possible order/path 
  // in which the food is collected that must be tested
  int pathNum = 0;
  std::vector<std::vector<Point>> paths;
  genPointPerms(food, food.size(), paths);
  std::cout<<"Number of possible paths: "<<paths.size()<<'\n';
  std::cout<<"Paths: \n";
  // insert starting dog position and home position into possible paths
  for(std::vector<Point> currPath : paths){
    currPath.push_back(home);
    std::vector<Point>::iterator pathIt = currPath.begin();
    currPath.insert(pathIt, dog);
    std::cout<<"Path #"<<pathNum<<'\n';
    for(Point currFood : currPath){
      std::cout<<currFood.x<<", "<<currFood.y<<'\n';
    }
    pathNum++;
    std::cout<<'\n';
  }
}

int main() {
  std::string matrix[] = {"FOOF", "OCOO", "OOOH", "FOOO"};
  charlieDogSearch(matrix);


}