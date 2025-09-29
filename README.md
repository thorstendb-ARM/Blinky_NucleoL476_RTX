# Blinky_NucleoL476_RTX

The **Blinky** project is a simple example that can be used to verify the
basic tool setup.
It provides as example forRTX5 on STM32 Nucleo-L476RG for RTOS Views plugin, see https://github.com/mcu-debug/rtos-views

It is compliant to the Cortex Microcontroller Software Interface Standard (CMSIS)
and uses the CMSIS-RTOS2 API interface for RTOS functionality. The CMSIS-RTOS2 API
is available with various real-time operating systems, for example RTX5 or FreeRTOS.

## Operation

- At start the vioLED0 blinks in 1 sec interval.
- The vioBUTTON0 changes the blink frequency and start/stops vioLED1.

### CMSIS-Driver Virtual I/O mapping

| CMSIS-Driver VIO      | Board component
|:----------------------|:--------------------------------------
| vioBUTTON0            | USER button (B1)
