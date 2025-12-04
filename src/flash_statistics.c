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

