#include "flash_statistics.h"

#include <msp430.h>
#include <stdint.h>
#include "flash_operations.h"
#include "flash_partial_operations.h"
#include "event_timer.h"
#include "SRAM_subroutine_copy.h"


#define STAT_READ_COUNT 11

void fs_checkBits(f_segment_t seg, fs_stats_s* stats, uint16_t expected_val)
// majority based voting
// position of bits not set correctly
{
  uint16_t word_bin;
  uint16_t differences;
  uint16_t* read_head = (uint16_t*)seg;
  uint8_t voted_bit;

  stats->incorrect_bit_count = 0;
  stats->unstable_bit_count = 0;
  
  // Iterate through entire segment
  while(read_head < (uint16_t*)(seg + 1)){

    uint16_t bit_votes[16] = {0};
    uint16_t difference_votes[16] = {0};


    // Perform repeated word reads
    for (uint8_t i = 0; i < STAT_READ_COUNT; i++){

      word_bin = *read_head; // read
      differences = word_bin ^ *read_head; // comparison read

      // Gather votes
      for (uint8_t b = 0; b < 16; b++){
        if (word_bin & (1 << b)){
            bit_votes[b]++;
        }

        if (differences & (1 << b)){
          difference_votes[b]++;
        }
      }

    }

    // Process vote arrays
    for (uint8_t b = 0; b < 16; b++) {

      // Form voted word based on majority (6 or more)
      voted_bit =  (bit_votes[b] >= (STAT_READ_COUNT / 2 + 1));
      if (voted_bit ^ ((expected_val >> b) & 1)) // ISSUE IN ORIGINAL CODE
        stats->incorrect_bit_count++;

      // Increment unstable bits if changed atleast once
      if (difference_votes[b] > 0)
        stats->unstable_bit_count++;
    }

    // increment to next word in segment
    read_head++;
  }

}

/*
void fs_getPartialWriteStats(f_segment_t target, fs_stats_s* stats, uint16_t val)
{
  void (*SRAM_p_write)(uint16_t, uint16_t*); // declare function pointer 

  SRAM_p_write = malloc_subroutine(f_wordPartialWrite_10, end_f_wordPartialWrite_10);
  SRAM_p_write(val, target);
  free(SRAM_p_write);
  if (*target ^ val){
    stats->partial_write_latency = FS_PARTIAL_WRITE_FAIL;
    return;
  }
  stats->partial_write_latency = _event_timer_value;
  stats->partial_nop_count = f_partialWriteNOPCountLUT[9];

  SRAM_p_write = malloc_subroutine(f_wordPartialWrite_9, end_f_wordPartialWrite_9);
  SRAM_p_write(val, target);
  free(SRAM_p_write);
  if (*target ^ val){
    return;
  }
  stats->partial_write_latency = _event_timer_value;
  stats->partial_nop_count = f_partialWriteNOPCountLUT[8];
  
  SRAM_p_write = malloc_subroutine(f_wordPartialWrite_8, end_f_wordPartialWrite_8);
  SRAM_p_write(val, target);
  free(SRAM_p_write);
  if (*target ^ val){
    return;
  }
  stats->partial_write_latency = _event_timer_value;
  stats->partial_nop_count = f_partialWriteNOPCountLUT[7];
  
  SRAM_p_write = malloc_subroutine(f_wordPartialWrite_7, end_f_wordPartialWrite_7);
  SRAM_p_write(val, target);
  free(SRAM_p_write);
  if (*target ^ val){
    return;
  }
  stats->partial_write_latency = _event_timer_value;
  stats->partial_nop_count = f_partialWriteNOPCountLUT[6];
  
  SRAM_p_write = malloc_subroutine(f_wordPartialWrite_6, end_f_wordPartialWrite_6);
  SRAM_p_write(val, target);
  free(SRAM_p_write);
  if (*target ^ val){
    return;
  }
  stats->partial_write_latency = _event_timer_value;
  stats->partial_nop_count = f_partialWriteNOPCountLUT[5];
  
  SRAM_p_write = malloc_subroutine(f_wordPartialWrite_5, end_f_wordPartialWrite_5);
  SRAM_p_write(val, target);
  free(SRAM_p_write);
  if (*target ^ val){
    return;
  }
  stats->partial_write_latency = _event_timer_value;
  stats->partial_nop_count = f_partialWriteNOPCountLUT[4];
  
  SRAM_p_write = malloc_subroutine(f_wordPartialWrite_7, end_f_wordPartialWrite_7);
  SRAM_p_write(val, target);
  free(SRAM_p_write);
  if (*target ^ val){
    return;
  }
  stats->partial_write_latency = _event_timer_value;
  stats->partial_nop_count = f_partialWriteNOPCountLUT[6];
}
*/

/*
void fs_getPartialEraseStats(f_segment_t seg, fs_stats_s* stats)
{
  void (*SRAM_p_erase)(uint16_t*, uint16_t);
  
  // load this array with the delays to test partial segment operations
  const uint16_t testDelayArray[6] = \
    {14 * 1024, 16 * 1024, 18 * 1024, 20 * 1024, 22 * 1024, 24 * 1024};

  SRAM_p_erase = malloc_subroutine(f_segment_partial_erase_x, \
      end_f_segment_partial_erase_x);

  stats->partial_erase_latency = FS_PARTIAL_ERASE_FAIL;

  for (uint8_t test = sizeof(testDelayArray) / sizeof(testDelayArray[0]); \
       test != 0; test--){
    // a delay of 1024 is equivalent to 1ms
    SRAM_p_erase((uint16_t*)seg, testDelayArray[test]);
    if (*((uint16_t*)seg) != 0xFFFF){
      free(SRAM_p_erase);
      return;
    }
    stats->partial_erase_latency = _event_timer_value;
  }
  free(SRAM_p_erase);
}
*/
