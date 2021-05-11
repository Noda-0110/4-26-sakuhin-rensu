//キーボードのソースファイル

//ヘッダファイル読み込み
#include "keyboard.h"

//グローバル変数
KEYBOARD keyboard;

//関数




/// <summary>
/// キーの入力状態を更新する
/// </summary>
VOID AllKeyUpdate(VOID)
{
	//直前のキー入力をとっておく　現在AllKeyを以前Oldへと移す
	for (int i = 0; i < KEY_KIND_MAX; i++)
	{
		keyboard.OldAllKeyState[i] = keyboard.AllKeyState[i];
	}

	//全てのキーの入力状態を得る、TempKeyStateの中に入っていく
	GetHitKeyStateAll(&keyboard.TempKeyState[0]);

	//押されているキーの時間を更新する、TempKeyStateを見る
	for (int i = 0; i < KEY_KIND_MAX; i++)
	{
		//キーボードを押しているとき、０でないとき
		if (keyboard.TempKeyState[i] != 0)
		{
			keyboard.AllKeyState[i]++;		//押している時間を加算
		}
		//押していないとき
		else
		{
			keyboard.AllKeyState[i] = 0;	//押している時間を0に戻す
		}
	}
	return;
}

/// <summary>
/// キーを押しているか、キーコードで判断する
/// </summary>
/// <param name="KEY_INPUT_">キーコード</param>
/// <returns>押していたら０でなければTRUE</returns>
BOOL KeyDown(int KEY_INPUT_)
{
	if (keyboard.AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// キーをあげているか（離したとき）、キーコードで判断する
/// </summary>
/// <param name="KEY_INPUT_">キーコード</param>
/// <returns>押していたらTRUE</returns>
BOOL KeyUp(int KEY_INPUT_)
{
	//以前は０でない、かつ現在は０でない
	if (
		keyboard.OldAllKeyState[KEY_INPUT_] != 0	//以前は押していた
		&& keyboard.AllKeyState[KEY_INPUT_] == 0	//現在は押していない
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/// <summary>
/// キーをクリックしたか、キーコードで判断する
/// </summary>
/// <param name="KEY_INPUT_">キーコード</param>
/// <returns>クリックしたらTRUE</returns>
BOOL KeyClick(int KEY_INPUT_)
{
	//以前は０でない、かつ現在は０でない
	if (
		keyboard.OldAllKeyState[KEY_INPUT_] != 0	//以前は押していた
		&& keyboard.AllKeyState[KEY_INPUT_] == 0	//現在は押していない
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/// <summary>
/// キーを押し続けているかを、キーコードで判断
/// </summary>
/// <param name="KEY_INPUT_intMilliTime">キーコード</param>
/// <returns>キーを押し続けているミリ秒</returns>
BOOL KeyDownKeep(int KEY_INPUT_,int MilliTime)
{
	//1秒は1000ミリ秒
	float MilliSec = 1000.0f;


	//押し続けている時間は、ミリ秒数×ＦＰＳ値
	//例　１５００ミリ秒押す/１０００ミリ　→　1.5秒 ×　60FPS = 90
	int UpdateTime = (MilliTime / MilliSec) * 60;

	if (keyboard.AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;	//押し続けている
	}

	else {
		return FALSE;	//押し続けていない
	}

}