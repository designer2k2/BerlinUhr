# BerlinUhr
Berlin Uhr or Mengenlehreuhr, a fancy retro style clock that's a bit tricky to read: https://en.wikipedia.org/wiki/Mengenlehreuhr

## What is this

The BerlinUhr is a standalone PCB with all the needed parts on it, just connect power over the USB-C and it will run.

The [ATtiny167](https://www.microchip.com/ATtiny167) receives the actual time from the [DS3231](https://www.maximintegrated.com/en/products/analog/real-time-clocks/DS3231M.html) real time clock chip. Then displays it with 24 leds driven by the [TLC5947](https://www.ti.com/product/TLC5947). The brightness is adopted by measuring the ambient light with a [KPS-3227](https://www.kingbright.com/attachments/file/psearch/000/00/watermark00/KPS-3227SP1C(Ver.9).pdf). 

## Building it

1. PCB, source it with the gerbers: https://github.com/designer2k2/BerlinUhr/tree/main/Berlin_Uhr/gerber
2. Schematic: https://github.com/designer2k2/BerlinUhr/blob/main/Berlin_Uhr/Schematic.pdf
3. BOM, interactive BOM here: https://github.com/designer2k2/BerlinUhr/tree/main/Berlin_Uhr/bom
4. Bootloader, burn the [Micronucleus](https://github.com/micronucleus/micronucleus) Bootloader to the ATtiny167 with ISP: https://github.com/ArminJo/DigistumpArduino 
5. Insert CR1216 battery for the RTC to keep time also while the USB power is gone.
6. Software, upload the Code from the scr folder: https://github.com/designer2k2/BerlinUhr/tree/main/src/berlin-uhr 


## Setting the time

1. The time gets set the first time the code is uploaded to the time from the pc.
2. To change it afterwards, push the A and B buttons on the edge / backside.

## Reading the time

This is special on the Berlin Clock. A practical explanation is given in the wikipedia article: https://en.wikipedia.org/wiki/Mengenlehreuhr#Time_encoding

The Berlin Clock is easy to read after a couple of days :-)

## Modifying it

- The PCB is made with KiCad (6.0.2): http://www.kicad-pcb.org/
- The PCB-Art graphics are made with Inkscape: https://inkscape.org/
- Parts of the graphics are converted to Footprints with svg2mod: https://github.com/mtl/svg2mod
- The BOM is made with InteractiveHtmlBom: https://github.com/openscopeproject/InteractiveHtmlBom

KiCad Version of at least 6.0.2 is needed due to a bug in the KPS-3227 Footprint in older Versions: https://gitlab.com/kicad/libraries/kicad-symbols/-/issues/3164

## How it looks

![grafik](https://user-images.githubusercontent.com/1591573/145868070-2f7428de-198e-460a-b480-efdc99f8b2e2.png)

Its 17:45 :smiley:

It can hang on a wall or sit on a table with the help of a USB-C phone docking station.


The bare PCB:

![grafik](https://user-images.githubusercontent.com/1591573/145869008-ae3285b8-d30f-4078-b08b-50e39cbf8119.png)

ISP Programming the Bootloader to the ATtiny with the help of a Arduino Uno:

![grafik](https://user-images.githubusercontent.com/1591573/145869260-832a9c8c-31fa-4e75-bda4-909f35da3e4d.png)

Populated backside:

![grafik](https://user-images.githubusercontent.com/1591573/145869349-2cbfdd7d-9a13-4a76-96ba-92dfe215c420.png)

