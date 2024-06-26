/*
 * Copyright (c) 2021 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file arm_mps3_io_drv.h
 * \brief Generic driver for ARM MPS3 FPGAIO.
    Features of ARM MPS3 FPGAIO driver:
        1. Read/Write LED status
        2. Read button status
        3. Read switches status
        4. Read/Write counter values
        5. Enable/Disable SHIELD0, SHIELD1, ADC
        6. Read/Write Misc control register
*/

#ifndef __ARM_MPS3_IO_DRV_H__
#define __ARM_MPS3_IO_DRV_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ARM MPS3 IO enumeration types */
enum arm_mps3_io_access_t {
    ARM_MPS3_IO_ACCESS_PIN = 0,  /*!< Pin access to MPS3 IO */
    ARM_MPS3_IO_ACCESS_PORT      /*!< Port access to MPS3 IO */
};

/* ARM MPS3 IO device configuration structure */
struct arm_mps3_io_dev_cfg_t {
    const uint32_t base;                 /*!< MPS3 IO base address */
};

/* ARM MPS3 IO device structure */
struct arm_mps3_io_dev_t {
    const struct arm_mps3_io_dev_cfg_t* const cfg; /*!< MPS3 IO configuration */
};

/**
 * \brief  Writes to output LEDs.
 *
 * \param[in] dev      MPS3 IO device where to write \ref arm_mps3_io_dev_t
 * \param[in] access   Access type \ref arm_mps3_io_access_t
 * \param[in] pin_num  Pin number.
 * \param[in] value    Value(s) to set.
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_mps3_io_write_leds(struct arm_mps3_io_dev_t* dev,
                            enum arm_mps3_io_access_t access,
                            uint8_t pin_num,
                            uint32_t value);

/**
 * \brief Reads the buttons status.
 *
 * \param[in] dev      MPS3 IO device where to read \ref arm_mps3_io_dev_t
 * \param[in] access   Access type \ref arm_mps3_io_access_t
 * \param[in] pin_num  Pin number.
 *
 * \return Returns bit value for Pin access or port value for port access.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_mps3_io_read_buttons(struct arm_mps3_io_dev_t* dev,
                                  enum arm_mps3_io_access_t access,
                                  uint8_t pin_num);

/**
 * \brief Reads the switches status.
 *
 * \param[in] dev      MPS3 IO device where to read \ref arm_mps3_io_dev_t
 * \param[in] access   Access type \ref arm_mps3_io_access_t
 * \param[in] pin_num  Pin number.
 *
 * \return Returns bit value for Pin access or port value for port access.
 *
 * \note This function doesn't check if dev is NULL.
 */
 uint32_t arm_mps3_io_read_switches(struct arm_mps3_io_dev_t* dev,
                                      enum arm_mps3_io_access_t access,
                                      uint8_t pin_num);

/**
 * \brief Reads the LED status.
 *
 * \param[in] dev      MPS3 IO device where to read \ref arm_mps3_io_dev_t
 * \param[in] access   Access type \ref arm_mps3_io_access_t
 * \param[in] pin_num  Pin number.
 *
 * \return Returns bit value for Pin access or port value for port access.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_mps3_io_read_leds(struct arm_mps3_io_dev_t* dev,
                               enum arm_mps3_io_access_t access,
                               uint8_t pin_num);

/**
 * \brief Reads the 100hz up counter value.
 *
 * \param[in] dev      MPS3 IO device where to read \ref arm_mps3_io_dev_t
 *
 * \return Returns 100hz up counter value
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_mps3_io_read_clk100hz(struct arm_mps3_io_dev_t* dev);

/**
 * \brief Sets value for 100hz up counter.
 *
 * \param[in] dev      MPS3 IO device where to write \ref arm_mps3_io_dev_t
 * \param[in] value    Counter value
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_mps3_io_write_clk100hz(struct arm_mps3_io_dev_t* dev,
                                   uint32_t value);

/**
 * \brief Reads the 1hz up counter value.
 *
 * \param[in] dev      MPS3 IO device where to read \ref arm_mps3_io_dev_t
 *
 * \return Returns 1hz up counter value
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_mps3_io_read_clk1hz(struct arm_mps3_io_dev_t* dev);

/**
 * \brief Sets value for 1hz up counter.
 *
 * \param[in] dev      MPS3 IO device where to write \ref arm_mps3_io_dev_t
 * \param[in] value    Counter value
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_mps3_io_write_clk1hz(struct arm_mps3_io_dev_t* dev,
                                   uint32_t value);

/**
 * \brief Reads the Cycle Up Counter value.
 *
 * \param[in] dev      MPS3 IO device where to read \ref arm_mps3_io_dev_t
 *
 * \return Returns cycle up counter value
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_mps3_io_read_counter(struct arm_mps3_io_dev_t* dev);

/**
 * \brief Sets value for Cycle up counter.
 *
 * \param[in] dev      MPS3 IO device where to write \ref arm_mps3_io_dev_t
 * \param[in] value    Counter value
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_mps3_io_write_counter(struct arm_mps3_io_dev_t* dev,
                                   uint32_t value);

/**
 * \brief Reads the Prescale counter value.
 *
 * \param[in] dev      MPS3 IO device where to read \ref arm_mps3_io_dev_t
 *
 * \return Returns current value of the pre-scaler counter.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_mps3_io_read_pscntr(struct arm_mps3_io_dev_t* dev);

/**
 * \brief Reads reload value for prescale counter.
 *
 * \param[in] dev      MPS3 IO device where to read \ref arm_mps3_io_dev_t
 *
 * \return Returns reload value for prescale counter.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_mps3_io_read_prescale(struct arm_mps3_io_dev_t* dev);

/**
 * \brief Sets reload value for prescale counter.
 *
 * \param[in] dev      MPS3 IO device where to write \ref arm_mps3_io_dev_t
 * \param[in] value    Reload value
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_mps3_io_write_prescale(struct arm_mps3_io_dev_t* dev,
                                   uint32_t value);

/**
 * \brief Get MISC register value.
 *
 * \param[in] dev      MPS3 IO device where to read \ref arm_mps3_io_dev_t
 *
 * \return Returns MISC register value
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_mps3_io_read_misc(struct arm_mps3_io_dev_t* dev);

/**
 * \brief Set MISC register value.
 *
 * \param[in] dev      MPS3 IO device where to write \ref arm_mps3_io_dev_t
 * \param[in] value    Value to set for MISC register value
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_mps3_io_write_misc(struct arm_mps3_io_dev_t* dev,
                                        uint32_t value);

/**
 * \brief Enable SHIELD0_SPI_nCS.
 *
 * \param[in] dev      MPS3 IO device where to write \ref arm_mps3_io_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_mps3_io_enable_shield0_spi_ncs(struct arm_mps3_io_dev_t* dev);

/**
 * \brief Disable SHIELD0_SPI_nCS.
 *
 * \param[in] dev      MPS3 IO device where to write \ref arm_mps3_io_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_mps3_io_disable_shield0_spi_ncs(struct arm_mps3_io_dev_t* dev);

/**
 * \brief Enable SHIELD1_SPI_nCS.
 *
 * \param[in] dev      MPS3 IO device where to write \ref arm_mps3_io_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_mps3_io_enable_shield1_spi_ncs(struct arm_mps3_io_dev_t* dev);

/**
 * \brief Disable SHIELD1_SPI_nCS.
 *
 * \param[in] dev      MPS3 IO device where to write \ref arm_mps3_io_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_mps3_io_disable_shield1_spi_ncs(struct arm_mps3_io_dev_t* dev);

/**
 * \brief Enable ADC_SPI_nCS.
 *
 * \param[in] dev      MPS3 IO device where to write \ref arm_mps3_io_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_mps3_io_enable_adc_spi_ncs(struct arm_mps3_io_dev_t* dev);

/**
 * \brief Disable ADC_SPI_nCS.
 *
 * \param[in] dev      MPS3 IO device where to write \ref arm_mps3_io_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_mps3_io_disable_adc_spi_ncs(struct arm_mps3_io_dev_t* dev);

#ifdef __cplusplus
}
#endif

#endif /* __ARM_MPS3_IO_DRV_H__ */
