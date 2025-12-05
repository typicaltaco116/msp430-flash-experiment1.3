#pragma once
#include <msp430.h>
#include <stdint.h>
#include "flash_operations.h"

//-------------------------------------------------------------------//
// flash_statistics.h
//-------------------------------------------------------------------//
// 
//-------------------------------------------------------------------//
#define FS_PARTIAL_WRITE_FAIL 0xFFFF
#define FS_PARTIAL_ERASE_FAIL 0xFFFF

typedef struct fs_stats_struct {
  uint16_t incorrect_bit_count; // bits that are not the value expected
  uint16_t unstable_bit_count; // bits that are incorrect atleast once in 11 reads
  uint16_t write_latency; // latency for a proper word write
  uint16_t erase_latency; // latency for proper segment erase
  uint16_t partial_write_latency;
  uint16_t partial_erase_latency;
  uint16_t partial_nop_count;
} fs_stats_s;

void fs_checkBits(f_segment_t seg, fs_stats_s* stats, uint16_t expected_val);
/*
  Function to get the number of incorrect bits and unstable bits in a segment

  incorrect bit - A bit in flash that differs from expected_val parameters
  unstable bit - Bit that reads differently atleast once out of STAT_READ_COUNT times
*/
