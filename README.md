# STM32_F476Evaluation
Evaluation project source with STM32L476RG
https://www.st.com/ja/microcontrollers-microprocessors/stm32l476rg.html

# おまけデバイス
温湿度センサ(i2cの動作確認で使用)
https://www.marutsu.co.jp/pc/i/253906/


# 課題メモ

## i2cの割り込みどうすんの？

マスターモードで読み出しを行う場合、リクエスト出してその後読み出しになるが、これを割り込みでやるにはどうするのか。
HAL_I2C_Master_TransmitとHAL_I2C_Master_Receiveを逐次実行するのではない方法が欲しい。

