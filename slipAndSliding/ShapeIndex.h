#pragma once

//�}�`�̕`��
#include "Shape.h"

//�Z�ʑ̂����ŕ`�悷��N���X
//�C���f�b�N�X���g�����}�`�̕`��
//Shape�N���X���I�[�o���C�h���Ă���̂��ȁH����Ƃ��C�p�����Ă���̂��ȁH
class ShapeIndex

	:public Shape 
{
protected:

	//�`��Ɏg�����_�̐�
	//����́C�h���N���X����Q�Ƃ���̂ŁCprotected�ɂ���D
	const GLsizei indexcount;

public:
	//�R���X�g���N�^
	//size: ���_�̈ʒu�̎���
	//vertexcount: ���_�̐�
	//vertex: ���_�������i�[�����z��
	//indexcount: ���_�̃C���f�b�N�X�̗v�f��
	//index: ���_�̃C���f�b�N�X���i�[�����z��
	ShapeIndex(GLint size, GLsizei vertexcount, const Object::Vertex *vertex,
		GLsizei indexcount, const GLuint* index)
		:Shape(size, vertexcount, vertex, indexcount, index),
		indexcount(indexcount)
	{
	}

	//�`������s����execute()���\�b�h���I�[�o�[���C�h����
	virtual void execute() const
	{
		//�����Q�ŕ`�悷��
		//indexcount�́C�`�悷�钸�_�̐�
		//�C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g�̐擪����g�p����
		glDrawElements(GL_LINES, indexcount, GL_UNSIGNED_INT, 0);
	}
};