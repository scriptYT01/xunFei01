#ifndef __CUSTOM_H__
#define __CUSTOM_H__


#define VOICE_STATION				// voice station model
#define THREAD_IR_LED_BLAST         // Activates IR LED blaster

#define THREAD_UART_W2M             // UART data through-pass from Xiaomi WIFI module to VIOMI MICOM

//#define EMULATE_HOOD_MCU            // Standalone C&C test. (No MCU connection is needed to hear response ment.)

#define WAKEUP_RESPONSE_MENT        // Response ment for wakeup command.

#if defined(EMULATE_HOOD_MCU)
//#define TEST_ONLY_OPPLE_DEMO1       // Controls OPPLE lights, but uses VIOMI voice commands.
#endif

#define MIIO_WIFI_COWORK            // Process miio_wifi data through shared memory.

#endif  /* __CUSTOM_H__ */


