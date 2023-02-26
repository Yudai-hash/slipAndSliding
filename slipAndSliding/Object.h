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

	//�C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g(private�����o)
	GLuint ibo;

	//public�����o
	public:
		//���_����
		struct Vertex
		{
			//�ʒu
			//���_�̈ʒu���O�����Ȃ̂ŁCObject�N���X�̒��Œ�`���Ă���Vertex�\���̂̃����oposition�̗v�f����3�ɂ���
			GLfloat position[3];

			//�F
			GLfloat color[3];
		};
		
		//�R���X�g���N�^
		//size: ���_�̈ʒu�̎���
		//vertexcount: ���_�̐�
		//vertex: ���_�������i�[�����z��
		//indexcount: ���_�̃C���f�b�N�X�̗v�f��
		//index: ���_�̃C���f�b�N�X���i�[�����z��
		Object(GLint size, GLsizei vertexcount, const Vertex *vertex,
			GLsizei indexcount = 0,const GLuint *index = NULL)
		{
			//���_�z��I�u�W�F�N�g
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			//���_�o�b�t�@�I�u�W�F�N�g
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			//indexcount = 0�Cvertex = NULL�ł��G���[�ɂ͂Ȃ�Ȃ�
			glBufferData(GL_ARRAY_BUFFER,
			vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

			//�C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g
			glGenBuffers(1, &ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				indexcount * sizeof(GLuint), index, GL_STATIC_DRAW);

			//��������Ă��钸�_�o�b�t�@�I�u�W�F�N�g��in�ϐ�����Q�Ƃł���悤�ɂ���
			//static_cast<Vertex *>(0)->position�́CVertex�^�̐擪�ɂ���̂ŁC�����I��0
			glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				static_cast<Vertex*>(0)->position);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				static_cast<Vertex*>(0)->color);
			glEnableVertexAttribArray(1);
		}

	//�f�X�g���N�^
		//���z�֐�
	virtual ~Object()
	{
		//���_�z��I�u�W�F�N�g���팸����
		glDeleteVertexArrays(1, &vao);

		//���_�o�b�t�@�I�u�W�F�N�g���폜����
		glDeleteBuffers(1, &vbo);

		//�C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g���폜����
		glDeleteBuffers(1, &ibo);
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




	