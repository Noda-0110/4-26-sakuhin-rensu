#pragma once


//�w�b�_�t�@�C����ǂݍ���
#include "DxLib.h"

//�L�[�{�[�h�̃w�b�_�t�@�C��

//�}�N����`
#define KEY_KIND_MAX	256	//��������L�[�̎��

//�L�[�{�[�h�\����
struct KEYBOARD
{
	char TempKeyState[KEY_KIND_MAX];	//���͏�Ԃ��Ǘ�
	int AllKeyState[KEY_KIND_MAX];		//�S�ẴL�[�̓��͎��Ԃ��Ǘ�
	int OldAllKeyState[KEY_KIND_MAX];	//�ȑO�̑S�ẴL�[�̓��͎��Ԃ��Ǘ�
};

//�O���̃O���[�o���ϐ�
extern KEYBOARD keyboard;

//�v���g�^�C�v�錾
extern VOID AllKeyUpdate(VOID);			//���ׂẴL�[�̏�Ԃ���������
extern BOOL KeyDown(int KEY_INPUT_);	//����̃L�[�������Ă��邩
extern BOOL KeyUp(int KEY_INPUT_);		//����̃L�[���グ�Ă��邩
extern BOOL KeyClick(int KEY_INPUT_);	//����̃L�[���N���b�N������
extern BOOL KeyDownKeep(int KEY_INPUT_, int millsec);	//����̃L�[���w��̃~���b�����������Ă��邩
