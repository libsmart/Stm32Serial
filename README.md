# Stm32Serial

Implements a handler and drivers for serial devices on STM32.

Does not require ThreadX.

Requires Stm32Common.





## Installation

Enable driver in `libsmart_config.hpp`

Create driver and serial instance in `globals.hpp`:

```c++
extern USBD_HandleTypeDef hUsbDeviceFS;
inline Stm32Common::StreamSession::Manager<Stm32Common::StreamSession::EchoStreamSession, 1> echoStreamSessionManager(&Logger);
inline Stm32Serial::Stm32UsbCdcDriver UsbSerialDriver(&hUsbDeviceFS, "UsbSerialDriver");
inline Stm32Serial::Stm32Serial Serial(&UsbSerialDriver, &echoStreamSessionManager, &Logger);

```

Initialize serial instance in `setup()`:

``` 
Serial.begin();
```

Call the loop() function repeatedly:

```
Serial.loop();
```







