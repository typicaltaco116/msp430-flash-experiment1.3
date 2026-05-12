# MSP430-flash-experiment1.3

## Experiment Description
Hardware: MSP430F5529 Launchpad (MSP-EXP430F5529LP)
Description:
 - Gathers statistics between each 200k Program Erase cycles
    till 2M Program Erase cycles
 - incorrect_bit_count is the number of bits in the segment that are not the
    expected value.
 - unstable_bit_count is the number of bits in the segment that changed
    atleast once in 11 reads.
 - partial_write_latency is the minimum successful write time of the first
    word in a segment
 - if partial_write_latency or partial_erase_latency return the value 0xFFFF
    the erase or write operation could not be completed in even the maximum
    alotted time. 

 VERSION 1.3:
 - Add bit status counting for both write and erase
 - Fix awful stress progress displaying
 - Increase clock speed to 8MHz

## Intended Build Instructions
This repository contains code intended for use with the Code Composer Studio (CCS) IDE. The required .ccsproject, .cproject, .project, and .settings files and directories are not inlcluded in this repository and it is expected that the user creates an empty CCS C project and copies the repo contents into the project directory.
Note: The makefile will likely be overwritten by the CCS IDE build system.
