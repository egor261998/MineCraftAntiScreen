#include <Windows.h>
#include <iostream>

#include <gl\GL.h>

typedef void (APIENTRY* glReadPixels_t) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);
typedef void(APIENTRY* glPixelStorei_t) (GLenum pname, GLint param);

typedef void(APIENTRY* glColor3f_t) (GLfloat red, GLfloat green, GLfloat blue);
typedef void(APIENTRY* glDisable_t) (GLenum cap);
typedef void(APIENTRY* glEnable_t) (GLenum cap);


unsigned long WINAPI initialize(void* instance) {

	glReadPixels_t funcReadPixels = (glReadPixels_t)GetProcAddress(GetModuleHandleA("opengl32.dll"), "glReadPixels");
	glPixelStorei_t funcPixelStore = (glPixelStorei_t)GetProcAddress(GetModuleHandleA("opengl32.dll"), "glPixelStorei");
	glColor3f_t funcColor3f = (glColor3f_t)GetProcAddress(GetModuleHandleA("opengl32.dll"), "glColor3f");
	glDisable_t funcDisable = (glDisable_t)GetProcAddress(GetModuleHandleA("opengl32.dll"), "glDisable");
	glEnable_t funcEnable = (glEnable_t)GetProcAddress(GetModuleHandleA("opengl32.dll"), "glEnable");
	
	//printf("\n(%x %x %x %x %x)", funcReadPixels, funcPixelStore, funcColor3f, funcDisable, funcEnable);

	funcColor3f(1.0F, 1.0F, 1.0F);

	funcDisable(32826);
	funcDisable(2896);
	funcDisable(2929);

	funcPixelStore(3333, 1);
	funcPixelStore(3317, 1);
	//
	int width = 845;
	int height = 480;
	int size = width * height;
	DWORD* ImageBuffer = new DWORD[size];
	memset(ImageBuffer, 0, size);
	funcReadPixels(0, 0, width, height, 32993, 33639, ImageBuffer);
	
	
	
	/*for(int i = 0 ; i < size; i++)
		if (ImageBuffer[i] != 0x0)
			printf("\n(%x)",ImageBuffer[i]);*/
	delete[] ImageBuffer;

	funcEnable(2896);
	funcEnable(2929);
	funcEnable(32826);
	
	Sleep(1000);
    FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}

std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] )
{
	// store the address of sum() in testprogram.exe here.

	if (reason == DLL_PROCESS_ATTACH)
	{
		if (auto handle = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
			CloseHandle(handle);
		

	}
	else if (reason == DLL_PROCESS_DETACH)
	{


	}
	return TRUE;
}