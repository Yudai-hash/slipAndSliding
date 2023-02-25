//#���v���v���Z�b�T���R���p�C�������@���߂���O�Ɏ��s�����
#pragma once
#include <memory> // ���ϐ��ȂǂŐݒ肳�ꂽ�p�X����T��

//�}�`�f�[�^
#include "Object.h" //�܂��́C�J�����g�f�B���N�g������T��

//�}�`�̕`��
class Shape
{
	//�}�`�f�[�^
	std::shared_ptr<const Object> object;

//vertexcount�����o�́C�h���N���X������Q�Ƃ���̂ŁCprotected�Ƃ���
protected:
	//�`�ʂɎg�����_�̐�(�ύX���邱�Ƃ��Ȃ��̂ŁCconst)
	const GLsizei vertexcount;

public:
	//�R���X�g���N�^
	//size: ���_�̈ʒu�̎���
	//vertexcount: ���_�̐�
	//vertex: ���_�������i�[�����z��
	Shape(GLint size, GLsizei vertexcount, const Object::Vertex * vertex):
		object(new Object(size,vertexcount,vertex)) //�����Ŏ󂯎�������_�������g����Object�N���X�̃C���X�^���X�𐶐����C����ɂ��object������������
		,vertexcount(vertexcount)
	{
	}

	//�`��
	void draw() const
	{
		//���_�z��I�u�W�F�N�g����������
		object->bind();
		
		//�`������s����
		execute();
	}

	//�`��̎��s
	//execute()�́C���z�֐��ɂ��āC���̃N���X(Shape�N���X)����h�������N���X�ŃI�[�o�[���C�h�ł���悤�ɂ���
	virtual void execute() const
	{
		//�܂���ŕ`�悷��
		//���_�o�b�t�@�I�u�W�F�N�g�̐擪�̒��_����`�悷��̂ŁC0
		glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
	}
};


