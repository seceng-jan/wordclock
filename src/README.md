# Wordclock

Code für die slebstgebaute Wortclock mit ESP32 und Apple HomeKit Anbidnung.

## Nutzung

Die LEDs vom Typ WS2812 sind verkabelt von oben links ("E" --> 0) bis unten rechts ("R" --> 109). Dabei sind die Spalten abwechselnd von oben nach unten und dann von unten nach oben verkabelt. D.h., das "B" unten links hat die Nummer 9, das "S" rechts daneben die Nummer 10, das "S" in der ersten Zeile oben die Nummer 19.

Damit die Uhr funktioniert, muss sie zwingend mit dem WLAN verbunden sein. Dazu muss die Uhr mit dem Computer verbunden werden (Mini-USB auf der Rückseite) und über eine serielle Verbindung Konfiguriert werden. Das geht zum Beispiel mit VSCode (+ Serial Monitor Extension). Die Baudrate ist 115200.

Für die WLAN Verbindung, nach Anschluss der Uhr über die serielle Schnittstelle "W" eingeben und den Anweisungen folgen.

## Entwicklung
Für die Entwicklund sind folgende Libraries nötig:

- HomeSpan Libary -- Version 1.9.1
- Adafruit NeoPixel -- Version 1.3.3
- esp32 Board -- Version 2.0.17

Der Code ist verfügbar auf: https://github.com/seceng-jan/wordclock