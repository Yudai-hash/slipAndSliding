#version 150 core
/*layout�C���q�ŁCattribute�ϐ��̏ꏊ��t���O�����g�V�F�[�_�̏o�͕ϐ��̏ꏊ��錾����@�\���g�p���邽��*/
/*#extension GL ARB explicit attrib location: enable*/

flat in vec4 vertex_color; /*�o�[�e�b�N�X�V�F�[�_���烉�X�^���C�U�ɑ����C����ɕ�Ԃ��ꂽ���_�F���󂯎��in�ϐ�vertex_color��錾*/
/*attribute�ϐ��̏ꏊ��t���O�����g�V�F�[�_�̏o�͕ϐ��̏ꏊ���w��*/
/*layout (location = 0) out vec4 fragment;*/
out vec4 fragment;

void main()
{
fragment = vertex_color; /*out�ϐ�fragment�ɑ�����āC�t���O�����g�V�F�[�_�̎��̃X�e�[�W�ł���t���[���o�b�t�@*/ 
}