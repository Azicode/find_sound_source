/**
 * @file mcp3208.cpp
 * @author sasaki
 * @brief pigpio spi library for MCP3208
 * @version 0.1
 * @date 2022-06-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <cstdio>
#include <cassert>
#include <iostream>
#include <pigpio.h>

#include "../include/mcp3208_pigpio/mcp3208.hpp"

#ifndef DEBUG_ON
#define DEBUG_ON
#define DEBUG_FUNC() std::cout << __FILE__ << "(" << __LINE__ << ") : " \
                               << __PRETTY_FUNCTION__ << std::endl;
#define DEBUG_LOG(...) std::cout << __VA_ARGS__ << std::endl;
#define ASSERT_MSG(x, msg)                               \
    if (x)                                               \
    {                                                    \
        DEBUG_LOG(msg << "\n"                            \
                      << __FILE__ << " : " << __LINE__); \
        abort();                                         \
    };
#endif  // DEBUG_ON

MCP3208::MCP3208(
    const uint8_t  &spi_ch,
    const uint8_t  &adc_ch,
    const uint32_t &baud,
    const bool     &flg
)
{
    DEBUG_FUNC();

    spi_ch_  = spi_ch;
    adc_ch_  = adc_ch;
    baud_    = baud / 2;
    aux_flg_ = flg;

    set_read_cmd();

    // gpio initialise
    ASSERT_MSG(gpioInitialise() < 0, "gpioInitialise() failed.");

    // spi open
    spi_hndl_ = spiOpen(spi_ch_, baud_, aux_flg_);
    ASSERT_MSG(spi_hndl_ < 0, "spiOpen() failed.");
}

MCP3208::~MCP3208()
{
    DEBUG_FUNC();

    spiClose(spi_hndl_);
    DEBUG_LOG("spi closed.");

    gpioTerminate();
    DEBUG_LOG("gpio terminated.");
}

void MCP3208::spi_read()
{
    spiXfer(spi_hndl_, read_cmd_, read_buf_, buf_size_);
    read_count++;
}

int32_t MCP3208::get_sample()
{
    return ( read_buf_[1] & 0xff ) << 4 |
           ( read_buf_[2] & 0xf0 ) >> 4;
}

void MCP3208::set_read_cmd()
{
    DEBUG_FUNC();
    read_cmd_[0] = (0b00011000 + adc_ch_) << 2;
}

void MCP3208::set_read_count(const int32_t count)
{
    read_count = count;
}