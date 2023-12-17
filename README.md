# mts (Modulový traťový souhlas)

## Co už umíme
* zablikat diodou

### Než spustíme projekt
Je potřeba si nastavit PIN, na kterém poběží dioda:
```
idf.py menuconfig
# MTS Configuration
```
Pak můžeme buildit a flashnout:
```
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

## Připrava na Linuxu
```
# abychom mohli flashovat ESP, je třeba být v dialout groupě
sudo usermod -a -G dialout $USER
```

## Materiály
* [ESP32 S3 technické parametry](https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/hw-reference/esp32s3/user-guide-devkitc-1.html)
* [ESP32 S3 PIN layout](https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32s3/_images/ESP32-S3_DevKitC-1_pinlayout_v1.1.jpg)
* [C reference](https://en.cppreference.com/w/c)
* [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/v5.1.2/esp32s3/index.html)
* [BLE MESH Terminology](https://docs.espressif.com/projects/esp-idf/en/v5.1.2/esp32s3/api-guides/esp-ble-mesh/ble-mesh-terminology.html)
