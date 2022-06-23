/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    i2c.h
 * @brief   This file contains all the function prototypes for
 *          the i2c.c file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdbool.h>

struct i2c_master_request;

/**
 * @brief I2C request callback.
 */
typedef void (*i2c_request_callback_t)(struct i2c_master_request *req);

struct i2c_master_request {
    uint16_t slave_addr;             // Slave address.(7bit or 10bit address)
    uint8_t *tx_data;                // Transmit data.
    uint8_t *rx_data;                // Receive buffer.
    uint16_t tx_length;              // Transmit size in bytes.
    uint16_t rx_length;              // Receive size in bytes.(rx data not need, set 0.)
    HAL_StatusTypeDef status;        // Status.
    i2c_request_callback_t callback; // Callback function to communication done.
};

/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_I2C1_Init(void);

/* USER CODE BEGIN Prototypes */
bool MX_I2C1_Request(struct i2c_master_request *req);
bool MX_I2C1_IsBusy(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */
