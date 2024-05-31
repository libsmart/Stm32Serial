# Stm32Serial

Implements a handler and drivers for serial devices on STM32.

Does not require ThreadX.

Requires Stm32Common.





## Installation

Enable driver in `libsmart_config.hpp`

Create driver and serial instance in `globals.hpp`:

```c++
inline Stm32Serial::Stm32HalUartItDriver Uart1SerialDriver(&huart1, "Uart1SerialDriver");
inline Stm32Serial::Stm32Serial Serial1(&Uart1SerialDriver);
```

Initialize serial instance in `setup()`:

``` 
Serial1.begin();
```

Call the loop() function repeatedly:

```
Serial1.loop();
```



You may want to overload the serial instance to do something useful with the sent data.



