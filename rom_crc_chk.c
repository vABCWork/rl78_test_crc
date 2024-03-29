
#include   "misratypes.h"

#pragma address rom_crc = 0x03dfe	// rom_crcの格納アドレス = 03DFE H
const uint16_t __far rom_crc = 0xbd98;  // リンカー出力 CRCの値 (.text と .textfの領域)


uint8_t  rom_crc_16_err;       // ROMのCRC一致(0)/不一致(1)   (初期値なしグローバル変数)
uint16_t rom_cal_crc;	       // マイコンで計算した CRC 

//  
//　ROM CRCの確認
//　リンカーが出力した ROM CRC値と、マイコンのCRC 演算機能（汎用CRC）で計算した値が一致するか確認する。
//
//  リンカー出力のCRC値は、 rom_crc（アドレス:0x07dfe) に格納されている。
//　 CRCの計算範囲は、.textと .textfの領域
//
//  出力:
//   一致の場合 　 rom_crc_16_err = 0x00
//   不一致の場合　rom_crc_16_err = 0x01
//
// 注: デバック時にソフトウェアのブレークを入れると、コードがかわるので、CRCは一致しない。
//      ブレークしたい場合は、ハードウェアブレークを入れる。

#pragma inline_asm  rom_crc_chk

void rom_crc_chk(void)
{
	
	CLRW	AX		; AX = 0
	MOVW	!CRCD,AX	; CRCDレジスタ(0xF02FA)を初期化 (拡張SFRの場合 !が必要)
	
	MOVW	HL,#LOWW(STARTOF(.text))    ; .textの先頭アドレス
	
	MOVW	AX,#LOWW(SIZEOF(.text))     ; .text のサイズ
	MOVW	BC,#LOWW(SIZEOF(.textf) )   ; .textf のサイズ
	ADDW    AX,BC			    ; AX = .text + .textf のサイズ
	MOVW    BC,AX			    ; BC =     :	

	MOV	ES,#0x00	;  ES = 00
	
CAL_CRC_LOOP:

	MOV	A,ES:[HL]	; ROM上のデータ(コード)読み出し
	
	MOV	CRCIN,A		; CRC 入力レジスタ CRCINレジスタにデータ入力	
	
	INCW	HL		; 読み出しアドレス インクリメント
	DECW	BC		; バイト数 デクリメント
	MOVW    AX,BC		; AX ← BC
	CMPW    AX,#0x0000	;　AX=0ならば Z=1　
	
	SKZ			; AX=0(Z=1)ならば、次の命令をスキップ, AX>0ならば、BRへ

	BR       $CAL_CRC_LOOP  ; 
	
	NOP			;  1クロック・ウェイト
	
	MOVW	AX,!CRCD	; CRC演算結果の読み出し
		
	MOVW    !LOWW(_rom_cal_crc),AX  ; CRC演算結果の格納
	
	MOVW	DE,ES:!LOWW(_rom_crc)   ; DE = リンカー出力 CRC値
	
	ONEB	!LOWW(_rom_crc_16_err)  ; rom_crc_16_err = 1 
	
	CMPW    AX,DE			; AX(CRC演算結果)とDE(リンカー出力 CRC値)の比較
	SKNZ				; 不一致(Z=1)ならば、次の命令をスキップ								
	CLRB	!LOWW(_rom_crc_16_err)  ; rom_crc_16_err = 0 
	
	
}

// sfrのシンボルは後方参照となるため、iodefine.h は、インラインアセンブラ記述関数の後に置く
#include   "iodefine.h"
