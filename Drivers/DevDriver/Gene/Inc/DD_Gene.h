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

#define DD_NUM_OF_MD 2
#define DD_NUM_OF_AB 0

int DD_I2CSend(uint8_t add,uint8_t *data,uint8_t size);

#endif
