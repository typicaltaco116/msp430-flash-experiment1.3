//-------------------------------------------------------------------//
// flash_operations.h
//-------------------------------------------------------------------//
// This header file contains functions for manipulating the flash
// memory on F5529 Lauchpad
// NOTES: 
// Intended for use with C11
// functions prepended with end are dummy functions for
//    determining the length of a function
// CODE_SECTION pragma is used to ensure that the functions are placed
//    sequentially
// SECTIONS MUST BE DEFINED IN LINKER COMMAND FILE
//-------------------------------------------------------------------//
#pragma once
#include <msp430.h>
#include <stdint.h>

#pragma CODE_SECTION(f_segment_partial_erase_4, ".f_segment_partial_erase_4")
#pragma CODE_SECTION(end_f_segment_partial_erase_4, ".f_segment_partial_erase_4")
#pragma CODE_SECTION(f_segment_partial_erase_x, ".f_segment_partial_erase_x")
#pragma CODE_SECTION(end_f_segment_partial_erase_x, ".f_segment_partial_erase_x")

#pragma CODE_SECTION(f_word_partial_write_7, ".f_word_partial_write_7")
#pragma CODE_SECTION(end_f_word_partial_write_7, ".f_word_partial_write_7")
#pragma CODE_SECTION(f_word_partial_write_8, ".f_word_partial_write_8")
#pragma CODE_SECTION(end_f_word_partial_write_8, ".f_word_partial_write_8")
#pragma CODE_SECTION(f_word_partial_write_9, ".f_word_partial_write_9")
#pragma CODE_SECTION(end_f_word_partial_write_9, ".f_word_partial_write_9")
#pragma CODE_SECTION(f_word_partial_write_10, ".f_word_partial_write_10")
#pragma CODE_SECTION(end_f_word_partial_write_10, ".f_word_partial_write_10")
#pragma CODE_SECTION(f_word_partial_write_11, ".f_word_partial_write_11")
#pragma CODE_SECTION(end_f_word_partial_write_11, ".f_word_partial_write_11")
#pragma CODE_SECTION(f_word_partial_write_12, ".f_word_partial_write_12")
#pragma CODE_SECTION(end_f_word_partial_write_12, ".f_word_partial_write_12")
#pragma CODE_SECTION(f_word_partial_write_13, ".f_word_partial_write_13")
#pragma CODE_SECTION(end_f_word_partial_write_13, ".f_word_partial_write_13")

#pragma CODE_SECTION(f_segmentWrite, ".f_segmentWrite")
#pragma CODE_SECTION(end_f_segmentWrite, ".f_segmentWrite")

#define F_BANK_N_SEGMENTS 64
#define F_SEGMENT_N_BYTES 512


// Both of these structures are not meant to be used as actual structures
// instead they will be used as pointers with custom increment amounts
// DO NOT INSTANTIATE ACTUAL STRUCTURE BECAUSE IT WILL TAKE UP 64K BYTES
typedef struct f_segment_struct {
  char bytesCount[512];
} *f_segment_t;

typedef struct f_bank_struct {
  f_segment_t segCount[64];
} *f_bank_t;

void f_segmentErase(f_segment_t segPtr);
void f_segmentEraseTimed(f_segment_t segPtr);

void f_bankErase(f_bank_t bankPtr);
void f_bankEraseTimed(f_bank_t bankPtr);

void f_wordWrite(uint16_t value, uint16_t* targetPtr);
void f_wordWriteTimed(uint16_t value, uint16_t* targetPtr);


void f_segmentWrite(uint16_t value, uint16_t* blockPtr);
void end_f_segmentWrite(void);

void f_segment_partial_erase_4(uint16_t* targetPtr);
void end_f_segment_partial_erase_4(void);

void f_segment_partial_erase_x(uint16_t* targetPtr, uint16_t x);
void end_f_segment_partial_erase_x(void);

void f_word_partial_write_7(uint16_t partialValue, uint16_t* targetPtr);
void end_f_word_partial_write_7(void);

void f_word_partial_write_8(uint16_t partialValue, uint16_t* targetPtr);
void end_f_word_partial_write_8(void);

void f_word_partial_write_9(uint16_t partialValue, uint16_t* targetPtr);
void end_f_word_partial_write_9(void);

void f_word_partial_write_10(uint16_t partialValue, uint16_t* targetPtr);
void end_f_word_partial_write_10(void);

void f_word_partial_write_11(uint16_t partialValue, uint16_t* targetPtr);
void end_f_word_partial_write_11(void);

void f_word_partial_write_12(uint16_t partialValue, uint16_t* targetPtr);
void end_f_word_partial_write_12(void);

void f_word_partial_write_13(uint16_t partialValue, uint16_t* targetPtr);
void end_f_word_partial_write_13(void);


void f_segmentStress(f_segment_t seg, uint16_t val, uint32_t iterations);

void f_bankStress(f_bank_t bank, uint16_t val, uint32_t iterations);

