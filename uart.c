
// source code from website: https://e2e.ti.com/support/microcontrollers/msp430/f/166/t/690756?Compiler-MSP432P401R-UART-in-MSP432
// by user: armondoferr


#include "msp.h"
#include "uart.h"

char RX_Buffer[2];


/*UART CONFIGURATION*/
void uart_init(void)
{
     /*Configure UART*/
     EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;                        /*PUT EUSCI IN RESET*/
     EUSCI_A2->CTLW0  = EUSCI_A_CTLW0_SWRST |                       /*REMAIN EUSCI IN RESET*/
                      EUSCI_B_CTLW0_SSEL__SMCLK;                    /*CONFIGURE EUSCI CLOCK SMCLK*/
     /*Baud Rate calculation
     *12000000/(16*9600) = 78.125
     *Fractional portion = 0.125
     *User's Guide Table 21-4: UCBRSx = 0x10
     *UCBRFx = int ( (78.125-78)*16) = 2
     */
     EUSCI_A2->BRW = 78;                                            /*12000000/16/9600*/
     EUSCI_A2->MCTLW = (2 << EUSCI_A_MCTLW_BRF_OFS) |
                       EUSCI_A_MCTLW_OS16;
     EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);                     /*INITIALIZE EUSCI*/
     EUSCI_A2->IFG   &= ~(EUSCI_A_IFG_RXIFG);
     EUSCI_A2->IE |= EUSCI_A_IE_RXIE;                               /*ENABLE THE UART RX INTERRUPT*/


     /*ENABLE GLOBAL INTERRUPT*/
     __enable_irq();
     /*ENABLE NVIC IN EUSCI_A2 INTERRUPT*/
     NVIC->ISER[0] = 1 << ((EUSCIA2_IRQn) & 31);

}
/*UART PORTS*/
void confg_uart_wired(void)
{
     P3->SEL0 |= UART_TX | UART_RX;                                /*SET 2 UART PIN AS SECOND FUNCTION*/
}
/*UART CLOCK SYSTEM*/
void uart_cs(void)
{
     CS->KEY  = CS_KEY_VAL;                                        /*UNLOCK CS MODULE REGISTER ACESS*/
     CS->CTL0 = 0;                                                 /*RESET TUNING PARAMETERS*/
     CS->CTL0 = CS_CTL0_DCORSEL_3;                                 /*SET DCO TO 12MHz (NOMINAL, CENTER OF 8-16MHz RANGE)*/
     CS->CTL1 = CS_CTL1_SELA_2 |                                   /*SELECT ACLK = REFO*/
                CS_CTL1_SELS_3 |                                   /*SMCLK = DCO*/
                CS_CTL1_SELM_3;                                    /*MCLK = DCO*/
     CS->KEY  = 0;
}

/*UART GET MULTIPLE BYTES*/
void uart_get(void)
{
    RX_Buffer[1] = EUSCI_A2->RXBUF;
}


int EUSCIA2_IRQHandler(target)
{
    if(EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG)
    {
        if (RX_Buffer[1] == 'a')
        {
            target = 1;
        }
        else
        {
            target = 0;
        }
    }
    EUSCI_A2->IFG &= ~EUSCI_A_IFG_RXIFG;

}




