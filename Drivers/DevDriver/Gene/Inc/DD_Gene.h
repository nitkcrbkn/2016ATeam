/* ===Kisarazu RBKN Library===
 *
 * autor          : Oishi
 * version        : v0.10
 * last update    : 20160703
 *
 * **overview***
 * 共通部分のコードと他のレイヤーとの通信部分
 *
 * ・I2Cのみのサポート
 */
#ifndef __DD_GENE_H
#define __DD_GENE_H

#define DD_NUM_OF_MD 1
#define DD_NUM_OF_AB 0

/*I2Cのサポート用関数*/
int DD_I2CSend(uint8_t add,const uint8_t *data,uint8_t size);
/*Deviceのハンドラーの表示用関数*/
void DD_print(void);
/*DeviceDriverのタスク*/
int DD_doTasks(void);
#endif
