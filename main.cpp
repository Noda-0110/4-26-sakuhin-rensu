//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"//DxLib���g�����ɕK�v


//�}�N����`
#define GAME_TITLE "�Q�[���^�C�g��"	//�Q�[���^�C�g��
#define GAME_WIDTH  1280		//�Q�[����ʂ̕�
#define GAME_HEIGHT 720			//�Q�[����ʂ̍���
#define GAME_COLOR  32			//�Q�[���̐F��

#define GAME_ICON_ID 333			//�Q�[����ICON��ID


#define GAME_WINDOW_BAR 0	//�E�C���h�E�o�[�̎��

// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O���~���O�@�œ����Ă���
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O���ȒP�Ɉ�����d�g��
int WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	//�ǉ���
	SetOutApplicationLogValidFlag(FALSE);	//Log.txt���o�͂��Ȃ�
	
	ChangeWindowMode(TRUE);					//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);			//�E�B���h�E�̃^�C�g������
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�F
	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C���̓ǂݍ���

	//�ǉ���
	SetWindowStyleMode(GAME_WINDOW_BAR);	//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);					//�f�B�X�v���C�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);					//�E�B���h�E�������ƃA�N�e�B�u�ɂ���

														
														//�c�w���C�u��������������-���A���Ă����狭���I��
	if (DxLib_Init() == -1)		
	{
	// �G���[���N�����璼���ɏI��
		return -1;			
	}

	//�l�p�̈ʒu�����߂�
	int X = GAME_WIDTH / 2;		//��ʕ��̒��S
	int Y = GAME_HEIGHT / 2;
	//�l�p�̑傫�������߂�
	int width = 32;		//��
	int height = 32;	//����


	//�~�̔��a
	int radius = 100;


	//�ړ�����X�s�[�h
	int speed = 1;
	int wspeed = speed;
	int hspeed = speed;

	//�_�u���o�b�t�@�����O��L��
	SetDrawScreen(DX_SCREEN_BACK);


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

		//��ʂ���������
		if (ClearDrawScreen() != 0) { break; }

		//�l�p��`�悷��
		//DrawBox(
		//	X, Y, X + width, Y + height,
		//	GetColor(255, 0, 0),	//�F���擾
		//	TRUE					//�擾�����F�œh��Ԃ����H
		//);

		//�~��`��
		DrawCircle(X, Y, radius,
			GetColor(0, 255, 0),
			FALSE, 5
		);

		//�l�p�𓮂���
		X += wspeed;		//�E�ɓ�����

		//�l�p���΂߉E����
		Y += hspeed;

		//�l�p����ʂ̒[�ɗ������ɁA�����𔽓]������
		if (X - radius < 0 || X+radius > GAME_WIDTH)	//X���O�ȉ��A�P�Q�W�O�ȏ�ɂȂ������̎�
		{
			//�ړ�����(����)�𔽓]
			wspeed = -wspeed;	//1��-1�@�[�ɂ��ēn�� -1��1�@�{�ɂ��ēn��

			//�ǂɓ�����Ɖ���
			if (wspeed > 0) { wspeed += 2; }
			else if (wspeed < 0) { wspeed -= 2; }
		}



		if (Y - radius < 0 || Y + radius > GAME_HEIGHT)	//X���O�ȉ��A720�ȏ�ɂȂ������̎�
		{
			//�ړ�����(����)�𔽓]
			hspeed = -hspeed;	//1��-1�@�[�ɂ��ēn�� -1��1�@�{�ɂ��ēn��

			if (hspeed > 0) { hspeed += 2; }
			else if (hspeed < 0) { hspeed -= 2; }
		}


		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��
	}

	
	

	// �c�w���C�u�����g�p�̏I�������i�����j
	DxLib_End();				

	return 0;				// �\�t�g�̏I���i���s�j 
}
