#include <stdio.h>

/* 线的条数: X0,X1,X2,X3,X4 */
#define LINE_NUM       5
/* 用于输出结点,如: X1上的第2个点，则为1,2 */
#define POINT_DATA_LEN 2

/*
在Windows下使用GCC编译：
Compile: gcc -c print_path.c
Build  : gcc -o print_path.exe print_path.o
 */

/*
初始化数据
  |      |      |      |      |
  |      |     X2.0    |      |
  |     X1.0    |     X3.0    |
  |      |     X2.1    |      |
  X0     |      |     X3.1    X4
  |      |     X2.2    |      | 
  |     X1.1    |     X3.2    |
  |      |     X2.3    |      |
  |      |      |      |      |
{ 0,     2,     4,     3,     0 }

说明：
1.数组第一个和最后一个元素代表开始和结束结点
2.其他元素代表对应线上的点的数量
 */
int DATA[LINE_NUM] = {0,2,4,3,0};

/*
函数功能说明：

  使用递归遍历所有数据,打印出X0--->Xn的所有路径

参数说明：
  curr_line 当前所在的线
  curr_point_in_line 正在处理的curr_line上的点
  final_line 最后一条线Xn
  path 存储遍历的路径上的结点信息
 */
void print_path(int curr_line, int curr_point_in_line, int final_line, int path[][POINT_DATA_LEN]);

int main() {
	/* 初始化路径上的结点信息 */
	int path[LINE_NUM][POINT_DATA_LEN] = {0};
	/* 遍历并输出路径信息 */
	print_path(1, 1, LINE_NUM - 1, path);
}

void print_path(int curr_line, int curr_point_in_line, int final_line, int path[][POINT_DATA_LEN]) {
	
	/* 如果是X0,则直接填入开始路径信息 */
	if(curr_line == 0) {
		path[0][0] = 0;
		path[0][1] = 0;
	}
	
	/* 对于非X0和Xn的结点,则填充路径信息,并继续遍历后续结点 */
	if(curr_line > 0 && curr_line < final_line) {
		int i = 0;
		for(i = 0; i < DATA[curr_line]; i++) {
			/* 填充路径信息 */
			path[curr_line][0] = curr_line;
			path[curr_line][1] = i;
			/* 继续遍历后续结点 */
			print_path(curr_line + 1, i, final_line, path);
		}
	}
	
	/* 如果是Xn,则填入结尾路径信息,并输出整个当次遍历的路径结果 */
	if(curr_line >= final_line) {
		path[final_line][0] = final_line;
		path[final_line][1] = 0;
		
		/* 输出当次遍历的路径 */
		int j = 0;
		for(j = 0; j < LINE_NUM - 1; j++) {
			printf("X%d.%d-->", path[j][0], path[j][1]);
		}
		printf("X%d.%d\n", path[j][0], path[j][1]);
	}
}
