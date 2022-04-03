調べたことをつらつらと書いておく。

# バージョン管理対象外

.setting Eclipseの設定フォルダ
Debug Debugビルドの中間・最終成果物
Backup CubeMXがコード生成したとき、バックアップとして保存されるファイルの置き場所。
       サブディレクトリに細かく生成されるので厄介。

# コード生成(CubeMX)周り

ProjectManager設定の「Code Generator」タブにある設定。

## Generate peripheral initialization as a pair of '.c/.h' files per peripheral

ペリフェラルの初期化コードを生成するかどうか。チェックすると、uart.h,uart.cのようなファイルが生成され、
ペリフェラルの初期化コードが生成される。
チェックしない場合には生成されず、自分で初期化コードを書く必要がある。
インスタンス管理を自分でやりたい場合にはチェックを外す？

## Backup Previously generated files when re-generating

コード生成したとき、前に生成したコードのバックアップを取るかどうか。
バックアップをとる設定にした場合、バージョン管理からBackupというフォルダ名を全て除外するように設定すること。
Gitなら、.gitignoreに「Backup」と書いた行を追加すればよい。

## Keep User Code when re-generating

コード生成したとき、ユーザーコード(「USER CODE BEGIN」と「USER CODE END」に囲まれた領域)を残すかどうか。
チェックを入れておくと、既存のユーザーコード部分を抽出してマージしてくれる。
マルチバイト文字（日本語とか）は、マージするときに文字化けするので使わないこと。

## Delete previously generated files when not re-generated

チェックすると、コード生成したとき、コード生成対象でないペリフェラルのコードを消去する。


# 基本的なこと

割り込み優先順位は、低い値ほど優先される。


# ドライバに関して

## 割り込み周り

各ペリフェラルだけでなく、NVIC(Nested Vector Interrupt Controller)の設定が必要。
コード生成するかどうかと、プライオリティ。

## GPIO周り

出力を変更するときは、ODR(OutputDataRegister)ではなく、BRR, BSRRを使用する。
BRRは指定したビットに対応するODRのビットを0にセットするレジスタ。
BSRRは指定したビットに対応するODRのビットを0または1にセットするレジスタ。
BSRRは下位ビットが1にするビット、上位ビットが0にするビットに割り当てられているみたい。
基本的にMCU依存なので、可能な限りHAL層ドライバのインタフェースを使った方がいい。
割り込みハンドラを使う場合には、NVICの設定を行った後、
HAL_GPIO_EXTICallbackを実装する。
引数にピン番号が入るので、GPIO_PIN_xxと比較することで、対象のピンかどうかを判定できる。


## UART周り

割り込みやDMAを使う場合には、必ずNVICの設定をすること。
割り込み送受信のインタフェースを使用する場合、
送受信完了は 「huartX.gState == HAL_UART_STATE_READY」で判定する。

## タイマー周り

ドキュメントを熟読しないとなかなか難しい。
「リファレンスマニュアル」参照。
タイマーと考えるよりも、システムクロックを元に動作する、超高機能なカウンターと考えた方がいいかも。

|用語|説明|
|---|---|
|APBxクロック|タイマペリフェラルの駆動クロックのこと。ClockConfigurationに書かれている通り。APB1やらABP2やらがある。|
|プリスケーラ|分周器とかそのあたりのこと。|
|ITRx|ITR1,ITR2,ITR3,ITR4とあり、タイマペリフェラル間の同期信号になる。|
|TIx|タイマーの各チャンネルに入力される入力信号のこと。|
|ETR|外部クロック入力信号のこと。|
|スレーブモード|同期動作時のふるまいのこと。|
|リセットモード|同期動作時のふるまい。トリガ入力でカウンタとプリスケーラを再初期化する。|
|ゲートモード|同期動作時のふるまい。入力が一定の条件の時だけカウントアップ/カウントダウンさせる。|
|トリガモード|同期動作時のふるまい。入力条件により、タイマのカウント開始を行う。|
|外部クロックモード1|TIx信号を外部入力信号として動作させるモード。|
|外部クロックモード2|ETR信号を外部トリガ入力として動作させるモード。|

### 用途例：通常の周期タイマー

500msec毎になんらかの処理を行わせる場合など。
「Clock Source」に「Internal Clock」を設定し、プリスケーラ、カウンターペイロードの設定をする。
例えばAPBxが80MHzなら、プリスケーラを(8000-1)にすれば8000/80MHz[sec]つまり、0.1[msec]毎にカウントアップする。
それから「Counter Period」を(5000-1)にすれば、500ミリ毎にアップデートイベントが発生する。
「auto-reloadpreload」をEnableにしておくと、カウンターオーバーフロー時にCounter Periodが切り替わる。
Disableにすると、ARRを書き替えた時にCounter Period（に相当するタイムアウトレジスタ）が切り替わる。
周期を動的に変更する必要がある場合には、事故を防ぐためにも「auto-reloadpreload」はEnableにしておく方が良い。
常に一定の周期でしか使わないなら、Disableにしても問題ない。
(https://stackoverflow.com/questions/66363735/stm32-timer-auto-reload-preload)
通知はNVICで「update/global」割り込みを有効に設定し、「void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)」を実装する。
また、HAL_TIM_Base_Start_IT(&htimX)を呼び出し、割り込み通知を許可する。

### 用途例：アウトプットコンペア

カウンタがxxxになったときに割り込みを入れて何らかの処理をする場合など。
通常の周期タイマーの設定をする。
その上で、「TIMx」の「ChannelX」に「Output Compare No Output」を設定する。
Modeを「Active Level on matach」などに設定し、Pulseを所定のカウンタ値に設定する。
また、HAL_TIM_OC_Start_IT(&htimX, TIM_CHANNEL_X)を呼び出し、アウトプットコンペア割り込みを許可する。

### 用途例：PWM出力

PWM信号を出力する場合など。
通常の周期タイマーの設定をする。
その上で、「TIMx」の「ChannelX」に「PWM Generation CHx」を設定する。
「PWM Generation CHx」にて、「Pulse」幅（周期からDuty比で計算）を設定する。
他のマイコンでもあったように、Duty比0％、Duty比100%を設定するには特別な処理が必要。
Duty比100％ -> CCRx レジスタの値を ARR + 1 に設定することで、PWMパルス信号のL/Hが切り替わらないようにする。
Duty比0％ -> OPxレジスタを操作し、I/Oレジスタの出力にする。（L固定 or H固定）
いずれにせよ、PWM周期が崩れる点には注意が必要。
Duty比を指定してCCRxレジスタを書き替えてくれるHAL層のインタフェースは無いようだ。

## スリープモード

SLEEP/低電力SLEEPモードに入るには、HAL_PWR_EnterSLEEPMode()を使用する。
割り込みまたはウェイクアップイベント（特定のGPIOピンによる操作）により、復帰する。
プログラムコードは続きから開始される。

```
HAL_SuspendTick();
HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
HAL_ResumeTick();
```

第2引数は復帰要因っぽい。WFI=Wait For Interrupt, WFE=Wait For Event.
ざっくり書くと、WFIは割り込みのみ。WFEは割り込みと特定のGPIOピンによる操作（で起こされるウェイクアップイベント）
SLEEP中もTickを動かしたいなら、HAL_SuspendTickとHAL_ResumeTick()を抜けば良い。但し消費電力も増える。

STANDBY/SHUTDOWNモードはもっと消費電力が下がるが、ペリフェラルも止まるので、長時間停止させる場合に使用する。
プログラムコードもリセットから開始するっぽい。

## RTC

クロックの設定が必要。
時間をカウントするためのクロックソースをどこから持ってくるか。
STM32L476RGでは、オンボードのMCUに外付けされた32768Hzの発振子(X2)、HSE入力を32分周したクロック、内蔵発振子から選択できる。
あとは、HAL_RTC_GetTime / HAL_RTC_GetDateを使用して日時を取得する。
RTC_FORMAT_BINを渡すと、通常の10進数表現になり、RTC_FORMAT_BCDを渡すと、BCD表現でのやりとり(13は0x13と表現される）になる。


