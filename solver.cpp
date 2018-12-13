#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

using namespace std;

#define MAXSIZE 100

typedef struct Point{
  int x, y;
}point;

point direction[8] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0},
                      {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};

int puzzle[MAXSIZE][MAXSIZE];
int width, height;
int cnt = 0; // 0을 제외한 숫자 (퍼즐 채워야 할 숫자)

// n : 퍼즐을 채워나가는 index
bool solve(int r, int c, int n){

  //n이 마지막번호까지 도달했음 = 퍼즐을 solve 함
  if(n > cnt)
    return true;

  //이미 채운곳일 경우 false
  if(puzzle[r][c] != -1 && puzzle[r][c] != n)
    return false;

  //벽일 경우 false
  if(puzzle[r][c] == 0)
    return false;

  //좌표를 계속 바꾸며 옮길때, 그 방향에서 움직일수 없을시 다시 돌아와야한다. 그때를 대비해 backup 값으로 저장.
  int back = puzzle[r][c];

  puzzle[r][c] = n;

  //direction에 저장해놓은 좌표 방향대로 움직이며, solve 재귀호출
  point nextpos;
  for(int i = 0; i < 8; i++){
    nextpos.x = r+direction[i].x;
    nextpos.y = c+direction[i].y;

    if(nextpos.x >= 0 && nextpos.x < width &&
       nextpos.y >= 0 && nextpos.y < height &&
       puzzle[nextpos.x][nextpos.y] != 0){  //세그멘테이션 오류방지
         if(solve(nextpos.x, nextpos.y, n+1))
           return true;
       }
  }

  // 위에서 8방향을 돌아본 데서 다음 단계로 가지 못하면 원래대로 돌아옴
  puzzle[r][c] = back;
  return false;
}

int main(){
  //generator의 output.txt를 input으로 받는다.
  ifstream input("output.txt");

  //solve 후 결과를 solved.txt에 출력한다.
  ofstream output("solved.txt");

  input >> width >> height;

  Point start;

  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      input >> puzzle[i][j];

      // 1이 있는 지점을 starting point로 잡는다.
      if(puzzle[i][j] == 1){
        start.x = i;
        start.y = j;
      }
      //0을 제외한 숫자 (퍼즐을 채워야될 숫자)
      if(puzzle[i][j] != 0)
        cnt++;
    }
  }

  // starting point와 넣을 시작값을 넘겨준다.
  solve(start.x, start.y, 1);

  //solved.txt에 solve된 결과를 출력한다.
  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      output << puzzle[i][j] << " ";
    }
    output << endl;
  }

}
