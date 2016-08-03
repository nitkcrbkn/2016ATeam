
## AB
### プロコトル
| bit   |  7  |  6  |  5  |  4  |  3  |  2 |  1  |  0  |
|:-----:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|data[1]| D7  | D6  | D5  | D4  | D3  | D2  | D1  | D0  |
|data[0]| !D7 | !D6 | !D5 | !D4 | !D3 | !D2 | !D1 | !D0 |
はあはあ。。。表ってこんなにつらいのか。。。
表示形式は
```
AB(Add:hex):[data:binary]
```
です

## 用意した型
用意した型はひとつだけで上からアドレス、データを格納します。
```c
typedef struct{
  uint8_t add;
  uint8_t dat;
}DD_ABHand_t;
```

### 用意された関数
- int DD_send2AB(DD_ABHand_t *dab);

ABにデータを送信します。
- void DD_ABHandPrint(DD_ABHand_t *dab);

ABのハンドラを表示します。　
です。
