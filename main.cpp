
//ヘッダファイル読み込み
#include "game.h"		//ゲームを管理する為のヘッダファイル　あくまでもゲームの管理
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理


//構造体の定義
//キャラクタの構造体
struct CHARACTOR
{
	int handle = -1;	//画像のハンドル（管理番号）
	char path[255];		//画像の場所（パス）
	int x;		//X位置
	int y;		//Y位置
	int width;	//幅
	int height;	//高さ
	int speed = 1;

	RECT coll;	//上下左右当たり判定の領域 RECTは四角形の位置を扱える
	BOOL IsDraw = FALSE;//画像が描画できるか
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

	int Volume = 255;	//ボリューム（最小）0〜255（最大）
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


VOID ChangeScene(GAME_SCENE scene);	//シーン切り替え

VOID collUpdateplayer(CHARACTOR* chara);	//当たり判定の領域を更新

VOID collUpdate(CHARACTOR* chara);	//当たり判定の領域を更新


BOOL colltouch(RECT player, RECT goal);//当たり判定の触れているか触れていないかの判定



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
		DxLib_End();	//強制終了
		return -1;		//エラー終了
	}
	//画像の幅と高さを所得
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//動画のボリューム
	playMovie.Volume = 255;


	//プレイヤーの画像を読み込み
	strcpyDx(player.path, ".\\image\\Player2.png");
	player.handle = LoadGraph(player.path);	//画像を読み込み
	
	//画像が読み込めなかったとき、エラー（ー１）が入る
	if (player.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			player.path,			//メッセージ本文
			"画像読み込みエラー！",	//メッセージタイトル
			MB_OK					//ボタン
		);
		DxLib_End();	//強制終了
		return -1;		//エラー終了
	}

	//画像の幅と高さを所得
	GetGraphSize(player.handle, &player.width, &player.height);


	//プレイヤーの初期化
	player.x = GAME_WIDTH / 2 - player.width / 2;
	player.y = GAME_HEIGHT / 2 - player.height / 2;
	player.speed = 500;
	player.IsDraw = TRUE;


	//当たり判定を更新（関数）
	collUpdateplayer(&player);


	//ゴールの画像を読み込み
	strcpyDx(Goal.path, ".\\image\\Goal2.png");
	Goal.handle = LoadGraph(Goal.path);	//画像を読み込み

	//画像が読み込めなかったとき、エラー（ー１）が入る
	if (Goal.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			Goal.path,			//メッセージ本文
			"画像読み込みエラー！",	//メッセージタイトル
			MB_OK					//ボタン
		);
		DxLib_End();	//強制終了
		return -1;		//エラー終了
	}

	//画像の幅と高さを所得
	GetGraphSize(Goal.handle, &Goal.width, &Goal.height);

	//ゴールの初期化
	Goal.x = GAME_WIDTH - Goal.width;
	Goal.y = 0;
	Goal.speed = 500;
	Goal.IsDraw = TRUE;

	//当たり判定を更新（関数）
	collUpdate(&Goal);


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
	DeleteGraph(player.handle);	//メモリ上から画像を削除
	DeleteGraph(Goal.handle);	//メモリ上から画像を削除

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
	//プレイヤーの操作
	if (KeyDown(KEY_INPUT_W) == TRUE)
	{
		player.y -= player.speed * fps.DeltaTime;
	}

	if (KeyDown(KEY_INPUT_S) == TRUE)
	{
		player.y += player.speed * fps.DeltaTime;
	}

	if (KeyDown(KEY_INPUT_A) == TRUE)
	{
		player.x -= player.speed * fps.DeltaTime;
	}

	if (KeyDown(KEY_INPUT_D) == TRUE)
	{
		player.x += player.speed * fps.DeltaTime;
	}

	//当たり判定を更新
	collUpdateplayer(&player);
	//当たり判定を更新（関数）
	collUpdate(&Goal);

	//プレイヤーがゴールに当たった時
	if (colltouch(player.coll, Goal.coll) == TRUE)
	{
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
	if (Goal.IsDraw == TRUE)
	{
		DrawGraph(Goal.x, Goal.y, Goal.handle, TRUE);


		//デバックの時は、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//プレイヤーを描画
	if (player.IsDraw == TRUE)
	{
		DrawGraph(player.x, player.y, player.handle, TRUE);


		//デバックの時は、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom, GetColor(255, 0, 0), FALSE);
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

/// <summary>
/// 当たり判定の更新(プレイヤー専用)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID collUpdateplayer(CHARACTOR* chara)
{
	chara->coll.left = chara->x + 30;		//当たり判定を微調整
	chara->coll.top = chara->y + 25;		//当たり判定を微調整
	chara->coll.right = chara->x + chara->width - 15;	//当たり判定を微調整
	chara->coll.bottom = chara->y + chara->height - 20;	//当たり判定を微調整

	return;
}

/// <summary>
/// 当たり判定の更新(ゴール用)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID collUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;
	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

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