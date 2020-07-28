#include <Windows.h>
#include <iostream>
#include <fstream>

#include <GL/glut.h>
HBITMAP hbit;
BITMAP bmp;
HBITMAP oldBmp;
HDC hdc = 0;
COLORREF** m_Matdw = 0;

int g_iWidth = 5;
int g_iHeight = 3;
int g_iSize = g_iWidth * g_iHeight;
DWORD* g_Buffer;
//GLuint  textura_id[16];
bool g_bStart = false;

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void SaveToFile(DWORD dwSize, DWORD* buf, int width, int height)
{
	std::ofstream fout;

	fout.open("c:\\all\\work\\CheatMaking\\MineCraftAntiScreen\\OUT\\output");

	if (fout.is_open())
	{
		fout << width << "\n";
		fout << height << "\n";


		for (DWORD i = 0; i < dwSize; i++)
			fout << buf[i] << "\n";
	}
	else
	{
		printf("\nError");
	}


	fout.close();
	exit(0);
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

	for (int x = 0; x < bmp.bmWidth; x++)
	{
		for (int y = 0; y < bmp.bmHeight; y++)
		{
			auto dwd = m_Matdw[x][y];

			glColor3f((GLfloat)((dwd & 0xff) / 255.0), (GLfloat)((dwd >> 8 & 0xff) / 255.0), (GLfloat)((dwd >> 16 & 0xff) / 255.0));

			glBegin(GL_POLYGON);
			glVertex2d(x * iX, y * iY);
			glVertex2d(x * iX + iXshift, y * iY);
			glVertex2d(x * iX + iXshift, y * iY + iYshift);
			glVertex2d(x * iX, y * iY + iYshift);
			glEnd();
			//printf("\nx=%d y=%d r=%d g=%d b=%d %X", x, y, dwd & 0xff, dwd >> 8 & 0xff, dwd >> 16 & 0xff, dwd);
		}
	}

	
	int _W = bmp.bmWidth;
	int _H = bmp.bmHeight;
	DWORD* dwB = new DWORD[_W * _H];

	memset(dwB, 0, _W * _H * 4);

	glReadPixels(0, 0, _W, _H, 32993, 33639, dwB);
	SaveToFile(_W * _H, dwB, _W, _H);

	glutSwapBuffers();
}

int InitialThread()
{
	

	hbit = (HBITMAP)::LoadImage(NULL, L"c:\\all\\work\\CheatMaking\\MineCraftAntiScreen\\OUT\\input.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	GetObject(hbit, sizeof(bmp), &bmp);

	hdc = 0;

	HWND hwndScreen = 0;
	HDC hdcScreen = GetDC(hwndScreen);
	hdc = CreateCompatibleDC(hdcScreen);
	ReleaseDC(hwndScreen, hdcScreen);

	//printf("\n(%d %d)", bmp.bmWidth, bmp.bmHeight);
	COLORREF dwd = 0;
	oldBmp = (HBITMAP)::SelectObject(hdc, hbit);

	m_Matdw = new COLORREF * [bmp.bmWidth];

	for (INT j = 0; j < bmp.bmWidth; j++)
	{
		m_Matdw[j] = new COLORREF[bmp.bmHeight];

		for (INT i = 0; i < bmp.bmHeight; i++)
			m_Matdw[j][i] = 0xFFFFFF;
	}


	for (int x = 0; x < bmp.bmWidth; x++)
	{
		for (int y = 0; y < bmp.bmHeight; y++)
		{
			dwd = GetPixel(hdc, x, bmp.bmHeight - y);
			m_Matdw[x][y] = dwd;
			//printf("\nx=%d y=%d r=%d g=%d b=%d %X", x, y, dwd & 0xff, dwd >> 8 & 0xff, dwd >> 16 & 0xff, dwd);
		}
	}

	g_bStart = true;

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