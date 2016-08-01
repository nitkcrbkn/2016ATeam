# SystemTaskManager

詳しくは別途pdfを参考。ここでは一連の処理の過程を書きます。mainのエントリーポイントもここにあります。

## 非公開

### 関数
関数の一覧です。

```c
static
int SY_init(void);
static
int SY_I2CConnTest(int timeout);
static
int SY_doAppTasks(void);
static
int SY_clockInit(void);
static
void SY_GPIOInit(void);
static
int SY_doDevDriverTasks(void);
```

- static int SY_init(void)

システムを初期化をします。非公開です。不成功でEXIT_FAILURE,成功でEXIT_SUCCESS（0）を返します。

- static int SY_I2CConnTest(int timeout)

I2Cテストをします。(未実装項目）非公開です。不成功でEXIT_FAILURE,成功でEXIT_SUCCESS（0）を返します。

- static SY_doAppTasks(void)

アプリケーションの処理を行います。内部でユーザアプリケーションを呼び出します。非公開です。不成功でEXIT_FAILURE,成功でEXIT_SUCCESS（0）を返します。

- static int SY_clockInit(void)

クロックの初期化を試みます。非公開です。不成功でEXIT_FAILURE,成功でEXIT_SUCCESS（0）を返します。

- static void SY_GPIOInit(void)

GPIOの初期化をします。非公開で返り値はありません。

- static int SY_doDevDriverTasks(void)

ドライバのタスクを行います。MD、ABへのデータ転送が主な仕事です。非公開です。不成功でEXIT_FAILURE,成功でEXIT_SUCCESS（0）を返します。


## 公開

エントリーポイント`int main(void)`は、プログラムの開始点です。ここからプログラムはスタートし、一連の流れの全てをここにて記述されます。また、いくつかのコールバック関数があります。

``` c
  MW_hadCompleted();
  DD_RCTask(rc_rcv, (uint8_t*)g_rc_data);
```
- MW_hadCompleted(void)

UARTのデータ転送完了時に呼び出されます。

- DD_RCTask(uint8_t *rc_rcv,uint8_t *g_rc_data)

リモコンのデータを8バイト受信するたびに呼び出されます。

## 動作

1. システムの初期化
2. I2Cの接続チェック
3. ループ中にそれぞれの処理を記述

という流れです。

### 1.システムの初期化
　最初にHALを初期化し、その次にクロックの初期化をします。メッセージを使う準備をし、UART2を初期化します。ここからMessageが利用可能になります。そしたらデバイスを使うための初期化をし、（MD,AB）アプリケーションを続いて初期化します。

　続いてRCの接続待ちを行います。これはタイムアウト値が決められており、その時間までに仮に間に合わなかった場合、初期化が失敗します。また、接続が完了したらニュートラルの設定をします。8回スキャンした平均値を持ちいります。また、以後は自動的に値は更新されていきます。
　
### 2.I2Cの接続チェック
　未実装項目。I2Cデバイスをスキャンし、それらをチェックします。未実装です。
　
### 3.処理ループ
以下の処理を行います。
5msごとにAppのTaskとDDのTaskを交互に実行する。また、30msごとに表示を行う。
