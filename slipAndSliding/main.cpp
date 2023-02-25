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
#include "Shape.h"

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

//��`�̒��_�̈ʒu
constexpr Object::Vertex rectangleVertex[] =
{
	{-0.5f,-0.5f},
	{1.5f,-0.5f},
	{1.5f,1.5f},
	{-0.5f,1.5f}
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

	//�f�o�C�X���W�`��Ƀr���[�|�[�g��ݒ肷��
	glViewport(100, 50, 300, 300);

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

	//�}�`�f�[�^���쐬����
	std::unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex));
	
	//�E�B���h�E���J���Ă���ԌJ��Ԃ�
	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		//�E�B���h�E����������
		glClear(GL_COLOR_BUFFER_BIT);

		//�V�F�[�_�I�u�W�F�N�g�̎g�p�J�n(�}�`�̕`�ʂɎg�p����v���O�����I�u�W�F�N�g���w�肷��)
		glUseProgram(program);


		//�}�`��`�悷��
		shape->draw();

		//�J���[�o�b�t�@�����ւ���
		glfwSwapBuffers(window);

		//�C�x���g�����o��
		glfwWaitEvents();
	}
}

