#ifndef HAL_SAR_H_
#define HAL_SAR_H_

#include "stm32f4xx_hal.h"

extern SPI_HandleTypeDef hsarSPI;

#ifndef SAR_SPI_Handler                 // SAR SPI Config

#define SAR_SPI_Handler     (hsarSPI)   // SPI configurations
#define SAR_SPI_Instance    SPI1
#define SAR_SPI_SCK_Pin     GPIO_PIN_5  // GPIO pin configurations
#define SAR_SPI_SCK_Port    GPIOA
#define SAR_SPI_SCK_Clk()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define SAR_SPI_MOSI_Pin    GPIO_PIN_7
#define SAR_SPI_MOSI_Port   GPIOA
#define SAR_SPI_MOSI_Clk()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define SAR_SPI_MISO_Pin    GPIO_PIN_6
#define SAR_SPI_MISO_Port   GPIOA
#define SAR_SPI_MISO_Clk()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define SAR_SPI_CEN_Pin     GPIO_PIN_5
#define SAR_SPI_CEN_Port    GPIOA
#define SAR_SPI_CEN_Clk()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define SAR_SPI_RST_Pin     GPIO_PIN_4
#define SAR_SPI_RST_Port    GPIOA
#define SAR_SPI_RST_Clk()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define SAR_SPI_TRIG_Pin    GPIO_PIN_3
#define SAR_SPI_TRIG_Port   GPIOA
#define SAR_SPI_TRIG_Clk()  __HAL_RCC_GPIOA_CLK_ENABLE()

#endif /* SAR_SPI_Handler */

#ifndef SAR_SPI_DELAY

#define SAR_SPI_DELAY(__delay__)    (HAL_Delay(__delay__))

#endif /* SAR_SPI_DELAY */

typedef enum {

    FSM_STATE_RESET,
    FSM_STATE_FIRSTRUN,
    FSM_STATE_LOWER,
    FSM_STATE_HIGHER,
    FSM_STATE_COMPLETE
} sarFSMState_e;

#define SAR_FSM_BYTE_RES    1
#define SAR_FSM_VALUE_MAX   0xFF

HAL_StatusTypeDef hal_sarADC_init(void);
uint16_t hal_sarADC_getReading(void);

#endif /* HAL_SAR_H_ */