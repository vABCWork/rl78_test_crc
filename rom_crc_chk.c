
#include   "misratypes.h"

#pragma address rom_crc = 0x03dfe	// rom_crc�̊i�[�A�h���X = 03DFE H
const uint16_t __far rom_crc = 0xbd98;  // �����J�[�o�� CRC�̒l (.text �� .textf�̗̈�)


uint8_t  rom_crc_16_err;       // ROM��CRC��v(0)/�s��v(1)   (�����l�Ȃ��O���[�o���ϐ�)
uint16_t rom_cal_crc;	       // �}�C�R���Ōv�Z���� CRC 

//  
//�@ROM CRC�̊m�F
//�@�����J�[���o�͂��� ROM CRC�l�ƁA�}�C�R����CRC ���Z�@�\�i�ėpCRC�j�Ōv�Z�����l����v���邩�m�F����B
//
//  �����J�[�o�͂�CRC�l�́A rom_crc�i�A�h���X:0x07dfe) �Ɋi�[����Ă���B
//�@ CRC�̌v�Z�͈͂́A.text�� .textf�̗̈�
//
//  �o��:
//   ��v�̏ꍇ �@ rom_crc_16_err = 0x00
//   �s��v�̏ꍇ�@rom_crc_16_err = 0x01
//
// ��: �f�o�b�N���Ƀ\�t�g�E�F�A�̃u���[�N������ƁA�R�[�h�������̂ŁACRC�͈�v���Ȃ��B
//      �u���[�N�������ꍇ�́A�n�[�h�E�F�A�u���[�N������B

#pragma inline_asm  rom_crc_chk

void rom_crc_chk(void)
{
	
	CLRW	AX		; AX = 0
	MOVW	!CRCD,AX	; CRCD���W�X�^(0xF02FA)�������� (�g��SFR�̏ꍇ !���K�v)
	
	MOVW	HL,#LOWW(STARTOF(.text))    ; .text�̐擪�A�h���X
	
	MOVW	AX,#LOWW(SIZEOF(.text))     ; .text �̃T�C�Y
	MOVW	BC,#LOWW(SIZEOF(.textf) )   ; .textf �̃T�C�Y
	ADDW    AX,BC			    ; AX = .text + .textf �̃T�C�Y
	MOVW    BC,AX			    ; BC =     :	

	MOV	ES,#0x00	;  ES = 00
	
CAL_CRC_LOOP:

	MOV	A,ES:[HL]	; ROM��̃f�[�^(�R�[�h)�ǂݏo��
	
	MOV	CRCIN,A		; CRC ���̓��W�X�^ CRCIN���W�X�^�Ƀf�[�^����	
	
	INCW	HL		; �ǂݏo���A�h���X �C���N�������g
	DECW	BC		; �o�C�g�� �f�N�������g
	MOVW    AX,BC		; AX �� BC
	CMPW    AX,#0x0000	;�@AX=0�Ȃ�� Z=1�@
	
	SKZ			; AX=0(Z=1)�Ȃ�΁A���̖��߂��X�L�b�v, AX>0�Ȃ�΁ABR��

	BR       $CAL_CRC_LOOP  ; 
	
	NOP			;  1�N���b�N�E�E�F�C�g
	
	MOVW	AX,!CRCD	; CRC���Z���ʂ̓ǂݏo��
		
	MOVW    !LOWW(_rom_cal_crc),AX  ; CRC���Z���ʂ̊i�[
	
	MOVW	DE,ES:!LOWW(_rom_crc)   ; DE = �����J�[�o�� CRC�l
	
	ONEB	!LOWW(_rom_crc_16_err)  ; rom_crc_16_err = 1 
	
	CMPW    AX,DE			; AX(CRC���Z����)��DE(�����J�[�o�� CRC�l)�̔�r
	SKNZ				; �s��v(Z=1)�Ȃ�΁A���̖��߂��X�L�b�v								
	CLRB	!LOWW(_rom_crc_16_err)  ; rom_crc_16_err = 0 
	
	
}

// sfr�̃V���{���͌���Q�ƂƂȂ邽�߁Aiodefine.h �́A�C�����C���A�Z���u���L�q�֐��̌�ɒu��
#include   "iodefine.h"
