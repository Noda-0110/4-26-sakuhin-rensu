
//�w�b�_�t�@�C���ǂݍ���
#include "game.h"		//�Q�[�����Ǘ�����ׂ̃w�b�_�t�@�C���@�����܂ł��Q�[���̊Ǘ�
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���


//�\���̂̒�`
//�L�����N�^�̍\����
struct CHARACTOR
{
	int handle = -1;	//�摜�̃n���h���i�Ǘ��ԍ��j
	char path[255];		//�摜�̏ꏊ�i�p�X�j
	int x;		//X�ʒu
	int y;		//Y�ʒu
	int width;	//��
	int height;	//����
	int speed = 1;

	RECT coll;	//�㉺���E�����蔻��̗̈� RECT�͎l�p�`�̈ʒu��������
	BOOL IsDraw = FALSE;//�摜���`��ł��邩
};

//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;	//�O����Q�[���̃V�[��
GAME_SCENE NextGameScene;	//���̃Q�[���̃V�[��

//�v���C���[
CHARACTOR player;

//�S�[��
CHARACTOR Goal;

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;	//�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;	//�t�F�[�h�C��

int FadeTimeMill = 2000;					//�؂�ւ��~���b
int FadeTimeMax = FadeTimeMill / 1000 * 60;	//�~���b���t���[���a�ɕϊ�

//�t�F�[�h�A�E�g
int FadeOutCntInit = 0;				//�����l
int FadeOutCnt = FadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���^
int FadeOutCntMax = FadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int FadeInCntInit = FadeTimeMax;	//�����l
int FadeInCnt = FadeInCntInit;		//�t�F�[�h�C���̃J�E���^
int FadeInCntMax = FadeTimeMax;				//�t�F�[�h�C���̃J�E���^MAX


//�v���g�^�C�v�錾
VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g����ʁi�����j
VOID TitleDraw(VOID);	//�^�C�g����ʁi�`��j

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C��ʁi�����j
VOID PlayDraw(VOID);	//�v���C��ʁi�`��j

VOID End(VOID);			//�G���h���
VOID EndProc(VOID);		//�G���h��ʁi�����j
VOID EndDraw(VOID);		//�G���h��ʁi�`��j

VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ���ʁi�����j
VOID ChangeDraw(VOID);	//�؂�ւ���ʁi�`��j


VOID ChangeScene(GAME_SCENE scene);	//�V�[���؂�ւ�

VOID collUpdateplayer(CHARACTOR* chara);	//�����蔻��̗̈���X�V

VOID collUpdate(CHARACTOR* chara);	//�����蔻��̗̈���X�V

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
											//���x�ȏ�����`����s�����ɂ�FALSE

	SetAlwaysRunFlag(TRUE);					//�E�B���h�E�������ƃA�N�e�B�u�ɂ���


	if (DxLib_Init() == -1)		//�c�w���C�u���������������@-1���A���Ă����狭���I��
	{
		// �G���[���N�����璼���ɏI��
		return -1;
	}

	//�_�u���o�b�t�@�����O��L��
	SetDrawScreen(DX_SCREEN_BACK);


	//�ŏ��̃V�[���̓^�C�g�����
	GameScene == GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//�v���C���[�̉摜��ǂݍ���
	strcpyDx(player.path, ".\\image\\Player2.png");
	player.handle = LoadGraph(player.path);	//�摜��ǂݍ���
	
	//�摜���ǂݍ��߂Ȃ������Ƃ��A�G���[�i�[�P�j������
	if (player.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			player.path,			//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);
		DxLib_End();	//�����I��
		return -1;		//�G���[�I��
	}

	//�摜�̕��ƍ���������
	GetGraphSize(player.handle, &player.width, &player.height);

	//�����蔻����X�V�i�֐��j
	collUpdateplayer(&player);

	//�v���C���[�̏�����
	player.x = GAME_WIDTH / 2 - player.width / 2;
	player.y = GAME_HEIGHT / 2 - player.height / 2;
	player.speed = 500;
	player.IsDraw = TRUE;



	//�S�[���̉摜��ǂݍ���
	strcpyDx(Goal.path, ".\\image\\Goal.png");
	Goal.handle = LoadGraph(Goal.path);	//�摜��ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��A�G���[�i�[�P�j������
	if (Goal.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			Goal.path,			//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);
		DxLib_End();	//�����I��
		return -1;		//�G���[�I��
	}

	//�摜�̕��ƍ���������
	GetGraphSize(Goal.handle, &Goal.width, &Goal.height);

	//�����蔻����X�V�i�֐��j
	collUpdate(&Goal);

	//�S�[���̏�����
	Goal.x = GAME_WIDTH - Goal.width;
	Goal.y = 0;
	Goal.speed = 500;
	Goal.IsDraw = TRUE;



	//�������[�v�@�󂯎�葱����
	while (1)
	{

		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; }	//��ʂ���������

		//�L�[�{�[�h�̓��͍X�V
		AllKeyUpdate();


		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }


		//DrawCircle(X, Y, radius, GetColor(255, 255, 0), TRUE); �~����ԉ�


		//�ȑO�̃V�[��������
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();	//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();		//�v���C���
			break;
		case GAME_SCENE_END:
			End();		//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();	//�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݃V�[�����؂�ւ���ʁi�^�C�g���v���C�G���h�̉�ʁj�łȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}



		//FPS�l��`��
		FPSDraw();
		//FPS������҂�
		FPSWait();

		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��
	}

	//�I��鎞�̏���
	DeleteGraph(player.handle);	//�������ォ��摜���폜
	DeleteGraph(Goal.handle);	//�������ォ��摜���폜

	// �c�w���C�u�����g�p�̏I�������i�����j
	DxLib_End();

	return 0;				// �\�t�g�̏I���i���s�j 
}



// ------------  �V�[���֐��@�@------------ //
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//�V�[����؂�ւ�
	IsFadeIn = FALSE;	//�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;	//�t�F�[�h�A�E�g����

	return;
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//����
	TitleDraw();	//�`��

	return;
}
/// <summary>
/// �^�C�g����ʁ@����
/// </summary>
VOID TitleProc(VOID)

{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy
		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);
	}

	return;
}
/// <summary>
/// �^�C�g����ʁ@���� 
/// </summary>
VOID TitleDraw(VOID)
{
	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();
	PlayDraw();
	return;
}
/// <summary>
/// �v���C��ʁ@����
/// </summary>
VOID PlayProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy
		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}


	//�v���C���[�̑���
	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		player.y -= player.speed * fps.DeltaTime;
	}

	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		player.y += player.speed * fps.DeltaTime;
	}

	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		player.x -= player.speed * fps.DeltaTime;
	}

	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		player.x += player.speed * fps.DeltaTime;
	}

	//�����蔻����X�V
	collUpdateplayer(&player);

	return;
}
/// <summary>
/// �v���C��ʁ@�`��
/// </summary>
VOID PlayDraw(VOID)

{
	//�S�[����`��
	if (Goal.IsDraw == TRUE)
	{
		DrawGraph(Goal.x, Goal.y, Goal.handle, TRUE);


		//�f�o�b�N�̎��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//�v���C���[��`��
	if (player.IsDraw == TRUE)
	{
		DrawGraph(player.x, player.y, player.handle, TRUE);


		//�f�o�b�N�̎��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}
	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();
	EndDraw();
	return;
}
/// <summary>
/// �G���h��ʁ@����
/// </summary>
VOID EndProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy
		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);
	}

	return;
}
/// <summary>
/// �G���h��� �`��
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();
	ChangeDraw();
	return;
}
/// <summary>
/// �؂�ւ���� ����
/// </summary>
VOID ChangeProc(VOID)
{
	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (FadeInCnt > FadeInCntMax)
		{
			FadeInCnt--;	//�J�E���^�����炷�A�����ɂ��Ă���
		}
		else
		{
			//�t�F�[�h�C�������̏I���� �����J�E���^��MAX�𒴂����Ƃ�
			FadeInCnt = FadeInCntInit;	//�J�E���^������
			IsFadeIn = FALSE;			//�t�F�[�h�C�������̏I��
		}

	}
	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (FadeOutCnt < FadeOutCntMax)
		{
			FadeOutCnt++;	//�J�E���^�𑝂₷�A�F��Z���ɂ��Ă���
		}
		else
		{
			//�t�F�[�h�C�������̏I���� �����J�E���^��MAX�𒴂����Ƃ�
			FadeOutCnt = FadeOutCntInit;	//�J�E���^������
			IsFadeOut = FALSE;			//�t�F�[�h�C�������̏I��
		}

	}

	//�؂�ւ������I���H
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�A�E�g���t�F�[�h�C�������Ă��Ȃ�
		GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;	//�ȑO�̃V�[���ɍX�V
	}
	return;
}

/// <summary>
/// �؂�ւ���ʁ@�`��
/// </summary>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();		//�G���h��ʂ̕`��
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)FadeInCnt / FadeInCntMax) * 255);	//�p�[�Z���e�[�W

	}
	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)FadeOutCnt / FadeOutCntMax) * 255);	//�p�[�Z���e�[�W

	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �����蔻��̍X�V(�v���C���[��p)
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID collUpdateplayer(CHARACTOR* chara)
{
	chara->coll.left = chara->x + 35;		//�����蔻��������
	chara->coll.top = chara->y + 30;		//�����蔻��������
	chara->coll.right = chara->x + chara->width - 25;	//�����蔻��������
	chara->coll.bottom = chara->y + chara->height - 30;	//�����蔻��������

	return;
}

/// <summary>
/// �����蔻��̍X�V(�S�[���p)
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID collUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;
	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

	return;
}