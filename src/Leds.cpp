#include "Leds.hpp"


void Leds::initialize() {
    __GPIOD_CLK_ENABLE();
    GPIO_InitTypeDef init_configuration;

    init_configuration.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    init_configuration.Mode = GPIO_MODE_OUTPUT_PP;
    init_configuration.Speed = GPIO_SPEED_LOW;
    init_configuration.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOD, &init_configuration);
}


void Leds::turn_on(std::initializer_list<Led> leds) {
    if (not __initialized) {
        initialize();
    }
    
	auto bitmask = std::bitset<16>();
	for (const auto &led : leds) {
		bitmask |= std::bitset<16>(static_cast<uint16_t>(led));
	}
	HAL_GPIO_TogglePin(GPIOD, static_cast<uint16_t>(bitmask.to_ulong()));
}


void Leds::turn_off(std::initializer_list<Led> leds) {
	auto bitmask = std::bitset<16>();
	for (const auto &led : leds) {
		bitmask |= std::bitset<16>(static_cast<uint16_t>(led));
	}
	HAL_GPIO_TogglePin(GPIOD, static_cast<uint16_t>(bitmask.to_ulong()));
}

bool Leds::__initialized = false;