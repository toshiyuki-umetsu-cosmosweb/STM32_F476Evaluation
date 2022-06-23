# STM32_F476Evaluation
Evaluation project source with STM32L476RG
https://www.st.com/ja/microcontrollers-microprocessors/stm32l476rg.html

# おまけデバイス
温湿度センサ(i2cの動作確認で使用)
https://www.marutsu.co.jp/pc/i/253906/

# i2c や SPIをノンブロッキングI/O(Transmit_ITなど)でやるには？

リクエストオブジェクトとして渡して、送信完了コールバックにて、次の受信処理を呼び出すようにする。
アプリケーション層では、非同期に処理するような設計にする必要がある。

