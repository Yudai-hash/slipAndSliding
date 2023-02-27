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

	//�E�B���h�E�̃T�C�Y
	//�`�悷��E�B���h�E�̕��ƍ�����ێ������̗v�f�̔z��
	GLfloat size[2];

	//���[���h���W�n�ɑ΂���f�o�C�X���W�n�̊g�嗦
	GLfloat scale;

	//�c����aspect�̑����scale���g�p����
	////�c����
	//GLfloat aspect;

	//�}�`�̐��K���f�o�C�X���W�n��ł̈ʒu
	GLfloat location[2];

	//�L�[�{�[�h�̏��
	int keyStatus;

public:
	//�R���X�g���N�^
	//glfwCreateWindow()�֐��ɂ��E�B���h�E���쐬��������́C�r���[�|�[�g�̓E�B���h�E�S�̂ɐݒ肳����
	//scale()(�������������I�u�W�F�N�g�ƌ������Ȃ��w�i�����̔{����ς��邱�Ƃ��ł���)
	Window(int width = 640, int height = 480, const char* title = "Which one is more human-like")
		:window(glfwCreateWindow(width, height, title, NULL, NULL))
		, scale(200.0f),location{0.0f, 0.0f},keyStatus(GLFW_RELEASE)
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
		//���������Ƃ́C�Q�[������fps�̏���l���C���j�^�̍ō����\�܂łɐݒ肷��
		//GPU�̐��\�����j�^�̃��t���b�V�����[�g��傫�����������ɔ�������e�B�A�����O
		//��h���@�\�D����́C�`��r���̉摜���f�B�X�v���C�ɕ\�����Ă��܂����Ƃ�����
		glfwSwapInterval(1);

		//���̃C���X�^���Xwindow�ɑ΂���pointer�Ɏw�肵�����[�U��`�̔C�ӂ�this�̃|�C���^���L�^���Ă���
		glfwSetWindowUserPointer(window, this);

		//�E�B���h�E�̃T�C�Y�ύX���ɌĂяo������(�R�[���o�b�N�֐�)�̓o�^
		//�߂�l�Ƃ��āCresize���ݒ肳��Ă���
		glfwSetWindowSizeCallback(window, resize);

		//�}�E�X�z�C�[�����쎞�ɌĂяo�������̓o�^
		glfwSetScrollCallback(window, wheel);

		//�L�[�{�[�h���쎞�ɌĂяo�������̓o�^
		glfwSetKeyCallback(window, keyboard);

		//���̃C���X�^���X��this�|�C���^���L�^���Ă���
		glfwSetWindowUserPointer(window, this);

		//�J�����E�B���h�E�̏����ݒ�
		resize(window, width, height);
		
	}
	//�f�X�g���N�^=�N���X�Ɠ������O�����C�����o�֐��ŁC�ړ�����~(�`���_)������D
	virtual ~Window()
	{
		//�E�B���h�E�ɐݒ肳���Ă���R�[���o�b�N�֐��͌Ăяo����Ȃ��Ȃ�
		glfwDestroyWindow(window);
	}
	//�`�惋�[�v�̌p������
	//�N���X�̃R���X�g���N�^�̐錾�ŁCexplicit������ƁC�y1�z�R�s�[�̏��������ł��Ȃ��C�y2�z�Öق̌^�ϊ����ł��Ȃ�
	//explicit: �m��Ȃ������Ɍ^���ς���Ă���Ƃ����S�z������K�v���Ȃ�
	//�߂�l�̌^���́Cbool�Ŋm�肷��̂ŁC�߂�l�̌^����operator���Z�q�̑O�ɂ���K�v�͂Ȃ�
	explicit operator bool()
	{
		//�C�x���g�����o��(�C�x���g����������܂Ńv���O�������~������)
		//glfwWaitEvents();

		//�C�x���g�����o��
		//glfwWaitEvents()�́C�C�x���g�̔�����҂�
		//glfwPollEvents()�́C�C�x���g�̔�����҂��Ȃ�(�����L�[���������u�Ԃ���X���[�Y�ɓ����o��)
		//if (keyStatus == GLFW_RELEASE)
		//	glfwWaitEvents();
		//else
		//	glfwPollEvents();

		glfwPollEvents();

		//�L�[�{�[�h�̏�Ԃ𒲂ׂ�
		//1�t���[��������1��f������
		if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE)
			location[0] -= 2.0f / size[0];
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE)
			location[0] += 2.0f / size[0];
		if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE)
			location[1] -= 2.0f / size[1];
		else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE)
			location[1] += 2.0f / size[1];



		//�}�E�X�̍��{�^���̏�Ԃ𒲂ׂ�
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE)
		{
			//�}�E�X�̍��{�^���������ꂽ��C�}�E�X�J�[�\���̈ʒu���擾����
			double x, y;
			glfwGetCursorPos(window, &x, &y);

			//�}�E�X�J�[�\���̐��K���f�o�C�X���W�n��ł̈ʒu�����߂�
			location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
			location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];
		}

		//�E�B���h�E�����K�v���������True��Ԃ��ŁC�E�B���h�E�����K�v������ꍇ��False��Ԃ�
		return !glfwWindowShouldClose(window) && !glfwGetKey(window,GLFW_KEY_ESCAPE);
	}

	//�_�u���o�b�t�@�����O
	// void swapBuffers() const{}�̂悤��const�����邱�Ƃɂ���āC�I�u�W�F�N�g�̃����o�ϐ���ύX���邱�Ƃ��ł��Ȃ��D
	//mutable member-variable-declaration;�̂悤�ɁCmutable�L�[���[�h�����邱�Ƃɂ���āC
	//�N���X�̔�ÓI�C��萔(static�Cconst�łȂ�)�̃����o�ϐ��̂ݎw��ł���D
//mutable�����邱�Ƃɂ���āC�����o�ϐ��ɑ΂��ăf�[�^�ύX���s�����Ƃ��ł���D
	void swapBuffers() const
	{
		//�J���[�o�b�t�@�����ւ���
		glfwSwapBuffers(window);
	}

	//�R�[���o�b�N�֐��Ƃ��ă����o�֐����g���ꍇ�ɂ́C�ÓI�����o�֐�(static)�ł���K�v������
	//�E�B���h�E�̃T�C�Y�ύX���̏���
	static void resize(GLFWwindow *const window, int width, int height)
	{
		//�t���[���o�b�t�@�̃T�C�Y�𒲂ׁC���ׂ��t���[���o�b�t�@�̃T�C�Y�́C���ꂼ�ꕝ�ƍ������|�C���^
		//�Ɋi�[����
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		//�t���[���o�b�t�@�S�̂��r���[�|�[�g�ɐݒ肷��
		glViewport(0, 0, fbWidth, fbHeight);

		//���̃C���X�^���X��this�|�C���^�𓾂�
		//glfwGetWindowUserPointer(window)�́C�L�^���ꂽ�|�C���^�����o���Ώۂ̃E�B���h�E�̃n���h��
		Window *const
			instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

		if (instance != NULL) 
		{
			//�J�����E�B���h�E�̃T�C�Y��ۑ�����
			instance->size[0] = static_cast<GLfloat>(width);
			instance->size[1] = static_cast<GLfloat>(height);
			//���̃C���X�^���X���ێ�����c������X�V����
			//instance->aspect =
			//	static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
		}
	}

	//�}�E�X�z�C�[�����쎞�̏���
	static void wheel(GLFWwindow* window, double x, double y)
	{
		Window* const
			instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
		if(instance != NULL)
		{
			//���[���h���W�n�ɑ΂���f�o�C�X���W�n�̊g�嗦���X�V����
			instance->scale += static_cast<GLfloat>(y);
		}
	}

	//�L�[�{�[�h���쎞�̏���
	//�R�[���o�b�N�֐��Ƃ��Ďg���̂ŁC�ÓI�����o�֐��ɂ���
	static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		//���̃C���X�^���X��this�|�C���^�𓾂�
		Window *const
			instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

			if (instance != NULL)
			{
				//�L�[�̏�Ԃ�ۑ�����
				instance->keyStatus = action;
			}
	}

	//�E�B���h�E�̃T�C�Y�����o��
	//size�̃|�C���^�����o�������o�֐�getSize()
	//�ϐ��̒l���N���X�O����Q�Ƃł��郁���o�ϐ�
	const GLfloat* getSize() const { return size; }

	//���[���h���W�n�ɑ΂���f�o�C�X���W�n�̊g�嗦�����o��
	//scale�̃|�C���^�����o�������o�֐�getScale
	GLfloat getScale() const { return scale; }
	
	//�c��������o��
	//GLfloat getAspect() const { return aspect; }

	//�ʒu�����o��
	const GLfloat* getLocation() const { return location; }
};