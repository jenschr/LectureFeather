# Dev board for lecturing Embedded & M2M


## Errata
Known bugs and limitations for this project

### Version 1.1
Yet to be produced & tested.

### Version 1.0
- APA102 used IO26 for data. This conflicts with PSRAM. Move to IO33.
- Temperature sensor is placed too close the the ESP-module, so it reads the temp of the board, not the air around it. Adafruit has a [similar problem](https://learn.adafruit.com/adafruit-esp32-s2-feather/pinouts#bme280-temperature-humidity-and-pressure-sensor-3106370) with the BME280 they use in theor design.
- Type-C receptacle footprint works great for industrial production, but gives 30% failure/bridges when producing locally. Reduce size of pads for next revision.
- Boot-button needs more space so it's easier to press

### Version 0.9 (not published to git)
- Swapped battery pins
- Swapped unstable BH1730 sensor