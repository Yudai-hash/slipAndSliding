//private�����o�ϐ���window�ɂ́C�J�����E�B���h�E�̃n���h��(���ʎq)��ێ�����
#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//�E�B���h�E�֘A�̏���
class Window
{
	//�E�B���h�E�̃n���h��
	GLFWwindow* const window;

public:
	//�R���X�g���N�^
	Window(int width = 640, int height = 480, const char* title = "Hello")
		:window(glfwCreateWindow(width, height, title, NULL, NULL))
	{
		if (window == NULL)
		{
			//�E�B���h�E���쐬�ł��Ȃ�����
			std::cerr << "Cannot create GLFW window" << std::endl;
			exit(1);
		}

		//���݂̃E�B���h�E�������Ώۂɂ���
		glfwMakeContextCurrent(window);

		//GLEW������������
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			//GLEW�̏������Ɏ��s����
			std::cerr << "Cannot initialize GLEW" << std::endl;
			exit(1);
		}

		//���������̃^�C�~���O��҂�
		glfwSwapInterval(1);

		//�E�B���h�E�̃T�C�Y�ύX���ɌĂяo������(�R�[���o�b�N�֐�)�̓o�^
		//�߂�l�Ƃ��āCresize���ݒ肳��Ă���
		glfwSetWindowSizeCallback(window, resize);

		//�J�����E�B���h�E�̏����ݒ�
		resize(window, width, height);
		
	}
	//�f�X�g���N�^
	virtual ~Window()
	{
		//�E�B���h�E�ɐݒ肳���Ă���R�[���o�b�N�֐��͌Ăяo����Ȃ��Ȃ�
		glfwDestroyWindow(window);
	}
	//�`�惋�[�v�̌p������
	explicit operator bool()
	{
		//�C�x���g�����o��
		glfwWaitEvents();

		//�E�B���h�E�����K�v���������true��Ԃ�
		return !glfwWindowShouldClose(window);
	}

	//�_�u���o�b�t�@�����O
	void swapBuffers() const
	{
		//�J���[�o�b�t�@�����ւ���
		glfwSwapBuffers(window);
	}
};