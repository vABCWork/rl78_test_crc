#include   "iodefine.h"
#include   "misratypes.h"
#include   "rom_crc_chk.h"

#define ALM_LED_PORT_MODE   PM1_bit.no3      // P13: ALM LED�p 
#define ALM_LED_OUT         P1_bit.no3       

uint8_t  test_byte_1 = 0x01;  // �����l�t���ϐ�
uint16_t test_word_1 = 0xABCD;

const uint8_t test_const_byte_1 = 0x02;   // �萔
const uint16_t test_const_word_1 =0x55AA;

// ���C������
// 
void main(void)
{
	ALM_LED_PORT_MODE = 0;	// �o�̓��[�h
	ALM_LED_OUT = 0;	// �o��=0, ALM LED=����
	
	rom_crc_chk();		// CRC�̊m�F  rom_crc_16_err = 0x00(CRC��v)
	
	if ( rom_crc_16_err == 1 ) {  // ROM CRC �s��v
	   ALM_LED_OUT = 1;	      // �o��=1, ALM LED=�_��
	}
	
	while(1){
	}
	
}


