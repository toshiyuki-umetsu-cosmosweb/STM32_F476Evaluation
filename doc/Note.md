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

## Delete previously generated files when not re-generated

チェックすると、コード生成したとき、コード生成対象でないペリフェラルのコードを消去する。

# ドライバに関して

## 割り込み周り

各ペリフェラルだけでなく、NVIC(Nested Vector Interrupt Controller)の設定が必要。
コード生成するかどうかと、プライオリティ。

##  GPIO周り

出力を変更するときは、ODR(OutputDataRegister)ではなく、BRR, BSRRを使用する。
BRRは指定したビットに対応するODRのビットを0にセットするレジスタ。
BSRRは指定したビットに対応するODRのビットを0または1にセットするレジスタ。
BSRRは下位ビットが1にするビット、上位ビットが0にするビットに割り当てられているみたい。
基本的にMCU依存なので、可能な限りHAL層ドライバのインタフェースを使った方がいい。

