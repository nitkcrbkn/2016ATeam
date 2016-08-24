## flash

### 用意した型
上から順にフラッシュの書き込み成功、ロックの失敗、アンロックの失敗、イレースの照合失敗、その他のイレースの失敗、書き込みの照合失敗、その他のフラッシュの書き込み失敗です。
```c
typedef enum {
  MW_FLASH_OK=0,
  MW_FLASH_LOCK_FAILURE,
  MW_FLASH_UNLOCK_FAILURE,
  MW_FLASH_ERASE_VERIFY_FAILURE,
  MW_FLASH_ERASE_FAILURE,
  MW_FLASH_WRITE_VERIFY_FAILURE,
  MW_FLASH_WRITE_FAILURE,
} flashError_t;
```

### 用意した関数
```c
static
flashError_t MW_flashUnlock(void);
static
flashError_t MW_flashLock(void);
static
flashError_t MW_flashElase(/*!erase here!*/const void *flash_add,size_t size);
flashError_t MW_flashWrite(const void *ptr,/*!write here!*/const void *flash_add,size_t size)
```
- static flashError_t MW_flashUnlock(void);

フラッシュのロックを解除します。失敗すると`MW_FLASH_UNLOCK_FAILURE`が帰り、成功で`MW_FLASH_OK`を返します。非公開です。

- static flashError_t MW_flashLock(void)

フラッシュをロックします。失敗すると、`MW_FLASH_LOCK_FAILURE`が帰り、成功で`MW_FLASH_OK`を返します。非公開です。

- static flashError_t MW_flashElase(/*!erase here!*/const void *flash_add,size_t size);

フラッシュのイレースをします。

`flash_add`にアドレスを入れます。**const型ですがかきこれます**。`size`に大きさを入れます。フラッシュの削除に失敗すると、照合エラーで`MW_FLASH_ELASE_VERIFY_FAILURE`、その他のエラーで`MW_FLASH_ELASE_FAILURE`を返します。正常終了は`MW_FLASH_OK`を返します。非公開です。

- flashError_t MW_flashWrite(const void *ptr,/*!write here!*/const void *flash_add,size_t size)

フラッシュの書き込みを試行します。

`ptr`に書き込み先を入れます。**const型ですがかきこれます**。`size`に大きさを入れます。フラッシュの書き込み時に照合にに失敗すると`MW_FLASH_WRITE_VERIFY_FAILURE`、書き込み時のその他のエラーは`MW_FLASH_WRITE_FAILURE`をかえします。成功すると`MW_FLASH_OK`が帰ります。その他のエラーは上を参照してください。他の関数でエラーが起きた場合はそのエラーがそのまま帰ります。
