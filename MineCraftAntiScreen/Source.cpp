#include <Windows.h>
#include <iostream>
#include <fstream>
using namespace std;
#include <gl\GL.h>

#include "MinHook.h"
#define PATH__ "C\\AntiScreen.txt"

// this is the function that the program
// will jump to when sum() is called in the original program (testprogram.exe)

DWORD g_dwAddress = 0;
// template for the original function
typedef void (APIENTRY* glReadPixels_t) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);

glReadPixels_t g_glReadPixels;
int g_iWidth = 0;
int g_iHeight = 0;
int g_iSize = 0;
DWORD* g_Buffer;

void LoadMyImage()
{
    std::ifstream fin;

    fin.open("c:\\all\\work\\CheatMaking\\MineCraftAntiScreen\\OUT\\output");

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
        printf("\nLOADING width = %d height = %d", g_iWidth, g_iHeight);
 
        while (!fin.eof())
        {
            fin >> str;

            g_Buffer[i] = atol(str.c_str());
            i++;
        }
        printf("\nLOADED %d pixels", i-1);
    }

    fin.close();
}

void SaveToFile(DWORD dwSize, DWORD* buf, int width, int height)
{
    char szPath[128];
    char szBufint[128];
    std::ofstream fout;

    static int inext = 0;
    inext++;
    strcpy_s(szPath, "c:\\all\\work\\CheatMaking\\MineCraftAntiScreen\\OUT\\TestScreen");
    _itoa_s(inext, szBufint, 10);
    strcat_s(szPath, szBufint);
    fout.open(szPath);

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
}
void APIENTRY HookFunc(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels)
{
    printf("\nCHECK SCREEN width = %d height = %d format = %d type = %d\n\n", width, height, format, type);
	// manipulate the arguments
   
    
	g_glReadPixels(x, y, width, height, format, type, pixels);

    DWORD* dwBuffer = (DWORD*)pixels;
    dwBuffer[2] = 1337;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if(0xFF000000 != g_Buffer[y * g_iWidth + x])
                dwBuffer[y * width + x] = g_Buffer[y * g_iWidth + x];

        }
    }
    
    SaveToFile(width * height, dwBuffer, width, height);

    printf("\n SAVED SCREEN %d %d\n******************************************************************\
        ", g_iWidth, g_iHeight);
}

unsigned long WINAPI initialize(void* instance) {
   
    AllocConsole();

    freopen_s((_iobuf**)__acrt_iob_func(0), "conin$", "r", (_iobuf*)__acrt_iob_func(0));
    freopen_s((_iobuf**)__acrt_iob_func(1), "conout$", "w", (_iobuf*)__acrt_iob_func(1));
    freopen_s((_iobuf**)__acrt_iob_func(2), "conout$", "w", (_iobuf*)__acrt_iob_func(2));

    SetConsoleTitleA("AntiScreen");
    
    LoadMyImage();

    if (MH_Initialize() != MH_OK) {
        throw std::runtime_error("failed to initialize MH_Initialize.");
        return false;
    }


    if (MH_CreateHook(GetProcAddress(GetModuleHandleA("opengl32.dll"), "glReadPixels"), &HookFunc, reinterpret_cast<void**>(&g_glReadPixels)) != MH_OK) {
        throw std::runtime_error("failed to initialize create_move. (outdated index?)");
        return false;
    }

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
        throw std::runtime_error("failed to enable hooks.");
        return false;
    }

    printf("\nHook loaded");
     

    while (!GetAsyncKeyState(VK_END))
        Sleep((50));

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
		fclose((_iobuf*)__acrt_iob_func(0));
		fclose((_iobuf*)__acrt_iob_func(1));
		fclose((_iobuf*)__acrt_iob_func(2));

		FreeConsole();
        MH_Uninitialize();

        MH_DisableHook(MH_ALL_HOOKS);
		

	}
	return TRUE;
}