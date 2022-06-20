/**
 * @file mcp3208.hpp
 * @author sasaki
 * @brief pigpio spi library for MCP3208
 * @version 0.1
 * @date 2022-06-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MCP3208_PIGPIO__MCP3208_HPP_
#define MCP3208_PIGPIO__MCP3208_HPP_

/**
 * @brief SPI通信を行うためのクラス
 * 
 */
class MCP3208
{

public:
    /**
     * @brief Construct a new MCP3208 object
     * 
     * @param spi_ch    ラズパイのSPIチャンネル
     * @param adc_ch    ADCのチャンネル
     * @param baud      サンプルクロック
     * @param flg       default SPI → 0, Auxiliary SPI → 1
     */
    explicit MCP3208(
        const uint8_t  &spi_ch,
        const uint8_t  &adc_ch,
        const uint32_t &baud,
        const bool     &flg
        );
    /**
     * @brief Destroy the MCP3208 object
     */
    ~MCP3208();

    /**
     * @brief 読み出し命令を使用しADCから情報を受け取る
     * 
     */
    void spi_read();
    /**
     * @brief バッファに入ったバイナリを int32_t に変換する
     * @return int32_t 読みだした数値
     */
    int32_t get_sample();
    /**
     * @brief read_count をセットする
     */
    void set_read_count(const int32_t count);

    /** spi_read() を呼び出した回数 */
    int32_t read_count = 0;

private:
    //! copyを禁止
    MCP3208(const MCP3208 &);
    //! moveを禁止
    MCP3208(const MCP3208 &&);
    //! 代入を禁止 
    MCP3208 &operator=(const MCP3208 &);

    //! 読み出し命令を設定
    void set_read_cmd();

    //! 通信に使うバッファサイズ
    static const uint8_t buf_size_ = 3;

    //! SPI -> 0 ~ 1, Auxiliary SPI -> 0 ~ 2
    uint8_t  spi_ch_;
    //! クロック周波数
    uint32_t baud_;
    //! SPI -> 0, Auxiliary SPI -> 256
    uint16_t aux_flg_;
    //! SPI接続機器を選べるハンドル。spiXferに使用。
    int8_t   spi_hndl_;

    //! MCP3208 のCH番号 (0 ~ 7)
    uint8_t adc_ch_;
    //! MCP3208 読み出し命令用のバッファ
    char read_cmd_[buf_size_] = {0};
    //! 読み出し用のバッファ 解像度 12 bit なので 3 Byte
    char read_buf_[buf_size_] = {0};
};

#endif  // MCP3208_PIGPIO__MCP3208_HPP_