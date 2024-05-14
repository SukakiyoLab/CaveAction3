#include "sdl_engine.h"

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "cryptlib.lib")

#pragma comment(lib, "pugixml.lib")

#include <Windows.h>

#define EIGEN_NO_DEBUG
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DONT_PARALLELIZE
#define EIGEN_MPL2_ONLY

#ifndef _DEBUG
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
int main(int args, char* argv[]) {
#endif
	CAT_SDLEngine engine;

	engine.InitEngine();
	engine.InitObject();


	while(!(engine.Judge())){
		engine.Update();
	}

	int result = engine.Finish();

	return result;

}


