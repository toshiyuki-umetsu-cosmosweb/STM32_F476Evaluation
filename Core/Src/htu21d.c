/*
 * mhtu21d.c
 *
 *  Created on: Mar 11, 2022
 *      Author: toshiyuki_umetsu
 *
 * @brief TE Connectivity社 HTU21Dドライバ
 */
#include <htu21d.h>
#include "i2c.h"

/**
 * HTU21DのI2Cスレーブアドレス。
 * どうやら固定っぽい。
 */
#define SLAVE_ADDR (0x40u)

/**
 * タイムアウト時間[msec]
 */
#define TIMEOUT_MILLIS (500u)

#define CMD_TRIGGER_TEMPERATURE_MEASUREMENT (0xE3)
#define CMD_TRIGGER_TEMPERATURE_MEASUREMENT_NOHOLD (0xF3u)
#define CMD_TRIGGER_HUMIDITY_MEASUREMENT (0xE5)
#define CMD_TRIGGER_HUMIDITY_MEASUREMENT_NOHOLD (0xF5u)
#define CMD_WRITE_USER_REGISTER (0xE6u)
#define CMD_READ_USER_REGISTER (0xE7u)
#define CMD_SOFTRESET (0xFEu)

/**
 * デバイスが利用可能かどうか
 */
static bool DeviceAvailable;

/**
 * 初期化する
 */
void
htu21d_init(void)
{
	HAL_StatusTypeDef status;

	uint16_t addr = (SLAVE_ADDR << 1u);
	uint8_t cmd[2u];

	cmd[0] = CMD_SOFTRESET;
	status = HAL_I2C_Master_Transmit(&hi2c1, addr, cmd, 1u, TIMEOUT_MILLIS);
	if (status != HAL_OK) {
		DeviceAvailable = false;
	} else {
		DeviceAvailable = true;
		cmd[0] = CMD_WRITE_USER_REGISTER;
		/* b7: 0 RH 12bits + Temp 14bits
		 * b6: 0 Status bit.
		 * b5-b3: 0 (Reserved)
		 * b2: 0 Disable on-chip heater
		 * b0: 0 RH 12bits + Temp 14bits
		 */
		cmd[1] = 0x00u;
		status = HAL_I2C_Master_Transmit(&hi2c1, addr, cmd, 2u, TIMEOUT_MILLIS);
		if (status != HAL_OK) {
			// do nothing.
		}
	}

}
/**
 * 破棄する
 */
void
htu21d_destroy(void)
{

}

/**
 * 温度を測定する。
 *
 * @param temperature 温度
 * @return 成功した場合にはtrue, それ以外はfalse.
 */
bool
htu21d_measure_temperature(float *temperature)
{
	HAL_StatusTypeDef status;
	uint16_t addr;
	uint8_t cmd[2u];

	addr = (SLAVE_ADDR << 1u);
	cmd[0u] = CMD_TRIGGER_TEMPERATURE_MEASUREMENT;
	status = HAL_I2C_Master_Transmit(&hi2c1, addr, cmd, 1u, TIMEOUT_MILLIS);
	if (status == HAL_OK) {
		uint8_t recvbuf[3u];
		addr = (SLAVE_ADDR << 1u) | 0x01u;
		status = HAL_I2C_Master_Receive(&hi2c1, addr, recvbuf, 3u, TIMEOUT_MILLIS);
		if (status == HAL_OK) {
            uint16_t value = ((uint16_t)(recvbuf[0u]) << 8u) | (uint16_t)(recvbuf[1]);
			*temperature = -46.85f + 175.72f * (float)(value) / 65536.0f;
		}
	}

	return (status == HAL_OK);
}

/**
 * 湿度を測定する。
 *
 * @param humidifier 湿度
 * @return 成功した場合にはtrue, それ以外はfalse.
 */
bool
mtu21d_measure_humidity(float *humidifier)
{
	HAL_StatusTypeDef status;
	uint16_t addr;
	uint8_t cmd[2u];

	addr = (SLAVE_ADDR << 1u);
	cmd[0u] = CMD_TRIGGER_TEMPERATURE_MEASUREMENT;
	status = HAL_I2C_Master_Transmit(&hi2c1, addr, cmd, 1u, TIMEOUT_MILLIS);
	if (status == HAL_OK) {
		uint8_t recvbuf[3u];
		addr = (SLAVE_ADDR << 1u) | 0x01u;
		status = HAL_I2C_Master_Receive(&hi2c1, addr, recvbuf, 3u, TIMEOUT_MILLIS);
		if (status == HAL_OK) {
            uint16_t value = ((uint16_t)(recvbuf[0u]) << 8u) | (uint16_t)(recvbuf[1]);
			*humidifier = -6.0 + 125.0f * (float)(value) / 65536.0f;
		}
	}

	return (status == HAL_OK);
}
