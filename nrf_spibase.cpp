#include "nrf_spibase.h"

nrf_SPIBase::nrf_SPIBase(SPI_TypeDef* spi):SPIBase(spi){
	_miso = 0;
	_mosi = 0;
	_sck = 0;
}

nrf_SPIBase::nrf_SPIBase(SPI_TypeDef* spi, uint32_t miso,
		uint32_t mosi, uint32_t sck):SPIBase(spi){
	_miso = miso;
	_mosi = mosi;
	_sck = sck;
}

nrf_SPIBase::~nrf_SPIBase(){

}

void nrf_SPIBase::configureCS(uint32_t csPin){
    SPIBase::configureCS(csPin);
    ss_init(csPin);
}

void nrf_SPIBase::setPrescaler(uint16_t prescaler){

}

bool nrf_SPIBase::init(){
	// Initialize the SPI peripheral
	if (_mosi == 0 && _miso == 0 && _sck == 0){
		// Should be initialized outside of this system */
		return true;
	} else {
		nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
		spi_config.ss_pin = NRF_DRV_SPI_PIN_NOT_USED;
		spi_config.miso_pin = _miso;
		spi_config.mosi_pin = _mosi;
		spi_config.sck_pin = _sck;
		if (_cpol == 0 && _cpha == 0)
			spi_config.mode = NRF_DRV_SPI_MODE_0;
		else if (_cpol == 0 && _cpha == 1)
			spi_config.mode = NRF_DRV_SPI_MODE_1;
		else if (_cpol == 1 && _cpha == 0)
			spi_config.mode = NRF_DRV_SPI_MODE_2;
		else
			spi_config.mode = NRF_DRV_SPI_MODE_3;
		// Set in blocking mode. Change as necessary
		if (nrf_drv_spi_init(_spi, &spi_config, 
				NULL/*spi_event_handler*/, NULL) == NRF_SUCCESS)
			return true; 
		else
			return false;
	}
}

void nrf_SPIBase::deinit(){
	// Deinit the SPI peripheral
	nrf_drv_spi_uninit(_spi);

}

void nrf_SPIBase::select(){
    ss_clear(_csPin);
}

void nrf_SPIBase::unselect(){
    ss_set(_csPin);
}

uint16_t nrf_SPIBase::transfer(const uint16_t send){
	// What is the address to send to?
	uint16_t receive = 0x0000;	// Guarantee that the second byte is 0x00 in case of 8-bit
	uint8_t m_length;
	if (_bits == 8)
		m_length = 1;
	else if (_bits == 16)
		m_length = 2;
	else
		return 0; // Bad setup
	
	APP_ERROR_CHECK(nrf_drv_spi_transfer(_spi, (uint8_t*)&send, m_length,
		(uint8_t*)&receive, m_length));

	return receive;
}

bool nrf_SPIBase::initClock(){
	// Assume they are already prepared
    return true;
}
