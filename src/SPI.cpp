#include "SPI.hpp"


void SPI::initialize() {
  

  SPI::initialized = true;
}

void SPI::setMaster(bool value) {
  SPI::master = value;
}

void SPI::setChannel(uint8_t value) {
  SPI::channel = value;
}

void SPI::sendData(std::string value) {
  if(SPI::initialized != true) SPI::initialize();

  
}

bool SPI::initialized = false;

bool SPI::master = false;

uint8_t SPI::channel = 2;