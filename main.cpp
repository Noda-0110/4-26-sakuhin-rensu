//ヘッダーファイルの読み込み
#include "DxLib.h"//DxLibを使う時に必要


//マクロ定義
#define GAME_TITLE "ゲームタイトル"	//ゲームタイトル
#define GAME_WIDTH  1280		//ゲーム画面の幅
#define GAME_HEIGHT 720			//ゲーム画面の高さ
#define GAME_COLOR  32			//ゲームの色域

#define GAME_ICON_ID 333			//ゲームのICONのID


#define GAME_WINDOW_BAR 0	//ウインドウバーの種類

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
	SetAlwaysRunFlag(TRUE);					//ウィンドウをずっとアクティブにする

														
														//ＤＸライブラリ初期化処理-が帰ってきたら強制終了
	if (DxLib_Init() == -1)		
	{
	// エラーが起きたら直ちに終了
		return -1;			
	}

	//四角の位置を決める
	int X = GAME_WIDTH / 2;		//画面幅の中心
	int Y = GAME_HEIGHT / 2;
	//四角の大きさを決める
	int width = 32;		//幅
	int height = 32;	//高さ


	//円の半径
	int radius = 100;


	//移動するスピード
	int speed = 1;
	int wspeed = speed;
	int hspeed = speed;

	//ダブルバッファリングを有効
	SetDrawScreen(DX_SCREEN_BACK);


	//無限ループ　受け取り続ける
	while (1)
	{
		//何かしらのキーが押されたとき
		if (CheckHitKeyAll() != 0)
		{
			break;	//無限ループを抜ける
		}

		//メッセージを受け取り続ける（本体）
		if (ProcessMessage() != 0) //-1のときに、エラーウやウィンドウが閉じられた
		{
			break;	//無限ループを抜ける
		}

		//画面を消去する
		if (ClearDrawScreen() != 0) { break; }

		//四角を描画する
		//DrawBox(
		//	X, Y, X + width, Y + height,
		//	GetColor(255, 0, 0),	//色を取得
		//	TRUE					//取得した色で塗りつぶすか？
		//);

		//円を描画
		DrawCircle(X, Y, radius,
			GetColor(0, 255, 0),
			FALSE, 5
		);

		//四角を動かす
		X += wspeed;		//右に動かす

		//四角を斜め右下へ
		Y += hspeed;

		//四角を画面の端に来た時に、向きを反転させる
		if (X - radius < 0 || X+radius > GAME_WIDTH)	//Xが０以下、１２８０以上になった横の時
		{
			//移動向き(符号)を反転
			wspeed = -wspeed;	//1→-1　ーにして渡す -1→1　＋にして渡す

			//壁に当たると加速
			if (wspeed > 0) { wspeed += 2; }
			else if (wspeed < 0) { wspeed -= 2; }
		}



		if (Y - radius < 0 || Y + radius > GAME_HEIGHT)	//Xが０以下、720以上になった横の時
		{
			//移動向き(符号)を反転
			hspeed = -hspeed;	//1→-1　ーにして渡す -1→1　＋にして渡す

			if (hspeed > 0) { hspeed += 2; }
			else if (hspeed < 0) { hspeed -= 2; }
		}


		ScreenFlip();	//ダブルバッファリングした画面を描画
	}

	
	

	// ＤＸライブラリ使用の終了処理（準備）
	DxLib_End();				

	return 0;				// ソフトの終了（実行） 
}
