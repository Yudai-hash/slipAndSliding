#pragma once

//�}�`�̕`��
#include "Shape.h"

//�O�p�`���g�����`��
//�C���f�b�N�X���g�����}�`�̕`��
class SolidShape
	:public Shape //Shape�N���X���p��
{
public:

	//�R���X�g���N�^
	//size: ���_�̈ʒu�̎���
	//vertexcount: ���_�̐�
	//vertex: ���_�������i�[�����z��
	SolidShape(GLint size, GLsizei vertexcount, const Object::Vertex *vertex)
		:Shape(size, vertexcount, vertex)
	{
	}

	//�`��̎��s
	//ShapeIndex�N���X��execute()���\�b�h�̃I�[�o�[���C�h�������s���D
	virtual void execute() const
	{
		//�O�p�`�ŕ`�悷��
		//�O�p�`��`�悷��̂ŁCGL_TRIANGLES���g�p����
		glDrawArrays(GL_TRIANGLES, 0, vertexcount);
	}
};