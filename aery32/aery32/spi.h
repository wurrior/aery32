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
\file aery32/spi.h
\brief Serial Peripheral Interface (SPI)
\example spi.c
\example displays/hd44780.c
*/

#ifndef __AERY32_SPI_H
#define __AERY32_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <inttypes.h>
#include <avr32/io.h>

#ifdef AERY_SHORTCUTS
	extern volatile avr32_spi_t *spi0;
	extern volatile avr32_spi_t *spi1;
#endif

/**
 * Spi mode
 */
enum Spi_mode { SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3 };

/**
 * Init SPI as a master
 * \param pspi Pointer to the SPI peripheral which to init
 *
 * \par CS multiplexer
 * If you are using multiplexed chip selects, enable cs multiplexing
 * by bitbanging PCSDEC bit in SPI MR register after initialization:
 *
 * \code
 * aery_spi_init_master(&AVR32_SPI0);
 * AVR32_SPI0.MR.pcsdec = 1;
 * \endcode
 */
void aery_spi_init_master(volatile avr32_spi_t *pspi);

/**
 * Setups the SPI mode and the shift register width of the Numeric
 * Processor Chip Select (NPCS, same as slave/chip select)
 * \param pspi Pointer to the SPI peripheral
 * \param npcs Chip Select: 0-3
 * \param mode SPI mode
 * \param bits Width of SPI shift register: 8-16 bits
 *
 * \par Making SPI SCK faster
 * Chip select baudrate is hard coded to MCK/255. To make it faster
 * you can bitbang the SCRB bit in CSRX register.
 *
 * \code
 * aery_spi_setup_npcs(&AVR32_SPI0, 0, SPI_MODE1, 16);
 * AVR32_SPI0.CSR0.scbr = 32; // baudrate is now MCK/32
 * \endcode
 *
 * \note Every chip select line can have different mode and size of shift
 *       register.
 */
void aery_spi_setup_npcs(volatile avr32_spi_t *pspi, uint8_t npcs,
                enum Spi_mode mode, uint8_t bits);

/**
 * Writes and reads SPI bus
 * \param pspi   Pointer to the SPI peripheral which to use.
 * \param data   Binary word to be send.
 * \param npcs   Chip select line number. Can also be number of cs multiplexer.
 *               Use dummy cs number, if you like to use some other gpio pin
 *               through bitbanging.
 * \param islast Is this the last transmit? If no, set 0 to leave chip select
 *               low.
 * \return Received data bits
 *
 * \par SPI read
 * When you only want to read from external device through SPI, ignore the
 * sent data and use dummy bits instead, for example 0x00.
 *
 * \code
 * uint16_t rd; // read data
 * rd = aery_spi_transmit(&AVR32_SPI0, 0, 0, true);
 * \endcode
 */
uint16_t aery_spi_transmit(volatile avr32_spi_t *pspi, uint16_t data,
                uint8_t npcs, bool islast);

/**
 * Enables the SPI peripheral
 * \param pspi Pointer to the SPI peripheral which to enable
 */
void aery_spi_enable(volatile avr32_spi_t *pspi);

/**
 * Disables the SPI peripheral
 * \param pspi Pointer to the SPI peripheral which to disable
 */
void aery_spi_disable(volatile avr32_spi_t *pspi);

#ifdef __cplusplus
}
#endif

#endif
