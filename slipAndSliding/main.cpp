//#pragma comment(lib,"openg132.lib")
//#pragma comment(lib,"glfw3.lib")
//#pragma comment(lib,"libglew32d.lib")
//#pragma comment(linker,"/subsystem:s\"windows\" /entry:\"mainCRTStartup\"")
//
//#define GLEW_STATIC
//#include <GL/glew.h>

#include <cstdlib> //atexit()�֐����`���Ă���w�b�_�t�@�C��cstdlib��#include����
#include <iostream>
#include <fstream> //�t�@�C���̓��o�͂��s��
#include <vector> //C++�̕W���e���v���[�g���C�u�����Ɋ܂܂��vector���g�p����
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Matrix.h"
#include "Shape.h"
#include "ShapeIndex.h"
#include "SolidShapeIndex.h"
#include "SolidShape.h"

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
//program: �v���O�����I�u�W�F�N�g��
GLboolean printProgramInfoLog(GLuint program)
{
	//�����N���ʂ��擾����
	GLint status;
	//�v���O�����I�u�W�F�N�g�̏������o��
	//program�Ɏw�肵���v���O�����I�u�W�F�N�g�̃����N�������������ǂ����𒲂ׂāCstatus�Ɋi�[����
	glGetProgramiv(program, GL_LINK_STATUS, & status);
	if (status == GL_FALSE) std::cerr << "Link Error." << std::endl;

	//�V�F�[�_�̃����N���̃��O�̒������擾����
	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

	if(bufSize > 1)
	{
		//�V�F�[�_�̃����N���̃��O�̓��e���擾����
		std::vector<GLchar> infoLog(bufSize);
		GLsizei lenghth;
		//�V�F�[�_�I�u�W�F�N�g�̃R���p�C�����̃��O�����o��
		glGetProgramInfoLog(program, bufSize, &lenghth, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}

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

		//�o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
		//�V�F�[�_�̃R���p�C������̃V�F�[�_�I�u�W�F�N�g�ɑ΂��ăG���[���`�F�b�N����
		if (printShaderInfoLog(vobj, "vertex shader"))
			glAttachShader(program, vobj);
		glDeleteShader(vobj);
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
		if(printShaderInfoLog(fobj,"fragment shader"))
			glAttachShader(program, fobj);
		//�g�����Ă��Ȃ��̂ŁC�V�F�[�_�I�u�W�F�N�g���폜(�폜�}�[�N������)
		glDeleteShader(fobj);
	}

	//�v���O�����I�u�W�F�N�g�������N����
	glBindAttribLocation(program, 0, "position");
	//�o�[�e�b�N�X�V�F�[�_��attribute�ϐ�color��ǉ�����Dcolor��attribute�ϐ��̏ꏊ�́C1�Ƃ���D
	glBindAttribLocation(program, 1, "color");
	//�t���O�����g�V�F�[�_�̃\�[�X�v���O��������out�ϐ��ɃJ���[�o�b�t�@�����蓖�Ă�
	glBindFragDataLocation(program, 0, "fragment");
	glLinkProgram(program);

	//�쐬�����v���O�����I�u�W�F�N�g��Ԃ�
	if(printProgramInfoLog(program))
		return program;

	//�v���O�����I�u�W�F�N�g���쐬�ł��Ȃ����0��Ԃ�
	glDeleteProgram(program);
	return 0;
}

//�V�F�[�_�̃\�[�X�t�@�C����ǂݍ��񂾃�������Ԃ�
//name: �V�F�[�_�̃\�[�X�t�@�C����
//buffer: �ǂݍ��񂾃\�[�X�t�@�C���̃e�L�X�g
bool readShaderSource(const char* name, std::vector<GLchar>& buffer)
{
	//�t�@�C������NULL������
	if (name == NULL) return false;

	//�\�[�X�t�@�C�����J��
	std::ifstream file(name, std::ios::binary);
	if (file.fail())
	{
		//�J���Ȃ�����
		std::cerr << "Error: Cannot open source file: " << name << std::endl;
		return false;
	}

	//�t�@�C���̖����Ɉړ������݈ʒu(���t�@�C���T�C�Y)�𓾂�
	file.seekg(0L, std::ios::end);
	GLsizei length = static_cast<GLsizei>(file.tellg());

	//�t�@�C���T�C�Y�̃��������m��
	buffer.resize(length + 1);

	//�t�@�C����擪����ǂݍ���
	file.seekg(0L, std::ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0';

	if (file.fail())
	{
		//���܂��ǂݍ��߂Ȃ�����
		std::cerr << "Error: Could not read source file" << name << std::endl;
		file.close();
		return false;
	}

	//�ǂݍ��ݐ���
	file.close();
	return true;
}

//readShaderSource()�Ń\�[�X�t�@�C����ǂݍ��݁CcreateProgram()���g�p���āC�v���O�����I�u�W�F�N�g���쐬����֐�
//vert: �o�[�e�b�N�X�V�F�[�_�̃\�[�X�t�@�C����
//frag: �t���O�����g�V�F�[�_�̃\�[�X�t�@�C����
GLuint loadProgram(const char* vert, const char* frag)
{
	//�V�F�[�_�̃\�[�X�t�B�C����ǂݍ���
	std::vector<GLchar> vsrc;
	const bool vstat(readShaderSource(vert, vsrc));
	std::vector<GLchar> fsrc;
	const bool fstat(readShaderSource(frag, fsrc));

	//�v���O�����I�u�W�F�N�g���쐬����
	//��1 ? ��2 : ��3�́C��1���]������Ctrue�Ȃ玮2���]������Cfalse�Ȃ玮3���]�������D
	return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

//���ʑ̂̒��_�̈ʒu
//���_�̐����ʂ̐��~��̖ʂɊ܂܂��ӂ̐�����̒��_�����L����ʂ̐���8�~3��4=6
//��M���������Ƃ���12�񒸓_���w�肷�邩��H
constexpr Object::Vertex octahedronVertex[] =
{
	{0.0f, 1.0f, 0.0f},
	{-1.0f, 0.0f, 0.0f},
	{0.0f, -1.0f, 0.0f},
	{1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
	{0.0f, -1.0f, 0.0f},
	{0.0f, 0.0f, -1.0f},
	{-1.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
	{1.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, -1.0f}
};

//�Z�ʑ̂̒��_�̈ʒu
constexpr Object::Vertex cubeVertex[] =
{
	{-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f}, //(0)
	{-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.8f}, //(1)
	{-1.0f,  1.0f, 1.0f, 0.0f, 0.8f, 0.0f}, //(2)
	{-1.0f,  1.0f, -1.0f, 0.0f, 0.8f, 0.8f}, //(3)
	{ 1.0f,  1.0f, -1.0f, 0.8f, 0.0f, 0.0f}, //(4)
	{ 1.0f, -1.0f, -1.0f, 0.8f, 0.0f, 0.8f}, //(5)
	{ 1.0f, -1.0f,  1.0f, 0.8f, 0.8f, 0.0f}, //(6)
	{ 1.0f,  1.0f,  1.0f, 0.8f, 0.8f, 0.8f} //(7)
};

//�Z�ʑ̗̂Ő��̗��[�_�̃C���f�b�N�X
constexpr GLuint wireCubeIndex[] =
{
	1, 0, //(a)
	2, 7, //(b)
	3, 0, //(c)
	4, 7, //(d)
	5, 0, //(e)
	6, 7, //(f)
	1, 2, //(g)
	2, 3, //(h)
	3, 4, //(i)
	4, 5, //(j)
	5, 6, //(k)
	6, 1, //(l)
};

//�Z�ʑ̖̂ʂ�h��Ԃ��O�p�`�̒��_�̃C���f�b�N�X
//constexpr GLuint solidCubeIndex[] =
//{
//	0, 1, 2, 0, 2, 3, //��
//	0, 3, 4, 0, 4, 5, //��
//	0, 5, 6, 0, 6, 1, //��
//	7, 6, 5, 7, 5, 4, //�E
//	7, 4, 3, 7, 3, 2, //��
//	7, 2, 1, 7, 1, 6 //�O
//};

//�ʂ��ƂɐF��ς����Z�ʑ̂̒��_����
constexpr Object::Vertex solidCubeVertex[] =
{
	//��
	{-1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.1f},
	{-1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.1f},
	{-1.0f, 1.0f, 1.0f, 0.1f, 0.8f, 0.1f},
	{-1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.1f},
	{-1.0f, 1.0f, 1.0f, 0.1f, 0.8f, 0.1f},
	{-1.0f, 1.0f, -1.0f, 0.1f, 0.8f, 0.1f},

	//��
	{1.0f, -1.0f, -1.0f, 0.8f, 0.1f, 0.8f},
	{-1.0f, -1.0f, -1.0f, 0.8f, 0.1f, 0.8f},
	{-1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.8f},
	{1.0f, -1.0f, -1.0f, 0.8f, 0.1f, 0.8f},
	{-1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.8f},
	{1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.8f},

	//��
	{-1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.8f},
	{1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.8f},
	{1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.8f},
	{-1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.8f},
	{1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.8f},
	{-1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.8f},

	//�E
	{1.0f, -1.0f, 1.0f, 0.1f, 0.1f, 0.8f},
	{1.0f, -1.0f, -1.0f, 0.1f, 0.1f, 0.8f},
	{1.0f, 1.0f, -1.0f, 0.1f, 0.1f, 0.8f},
	{1.0f, -1.0f, 1.0f, 0.1f, 0.1f, 0.8f},
	{1.0f, 1.0f, -1.0f, 0.1f, 0.1f, 0.8f},
	{1.0f, 1.0f, 1.0f, 0.1f, 0.1f, 0.8f},
	
	//��
	{-1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.1f},
	{-1.0f, 1.0f, 1.0f, 0.8f, 0.1f, 0.1f},
	{1.0f, 1.0f, 1.0f, 0.8f, 0.1f, 0.1f},
	{-1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.1f},
	{1.0f, 1.0f, 1.0f, 0.8f, 0.1f, 0.1f},
	{1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.1f},
	
	//�O
	{-1.0f, -1.0f, 1.0f, 0.8f, 0.8f, 0.1f},
	{1.0f, -1.0f, 1.0f, 0.8f, 0.8f, 0.1f},
	{1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.1f},
	{-1.0f, -1.0f, 1.0f, 0.8f, 0.8f, 0.1f},
	{1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.1f},
	{-1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.1f},
};

//�Z�ʑ̖̂ʂ�h��Ԃ��O�p�`�̒��_�̃C���f�b�N�X
//�ʂ��ƂɓƗ�����(�������_�ԍ��������Ȃ�)�悤�ɕύX���܂�
constexpr GLuint solidCubeIndex[] =
{
	0, 1, 2, 3, 4, 5, //��
	6, 7, 8, 9, 10, 11, //��
	12, 13, 14, 15, 16, 17, //��
	18, 19, 20, 21, 22, 23, //�E
	24, 25, 26, 27, 28, 29, //��
	30, 31, 32, 33, 34, 35, //�O
};

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
	Window window;


	//GLFWwindow* const window(glfwCreateWindow(640, 480, "Hello", NULL, NULL));
	//if (window == NULL) {
	//	//�E�B���h�E���쐬�ł��Ȃ�����
	//	std::cerr << "Cannot create GLFW window." << std::endl;
	//	return 1;
	//}
	
	////�쐬�����E�B���h�E��OpenGL�̏����Ώۂɂ���
	//glfwMakeContextCurrent(window);

	////GLEW������������
	//glewExperimental = GL_TRUE;
	//if (glewInit() != GLEW_OK) {
	//	//GLEW�̏������Ɏ��s����
	//	std::cerr << "Cannot initialize GLEW" << std::endl;
	//	return 1;
	//}

	////���������̃^�C�~���O��҂�
	//glfwSwapInterval(1);

	//�w�i�F���w�肷��
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	//�w�ʃJ�����O��L���ɂ���
	glFrontFace(GL_CCW); //���_�������̖ʂ�\
	glCullFace(GL_BACK); //���ʂ��폜����
	glEnable(GL_CULL_FACE); //�w�ʃJ�����O��L���ɂ���

	//�f�v�X�o�b�t�@��L���ɂ���
	//�f�v�X�o�b�t�@�ɐݒ肷��[�x
	glClearDepth(1.0);
	//���s���̔�r�֐�(�|���S���̐[�x���f�v�X�o�b�t�@��菬������Ε\������)
	glDepthFunc(GL_LESS);
	//�f�v�X�o�b�t�@�@�ɂ��A�ʏ���������L��
	glEnable(GL_DEPTH_TEST);
	
	////�f�o�C�X���W�`��Ƀr���[�|�[�g��ݒ肷��
	//glViewport(100, 50, 300, 300);

	//�V�F�[�_�̃\�[�X�t�@�C���́CC++�̃\�[�X�t�@�C���Ƃ͕ʂɍ쐬����
	//�o�[�e�b�N�X�V�F�[�_�̃\�[�X�v���O����
	//static constexpr GLchar vsrc[] =
	//	"#version 150 core\n"
	//	"in vec4 position;\n"
	//	"void main()\n"
	//	"{\n"
	//	" gl_Position = position;\n"
	//	"}\n";

	//�t���O�����g�V�F�[�_�̃\�[�X�v���O����
	//��f�ɐԐF��ݒ�
	//�t���O�����g�̐F�̏o�͐��out�ϐ���fragment�Ƃ����ϐ����Ő錾
	//static constexpr GLchar fsrc[] =
	//   	"#version 150 core\n"
	//	"out vec4 fragment;\n"
	//	"void main()\n"
	//	"{\n"
	//	" fragment = vec4(1.0, 0.0, 0.0, 1.0);\n"
	//	"}\n";

	//�v���O�����I�u�W�F�N�g���쐬����
	const GLuint program(loadProgram("point.vert", "point.frag"));

	//uniform�ϐ��̏ꏊ���擾����
	//aspect�́C�I�u�W�F�N�g��
	//const GLint aspectLoc(glGetUniformLocation(program, "aspect"));

	//uniform�ϐ��̏ꏊ���擾����
	//const GLint sizeLoc(glGetUniformLocation(program, "size"));
	//const GLint scaleLoc(glGetUniformLocation(program, "scale"));
	//const GLint locationLoc(glGetUniformLocation(program, "location"));

	//uniform�ϐ��̏ꏊ���擾����
	const GLint modelviewLoc(glGetUniformLocation(program, "modelview"));
	const GLint projectionLoc(glGetUniformLocation(program, "projection")); //���e�ϊ��s����o�[�e�b�N�X�V�F�[�_�ɓn������

	//�}�`�f�[�^���쐬����
	//�P���_������̗v�f���R�C���_���͂P�Q
	std::unique_ptr<const Shape> shape(new SolidShapeIndex(3, 36, solidCubeVertex,36,solidCubeIndex));
	
	//�^�C�}��0�ɃZ�b�g
	glfwSetTime(0.0);

	//�E�B���h�E���J���Ă���ԌJ��Ԃ�
	//glfwWindowShouldClose(window) == GL_FALSE
	//��
	//window
	while (window)
	{
		//�E�B���h�E����������
		//GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT�Ƃ��邱�ƂŁC�J���[�o�b�t�@�ƃf�v�X�o�b�t�@�𓯎��ɓh��Ԃ����Ƃ��ł���
		//�f�v�X�o�b�t�@�ɐݒ肷��[�x���w��
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//�V�F�[�_�I�u�W�F�N�g�̎g�p�J�n(�}�`�̕`�ʂɎg�p����v���O�����I�u�W�F�N�g���w�肷��)
		glUseProgram(program);

		//uniform�ϐ��ɒl��ݒ肷��
		//glUniform2fv(sizeLoc, 1, window.getSize());
		//glUniform1f(scaleLoc, window.getScale());
		//glUniform2fv(locationLoc, 1,window.getLocation());
		
		//uniform�ϐ��ɒl(float�^�̒P��̕ϐ�)��ݒ肷��
		//glUniform1f(aspectLoc, window.getAspect());

		//�������e�ϊ��s������߂�
		const GLfloat* const size(window.getSize());
		const GLfloat fovy(window.getScale() * 0.01f);
		//const GLfloat scale(window.getScale() * 2.0f);
		const GLfloat aspect(size[0] / size[1]);
		//const GLfloat w(size[0] / scale), h(size[1] / scale);
		const Matrix projection(Matrix::perspective(fovy, aspect, 1.0f, 10.0f));
		//const Matrix projection(Matrix::frustum(-w, w, -h, h, 1.0f, 10.0f));
		//const Matrix scaling(Matrix::scale(scale / size[0], scale / size[1], 1.0f));

		//���s�ړ��̕ϊ��s������߂�
		//const GLfloat* const position(window.getLocation());
		//const Matrix traslation(Matrix::translate(position[0], position[1], 0.0f));

		//���f���ϊ��s������߂�
		//const Matrix model(traslation * scaling);
		const GLfloat* const location(window.getLocation());
		const Matrix r(Matrix::rotate(static_cast<GLfloat>(glfwGetTime()),
			0.0f, 1.0f, 0.0f));
		const Matrix model(Matrix::translate(location[0], location[1], 0.0f) * r);

		//�r���[�ϊ��s������߂�
		//0.0f, 0.0f, 0.0f���C�ڕW�_(���_)
		const Matrix view(Matrix::lookat(3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f));
		
		//���f���r���[�ϊ��s������߂�
		const Matrix modelview(view * model);

		//uniform�ϐ��ɒl��ݒ肷��
		//model�̃����o�z��matrix�̃|�C���^��data()���\�b�h�Ŏ��o��
		//�z��ϐ���]�u���Ċi�[����
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
		glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, modelview.data());

		//�}�`��`�悷��
		shape->draw();

		//���W�ϊ�����ڂ̐}�`�̃��f���r���[�ϊ��̑O�ɓK������ƁC��ڂ̐}�`����ڂ̐}�`�ƈꏏ�ɓ����悤�ɂȂ�D
		//��ڂ̃��f���r���[�ϊ��s������߂�
		const Matrix modelview1(modelview * Matrix::translate(0.0f, 0.0f, 3.0f));

		//uniform�ϐ��ɒl��ݒ肷��
		glUniformMatrix4fv(modelviewLoc,1,GL_FALSE,modelview1.data());

		//��ڂ̐}�`��`�悷��
		shape->draw();

		//�J���[�o�b�t�@�����ւ���
		//glfwSwapBuffers(window);
		//��
		//window.swapBuffers();
		window.swapBuffers();

		//�C�x���g�����o��
		//glfwWaitEvents();
	}
}

