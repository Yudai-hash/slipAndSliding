//#pragma comment(lib,"openg132.lib")
//#pragma comment(lib,"glfw3.lib")
//#pragma comment(lib,"libglew32d.lib")
//#pragma comment(linker,"/subsystem:s\"windows\" /entry:\"mainCRTStartup\"")
//
//#define GLEW_STATIC
//#include <GL/glew.h>

#include <cstdlib> //atexit()�֐����`���Ă���w�b�_�t�@�C��cstdlib��#include����
#include <iostream>
#include <vector> //C++�̕W���e���v���[�g���C�u�����Ɋ܂܂��vector���g�p����
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//�V�F�[�_�I�u�W�F�N�g�̃R���p�C�����ʂ�\������
//shader: �V�F�[�_�I�u�W�F�N�g��
//str: �R���p�C���G���[�����������ꏊ������������
GLboolean printShaderInfoLog(GLuint shader, const char* str)
{
	//�R���p�C�����ʂ��擾����
	GLint status;
	//�V�F�[�_�I�u�W�F�N�g�̏������o���C������status�Ɋi�[����
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) std::cerr << "Compile Error in" << std::endl;

	//�V�F�[�_�̃R���p�C�����̃��O�̒������擾����
	GLsizei bufSize;
	glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&bufSize);

	if (bufSize > 1) {
		//�V�F�[�_�̃R���p�C�����̃��O�̓��e���擾����
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		//���o�����O�̍ő�̒������CbufSize��菬�����Ȃ���΂����Ȃ�
		//���o�������O�̎��ۂ̒����̊i�[��
		//infoLog[0]�Ɏ��o�������O���i�[����
		glGetShaderInfoLog(shader,bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

//�v���O�����I�u�W�F�N�g�̃����N���ʂ�\������


//�v���O�����I�u�W�F�N�g���쐬����
//vsrc:�o�[�e�b�N�X�V�F�[�_�̃\�[�X�v���O�����̕�����
//fsrc:�t���O�����g�V�F�[�_�̃\�[�X�v���O�����̕�����
GLuint createProgram(const char* vsrc, const char* fsrc) {
	//��̃v���O�����I�u�W�F�N�g���쐬����
	//glCreateProgram()�̖߂�l�́C�쐬���ꂽ�v���O�����I�u�W�F�N�g��(�ԍ�)
	const GLuint program(glCreateProgram());

	if (vsrc != NULL) {
		//�o�[�e�b�N�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���쐬����
		//�߂�l�́C�쐬���ꂽ�V�F�[�_�I�u�W�F�N�g��(�ԍ�)
		const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));

		glShaderSource(vobj, 1, &vsrc, NULL);
		glCompileShader(vobj);
	}

	//�\�[�X�v���O�����̕�����NULL�̂Ƃ��́C�V�F�[�_�I�u�W�F�N�g���쐬���Ȃ��悤�ɂ���
	if (fsrc != NULL) {
		//�t���O�����g�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���쐬����
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		//�V�F�[�_�̃\�[�X�v���O������ǂݍ���
		//1�́C����string�Ɏw�肵���z��̗v�f��
		glShaderSource(fobj, 1, &fsrc, NULL);
		//�V�F�[�_�I�u�W�F�N�g�ɓǂݍ��܂ꂽ�\�[�X�t�@�C�����R���p�C������
		glCompileShader(fobj);

		//�t���O�����g�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
		glAttachShader(program, fobj);
		//�g�����Ă��Ȃ��̂ŁC�V�F�[�_�I�u�W�F�N�g���폜(�폜�}�[�N������)
		glDeleteShader(fobj);
	}

	//�v���O�����I�u�W�F�N�g�������N����
	glBindAttribLocation(program, 0, "position");
	//�t���O�����g�V�F�[�_�̃\�[�X�v���O��������out�ϐ��ɃJ���[�o�b�t�@�����蓖�Ă�
	glBindFragDataLocation(program, 0, "fragment");
	glLinkProgram(program);

	//�쐬�����v���O�����I�u�W�F�N�g��Ԃ�
	return program;
}


int main() {
	//GLFW������������
	//
	if (glfwInit() == GL_FALSE)
	{
		//�������Ɏ��s����
		std::cerr << "Cannot initialize GLFW" << std::endl;
		return 1;
	}

	//�v���O�����C�����̏�����o�^����
	atexit(glfwTerminate);

	//OpenGL Version 3.2 Core Profile��I������(�O���t�B�b�N�n�[�h�E�F�A��glfwWindowHint()�֐��Ŏw�肵�������ɑΉ����Ă��Ȃ���΁CglfwCreateWindow()�֐��̎��s�͎��s���ăE�B���h�E�͊J����Ȃ�)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	//�E�B���h�E���쐬����
	GLFWwindow* const window(glfwCreateWindow(640, 480, "Hello", NULL, NULL));
	if (window == NULL) {
		//�E�B���h�E���쐬�ł��Ȃ�����
		std::cerr << "Cannot create GLFW window." << std::endl;
		return 1;
	}

	//�쐬�����E�B���h�E��OpenGL�̏����Ώۂɂ���
	glfwMakeContextCurrent(window);

	//GLEW������������
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		//GLEW�̏������Ɏ��s����
		std::cerr << "Cannot initialize GLEW" << std::endl;
		return 1;
	}

	//���������̃^�C�~���O��҂�
	glfwSwapInterval(1);

	//�w�i�F���w�肷��
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	//�o�[�e�b�N�X�V�F�[�_�̃\�[�X�v���O����
	static constexpr GLchar vsrc[] =
		"#version 150 core\n"
		"in vec4 position;\n"
		"void main()\n"
		"{\n"
		" gl_Position = position;\n"
		"}\n";

	//�t���O�����g�V�F�[�_�̃\�[�X�v���O����
	//��f�ɐԐF��ݒ�
	//�t���O�����g�̐F�̏o�͐��out�ϐ���fragment�Ƃ����ϐ����Ő錾
	static constexpr GLchar fsrc[] =
	   	"#version 150 core\n"
		"out vec4 fragment;\n"
		"void main()\n"
		"{\n"
		" fragment = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";

	//�v���O�����I�u�W�F�N�g���쐬����
	const GLuint program(createProgram(vsrc,fsrc));

	//�E�B���h�E���J���Ă���ԌJ��Ԃ�
	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		//�E�B���h�E����������
		glClear(GL_COLOR_BUFFER_BIT);

		//�V�F�[�_�I�u�W�F�N�g�̎g�p�J�n(�}�`�̕`�ʂɎg�p����v���O�����I�u�W�F�N�g���w�肷��)
		glUseProgram(program);


		//�����ŕ`�ʏ������s��

		//�J���[�o�b�t�@�����ւ���
		glfwSwapBuffers(window);

		//�C�x���g�����o��
		glfwWaitEvents();
	}
}

