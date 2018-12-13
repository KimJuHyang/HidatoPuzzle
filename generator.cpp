#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>

using namespace std;

#define MAXSIZE 100
#define MAXN 10001

typedef struct Point{
  int x, y;
}point;

point direction[8] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0},
                      {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};

// puzzle : generator , check : 숫자 입력되었는지 확인하는 배열
int puzzle[MAXSIZE][MAXSIZE], check[MAXSIZE][MAXSIZE];
int arr[MAXN];

int Gen(int width, int height, point pos, int cnt, int N)
{
  int i;
  point next;

  if(cnt == N)
    return 1;

  for(i = 0; i < 8; i++)
  {
    //현재 좌표와 이웃된 좌표 구성 : next
    next.x = pos.x + direction[i].x;
    next.y = pos.y + direction[i].y;

    if(next.x >= 0 && next.x < width &&
       next.y >= 0 && next.y < height &&
       check[next.x][next.y] != 1 &&
       puzzle[next.x][next.y] != 0)
    {
      check[next.x][next.y] = 1;

      // puzzle에서 -1로 나타내는 경우
      if(arr[cnt+1] == 0)
        puzzle[next.x][next.y] = -1;

      // puzzle에서 숫자로 나타내는 경우
      else
        puzzle[next.x][next.y] = cnt+1;

      if(Gen(width, height, next, cnt+1, N))
        return 1;

      check[next.x][next.y] = 0;
    }
  }
  return 0;
}

int main(){
  /* 입력 파일 */
  ifstream input;
  input.open("input.txt");

  /* 출력 파일 */
  ofstream output;
  output.open("output.txt");

  int width, height; //가로, 세로
  point start; //시작점 좌표

  input >> width >> height;

  int N = 0;
  for(int i=0; i<width; i++)
  {
    for(int j=0; j<height; j++)
    {
      input >> puzzle[i][j];
      if(puzzle[i][j] == 1)
        N++;
      check[i][j] = 0;
    }
  }

  for(int i = 0; i <= N; i++)
    arr[i] = 1;

  srand(time(NULL));

  /* grid에서의 값이 1인 좌표를 시작점 잡기 */
  while(1)
  {
    start.x = rand() % width;
    start.y = rand() % height;
    if(puzzle[start.x][start.y] == 1)
      break;
  }

  /* 1에서 4 사이의 중간값 결정  */
  int index = 1;
  while(index < N)
  {
    int randtmp = rand()%4 +1;
    for(int i=1; i<=randtmp; i++)
    {
      index++;
      if(index >= N)
        break;
      arr[index] = 0;
    }
    index++;
  }

  /* puzzle의 시작지점은 먼저 체크 */
  check[start.x][start.y] = 1;
  puzzle[start.x][start.y] = 1;

  /* puzzle generate */
  Gen(width, height, start, 1, N);

  /* output파일에 puzzle 출력*/
  output << width << “ “ << height << endl;
  for(int i = 0; i < width; i++)
  {
    for(int j = 0; j < height; j++)
    {
      output << puzzle[i][j] << " ";
    }
    output << endl;
  }
}
