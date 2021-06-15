//#CRT_SECURE_NO_WARNINGS
//ヘッダファイル読み込み
#include "game.h"		//ゲームを管理する為のヘッダファイル　あくまでもゲームの管理
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理


//構造体の定義

//画像の構造体
struct IMAGE
{
	int handle = -1;	//画像のハンドル（管理番号）
	char path[255];		//画像の場所（パス）

	int x;		//X位置
	int y;		//Y位置
	int width;	//幅
	int height;	//高さ

	BOOL IsDraw = FALSE;//画像が描画できるか
};

//キャラクタの構造体
struct CHARACTOR
{
	IMAGE img;	//画像構造体
	int speed = 1;

	RECT coll;	//上下左右当たり判定の領域 RECTは四角形の位置を扱える
};

//動画の構造体
struct MOVIE
{
	int handle = -1;	//動画のハンドル
	char path[255];		//動画のパス

	int x;		//ｘ位置
	int y;		//ｙ位置
	int width;	//幅
	int height;	//高さ
	int Volume = 255;	//ボリューム（最小）0～255（最大）
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;	//音楽のハンドル
	char path[255];		//音楽のパス

	int volume = -1;	//ボリューム（MIN 0　～　２５５　MAX）
	int playType = -1;
};


//グローバル変数
//シーンを管理する変数
GAME_SCENE GameScene;		//現在のゲームのシーン
GAME_SCENE OldGameScene;	//前回もゲームのシーン
GAME_SCENE NextGameScene;	//次のゲームのシーン
//プレイ背景の動画
MOVIE playMovie;
//プレイヤー
CHARACTOR player;
//ゴール
CHARACTOR Goal;
//エネミー
CHARACTOR enemy;

//画像を読み込む
IMAGE TitleLogo;
IMAGE TitleEnter;
IMAGE EndLogo;

//音楽
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EndBGM;

AUDIO MoobSE;

//画面の切り替え
BOOL IsFadeOut = FALSE;	//フェードアウト
BOOL IsFadeIn = FALSE;	//フェードイン

int FadeTimeMill = 2000;					//切り替えミリ秒
int FadeTimeMax = FadeTimeMill / 1000 * 60;	//ミリ秒をフレーム病に変換

//フェードアウト
int FadeOutCntInit = 0;				//初期値
int FadeOutCnt = FadeOutCntInit;	//フェードアウトのカウンタ
int FadeOutCntMax = FadeTimeMax;	//フェードアウトのカウンタMAX

//フェードイン
int FadeInCntInit = FadeTimeMax;	//初期値
int FadeInCnt = FadeInCntInit;		//フェードインのカウンタ
int FadeInCntMax = FadeTimeMax;				//フェードインのカウンタMAX

//PushEnterの点滅
int PushEnterCnt = 0;
const int PushEnterCntMAX = 60;
BOOL PushEnterBrink = FALSE;

//プロトタイプ宣言
VOID Title(VOID);		//タイトル画面
VOID TitleProc(VOID);	//タイトル画面（処理）
VOID TitleDraw(VOID);	//タイトル画面（描画）

VOID Play(VOID);		//プレイ画面
VOID PlayProc(VOID);	//プレイ画面（処理）
VOID PlayDraw(VOID);	//プレイ画面（描画）

VOID End(VOID);			//エンド画面
VOID EndProc(VOID);		//エンド画面（処理）
VOID EndDraw(VOID);		//エンド画面（描画）

VOID Change(VOID);		//切り替え画面
VOID ChangeProc(VOID);	//切り替え画面（処理）
VOID ChangeDraw(VOID);	//切り替え画面（描画）


VOID ChangeScene(GAME_SCENE scene);	//シーン切り替え

VOID collUpdateplayer(CHARACTOR* chara);	//当たり判定の領域を更新

VOID collUpdateGoal(CHARACTOR* chara);	//当たり判定の領域を更新

VOID collUpdateenemy(CHARACTOR* chara);	//当たり判定の領域を更新


BOOL colltouch(RECT player, RECT goal);//当たり判定の触れているか触れていないかの判定

BOOL GameLoad(VOID);	//ゲームデータの読み込み
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	
BOOL LoadImageMem(IMAGE* image, const char* path);		//音楽の読み込み
VOID GameInit(VOID);	//ゲームの初期化

// プログラムは WinMain から始まります
//Windowsのプログラミング法で動いている
//DxLibは、DirectXという、ゲームプログラミングを簡単に扱える仕組み
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	//追加分
	SetOutApplicationLogValidFlag(FALSE);	//Log.txtを出力しない

	ChangeWindowMode(TRUE);					//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);			//ウィンドウのタイトル文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景色
	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルの読み込み

	//追加分
	SetWindowStyleMode(GAME_WINDOW_BAR);	//ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);					//ディスプレイの垂直同期を有効にする
											//高度な処理や描画を行う時にはFALSE

	SetAlwaysRunFlag(TRUE);					//ウィンドウをずっとアクティブにする


	if (DxLib_Init() == -1)		//ＤＸライブラリ初期化処理　-1が帰ってきたら強制終了
	{
		// エラーが起きたら直ちに終了
		return -1;
	}

	//ダブルバッファリングを有効
	SetDrawScreen(DX_SCREEN_BACK);


	//最初のシーンはタイトル画面
	GameScene == GAME_SCENE_TITLE;

	//ゲーム全体の初期化

	//ゲームの読み込み
	if (!GameLoad())
	{
		//データの読み込み
		DxLib_End();	//DXLib終了
		return -1;		//異常終了
	}

	//ゲームの初期化
	GameInit();

	//無限ループ　受け取り続ける
	while (1)
	{

		if (ProcessMessage() != 0) { break; }	//メッセージを受け取り続ける
		if (ClearDrawScreen() != 0) { break; }	//画面を消去する

		//キーボードの入力更新
		AllKeyUpdate();


		//FPS値の更新
		FPSUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }


		//DrawCircle(X, Y, radius, GetColor(255, 255, 0), TRUE); 円が一番下


		//以前のシーンを所得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();	//タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();		//プレイ画面
			break;
		case GAME_SCENE_END:
			End();		//エンド画面
			break;
		case GAME_SCENE_CHANGE:
			Change();	//切り替え画面
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//現在シーンが切り替え画面（タイトルプレイエンドの画面）でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;	//画面切り替えシーンに変える
			}
		}

		//FPS値を描画
		FPSDraw();
		//FPS処理を待つ
		FPSWait();

		ScreenFlip();	//ダブルバッファリングした画面を描画
	}

	//終わる時の処理
	DeleteGraph(playMovie.handle);	//メモリ上から動画を削除
	DeleteGraph(player.img.handle);	//メモリ上から画像を削除
	DeleteGraph(enemy.img.handle);	//メモリ上から画像を削除

	DeleteGraph(Goal.img.handle);	//メモリ上から画像を削除

	DeleteSoundMem(TitleBGM.handle);	//メモリ上から音楽を削除
	DeleteSoundMem(PlayBGM.handle);	//メモリ上から音楽を削除
	DeleteSoundMem(EndBGM.handle);	//メモリ上から音楽を削除
	DeleteSoundMem(MoobSE.handle);	//メモリ上から音楽を削除

	// ＤＸライブラリ使用の終了処理（準備）
	DxLib_End();

	return 0;				// ソフトの終了（実行） 
}




// - - - - データロード - - - - //

/// <summary>
/// ゲームのデータを読み込み
/// </summary>
/// <returns>読み込めたらTRUE / 読み込めなかったらFALSE</returns>
BOOL GameLoad()
{
	//プレイ動画の背景を読み込み
	strcpyDx(playMovie.path, ".\\Movie\\PlayForest.mp4");
	playMovie.handle = LoadGraph(playMovie.path);	//動画を読み込み
		//動画が読み込めなかったとき、エラー（ー１）が入る
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			playMovie.path,			//メッセージ本文
			"動画読み込みエラー！",	//メッセージタイトル
			MB_OK					//ボタン
		);
		return FALSE;				//エラー終了
	}
	//動画の幅と高さを所得
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//動画のボリューム
	playMovie.Volume = 255;


	//画像の幅と高さを所得
	GetGraphSize(Goal.img.handle, &Goal.img.width, &Goal.img.height);

	//画像の読み込み
	if (!LoadImageMem(&player.img,".\\image\\player.\png")) { return FALSE; }		//プレイヤー
	if (!LoadImageMem(&Goal.img,".\\image\\Goal.\png")) { return FALSE; }			//ゴール
	if (!LoadImageMem(&enemy.img,".\\image\\enemy_bee.\png")) { return FALSE; }	//エネミー

	if (!LoadImageMem(&TitleLogo,".\\image\\タイトルロゴ.\png")) { return FALSE; }
	if (!LoadImageMem(&TitleEnter,".\\image\\PushEnter.\png")) { return FALSE; }
	if (!LoadImageMem(&EndLogo,".\\image\\クリアロゴ.\png")) { return FALSE; }


	//音楽の読み込み
	if (!LoadAudio(&TitleBGM, ".\\Audio\\タイトル.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&PlayBGM, ".\\Audio\\プレイ.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&EndBGM, ".\\Audio\\エンド.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&MoobSE, ".\\Audio\\歩くSE.mp3", 255, DX_PLAYTYPE_BACK)) { return FALSE; }


	return TRUE;				//全て読み込めた
}

/// <summary>
/// タイトルロゴの読み込み
/// </summary>
/// <param name="image"></param>
/// <param name="path"></param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//画像を読み込み
	strcpyDx(image->path,path);
	image->handle = LoadGraph(image->path);	//画像を読み込み

	//画像が読み込めなかったとき、エラー（ー１）が入る
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			image->path,				//メッセージ本文
			"画像読み込みエラー！",	//メッセージタイトル
			MB_OK					//ボタン
		);
		return FALSE;				//エラー終了
	}

	//画像の幅と高さを所得
	GetGraphSize(image->handle, &image->width, &image->height);
	return TRUE;

}
// - - - - - データロード - - - - - //

VOID GameInit(VOID)
{
	//プレイヤーの初期化
	player.img.x = 0;
	player.img.y = GAME_HEIGHT / 2 - player.img.height / 2;
	player.speed = 500;
	player.img.IsDraw = TRUE;

	//当たり判定を更新（関数）
	collUpdateplayer(&player);


	//ゴールの初期化
	Goal.img.x = GAME_WIDTH - Goal.img.width;
	Goal.img.y = 0;
	Goal.speed = 500;
	Goal.img.IsDraw = TRUE;

	//当たり判定を更新（関数）
	collUpdateGoal(&Goal);


	//エネミーの初期化
	enemy.img.x = GAME_WIDTH / 2 - enemy.img.width / 2;
	enemy.img.y = GAME_HEIGHT / 2 - enemy.img.height / 2;
	enemy.speed = 100;
	enemy.img.IsDraw = TRUE;
	//当たり判定の更新(関数)
	collUpdateenemy(&enemy);


	//タイトルロゴの位置を決める
	TitleLogo.x = GAME_WIDTH / 2 - TitleLogo.width / 2;		//中央ぞろえ
	TitleLogo.y = 100;

	//PushEnterの位置を決める
	TitleEnter.x = GAME_WIDTH / 2 - TitleEnter.width / 2;	//中央ぞろえ	
	TitleEnter.y = 600;

	//PushEnterの点滅
	PushEnterCnt = 0;
	PushEnterBrink = FALSE;

	//クリアロゴの位置を決め
	EndLogo.x = GAME_WIDTH / 2 - EndLogo.width / 2;
	EndLogo.y = GAME_HEIGHT / 2 - EndLogo.height / 2;

}

/// <summary>
/// 音楽をメモリへと読み込む
/// </summary>
/// <param name="audio">Audio構造体のアドレス &～～～</param>
/// <param name="path">Audioのパス　.\\Audio\\～～～</param>
/// <param name="volume">ボリューム</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{
	strcpyDx(audio->path,path);
	audio->handle = LoadSoundMem(audio->path);

	//音楽を読み込みができなかった
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			audio->path,				//メッセージ本文
			"音楽読み込みエラー！",	//メッセージタイトル
			MB_OK					//ボタン
		);
		return FALSE;				//エラー終了
	}

	audio->volume = volume;
	audio->playType = playType;

	return TRUE;
}

// ------------  シーン関数　　------------ //
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//シーンを切り替え
	IsFadeIn = FALSE;	//フェードインしない
	IsFadeOut = TRUE;	//フェードアウトする

	return;
}

/// <summary>
/// タイトル画面
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//処理
	TitleDraw();	//描画

	return;
}
/// <summary>
/// タイトル画面　処理
/// </summary>
VOID TitleProc(VOID)

{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGMを止める
		StopSoundMem(TitleBGM.handle);
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽
		//プレイ画面に切り替え
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
/// タイトル画面　処理 
/// </summary>
VOID TitleDraw(VOID)
{

	//タイトルロゴの描画
	DrawGraph(TitleLogo.x, TitleLogo.y, TitleLogo.handle, TRUE);

	//MAX値まで待つ
	if (PushEnterCnt < PushEnterCntMAX) { PushEnterCnt++; }
	else
	{
		if (PushEnterBrink == TRUE)PushEnterBrink = FALSE;
		else if (PushEnterBrink == FALSE)PushEnterBrink = TRUE;

		PushEnterCnt = 0;	//カウンタを初期化
	}

	if (PushEnterBrink == TRUE)
	{
		//半透明にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)PushEnterCnt / PushEnterCntMAX) * 255);

		//PushEnterの描画
		DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

		//半透明終了
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (PushEnterBrink == FALSE)
	{
		//半透明にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMAX - PushEnterCnt) / PushEnterCntMAX) * 255);

		//PushEnterの描画
		DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

		//半透明終了
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();
	PlayDraw();
	return;
}
/// <summary>
/// プレイ画面　処理
/// </summary>
VOID PlayProc(VOID)
{
	if (CheckSoundMem(PlayBGM.handle) == 0)
	{
		PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
	}

	//プレイヤーの操作
	if (KeyDown(KEY_INPUT_W) == TRUE)
	{
		player.img.y -= player.speed * fps.DeltaTime;


		//動く時の効果音
		if (CheckSoundMem(MoobSE.handle) == 0)
		{
			PlaySoundMem(MoobSE.handle, MoobSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_S) == TRUE)
	{
		player.img.y += player.speed * fps.DeltaTime;

		//動く時の効果音
		if (CheckSoundMem(MoobSE.handle) == 0)
		{
			PlaySoundMem(MoobSE.handle, MoobSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_A) == TRUE)
	{
		player.img.x -= player.speed * fps.DeltaTime;


		//動く時の効果音
		if (CheckSoundMem(MoobSE.handle) == 0)
		{
			PlaySoundMem(MoobSE.handle, MoobSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_D) == TRUE)
	{
		player.img.x += player.speed * fps.DeltaTime;


		//動く時の効果音
		if (CheckSoundMem(MoobSE.handle) == 0)
		{
			PlaySoundMem(MoobSE.handle, MoobSE.playType);
		}
	}

	enemy.img.y += enemy.speed * fps.DeltaTime;
	//敵の動き
	if (enemy.img.y <= 0 || enemy.img.y + enemy.img.height >= GAME_HEIGHT)
	{
		enemy.speed = -enemy.speed;
	}



	//当たり判定を更新
	collUpdateplayer(&player);
	//当たり判定を更新
	collUpdateGoal(&Goal);
	//当たり判定を更新
	collUpdateenemy(&enemy);

	//プレイヤーがゴールに当たった時
	if (colltouch(player.coll, Goal.coll) == TRUE)
	{
		//BGMを止める
		StopSoundMem(PlayBGM.handle);

		ChangeScene(GAME_SCENE_END);
		return;
	}
	//プレイヤーが敵に当たった時
	if (colltouch(player.coll, enemy.coll) == TRUE)
	{
		//BGMを止める
		StopSoundMem(PlayBGM.handle);

		ChangeScene(GAME_SCENE_END);
		return;
	}
	return;
}
/// <summary>
/// プレイ画面　描画
/// </summary>
VOID PlayDraw(VOID)

{
	//プレイ動画を背景に描画
	//もし動画が再生されていなければ
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		//再生する
		SeekMovieToGraph(playMovie.handle, 0);	//シークバーを最初に戻す
		PlayMovieToGraph(playMovie.handle);		//動画を再生（描画はしていない）
	}

	//動画を描画(画像を画面に合わせて引き延ばす)
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, TRUE);
	//ゴールを描画
	if (Goal.img.IsDraw == TRUE)
	{
		DrawGraph(Goal.img.x, Goal.img.y, Goal.img.handle, TRUE);


		//デバックの時は、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//プレイヤーを描画
	if (player.img.IsDraw == TRUE)
	{
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);


		//デバックの時は、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//敵を描画
	if (enemy.img.IsDraw == TRUE)
	{
		DrawGraph(enemy.img.x, enemy.img.y, enemy.img.handle, TRUE);


		//デバックの時は、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(enemy.coll.left, enemy.coll.top, enemy.coll.right, enemy.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// エンド画面
/// </summary>
VOID End(VOID)
{
	EndProc();
	EndDraw();
	return;
}
/// <summary>
/// エンド画面　処理
/// </summary>
VOID EndProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGMを止める
		StopSoundMem(EndBGM.handle);
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽
		//タイトル画面に切り替え
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
/// エンド画面 描画
/// </summary>
VOID EndDraw(VOID)
{
	//EndLogoの描画
	DrawGraph(EndLogo.x, EndLogo.y, EndLogo.handle, TRUE);
	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// 切り替え画面
/// </summary>
VOID Change(VOID)
{
	ChangeProc();
	ChangeDraw();
	return;
}
/// <summary>
/// 切り替え画面 処理
/// </summary>
VOID ChangeProc(VOID)
{
	//フェードイン
	if (IsFadeIn == TRUE)
	{
		if (FadeInCnt > FadeInCntMax)
		{
			FadeInCnt--;	//カウンタを減らす、透明にしていく
		}
		else
		{
			//フェードイン処理の終了時 透明カウンタのMAXを超えたとき
			FadeInCnt = FadeInCntInit;	//カウンタ初期化
			IsFadeIn = FALSE;			//フェードイン処理の終了
		}

	}
	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		if (FadeOutCnt < FadeOutCntMax)
		{
			FadeOutCnt++;	//カウンタを増やす、色を濃くにしていく
		}
		else
		{
			//フェードイン処理の終了時 透明カウンタのMAXを超えたとき
			FadeOutCnt = FadeOutCntInit;	//カウンタ初期化
			IsFadeOut = FALSE;			//フェードイン処理の終了
		}

	}

	//切り替え処理終了？
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//フェードアウトもフェードインもしていない
		GameScene = NextGameScene;	//次のシーンに切り替え
		OldGameScene = GameScene;	//以前のシーンに更新
	}
	return;
}

/// <summary>
/// 切り替え画面　描画
/// </summary>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();		//エンド画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)FadeInCnt / FadeInCntMax) * 255);	//パーセンテージ

	}
	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)FadeOutCnt / FadeOutCntMax) * 255);	//パーセンテージ

	}

	//四角を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 当たり判定の更新(プレイヤー専用)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID collUpdateplayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x + 30;		//当たり判定を微調整
	chara->coll.top = chara->img.y + 25;		//当たり判定を微調整
	chara->coll.right = chara->img.x + chara->img.width - 15;	//当たり判定を微調整
	chara->coll.bottom = chara->img.y + chara->img.height - 20;	//当たり判定を微調整

	return;
}

/// <summary>
/// 当たり判定の更新(ゴール用)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID collUpdateGoal(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// 当たり判定の更新(敵用)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
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
		p.left < g.right &&	//pの左辺x < gの右辺x座標
		p.right > g.left &&	//pの右辺x < gの左辺x座標
		p.bottom > g.top &&	//pの上辺y < gの下辺y座標
		p.top < g.bottom	//pの下辺y < gの上辺y座標
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}