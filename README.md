# Drone Arm NeoPixel Controller

Sterownik taśm NeoPixel zamontowanych na ramionach drona.
System reaguje na sygnał PWM (np. z kontrolera lotu / odbiornika RC) i zmienia tryb oświetlenia w zależności od szerokości impulsu.

Projekt oparty na ESP32.

---

## Tryby pracy (na podstawie PWM)

### PWM 800–1200 µs (tryb niski)
- Większość ramion: jasne białe światło
- Boki (strip 2 i 5): zielony blink

---

### PWM 1800–2200 µs (tryb wysoki)
- Przód (strip 0, 1): czerwony
- Środek (strip 3, 4): zielony
- Boki (strip 2, 5): zielony blink

---

### Brak PWM / inna wartość
- Wszystkie paski: delikatne białe podświetlenie (niska jasność)

---

## PWM

- Wejście PWM mierzone jest w mikrosekundach
- Obsługiwany zakres: ~1000–2000 µs
- Pomiar realizowany przez przerwania (zbocze narastające i opadające)
- Pin wejściowy: PWM_IN = GPIO16

---

## Konfiguracja LED

- Liczba pasków: `STRIP_NUM`
- Liczba LED na pasku: `LED_NUM`
- Mapowanie pinów:

| Strip Index | GPIO |
|-------------|------|
| 0 | 0  |
| 1 | 1  |
| 2 | 2  |
| 3 | 21 |
| 4 | 22 |
| 5 | 23 |

Sterowanie realizowane przez RMT (ESP32).

---

## Dostępne efekty

- `backlight(brightness)` – białe podświetlenie
- `colour(R,G,B)` – stały kolor
- `blink(R,G,B)` – mruganie kolorem
- `animate()` – animacja z płynną zmianą koloru (HSV)

---

## Podłączenie

### NeoPixele
- Każdy pasek podłączony do osobnego GPIO
- Wspólna masa (GND)

### PWM
- Sygnał PWM → GPIO16
- Wspólna masa z kontrolerem lotu

---

## Uwagi

- Aktualnie animacja jest wyłączona w trybie wysokiego PWM (zakomentowana sekcja).
- System nie sprawdza błędów LED – zakłada poprawne podłączenie.
