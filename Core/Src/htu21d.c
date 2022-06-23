/*
 * mhtu21d.c
 *
 *  Created on: Mar 11, 2022
 *      Author: toshiyuki_umetsu
 *
 * @brief TE Connectivity社 HTU21Dドライバ
 */
#include "i2c.h"
#include <htu21d.h>

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
 * @brief データが利用可能かどうか
 */
static bool IsDataAvailable;

static struct i2c_master_request Request;

static uint8_t TxBuffer[4u];

static uint8_t RxBuffer[4u];

static float TemperatureCache = 0.0f;

static float HumidifierCache = 0.0f;

static bool start_update(void);
static void get_tempdata_done(struct i2c_master_request *req);
static void get_humidata_done(struct i2c_master_request *req);

/**
 * HTU21D温湿度計を初期化する
 */
void htu21d_init(void) {
    IsDataAvailable = false;
    HumidifierCache = 0.0f;
    TemperatureCache = 0.0f;
    Request.slave_addr = SLAVE_ADDR;
    Request.tx_data = TxBuffer;
    Request.tx_length = 0u;
    Request.rx_data = RxBuffer;
    Request.rx_length = 0u;
    Request.status = HAL_OK;
    Request.callback = NULL;
    return;
}
/**
 * 破棄する
 */
void htu21d_destroy(void) {
    IsDataAvailable = false;
    return;
}

/**
 * 温度を測定する。
 *
 * @param temperature 温度
 * @return 成功した場合にはtrue, それ以外はfalse.
 */
bool htu21d_measure_temperature(float *temperature) {
    if (!MX_I2C1_IsBusy()) {
        if (!start_update()) {
            return false;
        }
    }

    if (!IsDataAvailable) {
        return false;
    }

    *temperature = TemperatureCache;

    return true;
}

/**
 * 湿度を測定する。
 *
 * @param humidifier 湿度
 * @return 成功した場合にはtrue, それ以外はfalse.
 */
bool htu21d_measure_humidity(float *humidifier) {
    if (!MX_I2C1_IsBusy()) {
        if (!start_update()) {
            return false;
        }
    }

    if (!IsDataAvailable) {
        return false;
    }

    *humidifier = HumidifierCache;

    return true;
}

static bool start_update(void) {
    Request.callback = get_tempdata_done;
    Request.tx_data[0] = CMD_TRIGGER_TEMPERATURE_MEASUREMENT;
    Request.tx_length = 1u;
    Request.rx_length = 3u;

    return MX_I2C1_Request(&Request);
}

static void get_tempdata_done(struct i2c_master_request *req) {
    if (req->status != HAL_OK) {
        return;
    }

    uint16_t value = ((uint16_t)(req->rx_data[0u]) << 8u) | (uint16_t)(req->rx_data[1]);
    TemperatureCache = -46.85f + 175.72f * (float)(value) / 65536.0f;

    Request.callback = get_humidata_done;
    Request.tx_data[0] = CMD_TRIGGER_TEMPERATURE_MEASUREMENT;
    Request.tx_length = 1u;
    Request.rx_length = 3u;
    MX_I2C1_Request(&Request);
}

static void get_humidata_done(struct i2c_master_request *req) {
    uint16_t value = ((uint16_t)(req->rx_data[0u]) << 8u) | (uint16_t)(req->rx_data[1]);
    HumidifierCache = -6.0 + 125.0f * (float)(value) / 65536.0f;
    IsDataAvailable = true;
}
