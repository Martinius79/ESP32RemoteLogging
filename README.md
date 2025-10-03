# ESP32 Remote Logging

This project demonstrates how to send log messages from an ESP32 (using PlatformIO) to a remote cloud logging system (SolarWinds Observability).

## Features
- Secure HTTPS logging to SolarWinds Observability
- Automatic selection of correct Root CA (Amazon or Let's Encrypt)
- WiFi credentials and API token configurable
- Serial output shows which CA is used

## Setup

1. **SolarWinds Observability Account**
   - Sign up for SolarWinds Observability and obtain your API token.
   - Use the endpoint given to you, depending on your location, i.e. `https://logs.collector.eu-01.cloud.solarwinds.com/v1/logs`.

2. **Configure Credentials**
   - Update `src/papertrail_credentials.h` (API token, host)
   - Update `src/wifi_credentials.h` (WiFi SSID and password)

3. **PlatformIO Build**
   - Build and upload the firmware using PlatformIO:
     ```sh
     pio run --target upload --environment esp32dev
     ```

4. **Certificate Handling**
   - The firmware automatically tests which Root CA (Amazon or Let's Encrypt) is accepted by the server and uses it for all future connections.
   - The selected CA is printed to the Serial console at startup.

## Logging
- Log messages are sent to SolarWinds Observability via HTTPS POST.
- The log format follows RFC5424 syslog standards.

## Demo Video (outdated)
[![Demo Video](https://img.youtube.com/vi/GLoXRyAtytY/0.jpg)](https://www.youtube.com/watch?v=GLoXRyAtytY)

## Notes
- This project is for ESP32 using PlatformIO, not Arduino IDE.
- Papertrail is not supported anymore; only SolarWinds Observability endpoints are used.
- For troubleshooting, check the Serial console for CA selection and error messages.
- Registration to Solarwinds can be a bit tricky, because most common free mail providers are not allowed.
