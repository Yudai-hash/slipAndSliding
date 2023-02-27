/*���W�ϊ��́C�o�[�e�b�N�X�V�F�[�_�ōs��*/
#version 150 core
/*layout�C���q�ŁCattribute�ϐ��̏ꏊ��t���O�����g�V�F�[�_��
�o�͕ϐ��̏ꏊ��錾����@�\���g�p���邽��*/
/*#extension GL ARB explicit attrib location: enable*/
uniform mat4 modelview;
uniform mat4 projection;

/*attribute�ϐ��̏ꏊ��t���O�����g�V�F�[�_�̏o�͕ϐ��̏ꏊ���w��*/
/*layout (location = 0) in vec4 position;*/ 
/*layout (location = 1) in vec4 color;*/

/*size,scale,location��model�ɓ��ꂷ��*/
/*model�̒l�́C��̐}�`�̕`�撆�ɕύX����邱�Ƃ͂Ȃ��̂ŁC
�V�F�[�_��uniform�ϐ���p����
�`�悷��}�`�̒��_�ʒu���o�[�e�b�N�X�V�F�[�_��in�ϐ�position��
�i�[����Ă���*/
/*uniform vec2 size;*/
/*uniform float scale;*/
/*uniform vec2 location;*/
in vec4 position;

/*���_�����ɒǉ��������_�F�̃f�[�^���󂯎��in�ϐ�color��錾����D*/
/*in�ϐ��́C�V�F�[�_�v���O�����������_�����O�p�C�v���C���̑O�̃X�e�[�W�f�[�^���󂯎��ϐ�*/
in vec4 color;

/*�����ɃO���f�[�V���������Ȃ��悤�ɁC
varying�ϐ��̐錾��flat�Ƃ����C���q������*/
flat out vec4 vertex_color; 

void main()
{
vertex_color = color; /*out�ϐ��Ɋi�[���ꂽ���e�́C
�����_�����O�p�C�v���C���̎��̃X�e�[�W�ł��郉�X�^���C�U�ɑ�����D*/
/*projectio�����f���r���[�ϊ���̒��_�̍��W�l�ɏ悶��*/
gl_Position = projection * modelview * position;
/*gl_Position = vec4(2.0 * scale / size, 1.0, 1.0) * position + vec4(location, 0.0, 0.0); position�������Ȃ������ꍇ�C�E�B���h�E��ɕ\������Ȃ�����*/
}