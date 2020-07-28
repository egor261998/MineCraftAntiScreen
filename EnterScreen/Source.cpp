#include <Windows.h>
#include <iostream>
#include <fstream>

#include <GL/glut.h>
HBITMAP hbit;
BITMAP bmp;
HBITMAP oldBmp;
HDC hdc = 0;
COLORREF** m_Matdw = 0;


bool bfg = false;
int g_iWidth = 5;
int g_iHeight = 3;
int g_iSize = g_iWidth * g_iHeight;
DWORD* g_Buffer;
//GLuint  textura_id[16];
bool g_bStart = false;

int g_iSm = 0;

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void display()
{
	if (!g_bStart)
		return;
	glClear(GL_COLOR_BUFFER_BIT);
	INT iX = 1;
	INT iY = 1;
	INT iXshift = iX;
	INT iYshift = iY;
	
	for (int y = 0; y < g_iHeight; y++)
	{
		for (int x = 0; x < g_iWidth; x++)
		{
			byte* dwd = (byte*)&g_Buffer[y* g_iWidth + x ];
			DWORD TEST = g_Buffer[y * (g_iWidth) + x];
			GLfloat b = (GLfloat)(dwd[0] / 255.0);
			GLfloat g = (GLfloat)(dwd[1] / 255.0);
			GLfloat r = (GLfloat)(dwd[2] / 255.0);
			GLfloat a = (GLfloat)(dwd[3] / 255.0);
			//GL_BGRA_EXT
			//printf("\n (%f %f %f) (x=%d y=%d)", GLfloat(dwd & 0xff) / 255.0, GLfloat(dwd >> 8 & 0xff) / 255.0, GLfloat(dwd >> 16 & 0xff) / 255.0, x, y);
			
			glBegin(GL_POLYGON);
			glColor4f(r, g, b, a);
			glVertex2d((x * iX), y * iY);
			glVertex2d(x * iX + iXshift, y * iY);
			glVertex2d(x * iX + iXshift, y * iY + iYshift);
			glVertex2d(x * iX, y * iY + iYshift);
			glEnd();
			//printf("\nx=%d y=%d r=%d g=%d b=%d %X", x, y, dwd & 0xff, dwd >> 8 & 0xff, dwd >> 16 & 0xff, dwd);
			
		}
	}
	glutSwapBuffers();
}

int InitialThread()
{
	std::ifstream fin;

	fin.open("c:\\all\\work\\CheatMaking\\MineCraftAntiScreen\\OUT\\TestScreen");

	int i = 0;
	if (fin.is_open())
	{
		std::string str;
		fin >> str;
		g_iWidth = atoi(str.c_str());
		fin >> str;
		g_iHeight = atoi(str.c_str());
		g_iSize = g_iWidth * g_iHeight;
		g_Buffer = new DWORD[g_iSize];

		while (!fin.eof())
		{
			fin >> str;

			g_Buffer[i] = _atoi64(str.c_str());
			i++;			
		}
	}
	
	g_bStart = true;

	fin.close();

	return 0;
}

void AppIdle()
{
	display();
	Sleep(1);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); /*¬ключаем двойную буферизацию и четырехкомпонентный цвет*/

	glutInitWindowSize(1920, 1080);
	glutCreateWindow("Neiro");

	glutReshapeFunc(reshape);

	glutDisplayFunc(display);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitialThread, NULL, NULL, NULL);
	glutIdleFunc(AppIdle);
	glutMainLoop();
	system("pause");
	return 0;
}