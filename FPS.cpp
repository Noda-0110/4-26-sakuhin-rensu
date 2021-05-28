//FPSのソースファイル

//ヘッダファイル読み込み
#include "FPS.h"

//グローバル変数
FPS fps;	//FPSの管理(FPSは構造体)

//関数

/// <summary>
/// FPS値を計測し、値を更新する
/// </summary>
/// <param name=""></param>
VOID FPSUpdate(VOID)
{
	if (FALSE == fps.IsInitFlg)
	{
		//測定開始時刻をマイクロ単位で所得
		fps.startTime = GetNowHiPerformanceCount();	//Windowsが起動してから経過した時間（マイクロ秒）

		fps.IsInitFlg = TRUE;	//フラグを立てる
	}

	//現在の時刻をマイクロ秒単位で取得
	fps.NowTime = GetNowHiPerformanceCount();

	//前回取得時間からの経過時間を秒（小数）に変換してからセット
	fps.DeltaTime = (fps.NowTime - fps.OldTime) / 1000000.0f;

	//今回所得した時間を保存
	fps.OldTime = fps.NowTime;

	//1フレーム目～FPS設定値までは、カウントアップ
	if (fps.count < fps.sampleRate)
	{
		//カウンタを増やす
		fps.count++;
	}
	else {
		//FPS設定値のフレームで、平均FPSを計算

		//現在の時刻から0フレーム目の時間を引きFPSの数値で割る
		//現在のFPSの値が出る
		fps.DrawValue = 1000000.f / ((fps.NowTime - fps.startTime) / (float)fps.sampleRate);

		fps.startTime = GetNowHiPerformanceCount();	//Windowsが起動してから経過した時間（マイクロ秒）

		//カウンタ初期化
		fps.count = 1;
	}
	return;
}

/// <summary>
/// 平均FPS値を描画する
/// </summary>
/// <param name=""></param>
VOID FPSDraw(VOID)
{
	if (GAME_DEBUG == TRUE) {
		//文字列を描画
		DrawFormatString(5,700, GetColor(0, 0, 0), "FPS:%.1f", fps.DrawValue);
	}
	return;
}


/// <summary>
/// FPSで処理を待つ
/// </summary>
/// <param name=""></param>
VOID FPSWait(VOID)
{
	//現在の時刻ー最初の時刻で、現在かかっている時刻を所得する
	LONGLONG resulTtime = fps.NowTime - fps.startTime;

	//待つべきミリ秒数（1秒/FPS値　*　現在のフレーム数から現在かかっている時刻を引く
	LONGLONG waitTime = 1000000.0f / fps.Value * fps.count - resulTtime;

	//マイクロ秒からミリ秒に変換
	waitTime /= 1000.0f;

	//処理が早かったら処理を待つ
	if (WaitTimer > 0)
	{
		WaitTimer(waitTime);	//引数ミリ秒松
	}


	//垂直同期をOFFにしているか
	if (GetWaitVSyncFlag() == FALSE)
	{
		//FPS最大値でない
		if (fps.Value < GAME_FPS_MAX)
		{
			//一秒間のFPS値よりも、待つ時間が小さい時は、もっとFPS値をあげてもよい
			//待つ時間10ミリ <= 1秒/60FPS = 16.6666ミリ　少し早くできる
			if (waitTime > 0
				&& waitTime <= 1000.0f / fps.Value)
			{
				fps.Value++;
			}
			else
			{
				//FPS値が追いついておらず、遅い時はFPS値を下げる
				if (fps.Value > GAME_FPS_MIN)
				{
					fps.Value--;
				}
			}
		}
	}
}