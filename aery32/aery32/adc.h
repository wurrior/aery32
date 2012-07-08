/*
 *  _____             ___ ___   |
 * |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
 * |     | -_|  _| | |_  |  _|  |  
 * |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
 *               |___|          |
 *
 * Copyright (c) 2012, Muiku Oy
 * All rights reserved.
 *
 * LICENSE
 *
 * New BSD License, see the LICENSE.txt bundled with this package. If you did
 * not receive a copy of the license and are unable to obtain it through the
 * world-wide-web, please send an email to contact@muiku.com so we can send
 * you a copy.
 */

/*!
\file aery32/adc.h
\brief Analog-to-digital converter (adc)

\note ADC clock frequency has its upper limits and its frequency is related
to the Periheral Bus A (PBA) clock. Thus care has to be taken to choose the
proper prescaler for the PBA during the ADC initialization. For UC3A0/A1,
consult the ADC charasteristics table from the datasheet page 772 for maximum
ADC clock.

\example adc.c
*/

#ifndef __AERY32_ADC_H
#define __AERY32_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <inttypes.h>
#include <avr32/io.h>

#define ADCLK_HIRES_MAX 5000000UL
#define ADCLK_LORES_MAX 8000000UL

#ifdef AERY_SHORTCUTS
	extern volatile avr32_adc_t *adc;
#endif

/**
 * Initializes Analog-to-digital converter
 * \param prescal Prescaler that has used to divide PBA clock for proper ADC
 *                clock, adclk = pba_clk / (2 * (prescal+1))
 * \param hires   Use 10-bit resolution, define false if 8-bit is preferred
 * \param shtime  ADC sample and hold time, time = (shtim + 1) / adclk
 * \param startup ADC startup time, time = (startup + 1) * 8 / adclk
 * \return 0 on success, -1 if the maximum ADC frequency has been exceeded
 *
 * \note Calls aery_pm_get_fclkdomain(CLKDOMAIN_PBA)
 */
int aery_adc_init(uint8_t prescal, bool hires, uint8_t shtime,
		uint8_t startup);

/**
 * Starts the conversion
 */
void aery_adc_start_cnv(void);

/**
 * Check if the conversion is ready
 * \param chamask Channel mask, e.g. (1<<2)|(1<<3) checks channels 2 and 3
 * \return 1 if ready, 0 if not, -1 if conversion wasn't even started
 */
int aery_adc_cnv_isrdy(uint8_t chamask);

/**
 * Get the conversion result
 * \param chanum Returns the result of the given channel
 */
uint16_t aery_adc_get_cnv(uint8_t chanum);

/**
 * Enables Analog-to-digital converter channels
 * \param chamask Channel mask for which channels should be enabled
 */
void aery_adc_enable(uint8_t chamask);

/**
 * Disables Analog-to-digital converter channels
 * \param chamask Channel mask for which channels should be disabled
 */
void aery_adc_disable(uint8_t chamask);

#ifdef __cplusplus
}
#endif

#endif
