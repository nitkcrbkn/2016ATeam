## MD
### プロコトル
| bit   |  7  |  6  |  5  |  4  |  3  |  2 |  1  |  0  |
|:-----:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|data[1]|  0  |  0  |  0  |  0  |dir1 |dir0 | D9  | D8  |
|data[0]| D7  | D6  | D5  | D4  | D3  | D2  | D1  | D0  |

表示形式は
```
MD(Add:hex):[Fr,Fw,Bw,Br],[duty:dec]
```
です。

## 用意した型
enumは上から順にフリーモード、正転後転ブレーキです。
構造体は上から順にアドレス、デューティ比、先ほどのモードとなります。
```c
typedef enum{
  D_MMOD_FREE=0,
  D_MMOD_FORWARD=1,
  D_MMOD_BACKWARD=2,
  D_MMOD_BRAKE=3,
} DD_MDMode_t;

typedef struct{
  uint8_t add;
  uint16_t duty;
  DD_MDMode_t mode;
}DD_MDHand_t;
```

## 用意した関数
- int DD_send2MD(DD_MDHand_t *dmd);

MDにデータを送信します。
- void DD_MDHandPrint(DD_MDHand_t *dmd);

MDのハンドラを表示します。　
