#ifndef __DD_RC_DEFINITION_H
#define __DD_RC_DEFINITION_H

/*  * File:   PS2ControalerDataFomat.h
 * Author: oishi
 *
 * Created on 2015/05/26, 9:26
 */

/*PS2 controaler 定義データ*/

#define _RC_BAUD 9600

#define __RC_Left    0x08
#define __RC_Down	0x04
#define __RC_Up  0x01
#define __RC_Right   0x02

#define __RC_Circle 0x02
#define __RC_Tri	0x01
#define __RC_Sq 0x08
#define __RC_Cross	0x04

#define __RC_L1	0x08
#define __RC_L2	0x04
#define __RC_R1	0x02
#define __RC_R2	0x01

#define __RC_None 0x00
#define __RC_Zyuzi	0x01
#define __RC_Marutoka	0x02
#define __RC_LR	0x03
#define __RC_LY	0x04
#define __RC_LX	0x05
#define __RC_RY	0x06
#define __RC_RX	0x07

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

#define __RC_GET_LX_VAL(x) (x[__RC_LX])
#define __RC_GET_RX_VAL(x) (x[__RC_RX])
#define __RC_GET_LY_VAL(x) (x[__RC_LY])
#define __RC_GET_RY_VAL(x) (x[__RC_RY])

#endif
