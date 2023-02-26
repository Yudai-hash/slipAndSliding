#pragma once

//�C���f�b�N�X���g�����}�`�̕`��
#include "ShapeIndex.h"

//�C���f�b�N�X���g�����}�`�̕`��
class SolidShapeIndex
	:public ShapeIndex //ShapeIndex�N���X���p��
{
public:

	//�R���X�g���N�^
	//size: ���_�̈ʒu�̎���
	//vertexcount: ���_�̐�
	//vertex: ���_�������i�[�����z��
	//indexcount: ���_�̃C���f�b�N�X�̗v�f��
	//index: ���_�̃C���f�b�N�X���i�[�����z��
	SolidShapeIndex(GLint size, GLsizei vertexcount, const Object::Vertex *vertex,
		GLsizei indexcount, const GLuint* index)
		:ShapeIndex(size, vertexcount, vertex, indexcount, index)
	{
	}

	//�`��̎��s
	//ShapeIndex�N���X��execute(9���\�b�h�̃I�[�o�[���C�h�������s���D
	virtual void execute() const
	{
		//�O�p�`��`�悷��
		glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, 0);
	}
};