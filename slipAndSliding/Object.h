#pragma once //�����w�b�_�t�@�C����������ǂݍ��܂�Ȃ��悤�ɂ��邨�܂��Ȃ�
#include <GL/glew.h>

//�N���XObject
//�}�`�f�[�^
class Object
{
	//�����o�ϐ� vao��vbo
	//���_�z��I�u�W�F�N�g��
	GLuint vao;

	//���_�o�b�t�@�I�u�W�F�N�g��
	GLuint vbo;

	//public�����o
	public:
		//���_����
		struct Vertex
		{
			//�ʒu
			GLfloat position[2];
		};
		
		//�R���X�g���N�^
		//size: ���_�̈ʒu�̎���
		//vertexcount: ���_�̐�
		//vertex: ���_�������i�[�����z��
		Object(GLint size, GLsizei vertexcount, const Vertex* vertex)
		{
			//���_�z��I�u�W�F�N�g
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			//���_�o�b�t�@�I�u�W�F�N�g
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER,
			vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

			//��������Ă��钸�_�o�b�t�@�I�u�W�F�N�g��in�ϐ�����Q�Ƃł���悤�ɂ���
			glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
		}

	//�f�X�g���N�^
		//���z�֐�
	virtual ~Object()
	{
		//���_�z��I�u�W�F�N�g���팸����
		glDeleteVertexArrays(1, &vao);

		//���_�o�b�t�@�I�u�W�F�N�g���폜����
		glDeleteBuffers(1, &vbo);
	}

	//�R�s�[�R���X�g���N�^�Ƒ�����Z�q��provate�����o�ɂ��āC�C���X�^���X�̃R�s�[���֎~���܂��D(�R�s�[���悤�Ƃ���ƁC�R���p�C���G���[�ɂȂ�)
	private:
		//�R�s�[�R���X�g���N�^�ɂ��R�s�[�֎~
		Object(const Object& o);
		
		//����ɂ��R�s�[�֎~
		Object& operator=(const Object& o);
	
    public:
		//���_�z��I�u�W�F�N�g�̌���
		//�C���X�^���X��ύX���Ȃ��̂ŁCconst���\�b�h
		void bind() const
		{
			//�`�ʂ��钸�_�z��I�u�W�F�N�g���w�肷��
		    glBindVertexArray(vao);
	     }
};




	