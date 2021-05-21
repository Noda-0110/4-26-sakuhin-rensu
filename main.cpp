
//ヘッダファイル読み込み
#include "game.h"		//ゲームを管理する為のヘッダファイル　あくまでもゲームの管理
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理

//グローバル変数
//シーンを管理する変数
GAME_SCENE GameScene;		//現在のゲームのシーン
GAME_SCENE OldGameScene;	//前回もゲームのシーン
GAME_SCENE NextGameScene;	//次のゲームのシーン

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
VOID ChangeScene(VOID);


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


	//円の中心点
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;
	//円の半径
	int radius = 50;

	//四角の位置　大きさ
	//int x = 0;
	//int y = 0;
	
	//int WidthB = 25;
	//int HeightB = 25;

	//最初のシーンはタイトル画面
	GameScene == GAME_SCENE_TITLE;


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


		
		//キーを入力(円)
		if (KeyDown(KEY_INPUT_UP) == TRUE)
		{
			Y--;	//上に移動
		}

		if (KeyDown(KEY_INPUT_DOWN) == TRUE)
		{
			Y++;	//下に移動
		}
		if (KeyDown(KEY_INPUT_LEFT) == TRUE)
		{
			X--;	//左に移動
		}
		if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
		{
			X++;	//右に移動
		}

		DrawCircle(X, Y, radius, GetColor(255, 255, 0), TRUE);

		//DrawBox(x, y, x + WidthB,y + HeightB, GetColor(0, 0, 255), TRUE);

		//FPS値を描画
		FPSDraw();
		//FPS処理を待つ
		FPSWait();

		ScreenFlip();	//ダブルバッファリングした画面を描画
	}


	// ＤＸライブラリ使用の終了処理（準備）
	DxLib_End();

	return 0;				// ソフトの終了（実行） 
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
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽
		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);
	}

	return;
}
/// <summary>
/// タイトル画面　処理 
/// </summary>
VOID TitleDraw(VOID)
{
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
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽
		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
	}

	return;
}
/// <summary>
/// プレイ画面　描画
/// </summary>
VOID PlayDraw(VOID)

{
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
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽
		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);
	}

	return;
}
/// <summary>
/// エンド画面 描画
/// </summary>
VOID EndDraw(VOID)
{
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