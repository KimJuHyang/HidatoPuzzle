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

point direction[8] = {{-1, -1}, {1, -1}, {1, 0},
                      {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};

int puzzle[MAXSIZE][MAXSIZE];
int width, height;

//이미 퍼즐에 채워진 숫자값들만 담을 벡터를 생성한다.
vector<int> betweens;

// row, col, between의 iterator 역할을 하는 n
bool solve(int r, int c, int n, int next){

  //between 배열을 다 돌았다면 모두 채워진것이므로 true
  if( n > betweens.at(betweens.size()-1))
    return true;

  //이미 채운곳일 경우 false
  if( puzzle[r][c] != -1 && puzzle[r][c] != n)
    return false;

  //벽이라면 false
  if( puzzle[r][c] == 0)
    return false;

  //좌표를 계속 바꾸며 옮길때, 그 방향에서 움직일수 없을시 다시 돌아와야한다. 그때를 대비해 backup 값으로 저장.
  int back = puzzle[r][c];
  //그러나 그 값이 이미 betweens의 값이라면 next++
  if(back == n) next++;

  puzzle[r][c] = n;
  point nextpos;

  //direction에 저장해놓은 좌표 방향대로 움직이며, solve 재귀호출
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

  //움직일 수 없는경우 다시 돌아가기
  puzzle[r][c] = back;
  return false;
}

int main(){

  //generator의 output.txt를 input으로 받는다.
  ifstream input;
  input.open("output.txt");

  //solve 후 결과를 solved.txt에 출력한다.
  ofstream output;
  output.open("solved.txt");

  input >> width >> height;
  Point start;

  //output 파일에 -1, 0을 제외한 이미 채워진 숫자값들만 betweens 벡터에 담는다.
  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      input >> puzzle[i][j];

      // 1이 있는 지점을 starting point로 잡는다.
      if(puzzle[i][j] == 1){
        start.x = i;
        start.y = j;
      }
      //벽을 의미하는 0, 채워야할 부분인 -1을 제외한 값들만 삽입하는 과정
      if(puzzle[i][j] != -1 && puzzle[i][j] != 0){
        betweens.push_back(puzzle[i][j]);
      }
    }
  }

  // 이미 있는 값을 정렬함으로써, 순서대로 이어져야할 숫자들을 알 수 있게 오름차순으로 정렬한다.
  sort(betweens.begin(), betweens.end());
  // starting point와 betweens 배열의 시작점, next값을 인자로 넘겨주어 solve 함수를 시작한다.
  solve(start.x, start.y, 1, 0);

  //solved.txt에 solve된 결과를 출력한다.
  output << "start point : " << start.x << " " << start.y << endl;
  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      output << puzzle[i][j] << " ";
    }
    output << endl;
  }

  //solve전 기존에 채워져있던 값들을 출력하여 이 숫자들을 기점으로 모든 숫자가 잘 이어졌는지 확인.
  output << "sorted between numbers : ";
  for(int i = 0; i < betweens.size(); i++){
    output << betweens.at(i) << " ";
  }

}
