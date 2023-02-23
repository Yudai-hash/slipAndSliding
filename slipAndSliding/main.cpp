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
	//GLFW������������
	//
	if (glfwInit() == GL_FALSE)
	{
		//�������Ɏ��s����
		std::cerr << "Cannot initialize GLFW" << std::endl;
		return 1;
	}

	//�E�B���h�E���쐬����
	GLFWwindow* const window(glfwCreateWindow(640, 480, "Hello", NULL, NULL));
	if (window == NULL) {
		//�E�B���h�E���쐬�ł��Ȃ�����
		std::cerr << "Cannot create GLFW window." << std::endl;
		return 1;
	}

	//�쐬�����E�B���h�E��OpenGL�̏����Ώۂɂ���
	glfwMakeContextCurrent(window);

	//�w�i�F���w�肷��
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}
