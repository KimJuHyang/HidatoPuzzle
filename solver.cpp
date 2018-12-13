#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm>
using namespace std;
#define MAXSIZE 100
#define MAXN 10001

typedef struct Point{
  int x, y;
}point;

point direction[8] = {{-1, -1}, {1, -1}, {1, -1}, {1, 0},
                      {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};

int puzzle[MAXSIZE][MAXSIZE];
int width, height;
vector<int> betweens;

bool solve(int r, int c, int n, int next){
  if( n > betweens.at(betweens.size()-1))
    return true;

  if( puzzle[r][c] != -1 && puzzle[r][c] != n)
    return false;

  if( puzzle[r][c] == 0 && betweens.at(next) == n)
    return false;

  int back = puzzle[r][c];
  if(back == n) next++;

  puzzle[r][c] = n;
  point nextpos;
  for(int i = 0; i < 8; i++){
    nextpos.x = r+direction[i].x;
    nextpos.y = c+direction[i].y;

    if(nextpos.x >= 0 && nextpos.x < width &&
       nextpos.y >= 0 && nextpos.y < height &&
       puzzle[nextpos.x][nextpos.y] != 0){  //세그멘테이션 오류방지
         if(solve(r+direction[i].x, c+direction[i].y, n+1, next))
           return true;
       }
  }

  puzzle[r][c] = back;
  return false;
}

int main(){
  ifstream input;
  input.open("output.txt");
  ofstream output;
  output.open("solved.txt");

  input >> width >> height;
  Point start;

  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      input >> puzzle[i][j];
      if(puzzle[i][j] == 1){
        start.x = i;
        start.y = j;
      } // catch start point
      if(puzzle[i][j] != -1 && puzzle[i][j] != 0){
        betweens.push_back(puzzle[i][j]);
      }
    }
  }
  sort(betweens.begin(), betweens.end());
  solve(start.x, start.y, 1, 0);

  output << "start point : " << start.x << " " << start.y << endl;
  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      output << puzzle[i][j] << " ";
    }
    output << endl;
  }
  output << "sorted between numbers : ";
  for(int i = 0; i < betweens.size(); i++){
    output << betweens.at(i) << " ";
  }

}
