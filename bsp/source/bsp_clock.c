/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                    MICRIUM BOARD SUPPORT PACKAGE
*
*                                         STM32F746G-DISCO
*                                         Evaluation Board
*
* Filename      : bsp_clock.c
* Version       : V1.00
* Programmer(s) : FF
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  "bsp_clock.h"
#include  "stm32f7xx_hal.h"


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                   BSP SYSTEM CLOCK INITIALIZATION
*
* Description : Board System clock initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : (1) The PLLISAI_R frequency is being used by the TFT LCD. the LCD configures the clock
*                   required for the LCD Clock (LTDC Clock) under the LCDConf.c file, within the
*                   HAL_LTDC_MspInit() function.
*********************************************************************************************************
*/

void  BSP_SystemClkCfg (void)
{
    RCC_OscInitTypeDef        RCC_OscInit;
    RCC_ClkInitTypeDef        RCC_ClkInit;
    RCC_PeriphCLKInitTypeDef  PeriphClkInit;
    HAL_StatusTypeDef         hal_status;

                                                                /* VCO out-freq = HSE * (PLLN / PLLM)    = 400MHz.      */
                                                                /* PLLCLK       = (VCO out-freq) / PLLP  = 200MHz.      */
    RCC_OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;        /* HSE freq     = 25MHz.                                */
    RCC_OscInit.HSEState       = RCC_HSE_ON;
    RCC_OscInit.HSIState       = RCC_HSI_OFF;
    RCC_OscInit.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInit.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInit.PLL.PLLM       = 25u;
    RCC_OscInit.PLL.PLLN       = 400u;
    RCC_OscInit.PLL.PLLP       = RCC_PLLP_DIV2;
    RCC_OscInit.PLL.PLLQ       = 8;                            /* PLL_Q out freq = (VCO out-freq) / PLLQ = 50MHz.      */

    hal_status = HAL_RCC_OscConfig(&RCC_OscInit);
    if (hal_status != HAL_OK) {
        while (DEF_TRUE) {                                      /* STOP if error                                        */
            ;
        }
    }

    hal_status = HAL_PWREx_EnableOverDrive();                   /* Activate the OverDrive to reach the 200 Mhz Freq     */
    if (hal_status != HAL_OK) {
        while (DEF_TRUE) {                                      /* STOP if error                                        */
            ;
        }
    }

                                                                /* ---- CFG PLLSAI FOR OTG-FS AND LTDC CLOCK SOURCE --- */
                                                                /* VCO out-freq = HSE * (PLLSAIN / PLLM) = 192MHz.      */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
    PeriphClkInit.Clk48ClockSelection  = RCC_CLK48SOURCE_PLLSAIP;
    PeriphClkInit.PLLSAI.PLLSAIN       = 192;
    PeriphClkInit.PLLSAI.PLLSAIQ       = 4;                     /* PLLISAI_Q freq = (VCO out-freq) / PLLSAIQ = 48MHz.   */
    PeriphClkInit.PLLSAI.PLLSAIR       = 5;                     /* PLLISAI_R freq = (VCO out-freq) / PLLSAIR = 38.4MHz. */
    PeriphClkInit.PLLSAI.PLLSAIP       = RCC_PLLSAIP_DIV4;      /* PLL48CLK freq  = (VCO out-freq) / PLLSAIP = 48MHz.   */

    hal_status = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
    if (hal_status != HAL_OK) {
        while (DEF_TRUE) {                                      /* STOP if error                                        */
            ;
        }
    }


    RCC_ClkInit.ClockType      = RCC_CLOCKTYPE_SYSCLK |
                                 RCC_CLOCKTYPE_HCLK   |
                                 RCC_CLOCKTYPE_PCLK1  |
                                 RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInit.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInit.AHBCLKDivider  = RCC_SYSCLK_DIV1;               /* HCLK    = AHBCLK  = PLLCLK / AHBPRES(1) = 200MHz.    */
    RCC_ClkInit.APB1CLKDivider = RCC_HCLK_DIV4;                 /* APB1CLK = AHBCLK  / APB1DIV(4)          =  50MHz.    */
    RCC_ClkInit.APB2CLKDivider = RCC_HCLK_DIV2;                 /* APB2CLK = AHBCLK  / APB2DIV(2)          = 100MHz.    */

    hal_status = HAL_RCC_ClockConfig(&RCC_ClkInit, FLASH_LATENCY_6);
    if (hal_status != HAL_OK) {
        while (DEF_TRUE) {                                      /* STOP if error                                        */
            ;
        }
    }
}


/*
*********************************************************************************************************
*                                          BSP_ClkFreqGet()
*
* Description : This function is used to retrieve System clocks frequencies.
*
* Argument(s) : clk_id    System clock identifier
*                             BSP_CLK_ID_SYSCLK     System clock frequency.
*                             BSP_CLK_ID_HCLK       CPU    clock frequency.
*                             BSP_CLK_ID_PCLK1      APB1   clock frequency.
*                             BSP_CLK_ID_PCLK2      APB2   clock frequency.
*
* Return(s)   : The specific clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_ClkFreqGet (BSP_CLK_ID  clk_id)
{
    CPU_INT32U  clk_freq;


    switch (clk_id) {
        case BSP_CLK_ID_SYSCLK:
             clk_freq = HAL_RCC_GetSysClockFreq();
             break;


        case BSP_CLK_ID_HCLK:
             clk_freq = HAL_RCC_GetHCLKFreq();
             break;


        case BSP_CLK_ID_PCLK1:
             clk_freq = HAL_RCC_GetPCLK1Freq();
             break;


        case BSP_CLK_ID_PCLK2:
             clk_freq = HAL_RCC_GetPCLK2Freq();
             break;


        default:
             clk_freq = 1u;                                     /* not a valid clock frequency.                         */
             break;
    }

    return (clk_freq);
}
