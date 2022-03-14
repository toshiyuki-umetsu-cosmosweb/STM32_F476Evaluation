/*
 * debug.c
 *
 *  Created on: Mar 14, 2022
 *      Author: toshiyuki_umetsu
 *
 */
/**
 * @note
 * デバッガの設定
 * (1) プロジェクトを右クリックし、「Debug As」-> 「Debug Configuration」を開き、
 *    「Debugger」タブのシリアルワイヤビューワ(SWV)のチェックを入れる。
 *     このとき、ボードのクロックを設定すること。
 * (2) CubeIDEにて、「Window」->「Show View」->「SWV」->「SWV ITM Data Console」
 *     を選択する。
 * (3) SWV ITM Data Console画面にて、設定ボタンを押して設定画面を開く。
 *     (デバッグ動作中は選択不可。)
 *     「ITM Simulasポート」の「0」をチェックする。
 *     他はチェックしない。
 * (4) SWV ITM Data Console画面にて、「Start Trace」を有効にする。
 * (5) デバッグ実行する。
 *     ソースコードに記載した、printfメッセージがSWV ITM Data Consoleに表示される。
 */


#include "debug.h"

#ifdef _DEBUG
#include <stdio.h>
#include <stdint.h>

#include "main.h"

/**
 * 文字を出力に送る
 *
 * @return 結果
 */
int
__io_putchar(uint8_t c)
{
	return ITM_SendChar(c);
}

#endif // _DEBUG
