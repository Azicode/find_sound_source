/**
 * @file get_sample.cpp
 * @author sasaki
 * @brief 一定時間SPIからデータを受けとりファイル出力する
 * @version 0.1
 * @date 2022-06-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <cstdio>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>

#include <pigpio.h>

#include <mcp3208.hpp>

using str = std::string;
#define reps(i, a, n) for (int64_t i = (a); i < (int64_t)(n); ++i)
#define rep(i, n) reps(i, 0, n)
#define repn(n) reps(i, 0, n)

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

#define THOUSAND 1000
#define MILLION  1000000
#define BILLION  1000000000


constexpr uint32_t BUF_SIZE_1M = 1024 * 1024; // 65536

int main(int argc, char *argv[])
{
    DEBUG_FUNC();

    /* 時間関係 */
    int32_t        count = 0;               // ループ回数をカウント
    uint32_t       st_t;                    // 基準時間
    const uint32_t ex_t = 10 * MILLION;     // 実行時間[us]
    uint32_t       el_t;                    // 経過時間

    /* サンプリング用変数 */
    const uint32_t sample_num = BUF_SIZE_1M;
    int16_t samples[sample_num];
    int32_t   times[sample_num];

    /* SPI設定 */
    const int8_t spi_ch = 0;
    const int8_t adc_ch = 0;
    const uint32_t baud = 2 * MILLION;
    const bool      flg = 0;

    std::unique_ptr<MCP3208> adc(new MCP3208(spi_ch, adc_ch, baud, flg));

    DEBUG_LOG("Loop Start!");
    st_t = gpioTick();
    while (el_t < ex_t)
    {
        el_t = gpioTick() - st_t;    // 経過時間を取得
        
        adc->spi_read();                     // SPI読み出し
        samples[count] = adc->get_sample();  // データを配列に格納
        times[count]   = el_t;

        count++;
    }
    std::cout << "Loop finished! time = " << el_t
              <<        " [us], count = " << count << std::endl;

    std::ofstream ofs;
    str fn = "../log/get_sample.csv";
    ofs.open(fn, std::ios::out);    // 追記：std::ios::app
    ASSERT_MSG(!ofs, "## Fatal Error ## could not open file : " << fn);
    rep(i, count)
    {
        ofs << times[i] << ", " << samples[i] << "\n";
    }
    ofs.close();
}