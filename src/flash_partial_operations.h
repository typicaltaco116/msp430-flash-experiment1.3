#include <msp430.h>
#include <stdint.h>

#include "flash_operations.h"

#pragma CODE_SECTION(f_wordPartialWrite_1, ".f_wordPartialWrite_1")
#pragma CODE_SECTION(end_f_wordPartialWrite_1, ".f_wordPartialWrite_1")
#pragma CODE_SECTION(f_wordPartialWrite_2, ".f_wordPartialWrite_2")
#pragma CODE_SECTION(end_f_wordPartialWrite_2, ".f_wordPartialWrite_2")
#pragma CODE_SECTION(f_wordPartialWrite_3, ".f_wordPartialWrite_3")
#pragma CODE_SECTION(end_f_wordPartialWrite_3, ".f_wordPartialWrite_3")
#pragma CODE_SECTION(f_wordPartialWrite_4, ".f_wordPartialWrite_4")
#pragma CODE_SECTION(end_f_wordPartialWrite_4, ".f_wordPartialWrite_4")
#pragma CODE_SECTION(f_wordPartialWrite_5, ".f_wordPartialWrite_5")
#pragma CODE_SECTION(end_f_wordPartialWrite_5, ".f_wordPartialWrite_5")
#pragma CODE_SECTION(f_wordPartialWrite_6, ".f_wordPartialWrite_6")
#pragma CODE_SECTION(end_f_wordPartialWrite_6, ".f_wordPartialWrite_6")
#pragma CODE_SECTION(f_wordPartialWrite_7, ".f_wordPartialWrite_7")
#pragma CODE_SECTION(end_f_wordPartialWrite_7, ".f_wordPartialWrite_7")
#pragma CODE_SECTION(f_wordPartialWrite_8, ".f_wordPartialWrite_8")
#pragma CODE_SECTION(end_f_wordPartialWrite_8, ".f_wordPartialWrite_8")
#pragma CODE_SECTION(f_wordPartialWrite_9, ".f_wordPartialWrite_9")
#pragma CODE_SECTION(end_f_wordPartialWrite_9, ".f_wordPartialWrite_9")
#pragma CODE_SECTION(f_wordPartialWrite_10, ".f_wordPartialWrite_10")
#pragma CODE_SECTION(end_f_wordPartialWrite_10, ".f_wordPartialWrite_10")

#pragma CODE_SECTION(f_segmentPartialErase_x, ".f_segmentPartialErase_x")
#pragma CODE_SECTION(end_f_segmentPartialErase_x, ".f_segmentPartialErase_x")

/******************************************************************************
                        Partial Write NOP count char
*******************************************************************************
  1.)   8
  2.)   9
  3.)   10
  4.)   11
  5.)   107
  6.)   108
  7.)   109
  8.)   110
  9.)   111
  10.)  112
******************************************************************************/

extern uint16_t f_partialWriteNOPCountLUT[10];

void __attribute__((noinline)) f_wordPartialWrite_1(uint16_t value, uint16_t* targetPtr);
void __attribute__((noinline)) end_f_wordPartialWrite_1(void);

void __attribute__((noinline)) f_wordPartialWrite_2(uint16_t value, uint16_t* targetPtr);
void __attribute__((noinline)) end_f_wordPartialWrite_2(void);

void __attribute__((noinline)) f_wordPartialWrite_3(uint16_t value, uint16_t* targetPtr);
void __attribute__((noinline)) end_f_wordPartialWrite_3(void);

void __attribute__((noinline)) f_wordPartialWrite_4(uint16_t value, uint16_t* targetPtr);
void __attribute__((noinline)) end_f_wordPartialWrite_4(void);

void __attribute__((noinline)) f_wordPartialWrite_5(uint16_t partialValue, uint16_t* targetPtr);
void __attribute__((noinline)) end_f_wordPartialWrite_5(void);

void __attribute__((noinline)) f_wordPartialWrite_6(uint16_t partialValue, uint16_t* targetPtr);
void __attribute__((noinline)) end_f_wordPartialWrite_6(void);

void __attribute__((noinline)) f_wordPartialWrite_7(uint16_t partialValue, uint16_t* targetPtr);
void __attribute__((noinline)) end_f_wordPartialWrite_7(void);

void __attribute__((noinline)) f_wordPartialWrite_8(uint16_t partialValue, uint16_t* targetPtr);
void __attribute__((noinline)) end_f_wordPartialWrite_8(void);

void __attribute__((noinline)) f_wordPartialWrite_9(uint16_t partialValue, uint16_t* targetPtr);
void __attribute__((noinline)) end_f_wordPartialWrite_9(void);

void __attribute__((noinline)) f_wordPartialWrite_10(uint16_t partialValue, uint16_t* targetPtr);
void __attribute__((noinline)) end_f_wordPartialWrite_10(void);

void __attribute__((noinline)) f_segmentPartialErase_x(f_segment_t targetPtr, uint16_t x);
void __attribute__((noinline)) end_f_segmentPartialErase_x(void);

void f_segmentPartialWrite_x(f_segment_t target, uint16_t partialValue, uint16_t x);
// NOTE: the x denotes the chosen partial write above. It does not indicate the
// exact number of clock cycles. See clock cycle chart at the top of this file.
