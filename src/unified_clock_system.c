#include "unified_clock_system.h"

#include <msp430.h>

static UCS_freq_e _current_MCLK_freq = UCS_1MHZ;

void UCS_setMCLK_16MHz(void)
{
  PMMCTL0 = PMMPW + PMMCOREV_2;            // Increase PMMCOREV to 2
  UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
  UCSCTL4 |= SELA_2 + SELS_4;               // Set ACLK = REFO

 	__bis_SR_register(SCG0);                  // Disable the FLL control loop
 	UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
 	UCSCTL1 = DCORSEL_5;                      // Select DCO range 24MHz operation
	UCSCTL2 = FLLD_1 + 488;                   // Set DCO Multiplier for 16MHz
                                            // (N + 1) * FLLRef = Fdco
                                            // (488 + 1) * 32768 = 16MHz
                                            // Set FLL Div = fDCOCLK/2
  __bic_SR_register(SCG0);                  // Enable the FLL control loop

  /* Worst-case settling time for the DCO when the DCO range bits have been
     changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
     UG for optimization.
     32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
   *
   */
  __delay_cycles(375000);

  // Loop until XT1,XT2 & DCO fault flag is cleared
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                             // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  
  _current_MCLK_freq = UCS_16MHZ;
}

void UCS_setMCLK_8MHz(void)
{
  // CHANGE MCLK = SMCLK = 8MHZ
  PMMCTL0 = PMMPW + PMMCOREV_0;            // reset PMMCOREV to 0
  UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFO
  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx

  // Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
	
  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL1 = DCORSEL_5;                      // Select DCO range 16MHz operation
  UCSCTL2 = 249;                           // Set DCO Multiplier for 8MHz
                                            // (N + 1) * FLLRef = Fdco
                                            // (249 + 1) * 32768 = 8MHz
  __bic_SR_register(SCG0);                  // Enable the FLL control loop

  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
  __delay_cycles(250000);
  
  _current_MCLK_freq = UCS_8MHZ;
}

void UCS_setMCLK_1MHz(void)
{
  UCSCTL3 = SELREF_0;                       // Set DCO FLL reference = XT1CLK
  UCSCTL4 |= SELA_0;                        // Set ACLK = XT1CLK
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx

  // Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  
  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL1 = DCORSEL_1;
  UCSCTL2 &= ~(0x00FF);                     // Clear FLL multipler bits
  UCSCTL2 |= 31;

  __bic_SR_register(SCG0);                  // Enable the FLL control loop

  __delay_cycles(250000);
  
  _current_MCLK_freq = UCS_1MHZ;
}

UCS_freq_e UCS_getMCLKFreq(void)
{
  return _current_MCLK_freq;
}
