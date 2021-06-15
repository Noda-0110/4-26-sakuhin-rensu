//#CRT_SECURE_NO_WARNINGS
//�w�b�_�t�@�C���ǂݍ���
#include "game.h"		//�Q�[�����Ǘ�����ׂ̃w�b�_�t�@�C���@�����܂ł��Q�[���̊Ǘ�
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���


//�\���̂̒�`

//�摜�̍\����
struct IMAGE
{
	int handle = -1;	//�摜�̃n���h���i�Ǘ��ԍ��j
	char path[255];		//�摜�̏ꏊ�i�p�X�j

	int x;		//X�ʒu
	int y;		//Y�ʒu
	int width;	//��
	int height;	//����

	BOOL IsDraw = FALSE;//�摜���`��ł��邩
};

//�L�����N�^�̍\����
struct CHARACTOR
{
	IMAGE img;	//�摜�\����
	int speed = 1;

	RECT coll;	//�㉺���E�����蔻��̗̈� RECT�͎l�p�`�̈ʒu��������
};

//����̍\����
struct MOVIE
{
	int handle = -1;	//����̃n���h��
	char path[255];		//����̃p�X

	int x;		//���ʒu
	int y;		//���ʒu
	int width;	//��
	int height;	//����
	int Volume = 255;	//�{�����[���i�ŏ��j0�`255�i�ő�j
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;	//���y�̃n���h��
	char path[255];		//���y�̃p�X

	int volume = -1;	//�{�����[���iMIN 0�@�`�@�Q�T�T�@MAX�j
	int playType = -1;
};


//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;	//�O����Q�[���̃V�[��
GAME_SCENE NextGameScene;	//���̃Q�[���̃V�[��
//�v���C�w�i�̓���
MOVIE playMovie;
//�v���C���[
CHARACTOR player;
//�S�[��
CHARACTOR Goal;
//�G�l�~�[
CHARACTOR enemy;

//�摜��ǂݍ���
IMAGE TitleLogo;
IMAGE TitleEnter;
IMAGE EndLogo;

//���y
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EndBGM;

AUDIO MoobSE;

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

//PushEnter�̓_��
int PushEnterCnt = 0;
const int PushEnterCntMAX = 60;
BOOL PushEnterBrink = FALSE;

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

VOID collUpdateGoal(CHARACTOR* chara);	//�����蔻��̗̈���X�V

VOID collUpdateenemy(CHARACTOR* chara);	//�����蔻��̗̈���X�V


BOOL colltouch(RECT player, RECT goal);//�����蔻��̐G��Ă��邩�G��Ă��Ȃ����̔���

BOOL GameLoad(VOID);	//�Q�[���f�[�^�̓ǂݍ���
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	
BOOL LoadImageMem(IMAGE* image, const char* path);		//���y�̓ǂݍ���
VOID GameInit(VOID);	//�Q�[���̏�����

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

	//�Q�[���̓ǂݍ���
	if (!GameLoad())
	{
		//�f�[�^�̓ǂݍ���
		DxLib_End();	//DXLib�I��
		return -1;		//�ُ�I��
	}

	//�Q�[���̏�����
	GameInit();

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
	DeleteGraph(playMovie.handle);	//�������ォ�瓮����폜
	DeleteGraph(player.img.handle);	//�������ォ��摜���폜
	DeleteGraph(enemy.img.handle);	//�������ォ��摜���폜

	DeleteGraph(Goal.img.handle);	//�������ォ��摜���폜

	DeleteSoundMem(TitleBGM.handle);	//�������ォ�特�y���폜
	DeleteSoundMem(PlayBGM.handle);	//�������ォ�特�y���폜
	DeleteSoundMem(EndBGM.handle);	//�������ォ�特�y���폜
	DeleteSoundMem(MoobSE.handle);	//�������ォ�特�y���폜

	// �c�w���C�u�����g�p�̏I�������i�����j
	DxLib_End();

	return 0;				// �\�t�g�̏I���i���s�j 
}




// - - - - �f�[�^���[�h - - - - //

/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE / �ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad()
{
	//�v���C����̔w�i��ǂݍ���
	strcpyDx(playMovie.path, ".\\Movie\\PlayForest.mp4");
	playMovie.handle = LoadGraph(playMovie.path);	//�����ǂݍ���
		//���悪�ǂݍ��߂Ȃ������Ƃ��A�G���[�i�[�P�j������
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			playMovie.path,			//���b�Z�[�W�{��
			"����ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);
		return FALSE;				//�G���[�I��
	}
	//����̕��ƍ���������
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//����̃{�����[��
	playMovie.Volume = 255;


	//�摜�̕��ƍ���������
	GetGraphSize(Goal.img.handle, &Goal.img.width, &Goal.img.height);

	//�摜�̓ǂݍ���
	if (!LoadImageMem(&player.img,".\\image\\player.\png")) { return FALSE; }		//�v���C���[
	if (!LoadImageMem(&Goal.img,".\\image\\Goal.\png")) { return FALSE; }			//�S�[��
	if (!LoadImageMem(&enemy.img,".\\image\\enemy_bee.\png")) { return FALSE; }	//�G�l�~�[

	if (!LoadImageMem(&TitleLogo,".\\image\\�^�C�g�����S.\png")) { return FALSE; }
	if (!LoadImageMem(&TitleEnter,".\\image\\PushEnter.\png")) { return FALSE; }
	if (!LoadImageMem(&EndLogo,".\\image\\�N���A���S.\png")) { return FALSE; }


	//���y�̓ǂݍ���
	if (!LoadAudio(&TitleBGM, ".\\Audio\\�^�C�g��.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&PlayBGM, ".\\Audio\\�v���C.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&EndBGM, ".\\Audio\\�G���h.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&MoobSE, ".\\Audio\\����SE.mp3", 255, DX_PLAYTYPE_BACK)) { return FALSE; }


	return TRUE;				//�S�ēǂݍ��߂�
}

/// <summary>
/// �^�C�g�����S�̓ǂݍ���
/// </summary>
/// <param name="image"></param>
/// <param name="path"></param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//�摜��ǂݍ���
	strcpyDx(image->path,path);
	image->handle = LoadGraph(image->path);	//�摜��ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��A�G���[�i�[�P�j������
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			image->path,				//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);
		return FALSE;				//�G���[�I��
	}

	//�摜�̕��ƍ���������
	GetGraphSize(image->handle, &image->width, &image->height);
	return TRUE;

}
// - - - - - �f�[�^���[�h - - - - - //

VOID GameInit(VOID)
{
	//�v���C���[�̏�����
	player.img.x = 0;
	player.img.y = GAME_HEIGHT / 2 - player.img.height / 2;
	player.speed = 500;
	player.img.IsDraw = TRUE;

	//�����蔻����X�V�i�֐��j
	collUpdateplayer(&player);


	//�S�[���̏�����
	Goal.img.x = GAME_WIDTH - Goal.img.width;
	Goal.img.y = 0;
	Goal.speed = 500;
	Goal.img.IsDraw = TRUE;

	//�����蔻����X�V�i�֐��j
	collUpdateGoal(&Goal);


	//�G�l�~�[�̏�����
	enemy.img.x = GAME_WIDTH / 2 - enemy.img.width / 2;
	enemy.img.y = GAME_HEIGHT / 2 - enemy.img.height / 2;
	enemy.speed = 100;
	enemy.img.IsDraw = TRUE;
	//�����蔻��̍X�V(�֐�)
	collUpdateenemy(&enemy);


	//�^�C�g�����S�̈ʒu�����߂�
	TitleLogo.x = GAME_WIDTH / 2 - TitleLogo.width / 2;		//�������낦
	TitleLogo.y = 100;

	//PushEnter�̈ʒu�����߂�
	TitleEnter.x = GAME_WIDTH / 2 - TitleEnter.width / 2;	//�������낦	
	TitleEnter.y = 600;

	//PushEnter�̓_��
	PushEnterCnt = 0;
	PushEnterBrink = FALSE;

	//�N���A���S�̈ʒu������
	EndLogo.x = GAME_WIDTH / 2 - EndLogo.width / 2;
	EndLogo.y = GAME_HEIGHT / 2 - EndLogo.height / 2;

}

/// <summary>
/// ���y���������ւƓǂݍ���
/// </summary>
/// <param name="audio">Audio�\���̂̃A�h���X &�`�`�`</param>
/// <param name="path">Audio�̃p�X�@.\\Audio\\�`�`�`</param>
/// <param name="volume">�{�����[��</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{
	strcpyDx(audio->path,path);
	audio->handle = LoadSoundMem(audio->path);

	//���y��ǂݍ��݂��ł��Ȃ�����
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			audio->path,				//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);
		return FALSE;				//�G���[�I��
	}

	audio->volume = volume;
	audio->playType = playType;

	return TRUE;
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
		//BGM���~�߂�
		StopSoundMem(TitleBGM.handle);
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy
		//�v���C��ʂɐ؂�ւ�
		GameInit();

		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
		PlaySoundMem(TitleBGM.handle, TitleBGM.playType);
	}
	return;
}
/// <summary>
/// �^�C�g����ʁ@���� 
/// </summary>
VOID TitleDraw(VOID)
{

	//�^�C�g�����S�̕`��
	DrawGraph(TitleLogo.x, TitleLogo.y, TitleLogo.handle, TRUE);

	//MAX�l�܂ő҂�
	if (PushEnterCnt < PushEnterCntMAX) { PushEnterCnt++; }
	else
	{
		if (PushEnterBrink == TRUE)PushEnterBrink = FALSE;
		else if (PushEnterBrink == FALSE)PushEnterBrink = TRUE;

		PushEnterCnt = 0;	//�J�E���^��������
	}

	if (PushEnterBrink == TRUE)
	{
		//�������ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)PushEnterCnt / PushEnterCntMAX) * 255);

		//PushEnter�̕`��
		DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

		//�������I��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (PushEnterBrink == FALSE)
	{
		//�������ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMAX - PushEnterCnt) / PushEnterCntMAX) * 255);

		//PushEnter�̕`��
		DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

		//�������I��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

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
	if (CheckSoundMem(PlayBGM.handle) == 0)
	{
		PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
	}

	//�v���C���[�̑���
	if (KeyDown(KEY_INPUT_W) == TRUE)
	{
		player.img.y -= player.speed * fps.DeltaTime;


		//�������̌��ʉ�
		if (CheckSoundMem(MoobSE.handle) == 0)
		{
			PlaySoundMem(MoobSE.handle, MoobSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_S) == TRUE)
	{
		player.img.y += player.speed * fps.DeltaTime;

		//�������̌��ʉ�
		if (CheckSoundMem(MoobSE.handle) == 0)
		{
			PlaySoundMem(MoobSE.handle, MoobSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_A) == TRUE)
	{
		player.img.x -= player.speed * fps.DeltaTime;


		//�������̌��ʉ�
		if (CheckSoundMem(MoobSE.handle) == 0)
		{
			PlaySoundMem(MoobSE.handle, MoobSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_D) == TRUE)
	{
		player.img.x += player.speed * fps.DeltaTime;


		//�������̌��ʉ�
		if (CheckSoundMem(MoobSE.handle) == 0)
		{
			PlaySoundMem(MoobSE.handle, MoobSE.playType);
		}
	}

	enemy.img.y += enemy.speed * fps.DeltaTime;
	//�G�̓���
	if (enemy.img.y <= 0 || enemy.img.y + enemy.img.height >= GAME_HEIGHT)
	{
		enemy.speed = -enemy.speed;
	}



	//�����蔻����X�V
	collUpdateplayer(&player);
	//�����蔻����X�V
	collUpdateGoal(&Goal);
	//�����蔻����X�V
	collUpdateenemy(&enemy);

	//�v���C���[���S�[���ɓ���������
	if (colltouch(player.coll, Goal.coll) == TRUE)
	{
		//BGM���~�߂�
		StopSoundMem(PlayBGM.handle);

		ChangeScene(GAME_SCENE_END);
		return;
	}
	//�v���C���[���G�ɓ���������
	if (colltouch(player.coll, enemy.coll) == TRUE)
	{
		//BGM���~�߂�
		StopSoundMem(PlayBGM.handle);

		ChangeScene(GAME_SCENE_END);
		return;
	}
	return;
}
/// <summary>
/// �v���C��ʁ@�`��
/// </summary>
VOID PlayDraw(VOID)

{
	//�v���C�����w�i�ɕ`��
	//�������悪�Đ�����Ă��Ȃ����
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		//�Đ�����
		SeekMovieToGraph(playMovie.handle, 0);	//�V�[�N�o�[���ŏ��ɖ߂�
		PlayMovieToGraph(playMovie.handle);		//������Đ��i�`��͂��Ă��Ȃ��j
	}

	//�����`��(�摜����ʂɍ��킹�Ĉ������΂�)
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, TRUE);
	//�S�[����`��
	if (Goal.img.IsDraw == TRUE)
	{
		DrawGraph(Goal.img.x, Goal.img.y, Goal.img.handle, TRUE);


		//�f�o�b�N�̎��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//�v���C���[��`��
	if (player.img.IsDraw == TRUE)
	{
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);


		//�f�o�b�N�̎��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//�G��`��
	if (enemy.img.IsDraw == TRUE)
	{
		DrawGraph(enemy.img.x, enemy.img.y, enemy.img.handle, TRUE);


		//�f�o�b�N�̎��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(enemy.coll.left, enemy.coll.top, enemy.coll.right, enemy.coll.bottom, GetColor(255, 0, 0), FALSE);
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
		//BGM���~�߂�
		StopSoundMem(EndBGM.handle);
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy
		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);
		return;
	}
	if (CheckSoundMem(EndBGM.handle) == 0)
	{
		PlaySoundMem(EndBGM.handle, EndBGM.playType);
	}
	return;
}
/// <summary>
/// �G���h��� �`��
/// </summary>
VOID EndDraw(VOID)
{
	//EndLogo�̕`��
	DrawGraph(EndLogo.x, EndLogo.y, EndLogo.handle, TRUE);
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
	chara->coll.left = chara->img.x + 30;		//�����蔻��������
	chara->coll.top = chara->img.y + 25;		//�����蔻��������
	chara->coll.right = chara->img.x + chara->img.width - 15;	//�����蔻��������
	chara->coll.bottom = chara->img.y + chara->img.height - 20;	//�����蔻��������

	return;
}

/// <summary>
/// �����蔻��̍X�V(�S�[���p)
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID collUpdateGoal(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// �����蔻��̍X�V(�G�p)
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID collUpdateenemy(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}


BOOL colltouch(RECT p,RECT g)
{
	if (
		p.left < g.right &&	//p�̍���x < g�̉E��x���W
		p.right > g.left &&	//p�̉E��x < g�̍���x���W
		p.bottom > g.top &&	//p�̏��y < g�̉���y���W
		p.top < g.bottom	//p�̉���y < g�̏��y���W
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}