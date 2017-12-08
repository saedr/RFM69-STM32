/* This is a nordic implementation to support the radio
 * communication driver by André Heßling
 */

#ifndef NRF_SPIBASE_H
#define NRF_SPIBASE_H

#include "nrf_drv_spi.h"

#include "spiUtil.h" //Internal utility functions

// Base class
#include "spibase.hpp"

#define SPI_TypeDef nrf_drv_spi_t
//#define GPIO_TypeDef uint32_t

class nrf_SPIBase : public SPIBase{
public:
    nrf_SPIBase(SPI_TypeDef* spi);
    nrf_SPIBase(SPI_TypeDef* spi, uint32_t miso, uint32_t mosi, uint32_t sck);
    ~nrf_SPIBase();

//    virtual void setBits(unsigned char bits);
    void configureCS(uint32_t csPin);
    void setPrescaler(uint16_t prescaler);
    bool init();
    void deinit();
    void select();
    void unselect();
    uint16_t transfer(uint16_t data);

protected:
    bool initClock();

    uint32_t _mosi, _miso, _sck;
    bool is_locally_enabled;
};

#endif
