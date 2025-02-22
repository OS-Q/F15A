// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <stdint.h>
#include "esp_err.h"
#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Logic function used for EXT1 wakeup mode.
 */
typedef enum {
    ESP_EXT1_WAKEUP_ALL_LOW = 0,    //!< Wake the chip when all selected GPIOs go low
    ESP_EXT1_WAKEUP_ANY_HIGH = 1    //!< Wake the chip when any of the selected GPIOs go high
} esp_ext1_wakeup_mode_t;

/**
 * @brief Power domains which can be powered down in deep sleep
 */
typedef enum {
    ESP_PD_DOMAIN_RTC_PERIPH,      //!< RTC IO, sensors and ULP co-processor
    ESP_PD_DOMAIN_RTC_SLOW_MEM,    //!< RTC slow memory
    ESP_PD_DOMAIN_RTC_FAST_MEM,    //!< RTC fast memory
    ESP_PD_DOMAIN_MAX              //!< Number of domains
} esp_deep_sleep_pd_domain_t;

/**
 * @brief Power down options
 */
typedef enum {
    ESP_PD_OPTION_OFF,      //!< Power down the power domain in deep sleep
    ESP_PD_OPTION_ON,       //!< Keep power domain enabled during deep sleep
    ESP_PD_OPTION_AUTO      //!< Keep power domain enabled in deep sleep, if it is needed by one of the wakeup options. Otherwise power it down.
} esp_deep_sleep_pd_option_t;


/**
 * @brief Enable wakeup by ULP coprocessor
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_STATE if ULP co-processor is not enabled.
 */
esp_err_t esp_esp_deep_sleep_enable_ulp_wakeup();

/**
 * @brief Enable wakeup by timer
 * @param time_in_us  time before wakeup, in microseconds
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if value is out of range (TBD)
 */
esp_err_t esp_esp_deep_sleep_enable_timer_wakeup(uint64_t time_in_us);

/**
 * @brief Enable wakeup using a pin
 *
 * This function uses external wakeup feature of RTC_IO peripheral.
 * It will work only if RTC peripherals are kept on during deep sleep.
 *
 * This feature can monitor any pin which is an RTC IO. Once the pin transitions
 * into the state given by level argument, the chip will be woken up.
 *
 * @note This function does not modify pin configuration. The pin is
 *       configured in esp_esp_deep_sleep_start, immediately before
 *       entering deep sleep.
 *
 * @param gpio_num  GPIO number used as wakeup source. Only GPIOs which are have RTC
 *             functionality can be used: 0,2,4,12-15,25-27,32-39.
 * @param level  input level which will trigger wakeup (0=low, 1=high)
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if the selected GPIO is not an RTC GPIO,
 *        or the mode is invalid
 */
esp_err_t esp_esp_deep_sleep_enable_ext0_wakeup(gpio_num_t gpio_num, int level);

/**
 * @brief Enable wakeup using multiple pins
 *
 * This function uses external wakeup feature of RTC controller.
 * It will work even if RTC peripherals are shut down during deep sleep.
 *
 * This feature can monitor any number of pins which are in RTC IOs.
 * Once any of the selected pins goes into the state given by mode argument,
 * the chip will be woken up.
 *
 * @note This function does not modify pin configuration. The pins are
 *       configured in esp_esp_deep_sleep_start, immediately before
 *       entering deep sleep.
 *
 * @note internal pullups and pulldowns don't work when RTC peripherals are
 *       shut down. In this case, external resistors need to be added.
 *       Alternatively, RTC peripherals (and pullups/pulldowns) may be
 *       kept enabled using esp_esp_deep_sleep_pd_config function.
 *
 * @param mask  bit mask of GPIO numbers which will cause wakeup. Only GPIOs
 *              which are have RTC functionality can be used in this bit map:
 *              0,2,4,12-15,25-27,32-39.
 * @param mode select logic function used to determine wakeup condition:
 *            - ESP_EXT1_WAKEUP_ALL_LOW: wake up when all selected GPIOs are low
 *            - ESP_EXT1_WAKEUP_ANY_HIGH: wake up when any of the selected GPIOs is high
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if any of the selected GPIOs is not an RTC GPIO,
 *        or mode is invalid
 */
esp_err_t esp_esp_deep_sleep_enable_ext1_wakeup(uint64_t mask, esp_ext1_wakeup_mode_t mode);


/**
 * @brief Get the bit mask of GPIOs which caused wakeup (ext1)
 *
 * If wakeup was caused by another source, this function will return 0.
 *
 * @return bit mask, if GPIOn caused wakeup, BIT(n) will be set
 */
uint64_t esp_esp_deep_sleep_get_ext1_wakeup_status();

/**
 * @brief Set power down mode for an RTC power domain in deep sleep
 *
 * If not set set using this API, all power domains default to ESP_PD_OPTION_AUTO.
 *
 * @param domain  power domain to configure
 * @param option  power down option (ESP_PD_OPTION_OFF, ESP_PD_OPTION_ON, or ESP_PD_OPTION_AUTO)
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_ARG if either of the arguments is out of range
 */
esp_err_t esp_esp_deep_sleep_pd_config(esp_deep_sleep_pd_domain_t domain,
                                   esp_deep_sleep_pd_option_t option);

/**
 * @brief Enter deep sleep with the configured wakeup options
 *
 * This function does not return.
 */
void esp_esp_deep_sleep_start() __attribute__((noreturn));

/**
 * @brief Enter deep-sleep mode
 *
 * The device will automatically wake up after the deep-sleep time
 * Upon waking up, the device calls deep sleep wake stub, and then proceeds
 * to load application.
 *
 * Call to this function is equivalent to a call to esp_esp_deep_sleep_enable_timer_wakeup
 * followed by a call to esp_esp_deep_sleep_start.
 *
 * This function does not return.
 *
 * @param time_in_us  deep-sleep time, unit: microsecond
 */
void esp_esp_deep_sleep(uint64_t time_in_us) __attribute__((noreturn));

/**
 * @brief Enter deep-sleep mode
 *
 * Function has been renamed to esp_esp_deep_sleep.
 * This name is deprecated and will be removed in a future version.
 *
 * @param time_in_us  deep-sleep time, unit: microsecond
 */
void esp_system_deep_sleep(uint64_t time_in_us) __attribute__((noreturn, deprecated));

/**
 * @brief Default stub to run on wake from deep sleep.
 *
 * Allows for executing code immediately on wake from sleep, before
 * the software bootloader or ESP-IDF app has started up.
 *
 * This function is weak-linked, so you can implement your own version
 * to run code immediately when the chip wakes from
 * sleep.
 *
 * See docs/deep-sleep-stub.rst for details.
 */
void esp_esp_wake_deep_sleep(void);

/**
 * @brief Function type for stub to run on wake from sleep.
 *
 */
typedef void (*esp_deep_sleep_wake_stub_fn_t)(void);

/**
 * @brief Install a new stub at runtime to run on wake from deep sleep
 *
 * If implementing esp_esp_wake_deep_sleep() then it is not necessary to
 * call this function.
 *
 * However, it is possible to call this function to substitute a
 * different deep sleep stub. Any function used as a deep sleep stub
 * must be marked RTC_IRAM_ATTR, and must obey the same rules given
 * for esp_esp_wake_deep_sleep().
 */
void esp_esp_set_deep_sleep_wake_stub(esp_deep_sleep_wake_stub_fn_t new_stub);

/**
 * @brief Get current wake from deep sleep stub
 * @return Return current wake from deep sleep stub, or NULL if
 *         no stub is installed.
 */
esp_deep_sleep_wake_stub_fn_t esp_esp_get_deep_sleep_wake_stub(void);

/**
 *  @brief The default esp-idf-provided esp_esp_wake_deep_sleep() stub.
 *
 *  See docs/deep-sleep-stub.rst for details.
 */
void esp_esp_default_wake_deep_sleep(void);


#ifdef __cplusplus
}
#endif
