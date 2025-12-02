#include "flash_partial_operations.h"

#include <msp430.h>
#include <stdint.h>

#include "flash_operations.h"
#include "event_timer.h"

#define COUNT 107

// MACROS FOR REPEATED NOPS
#define REPEAT_5(x) {x;x;x;x;x;}
#define REPEAT_10(x) {REPEAT_5(x) REPEAT_5(x)}
#define REPEAT_50(x) {REPEAT_10(x) REPEAT_10(x) REPEAT_10(x) REPEAT_10(x) REPEAT_10(x)}


void f_wordPartialWrite_1(uint16_t partialValue, uint16_t* targetPtr)
{
  EVENT_TIMER_START;
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = partialValue; // write value

  REPEAT_5( __no_operation() );
  __no_operation();
  __no_operation();
  __no_operation();

  FCTL3 = FWPW + EMEX; // emergency exit
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
  while(FCTL3 & BUSY);
  EVENT_TIMER_STOP;
}
void end_f_wordPartialWrite_1(void) {}

void f_wordPartialWrite_2(uint16_t partialValue, uint16_t* targetPtr)
{
  EVENT_TIMER_START;
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = partialValue; // write value

  REPEAT_5( __no_operation() );
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();

  FCTL3 = FWPW + EMEX; // emergency exit
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
  while(FCTL3 & BUSY);
  EVENT_TIMER_STOP;
}
void end_f_wordPartialWrite_2(void) {}

void f_wordPartialWrite_3(uint16_t partialValue, uint16_t* targetPtr)
{
  EVENT_TIMER_START;
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = partialValue; // write value

  REPEAT_5( __no_operation() );
  REPEAT_5( __no_operation() );

  FCTL3 = FWPW + EMEX; // emergency exit
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
  while(FCTL3 & BUSY);
  EVENT_TIMER_STOP;
}
void end_f_wordPartialWrite_3(void) {}

void f_wordPartialWrite_4(uint16_t partialValue, uint16_t* targetPtr)
{
  EVENT_TIMER_START;
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = partialValue; // write value

  REPEAT_5( __no_operation() );
  REPEAT_5( __no_operation() );
  __no_operation();

  FCTL3 = FWPW + EMEX; // emergency exit
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
  while(FCTL3 & BUSY);
  EVENT_TIMER_STOP;
}
void end_f_wordPartialWrite_4(void) {}

void f_wordPartialWrite_5(uint16_t partialValue, uint16_t* targetPtr)
// THIS FUNCTION MUST BE EXECUTED FROM RAM
// This function is timed!!! the value in _event_timer_value is the write time
{
  EVENT_TIMER_START;
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = partialValue; // write value

  REPEAT_50( __no_operation() );
  REPEAT_50( __no_operation() );
  REPEAT_5( __no_operation() );
  __no_operation();
  __no_operation();

  FCTL3 = FWPW + EMEX; // emergency exit
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
  while(FCTL3 & BUSY);
  EVENT_TIMER_STOP;
}
void end_f_wordPartialWrite_5(void) {}

void f_wordPartialWrite_6(uint16_t partialValue, uint16_t* targetPtr)
// emergency exit after ~10 us
{
  EVENT_TIMER_START;
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = partialValue; // write value

  REPEAT_50( __no_operation() );
  REPEAT_50( __no_operation() );
  REPEAT_5( __no_operation() );
  __no_operation();
  __no_operation();
  __no_operation();

  FCTL3 = FWPW + EMEX; // emergency exit
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
  while(FCTL3 & BUSY);
  EVENT_TIMER_STOP;
}
void end_f_wordPartialWrite_6(void) {}

void f_wordPartialWrite_7(uint16_t partialValue, uint16_t* targetPtr)
// emergency exit after ~12 us
{
  EVENT_TIMER_START;
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = partialValue; // write value

  REPEAT_50( __no_operation() );
  REPEAT_50( __no_operation() );
  REPEAT_5( __no_operation() );
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();

  FCTL3 = FWPW + EMEX; // emergency exit
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
  while(FCTL3 & BUSY);
  EVENT_TIMER_STOP;
}
void end_f_wordPartialWrite_7(void) {}

void f_wordPartialWrite_8(uint16_t partialValue, uint16_t* targetPtr)
// emergency exit after ~12 us
{
  EVENT_TIMER_START;
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = partialValue; // write value

  REPEAT_50( __no_operation() );
  REPEAT_50( __no_operation() );
  REPEAT_10( __no_operation() );

  FCTL3 = FWPW + EMEX; // emergency exit
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
  while(FCTL3 & BUSY);
  EVENT_TIMER_STOP;
}
void end_f_wordPartialWrite_8(void) {}

void f_wordPartialWrite_9(uint16_t partialValue, uint16_t* targetPtr)
// emergency exit after ~12 us
{
  EVENT_TIMER_START;
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = partialValue; // write value

  REPEAT_50( __no_operation() );
  REPEAT_50( __no_operation() );
  REPEAT_10( __no_operation() );
  __no_operation();

  FCTL3 = FWPW + EMEX; // emergency exit
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
  while(FCTL3 & BUSY);
  EVENT_TIMER_STOP;
}
void end_f_wordPartialWrite_9(void) {}

void f_wordPartialWrite_10(uint16_t partialValue, uint16_t* targetPtr)
// emergency exit after ~12 us
{
  EVENT_TIMER_START;
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = partialValue; // write value

  REPEAT_50( __no_operation() );
  REPEAT_50( __no_operation() );
  REPEAT_10( __no_operation() );
  __no_operation();
  __no_operation();

  FCTL3 = FWPW + EMEX; // emergency exit
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
  while(FCTL3 & BUSY);
  EVENT_TIMER_STOP;
}
void end_f_wordPartialWrite_10(void) {}

void f_segmentPartialErase_x(f_segment_t targetPtr, uint16_t x)
/*
  This function is the partial erase function with a timer delay
  x is the number of clock cycles to delay using the timer
  1.024 MHZ clock as input to the timer
 */
{
  TA1CTL |= TACLR;

  while(FCTL3 & BUSY);
  EVENT_TIMER_START;

  FCTL3 = FWPW;          // clear lock
  FCTL1 = FWPW + ERASE; // enable segment erase
  *(uint16_t*)targetPtr = 0x0000; // dummy write to initiate erase

  //USE TIMER TO HALT UNTIL 10MS
  TA1CTL |= TASSEL_2 + ID__1 + MC_2; // use SCLK
  while(TA1R < x);
  TA1CTL &= ~MC_3; // halt timer

  FCTL3 = FWPW + EMEX; // emergency exit
  FCTL1 = FWPW; // clear ERASE
  FCTL3 = FWPW + LOCK; // lock
  EVENT_TIMER_STOP;
}
void end_f_segmentPartialErase_x(void) {}
