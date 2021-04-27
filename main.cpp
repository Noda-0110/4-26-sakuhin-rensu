//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"//DxLib���g�����ɕK�v


//�}�N����`
#define GAME_TITLE "�Q�[���^�C�g��"	//�Q�[���^�C�g��
#define GAME_WIDTH  1280		//�Q�[����ʂ̕�
#define GAME_HEIGHT 720			//�Q�[����ʂ̍���
#define GAME_COLOR  32			//�Q�[���̐F��

#define GAME_ICON_ID 333			//�Q�[����ICON��ID
// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O���~���O�@�œ����Ă���
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O���ȒP�Ɉ�����d�g��
int WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	ChangeWindowMode(TRUE);			//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);	//�E�B���h�E�̃^�C�g������
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�F
	SetWindowIconID(GAME_ICON_ID);
														
														//�c�w���C�u��������������-���A���Ă����狭���I��
	if (DxLib_Init() == -1)		
	{
	// �G���[���N�����璼���ɏI��
		return -1;			
	}

	//�������[�v�@�󂯎�葱����
	while (1)
	{
		//��������̃L�[�������ꂽ�Ƃ�
		if (CheckHitKeyAll() != 0)
		{
			break;	//�������[�v�𔲂���
		}

		//���b�Z�[�W���󂯎�葱����i�{�́j
		if (ProcessMessage() != 0) //-1�̂Ƃ��ɁA�G���[�E��E�B���h�E������ꂽ
		{
			break;	//�������[�v�𔲂���
		}
	}
	// �c�w���C�u�����g�p�̏I�������i�����j
	DxLib_End();				

	return 0;				// �\�t�g�̏I���i���s�j 
}
