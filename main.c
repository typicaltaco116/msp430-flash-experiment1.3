/***************************************************************
* FILENAME: main.c
* AUTHOR: Jack Pyburn
* DATE: 10/24/2025
* STATUS: 
*        
* DESCRIPTION:
*  - Gathers statistics between each 200k Program Erase cycles
*     till 2M Program Erase cycles
*  - incorrect_bit_count is the number of bits in the segment that are not the
*     expected value.
*  - unstable_bit_count is the number of bits in the segment that changed
*     atleast once in 11 reads.
*  - partial_write_latency is the minimum successful write time of the first
*     word in a segment
*  - if partial_write_latency or partial_erase_latency return the value 0xFFFF
*     the erase or write operation could not be completed in even the maximum
*     alotted time. 
*
*  - VERSION 1.3: Add bit status counting for both write and erase
*  - Fix awful stress progress displaying
*  - Increase clock speed to 8MHz
****************************************************************/
#include <msp430.h> 
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "src/flash_operations.h"
#include "src/flash_statistics.h"
#include "src/Serial.h"
#include "src/unified_clock_system.h"
#include "src/event_timer.h"


#define PRINT_CSV


/* FLASH BANK D starts at 0x1_C400 and ends at 0x2_43FF */
#define F5529_FLASH_BANK_D      0x1C400
#define CHIP_ID_ADR             0x1A0A

#define TOTAL_PE_CYCLES         2E6 
#define STAT_INCREMENT_CYCLES   200000 // number of PE cycles to stress between stats
#define STRESS_INDICATOR_CYCLES 25000

#define ENDL "\r\n"
#define BUF_SIZE                64

void haltUntilInput(void);
uint64_t get_chipID(void);

void doRoutineStatistics(f_bank_t, char*);
void doRoutineStatisticsCSV(f_bank_t, char*);


int main(void)
{
  char outputBuffer[BUF_SIZE];
  f_bank_t bank_D = (f_bank_t)F5529_FLASH_BANK_D;

  WDTCTL = WDTPW + WDTHOLD;	// stop watchdog timer

  // CONSIDER REMOVAL
  haltUntilInput(); // holds program until user presses KEY1

  UCS_setMCLK_8MHz();

  Serial0_setup();

  /* PRINT HEADER */
  Serial0_write("----------------------------------------------------------------------------------------------------" ENDL);
  Serial0_write("- Experiment 01.3" ENDL);
  snprintf(outputBuffer, BUF_SIZE, "- Subject Chip ID: 0x%08llX" ENDL, get_chipID());
  Serial0_write(outputBuffer);
  Serial0_write("----------------------------------------------------------------------------------------------------" ENDL);


  /* INITIAL STATISTICS */
  // print out number of cycles
  Serial0_write(ENDL "Cycle count: 0" ENDL ENDL);

  // RESET ENTIRE BANK, INITIAL STATISTICS
  f_bankStress(bank_D, 0x0000, 1);

  // PERFORM INITIAL STATISTICS
  #ifdef PRINT_CSV
  doRoutineStatisticsCSV(bank_D, outputBuffer);
  #else
  doRoutineStatistics(bank_D, outputBuffer);
  #endif

  Serial0_write("----------------------------------------------------------------------------------------------------" ENDL);

  /* MAIN LOOP */
  for(uint32_t i = 0; i < TOTAL_PE_CYCLES / STAT_INCREMENT_CYCLES; i++){

    Serial0_write("----------------------------------------------------------------------------------------------------" ENDL);
    snprintf(outputBuffer, BUF_SIZE, ENDL "Cycle count: %lu" ENDL ENDL, (uint32_t)((i) * STAT_INCREMENT_CYCLES));
    Serial0_write(outputBuffer);
    Serial0_write("----------------------------------------------------------------------------------------------------" ENDL);

    // STRESS BANK BY STAT_INCREMENT (200K)
    for(uint32_t s = 0; s < STAT_INCREMENT_CYCLES/STRESS_INDICATOR_CYCLES; s++){

      // PERFORM STRESS_INDICATOR_CYCLES (25k) STRESS CYCLES
      f_bankStress(bank_D, 0x0000, STRESS_INDICATOR_CYCLES);
      /* 0x0000 indicates 100% flash bit wear
         f_stress_bank will return with all words written to 0x0000 */

      // DISPLAY STRESSING PROGRESS IN 25K INCREMENTS
      snprintf(outputBuffer, BUF_SIZE, ENDL "STRESSING BANK PROGRESS : %lu" ENDL,
              (uint32_t)(i * STAT_INCREMENT_CYCLES + (s + 1) * STRESS_INDICATOR_CYCLES));
      Serial0_write(outputBuffer);
    }

    // DISPLAY PROGRESS SO FAR
    Serial0_write("----------------------------------------------------------------------------------------------------" ENDL);
    snprintf(outputBuffer, BUF_SIZE, ENDL "Cycle count: %lu" ENDL ENDL, (uint32_t)((i + 1) * STAT_INCREMENT_CYCLES));
    Serial0_write(outputBuffer);
    Serial0_write("----------------------------------------------------------------------------------------------------" ENDL);

    // PERFORM STATISTICS FOR ENTIRE BANK
    #ifdef PRINT_CSV
    doRoutineStatisticsCSV(bank_D, outputBuffer);
    #else
    doRoutineStatistics(bank_D, outputBuffer);
    #endif
  }

  Serial0_write(" DONE..." ENDL ENDL);

  return 0;

}

void doRoutineStatistics(f_bank_t bankPtr, char* charBuffer)
{
  f_segment_t seg;
  fs_stats_s stats;
  f_segment_t bottom_segment, middle_segment, top_segment;
  float segEraseTime1, segEraseTime2, segEraseTime3;
  float wordWriteTime1, wordWriteTime2, wordWriteTime3;

  // SET POINTER BACK TO BASE SEGMENT
  seg = (f_segment_t)bankPtr; 
  bottom_segment = (f_segment_t)bankPtr + 0;
  middle_segment = (f_segment_t)bankPtr + 31;
  top_segment = (f_segment_t)bankPtr + 63;

  Serial0_write("--------------------------------------------------------------" ENDL);
  Serial0_write("  Segment Bit Errors" ENDL);
  Serial0_write("--------------------------------------------------------------" ENDL);

  // STATS FOR EACH SEGMENT IN BANK
  for(uint16_t s = 0 ; s < F_BANK_N_SEGMENTS; s++){
    // DISPLAY SEGMENT HEADER
    snprintf(charBuffer, BUF_SIZE, "Segment #%02u (0x%p) Statistics" ENDL, s, seg);
    Serial0_write(charBuffer);

    // CHECK EACH BIT AFTER LAST WRITE
    fs_checkBits(seg, &stats, 0x0000); // ~4 seconds!
    snprintf(charBuffer, BUF_SIZE, "    Write incorrect bit count   : %u" ENDL,
             stats.incorrect_bit_count);
    Serial0_write(charBuffer);
    snprintf(charBuffer, BUF_SIZE, "    Write unstable bit count    : %u" ENDL,
             stats.unstable_bit_count);
    Serial0_write(charBuffer);

    f_segmentErase(seg);

    // CHECK EACH VALUE AFTER ERASE OPERATION
    fs_checkBits(seg, &stats, 0xFFFF);
    snprintf(charBuffer, BUF_SIZE, "    Erase incorrect bit count   : %u" ENDL, stats.incorrect_bit_count);
    Serial0_write(charBuffer);
    snprintf(charBuffer, BUF_SIZE, "    Erase unstable bit count    : %u" ENDL, stats.unstable_bit_count);
    Serial0_write(charBuffer);

    seg++;
  }

  segEraseTime1 = segEraseTime2 = segEraseTime3 = 0;
  wordWriteTime1 = wordWriteTime2 = wordWriteTime3 = 0;

  // GET AVERAGE TIME ERASES AND WRITES
  for(int i = 0; i < 3; i++){
    // SEGMENT ERASES
    f_segmentEraseTimed(bottom_segment);
    segEraseTime1 += (float)_event_timer_value * SLOW_EVENT_TIMER_USEC_FLT;

    f_segmentEraseTimed(middle_segment);
    segEraseTime2 += (float)_event_timer_value * SLOW_EVENT_TIMER_USEC_FLT;

    f_segmentEraseTimed(top_segment);
    segEraseTime3 += (float)_event_timer_value * SLOW_EVENT_TIMER_USEC_FLT;

    // WORD WRITES
    f_wordWriteTimed(0x0000, (uint16_t*)bottom_segment);
    wordWriteTime1 += (float)_event_timer_value * EVENT_TIMER_USEC_FLT;

    f_wordWriteTimed(0x0000, (uint16_t*)(middle_segment));
    wordWriteTime2 += (float)_event_timer_value * EVENT_TIMER_USEC_FLT;

    f_wordWriteTimed(0x0000, (uint16_t*)(top_segment));
    wordWriteTime3 += (float)_event_timer_value * EVENT_TIMER_USEC_FLT;
  }

  segEraseTime1 /= 3.0;
  segEraseTime2 /= 3.0;
  segEraseTime3 /= 3.0;
  wordWriteTime1 /= 3.0;
  wordWriteTime2 /= 3.0;
  wordWriteTime3 /= 3.0;

  Serial0_write("--------------------------------------------------------------" ENDL);
  Serial0_write("  Nominal flash operation times" ENDL);
  Serial0_write("--------------------------------------------------------------" ENDL);

  snprintf(charBuffer, BUF_SIZE, "Segment %p nominal erase time = %.3f us" ENDL, bottom_segment, segEraseTime1);
  Serial0_write(charBuffer);
  snprintf(charBuffer, BUF_SIZE, "Segment %p nominal erase time = %.3f us" ENDL, middle_segment, segEraseTime2);
  Serial0_write(charBuffer);
  snprintf(charBuffer, BUF_SIZE, "Segment %p nominal erase time = %.3f us" ENDL, top_segment, segEraseTime3);
  Serial0_write(charBuffer);
  snprintf(charBuffer, BUF_SIZE, "Word %p nominal write time = %.3f us" ENDL, bottom_segment, wordWriteTime1);
  Serial0_write(charBuffer);
  snprintf(charBuffer, BUF_SIZE, "Word %p nominal write time = %.3f us" ENDL, middle_segment, wordWriteTime2);
  Serial0_write(charBuffer);
  snprintf(charBuffer, BUF_SIZE, "Word %p nominal write time = %.3f us" ENDL, top_segment, wordWriteTime3);
  Serial0_write(charBuffer);


}

void doRoutineStatisticsCSV(f_bank_t bankPtr, char* charBuffer)
{
  f_segment_t seg;
  fs_stats_s stats;
  f_segment_t bottom_segment, middle_segment, top_segment;
  float segEraseTime1, segEraseTime2, segEraseTime3;
  float wordWriteTime1, wordWriteTime2, wordWriteTime3;

  // SET POINTER BACK TO BASE SEGMENT
  seg = (f_segment_t)bankPtr; 
  bottom_segment = (f_segment_t)bankPtr + 0;
  middle_segment = (f_segment_t)bankPtr + 31;
  top_segment = (f_segment_t)bankPtr + 63;

  Serial0_write("----------------------------------------------------------------------------------------------------" ENDL);
  Serial0_write("  Segment Bit Errors (CSV)" ENDL);
  Serial0_write("----------------------------------------------------------------------------------------------------" ENDL);
  Serial0_write("Segment #N, Program Incorrect Bits, Program Unstable Bits, Erase Incorrect Bits, Erase Unstable Bits" ENDL);

  // STATS FOR EACH SEGMENT IN BANK
  for(uint16_t s = 0 ; s < F_BANK_N_SEGMENTS; s++){

    // OUTPUT SEGMENT INDEX
    snprintf(charBuffer, BUF_SIZE, "%02u (0x%p),", s, seg);
    Serial0_write(charBuffer);
    // CHECK EACH BIT AFTER LAST WRITE
    fs_checkBits(seg, &stats, 0x0000); // ~4 seconds!
    snprintf(charBuffer, BUF_SIZE, " %u,",
             stats.incorrect_bit_count);
    Serial0_write(charBuffer);
    snprintf(charBuffer, BUF_SIZE, " %u,",
             stats.unstable_bit_count);
    Serial0_write(charBuffer);

    f_segmentErase(seg);

    // CHECK EACH VALUE AFTER ERASE OPERATION
    fs_checkBits(seg, &stats, 0xFFFF);
    snprintf(charBuffer, BUF_SIZE, " %u,", stats.incorrect_bit_count);
    Serial0_write(charBuffer);
    snprintf(charBuffer, BUF_SIZE, " %u" ENDL, stats.unstable_bit_count);
    Serial0_write(charBuffer);

    seg++;
  }

  segEraseTime1 = segEraseTime2 = segEraseTime3 = 0;
  wordWriteTime1 = wordWriteTime2 = wordWriteTime3 = 0;

  // GET AVERAGE TIME ERASES AND WRITES
  for(int i = 0; i < 3; i++){
    // SEGMENT ERASES
    f_segmentEraseTimed(bottom_segment);
    segEraseTime1 += (float)_event_timer_value * SLOW_EVENT_TIMER_USEC_FLT;

    f_segmentEraseTimed(middle_segment);
    segEraseTime2 += (float)_event_timer_value * SLOW_EVENT_TIMER_USEC_FLT;

    f_segmentEraseTimed(top_segment);
    segEraseTime3 += (float)_event_timer_value * SLOW_EVENT_TIMER_USEC_FLT;

    // WORD WRITES
    f_wordWriteTimed(0x0000, (uint16_t*)bottom_segment);
    wordWriteTime1 += (float)_event_timer_value * EVENT_TIMER_USEC_FLT;

    f_wordWriteTimed(0x0000, (uint16_t*)(middle_segment));
    wordWriteTime2 += (float)_event_timer_value * EVENT_TIMER_USEC_FLT;

    f_wordWriteTimed(0x0000, (uint16_t*)(top_segment));
    wordWriteTime3 += (float)_event_timer_value * EVENT_TIMER_USEC_FLT;
  }

  segEraseTime1 /= 3.0;
  segEraseTime2 /= 3.0;
  segEraseTime3 /= 3.0;
  wordWriteTime1 /= 3.0;
  wordWriteTime2 /= 3.0;
  wordWriteTime3 /= 3.0;

  Serial0_write("----------------------------------------------------------------------------------------------------" ENDL);
  Serial0_write("  Nominal flash operation times (CSV)" ENDL);
  Serial0_write("----------------------------------------------------------------------------------------------------" ENDL);

  snprintf(charBuffer, BUF_SIZE, "%.3f,", segEraseTime1);
  Serial0_write(charBuffer);
  snprintf(charBuffer, BUF_SIZE, " %.3f,", segEraseTime2);
  Serial0_write(charBuffer);
  snprintf(charBuffer, BUF_SIZE, " %.3f,", segEraseTime3);
  Serial0_write(charBuffer);
  snprintf(charBuffer, BUF_SIZE, " %.3f,", wordWriteTime1);
  Serial0_write(charBuffer);
  snprintf(charBuffer, BUF_SIZE, " %.3f,", wordWriteTime2);
  Serial0_write(charBuffer);
  snprintf(charBuffer, BUF_SIZE, " %.3f" ENDL, wordWriteTime3);
  Serial0_write(charBuffer);


}

void haltUntilInput(void)
{
  P1REN |= BIT1;
  P1DIR |= BIT0;
  P1OUT |= BIT1 + BIT0;
  while(P1IN & BIT1);// wait till button press to start
  P1OUT &= ~BIT0;
}

uint64_t get_chipID(void)
{
  return *(uint64_t*)CHIP_ID_ADR;
}
