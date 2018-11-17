#include "Color.h"
#include <GL\glut.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
using namespace std;
int step = 0;
int color_step = 0;
int row, col;
vector<int> painted;
vector<vector<int>> copy_board;
Color rand_color;

bool is_colored(int color)
{
	for (int i = 0; i < painted.size(); i++)
	{
		if (painted[i] == color)
			return true;
	}
	return false;
}

Color generateRandomColor()
{
	Color c;
	int count = 3;
	mt19937 range;
	range.seed(random_device()());
	uniform_real_distribution<double> distV(0.0, 1.0);
	c.R = (GLdouble)distV(range);
	c.G = (GLdouble)distV(range);
	c.B = (GLdouble)distV(range);
	return c;
}

void init2D(float r, float g, float b, int width, int height)
{
	GLdouble aspect = (GLdouble)width / (GLdouble)height;
	glClearColor(r, g, b, 0.0);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (width >= height)
		gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
	else
		gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
}

void mappingIndex(int& row, int& column, GLdouble& x1, GLdouble& y1, GLdouble& x2, GLdouble& y2, int n)
{
	x1 = -1.0f + (GLdouble)column*(2.0f / (GLdouble)n);
	y1 = 1.0f - (GLdouble)row*(2.0f / (GLdouble)n);
	x2 = x1 + (GLdouble)(2.0f / (GLdouble)n);
	y2 = y1 - (GLdouble)(2.0f / (GLdouble)n);
}

void board_display()
{
	GLdouble x1, y1, x2, y2;
	mappingIndex(row, col, x1, y1, x2, y2, copy_board.size());
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	GLdouble i;
	glBegin(GL_LINES);
	for (i = 0.0f; i <= (GLdouble)copy_board.size(); i = i + (GLdouble)(2.0f / copy_board.size()))
	{
		glVertex2d(-1.0f + i, 1.0f);
		glVertex2d(-1.0f + i, -1.0f);
		glVertex2d(1.0f, -1.0f + i);
		glVertex2d(-1.0f, -1.0f + i);
	}
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
		glVertex2d(x1, y1);
		glVertex2d(x1 + (GLdouble)(2.0f / copy_board.size()), y1);
		glVertex2d(x2, y2);
		glVertex2d(x2 - (GLdouble)(2.0f / copy_board.size()), y2);
	glEnd();
	color_step = step;
	while (color_step > 0)
	{
		Color c = generateRandomColor();
		glColor3f(c.R, c.G, c.B);
		for (int i = 0; i < copy_board.size(); i++)
		{
			for (int j = 0; j < copy_board.size(); j++)
			{
				if (copy_board[i][j] == color_step)
				{
					mappingIndex(i, j, x1, y1, x2, y2, copy_board.size());
					glBegin(GL_QUADS);
					glVertex2d(x1, y1);
					glVertex2d(x1 + (GLdouble)(2.0f / copy_board.size()), y1);
					glVertex2d(x2, y2);
					glVertex2d(x2 - (GLdouble)(2.0f / copy_board.size()), y2);
					glEnd();
				}
				glutSwapBuffers();
			}
		}
		color_step--;
	}
}
void idle(int value)
{
	glutPostRedisplay();
	glutTimerFunc(50000, idle, 0);
}
void tromino(int board_size, int x_missing, int y_missing, int x_board, int y_board, int size, vector<vector<int>>& board)
{
	if (board_size == 2)
	{
		step++;
		int i, j;
		for (i = x_board; i < (x_board + board_size); ++i)
		{
			for (j = y_board; j < (y_board + board_size); ++j)
			{
				if (!(i == x_missing && j == y_missing)) {
					board[i][j] = step;
				}
			}
		}
		return;
	}
	step++;
	int half_size = board_size / 2, x_center, y_center;
	int x_upper_right, y_upper_right, x_upper_left, y_upper_left, x_lower_right, y_lower_right, x_lower_left, y_lower_left;
	x_center = x_board + half_size;
	y_center = y_board + half_size;
	if (x_missing<x_center && y_missing< y_center)
	{
		board[x_center - 1][y_center] = board[x_center][y_center - 1] = board[x_center][y_center] = step;
		x_upper_left = x_missing; y_upper_left = y_missing;
		x_upper_right = x_center - 1; y_upper_right = y_center;
		x_lower_left = x_center; y_lower_left = y_center - 1;
		x_lower_right = x_center; y_lower_right = y_center;
	}
	else if (x_missing<x_center && y_missing >= y_center)
	{
		board[x_center - 1][y_center - 1] = board[x_center][y_center - 1] = board[x_center][y_center] = step;
		x_upper_left = x_center - 1; y_upper_left = y_center - 1;
		x_upper_right = x_missing; y_upper_right = y_missing;
		x_lower_left = x_center; y_lower_left = y_center - 1;
		x_lower_right = x_center; y_lower_right = y_center;
	}
	else if (x_missing >= x_center && y_missing<y_center)
	{
		board[x_center - 1][y_center - 1] = board[x_center - 1][y_center] = board[x_center][y_center] = step;
		x_upper_left = x_center - 1; y_upper_left = y_center - 1;
		x_upper_right = x_center - 1; y_upper_right = y_center;
		x_lower_left = x_missing; y_lower_left = y_missing;
		x_lower_right = x_center; y_lower_right = y_center;
	}
	else if (x_missing >= x_center && y_missing >= y_center)
	{
		board[x_center - 1][y_center - 1] = board[x_center - 1][y_center] = board[x_center][y_center - 1] = step;
		x_upper_left = x_center - 1; y_upper_left = y_center - 1;
		x_upper_right = x_center - 1; y_upper_right = y_center;
		x_lower_left = x_center; y_lower_left = y_center - 1;
		x_lower_right = x_missing; y_lower_right = y_missing;
	}
	tromino(half_size, x_upper_left, y_upper_left, x_board, y_board, size, board);
	tromino(half_size, x_upper_right, y_upper_right, x_board, y_board + half_size, size, board);
	tromino(half_size, x_lower_left, y_lower_left, x_board + half_size, y_board, size, board);
	tromino(half_size, x_lower_right, y_lower_right, x_board + half_size, y_board + half_size, size, board);
}
int main(int argc, char *argv[])
{
	if ((atoi(argv[1]) & (atoi(argv[1]) - 1)) != 0)
		cout << "Program only support size equal power of 2" << endl;
	else
	{
		int board_size = atoi(argv[1]);
		int i, j, x, y;
		step = 0;
		x = atoi(argv[2]);
		row = x;
		y = atoi(argv[3]);
		col = y;
		vector<vector<int>> board(board_size);
		for (i = 0; i < board_size; ++i)
			board[i].resize(board_size);
		for (i = 0; i < board_size; i++)
		{
			for (j = 0; j < board_size; j++)
				board[i][j] = 0;
		}
		board[x][y] = 0;
		tromino(board_size, x, y, 0, 0, board_size, board);
		copy_board = board;
		int width, height;
		width = 600;
		height = 600;
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
		glutInitWindowSize(width, height);
		glutInitWindowPosition(0, 0);
		glutCreateWindow("Tromino Puzzle");
		init2D(1.0f, 1.0f, 1.0f, width, height);
		glutDisplayFunc(board_display);
		glutTimerFunc(0, idle, 0);
		glutMainLoop();
	}
	return 0;
}