//테스트데이터통과

#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define STACK_SIZE 1000
#define WALL 0
#define PATH 1
#define VISITED 2
#define BACKTRACKED 3

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define RIGHT_UP 4
#define RIGHT_DOWN 5
#define LEFT_DOWN 6
#define LEFT_UP 7


typedef struct testcase{
    int maze_size;
    int comp_num;
    int comp_arr[MAX];
    int maze[MAX][MAX];
}Testcase;

typedef struct pos{
    int x, y;
}Position;

typedef struct stack {
    Position pos[STACK_SIZE];
    int top;
    int comp_size;
}Stack;

void create(int test_case);
void read_maze();
void search(Testcase*);
bool movable(Testcase*, Position cur, int dir);
Position startPos(Testcase*);
void print(Testcase*);

Position moveto(Position pos, int dir);

Stack* create();
void initStack(Stack*);
void push(Stack *, Position pos);
Position pop(Stack *);
bool isEmpty(Stack *);

int test_case;
Testcase *test[MAX];

int main(){
    read_maze();
    for(int i = 0; i< test_case; i++){
        search(test[i]);
        print(test[i]);
    }
    return 0;
}

void create(int test_case){
    test[test_case] = (Testcase *)malloc(sizeof(Testcase));
    test[test_case]->comp_num = 0;
}

void read_maze(){
    int N, tmp;
	FILE* fp = fopen("input.txt", "r");
	if (fp == NULL) {
		return;
	}

	fscanf(fp, "%d", &test_case); //몇 개의 case가 있는지
	for (int i = 0; i < test_case; i++) {
		create(i); //case가 저장될 공간 할당
		fscanf(fp, "%d", &N); //해당 case의 maze 크기는 
		test[i]->maze_size = N; 
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				fscanf(fp, "%d", &tmp); 
				test[i]->maze[j][k] = tmp;
			}
		}
	}
	fclose(fp);
}

void search(Testcase *test){
    Position cur;
    Stack *s = create();
    while (1){
        cur = startPos(test);
        if(cur.x == -1 && cur.y == -1){ 
			return;
    }
    while(1){
        test->maze[cur.x][cur.y] = VISITED;
        if (isEmpty(s) && s->comp_size != 0) { // stack이 비어있을 때만 검사
				test->comp_arr[test->comp_num++] = s->comp_size+1;
				initStack(s); // 스택 초기화
				break;
			}

            bool forwarded = false;
            for(int dir = 0; dir < 8; dir++){
                if (movable(test,cur,dir)){
                    push(s,cur);
                    cur = moveto(cur,dir);
                    forwarded = true;
                    break;
                }
            }
            if(!forwarded){
                test->maze[cur.x][cur.y] = BACKTRACKED;
                if (isEmpty(s)){
                    printf("No path.\n");
                    break;
                }
                cur = pop(s);
            }
        }
    }
}

bool movable(Testcase *test, Position pos, int dir){
    Position can = moveto(pos,dir);
    if (test->maze[can.x][can.y] == PATH){
        return true;
    }
    return false;
}

Position startPos(Testcase *test){
    Position start_pos;
    for (int i =0; i< test->maze_size; i++){
        for(int j =0; j < test->maze_size; j++){
            if (test->maze[i][j] == PATH){
                start_pos.x = i;
                start_pos.y = j;

                return start_pos;
            }
        }
    }
    start_pos.x = -1;
	start_pos.y = -1;
	return start_pos;
}

void print(Testcase* test){
    for (int i = 0; i < test->comp_num; i++) {
		printf("%d ", test->comp_arr[i]);
	}
	printf("\n");
}



int offset[8][2] = { 
	{-1,0}, //0
	{0,1}, //1
	{1,0},  //2
	{0,-1},  //3
	{-1,1}, //4
	{1,1}, //5
	{1,-1}, //6
	{-1,-1} //7
};

Position moveto(Position pos, int dir) {
	Position next;
	next.x = pos.x + offset[dir][0];
	next.y = pos.y + offset[dir][1];
	return next;
}

Stack* create() {
    Stack *s = (Stack *)malloc(sizeof(Stack));
    s->top = -1;
    s->comp_size = 0;

    return s;
}

void initStack(Stack* s) {
    s->top = -1;
    s->comp_size = 0;
}

void push(Stack *s, Position pos) {
    s->comp_size++;
    s->top++;
    s->pos[s->top] = pos;
}

Position pop(Stack *s) {
    Position tmp = s->pos[s->top];
    s->top--;
    
    return tmp;
}

bool isEmpty(Stack* s) {
    if (s->top < 0) {
        return true;
    }
    return false;
}

