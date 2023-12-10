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
