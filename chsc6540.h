/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "port/esp_lcd_touch_chsc6540.h"
#include "esp_panel_touch_conf_internal.h"
#include "esp_panel_touch.hpp"

namespace esp_panel::drivers {

/**
 * @brief CHSC6540 touch controller
 *
 * This class provides implementation for CHSC6540 touch controller, inheriting from
 * the base Touch class to provide common touch functionality
 */
class TouchCHSC6540 : public Touch {
public:
    /**
     * @brief Default basic attributes for CHSC6540
     */
    static constexpr BasicAttributes BASIC_ATTRIBUTES_DEFAULT = {
        .name = "CHSC6540",
        .max_points_num = 1,
    };

    /**
     * @brief Construct a touch device instance with individual configuration parameters
     *
     * @param bus Bus interface for communicating with the touch device
     * @param width Panel width in pixels
     * @param height Panel height in pixels
     * @param rst_io Reset GPIO pin number (-1 if unused)
     * @param int_io Interrupt GPIO pin number (-1 if unused)
     */
    TouchCHSC6540(Bus *bus, uint16_t width, uint16_t height, int rst_io = -1, int int_io = -1):
        Touch(BASIC_ATTRIBUTES_DEFAULT, bus, width, height, rst_io, int_io)
    {
    }

    /**
     * @brief Construct a touch device instance with configuration
     *
     * @param[in] bus Pointer to the bus interface for communicating with the touch device
     * @param[in] config Configuration structure containing device settings and parameters
     */
    TouchCHSC6540(Bus *bus, const Config &config): Touch(BASIC_ATTRIBUTES_DEFAULT, bus, config) {}

    /**
     * @brief Construct a touch device instance with bus configuration and device configuration
     *
     * @param[in] bus_config Bus configuration
     * @param[in] touch_config Touch configuration
     * @note This constructor creates a new bus instance using the provided bus configuration
     */
    TouchCHSC6540(const BusFactory::Config &bus_config, const Config &touch_config):
        Touch(BASIC_ATTRIBUTES_DEFAULT, bus_config, touch_config)
    {
    }

    /**
     * @brief Destruct touch device
     */
    ~TouchCHSC6540() override;

    /**
     * @brief Startup the touch device
     *
     * @return `true` if success, otherwise false
     *
     * @note This function should be called after `init()`
     */
    bool begin() override;
};

} // namespace esp_panel::drivers

/**
 * @brief Deprecated type alias for backward compatibility
 * @deprecated Use `esp_panel::drivers::TouchCHSC6540` instead
 */
using ESP_PanelTouch_CHSC6540 [[deprecated("Use `esp_panel::drivers::TouchCHSC6540` instead")]] =
    esp_panel::drivers::TouchCHSC6540;
