# BerlinUhr hardware
Berlin Uhr or Mengenlehreuhr, a fancy retro style clock thats a bit tricky to read: https://en.wikipedia.org/wiki/Mengenlehreuhr

## What is this

The BerlinUhr is a standalone PCB with all the needed parts on it, just connect power over the USB-C and it will run.

The [ATtiny167](https://www.microchip.com/ATtiny167) receives the actual time from the [DS3231](https://www.maximintegrated.com/en/products/analog/real-time-clocks/DS3231M.html) real time clock chip. Then displays it with 24 leds driven by the [TLC5947](https://www.ti.com/product/TLC5947). The brightness is adopted by measuring the ambient light with a [KPS-3227](https://www.kingbright.com/attachments/file/psearch/000/00/watermark00/KPS-3227SP1C(Ver.9).pdf). 

## Building it

1. PCB, source it with the gerbers: https://github.com/designer2k2/BerlinUhr_HW/tree/main/Berlin_Uhr/gerber
2. BOM, interactive BOM here: https://github.com/designer2k2/BerlinUhr_HW/tree/main/Berlin_Uhr/bom
3. Bootloader, burn the [Micronucleus](https://github.com/micronucleus/micronucleus) Bootloader to the ATtiny167 with ISP: https://github.com/ArminJo/DigistumpArduino 
4. Software, upload the Code from the scr folder: https://github.com/designer2k2/BerlinUhr_HW/tree/main/src/berlin-uhr 

## Modifiying it

- The PCB is made with KiCad: http://www.kicad-pcb.org/
- The graphics on it are made with Inkscape: https://inkscape.org/
- Parts of the graphics are converted to Footprints with svg2mod: https://github.com/mtl/svg2mod
- The BOM is made with InteractiveHtmlBom: https://github.com/openscopeproject/InteractiveHtmlBom

![Preview](https://user-images.githubusercontent.com/1591573/142769529-5a30a746-61aa-49ff-9e78-8e52ba1644e3.png)
