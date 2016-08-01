## RC

表示形式は
```
RC btn:[Up,Lf,Dw,Ri,Cr,Tr,Sq,Cs,L1,L2,R1,R2]
RC analog[<LX>:dec,<LY>:dec][<RX>:dec,<RY>:dec]
```
です。

### 用意した関数
#### システム系
- int DD_RCInit(uint8_t setdata[RC_DATA_NUM],uint32_t timeout);

RCと通信するための初期化をします。接続街も含み、timeoutで諦めます。

- DD_RCTask(uint8_t rcv_data[RC_DATA_NUM],uint8_t setdata[RC_DATA_NUM]);

リモコンのデータを処理し、並び替えてsetdataに格納します。

- void DD_RCPrint(DD_MDHand_t *dmd);
RCの中身を表示します。　

#### データ取得系
アナログデータを取得します。
```c
/*アナログデータ取得*/
int DD_RCGetLX(volatile uint8_t data[RC_DATA_NUM]);
int DD_RCGetLY(volatile uint8_t data[RC_DATA_NUM]);
int DD_RCGetRX(volatile uint8_t data[RC_DATA_NUM]);
int DD_RCGetRY(volatile uint8_t data[RC_DATA_NUM]);
```

ボタンの状態を得ます。
```c
#define __RC_ISPRESSED_CIRCLE(x) !(x[__RC_Marutoka]&__RC_Circle)
#define __RC_ISPRESSED_TRIANGLE(x) !(x[__RC_Marutoka]&__RC_Tri)
#define __RC_ISPRESSED_SQARE(x) !(x[__RC_Marutoka]&__RC_Sq)
#define __RC_ISPRESSED_CROSS(x) !(x[__RC_Marutoka]&__RC_Cross)

#define __RC_ISPRESSED_LEFT(x) !(x[__RC_Zyuzi]&__RC_Left)
#define __RC_ISPRESSED_RIGHT(x) !(x[__RC_Zyuzi]&__RC_Right)
#define __RC_ISPRESSED_UP(x) !(x[__RC_Zyuzi]&__RC_Up)
#define __RC_ISPRESSED_DOWN(x) !(x[__RC_Zyuzi]&__RC_Down)

#define __RC_ISPRESSED_L1(x) !(x[__RC_LR]&__RC_Left)
#define __RC_ISPRESSED_R1(x) !(x[__RC_LR]&__RC_Right)
#define __RC_ISPRESSED_L2(x) !(x[__RC_LR]&__RC_Up)
#define __RC_ISPRESSED_R2(x) !(x[__RC_LR]&__RC_Down)
```
