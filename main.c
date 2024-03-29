#include   "iodefine.h"
#include   "misratypes.h"
#include   "rom_crc_chk.h"

#define ALM_LED_PORT_MODE   PM1_bit.no3      // P13: ALM LED用 
#define ALM_LED_OUT         P1_bit.no3       

uint8_t  test_byte_1 = 0x01;  // 初期値付き変数
uint16_t test_word_1 = 0xABCD;

const uint8_t test_const_byte_1 = 0x02;   // 定数
const uint16_t test_const_word_1 =0x55AA;

// メイン処理
// 
void main(void)
{
	ALM_LED_PORT_MODE = 0;	// 出力モード
	ALM_LED_OUT = 0;	// 出力=0, ALM LED=消灯
	
	rom_crc_chk();		// CRCの確認  rom_crc_16_err = 0x00(CRC一致)
	
	if ( rom_crc_16_err == 1 ) {  // ROM CRC 不一致
	   ALM_LED_OUT = 1;	      // 出力=1, ALM LED=点灯
	}
	
	while(1){
	}
	
}


