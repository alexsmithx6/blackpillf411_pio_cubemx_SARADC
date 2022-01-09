#include "hal_sar.h"

HAL_StatusTypeDef sarADC_initGPIO(void);
HAL_StatusTypeDef sarADC_initSPI(void);
uint8_t sarADC_fsm(sarFSMState_e* state, uint16_t* sarCCVal, uint8_t* cnt);

SPI_HandleTypeDef hsarSPI;

HAL_StatusTypeDef hal_sarADC_init(void) {

    if ((sarADC_initGPIO() != HAL_OK) || (sarADC_initSPI() != HAL_OK)) {

        return HAL_ERROR;
    }

    /* Disable SAR and put into reset mode */
    HAL_GPIO_WritePin(SAR_SPI_CEN_Port, SAR_SPI_CEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SAR_SPI_CEN_Port, SAR_SPI_CEN_Pin, GPIO_PIN_RESET);

    return HAL_OK;
}

HAL_StatusTypeDef sarADC_initGPIO(void) {

    /* Initialise GPIO struct */
    GPIO_InitTypeDef SPI_Config = {0};

    /* GPIO initialisation struct configured for SPI AF */
    SPI_Config.Mode = GPIO_MODE_AF_PP;
    SPI_Config.Pull = GPIO_NOPULL;
    SPI_Config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    SPI_Config.Alternate = GPIO_AF5_SPI1;

    /* Configure SCK pin */
    SAR_SPI_SCK_Clk();
    SPI_Config.Pin = SAR_SPI_SCK_Pin;
    HAL_GPIO_Init(SAR_SPI_SCK_Port, &SPI_Config);

    /* Configure MOSI pin */
    SAR_SPI_MOSI_Clk();
    SPI_Config.Pin = SAR_SPI_MOSI_Pin;
    HAL_GPIO_Init(SAR_SPI_MOSI_Port, &SPI_Config);

    /* Configure MISO pin */
    SAR_SPI_MISO_Clk();
    SPI_Config.Pin = SAR_SPI_MISO_Pin;
    HAL_GPIO_Init(SAR_SPI_MISO_Port, &SPI_Config);

    /* Initialise GPIO struct */
    GPIO_InitTypeDef GPIO_Config = {0};

    /* GPIO initialisation struct configured for general purpose output */
    GPIO_Config.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Config.Pull = GPIO_PULLDOWN;
    GPIO_Config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    /* Configure CEN pin */
    SAR_SPI_CEN_Clk();
    GPIO_Config.Pin = SAR_SPI_CEN_Pin;
    HAL_GPIO_Init(SAR_SPI_CEN_Port, &GPIO_Config);

    /* Configure RST pin */
    SAR_SPI_RST_Clk();
    GPIO_Config.Pin = SAR_SPI_RST_Pin;
    HAL_GPIO_Init(SAR_SPI_RST_Port, &GPIO_Config);

    /* Configure TRIG pin */
    SAR_SPI_TRIG_Clk();
    GPIO_Config.Pin = SAR_SPI_TRIG_Pin;
    GPIO_Config.Mode = GPIO_MODE_INPUT;
    GPIO_Config.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(SAR_SPI_TRIG_Port, &GPIO_Config);

    return HAL_OK;
}

HAL_StatusTypeDef sarADC_initSPI(void) {

    __HAL_RCC_SPI1_CLK_ENABLE();

    SAR_SPI_Handler.Instance = SAR_SPI_Instance;
    SAR_SPI_Handler.Init.Mode = SPI_MODE_MASTER;
    SAR_SPI_Handler.Init.Direction = SPI_DIRECTION_2LINES;
    SAR_SPI_Handler.Init.DataSize = SPI_DATASIZE_8BIT;
    SAR_SPI_Handler.Init.CLKPolarity = SPI_POLARITY_LOW;
    SAR_SPI_Handler.Init.CLKPhase = SPI_PHASE_1EDGE;
    SAR_SPI_Handler.Init.NSS = SPI_NSS_SOFT;
    SAR_SPI_Handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    SAR_SPI_Handler.Init.FirstBit = SPI_FIRSTBIT_MSB;
    SAR_SPI_Handler.Init.TIMode = SPI_TIMODE_DISABLE;
    SAR_SPI_Handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    SAR_SPI_Handler.Init.CRCPolynomial = 10;

    if (HAL_SPI_Init(&SAR_SPI_Handler) != HAL_OK) {

        return HAL_ERROR;
    }

    return HAL_OK;
}

uint8_t sarADC_fsm(sarFSMState_e* state, uint16_t* sarCCVal, uint8_t* cnt) {

    *cnt += 1;

    if (*cnt > 8) {

        *state = FSM_STATE_COMPLETE;
        return 0;
    }

    // Get (power ** cnt):
    uint16_t pwr = 1;
    for (int ind = 0; ind < *cnt; ind++) {
        pwr *= 2;
    }

    if (*state == FSM_STATE_RESET) {

        *state = FSM_STATE_FIRSTRUN;
        *sarCCVal = (SAR_FSM_VALUE_MAX >> 1); // divide by 2
        return 1;
    }

    if (HAL_GPIO_ReadPin(SAR_SPI_TRIG_Port, SAR_SPI_TRIG_Pin) 
            == GPIO_PIN_SET) {

        *state = FSM_STATE_LOWER;
        *sarCCVal -= (SAR_FSM_VALUE_MAX / pwr);

    } else {

        *state = FSM_STATE_HIGHER;
        *sarCCVal += (SAR_FSM_VALUE_MAX / pwr);

    }

    uint8_t buffer[SAR_FSM_BYTE_RES];
    for (int ind = 0; ind < SAR_FSM_BYTE_RES; ind++) {

        buffer[ind] = ((*sarCCVal >> (8 * ind)) & 0x00FF);
    }

    HAL_SPI_Transmit(&SAR_SPI_Handler, (uint8_t*) buffer, 
            SAR_FSM_BYTE_RES * sizeof(uint8_t), 100);

    return 1;
}

uint16_t hal_sarADC_getReading(void) {

    sarFSMState_e state = FSM_STATE_RESET;
    uint16_t sarCCVal = SAR_FSM_VALUE_MAX;
    uint8_t cnt = 0;
    uint8_t res = 1;

    HAL_GPIO_WritePin(SAR_SPI_CEN_Port, SAR_SPI_CEN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SAR_SPI_RST_Port,SAR_SPI_RST_Pin, GPIO_PIN_SET);

    while (res == 1) {

        res = sarADC_fsm(&state, &sarCCVal, &cnt);
        SAR_SPI_DELAY(100);
    }

    HAL_GPIO_WritePin(SAR_SPI_CEN_Port, SAR_SPI_CEN_Pin, GPIO_PIN_RESET);

    return sarCCVal;
}