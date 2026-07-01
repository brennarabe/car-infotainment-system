# car-infotainment-system

This project is a microcontroller-based car infotainment system designed to display real-time vehicle information while providing a simple FM radio interface. The system displays vehicle speed and direction from a GPS receiver on a small OLED display as well as the current FM radio station, which users can change using a potentiometer and a button.

This project demonstrates embedded systems development through the integration of multiple hardware peripherals communicating over different protocols. An ATmega 3208 AVR microcontroller coordinates communication with a GPS reciever over UART, FM radio tuner over TWI (I<sup>2</sup>C), and an OLED display over SPI while processing GPIO & ADC user input.

Although inspired by commercial infotainment systems, this project was developed to explore peripheral communication, embedded software architecture, and real-time coordination of multiple devices on a resource-constrained microcontroller.

<img width="50%" height="50%" alt="oled_display" src="https://github.com/brennarabe/car-infotainment-system/blob/main/media/oled_display.jpg" />

## Highlights

- ATmega microcontroller (on an AVR-BLE development board) programmed in C
- UART communication with a GPS receiver
- TWI (I<sup>2</sup>C) communication with a FM radio tuner
- SPI communication with an OLED display
- Real-time GPS speed and heading display
- FM radio tuning
- Physical user interface using buttons (GPIO) and a potentiometer (ADC)

## Demo

https://github.com/user-attachments/assets/52c336fd-0a40-4655-9510-fe2e2c7428ee





