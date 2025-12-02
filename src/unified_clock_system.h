#include <msp430.h>

typedef enum{
  UCS_1MHZ,
  UCS_8MHZ,
  UCS_16MHZ,
} UCS_freq_e;

void UCS_setMCLK_16MHz(void);

void UCS_setMCLK_8MHz(void);

// UNTESTED !!!!!
void UCS_setMCLK_1MHz(void);

UCS_freq_e UCS_getMCLKFreq(void);
