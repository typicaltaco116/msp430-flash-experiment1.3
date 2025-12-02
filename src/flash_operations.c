#include "flash_operations.h"
#include <msp430.h>
#include <stdint.h>
#include "event_timer.h"
#include "SRAM_subroutine_copy.h"
#include "Serial.h"

#define BANK_SEGMENT_SIZE 512
#define INFO_SEGMENT_SIZE 128

void f_segmentErase(f_segment_t segPtr)
{
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + ERASE; // enable segment erase
  *(uint16_t*)segPtr = 0x0000; // dummy write to initiate erase

  while(FCTL3 & BUSY); // loop while busy
  // not really necessary when executing from flash

  FCTL1 = FWPW; // clear ERASE
  FCTL3 = FWPW + LOCK; // lock
}


void f_segmentEraseTimed(f_segment_t segPtr)
{
  SLOW_EVENT_TIMER_START; // defined in event_timer.h
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + ERASE; // enable segment erase
  *(uint16_t*)segPtr = 0x0000; // dummy write to initiate erase

  while(FCTL3 & BUSY); // loop while busy
  // not really necessary when executing from flash

  FCTL1 = FWPW; // clear ERASE
  FCTL3 = FWPW + LOCK; // lock
  EVENT_TIMER_STOP;
}

void f_bankErase(f_bank_t bankPtr)
{
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + MERAS; // enable bank erase
  *(uint16_t*)bankPtr = 0x0000; // dummy write to initiate erase

  while(FCTL3 & BUSY); // loop while busy
  // not really necessary when executing from flash

  FCTL1 = FWPW; // clear MERASE
  FCTL3 = FWPW + LOCK; // lock
}

void f_bankEraseTimed(f_bank_t bankPtr)
{
  EVENT_TIMER_START;
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + MERAS; // enable bank erase
  *(uint16_t*)bankPtr = 0x0000; // dummy write to initiate erase

  while(FCTL3 & BUSY); // loop while busy
  // not really necessary when executing from flash

  FCTL1 = FWPW; // clear MERASE
  FCTL3 = FWPW + LOCK; // lock
  EVENT_TIMER_STOP;
}

void f_wordWrite(uint16_t value, uint16_t* targetPtr)
{
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = value; // write value

  while(FCTL3 & BUSY);

  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
}

void f_wordWriteTimed(uint16_t value, uint16_t* targetPtr)
{
  EVENT_TIMER_START;
  while(FCTL3 & BUSY);


  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = value; // write value

  while(FCTL3 & BUSY);

  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
  EVENT_TIMER_STOP;
}


void f_segmentWrite(uint16_t value, uint16_t* blockPtr)
// magic numbers for a block size of 512
// Only a 128 byte row can be written at once
// must be executed from RAM
{
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; // clear lock

  for(uint8_t s = 4; s != 0; s--){
    FCTL1 = FWPW + WRT + BLKWRT;

    for(int i = 32; i != 0; i--){
      *blockPtr++ = value;
      *blockPtr++ = value;
      while(!(FCTL3 & WAIT));
    }

    FCTL1 = FWPW + WRT; // clear BLKWRT
    while(FCTL3 & BUSY);
  }

  FCTL1 = FWPW; // clear BLKWRT and WRT
  FCTL3 = FWPW + LOCK; // lock
}
void end_f_segmentWrite(void) {}


void f_segmentStress(f_segment_t seg, uint16_t val, uint32_t iterations)
// Since erasing flash forces all bits high, a value of 0x0000 will result in
//    the highest possible stresssing of all bits.
// LEAVES A SEGMENT WITH THE VALUE OF VAL IN EVERY WORD
{
  void (*SRAM_f_block_set)(uint16_t, uint16_t*); // declare function pointer

  SRAM_f_block_set = malloc_subroutine(f_segmentWrite, end_f_segmentWrite);
  if(!(void*)SRAM_f_block_set)
    return; // null pointer means the memory cannot be allocated

  for (uint32_t i = iterations; i != 0; i--){
    f_segmentErase(seg);

    SRAM_f_block_set(val, (uint16_t*)seg);
  }

  free((void*)SRAM_f_block_set); // deallocate memory
}

void f_bankStress(f_bank_t bank, uint16_t val, uint32_t iterations)
{
  f_segment_t target;
  void (*SRAM_f_block_set)(uint16_t, uint16_t*); // declare function pointer

  SRAM_f_block_set = malloc_subroutine(f_segmentWrite, end_f_segmentWrite);
  if(!(void*)SRAM_f_block_set)
    return; // null pointer means the memory cannot be allocated


  while(iterations){
    f_bankErase(bank);
    target = (f_segment_t)bank;

    for(uint8_t s = F_BANK_N_SEGMENTS; s != 0; s--) // set all segments in the bank
      SRAM_f_block_set(val, (uint16_t*)(target++));

    iterations--;
  }

  free(SRAM_f_block_set); // deallocate memory
}
