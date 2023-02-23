//#pragma comment(lib,"openg132.lib")
//#pragma comment(lib,"glfw3.lib")
//#pragma comment(lib,"libglew32d.lib")
//#pragma comment(linker,"/subsystem:s\"windows\" /entry:\"mainCRTStartup\"")
//
//#define GLEW_STATIC
//#include <GL/glew.h>

#include <iostream>
#include <GLFW/glfw3.h>

int main() {
	//GLFWを初期化する
	//
	if (glfwInit() == GL_FALSE)
	{
		//初期化に失敗した
		std::cerr << "Cannot initialize GLFW" << std::endl;
		return 1;
	}

	//ウィンドウを作成する
	GLFWwindow* const window(glfwCreateWindow(640, 480, "Hello", NULL, NULL));
	if (window == NULL) {
		//ウィンドウが作成できなかった
		std::cerr << "Cannot create GLFW window." << std::endl;
		return 1;
	}

	//作成したウィンドウをOpenGLの処理対象にする
	glfwMakeContextCurrent(window);

	//背景色を指定する
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}
