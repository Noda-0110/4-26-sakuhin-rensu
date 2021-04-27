//ヘッダーファイルの読み込み
#include "DxLib.h"//DxLibを使う時に必要


//マクロ定義
#define GAME_TITLE "ゲームタイトル"	//ゲームタイトル
#define GAME_WIDTH  1280		//ゲーム画面の幅
#define GAME_HEIGHT 720			//ゲーム画面の高さ
#define GAME_COLOR  32			//ゲームの色域

#define GAME_ICON_ID 333			//ゲームのICONのID
// プログラムは WinMain から始まります
//Windowsのプログラミング法で動いている
//DxLibは、DirectXという、ゲームプログラミングを簡単に扱える仕組み
int WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	ChangeWindowMode(TRUE);			//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);	//ウィンドウのタイトル文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景色
	SetWindowIconID(GAME_ICON_ID);
														
														//ＤＸライブラリ初期化処理-が帰ってきたら強制終了
	if (DxLib_Init() == -1)		
	{
	// エラーが起きたら直ちに終了
		return -1;			
	}

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
	}
	// ＤＸライブラリ使用の終了処理（準備）
	DxLib_End();				

	return 0;				// ソフトの終了（実行） 
}
