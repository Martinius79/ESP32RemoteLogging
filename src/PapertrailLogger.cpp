#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include "PapertrailLogger.h"

// Amazon Root CA 1
const char* amazon_root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEkjCCA3qgAwIBAgITBn+USionzfP6wq4rAfkI7rnExjANBgkqhkiG9w0BAQsF\n" \
"ADCBmDELMAkGA1UEBhMCVVMxEDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTClNj\n" \
"b3R0c2RhbGUxJTAjBgNVBAoTHFN0YXJmaWVsZCBUZWNobm9sb2dpZXMsIEluYy4x\n" \
"OzA5BgNVBAMTMlN0YXJmaWVsZCBTZXJ2aWNlcyBSb290IENlcnRpZmljYXRlIEF1\n" \
"dGhvcml0eSAtIEcyMB4XDTE1MDUyNTEyMDAwMFoXDTM3MTIzMTAxMDAwMFowOTEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
"jgSubJrIqg0CAwEAAaOCATEwggEtMA8GA1UdEwEB/wQFMAMBAf8wDgYDVR0PAQH/\n" \
"BAQDAgGGMB0GA1UdDgQWBBSEGMyFNOy8DJSULghZnMeyEE4KCDAfBgNVHSMEGDAW\n" \
"gBScXwDfqgHXMCs4iKK4bUqc8hGRgzB4BggrBgEFBQcBAQRsMGowLgYIKwYBBQUH\n" \
"MAGGImh0dHA6Ly9vY3NwLnJvb3RnMi5hbWF6b250cnVzdC5jb20wOAYIKwYBBQUH\n" \
"MAKGLGh0dHA6Ly9jcnQucm9vdGcyLmFtYXpvbnRydXN0LmNvbS9yb290ZzIuY2Vy\n" \
"MD0GA1UdHwQ2MDQwMqAwoC6GLGh0dHA6Ly9jcmwucm9vdGcyLmFtYXpvbnRydXN0\n" \
"LmNvbS9yb290ZzIuY3JsMBEGA1UdIAQKMAgwBgYEVR0gADANBgkqhkiG9w0BAQsF\n" \
"AAOCAQEAYjdCXLwQtT6LLOkMm2xF4gcAevnFWAu5CIw+7bMlPLVvUOTNNWqnkzSW\n" \
"MiGpSESrnO09tKpzbeR/FoCJbM8oAxiDR3mjEH4wW6w7sGDgd9QIpuEdfF7Au/ma\n" \
"eyKdpwAJfqxGF4PcnCZXmTA5YpaP7dreqsXMGz7KQ2hsVxa81Q4gLv7/wmpdLqBK\n" \
"bRRYh5TmOTFffHPLkIhqhBGWJ6bt2YFGpn6jcgAKUj6DiAdjd4lpFw85hdKrCEVN\n" \
"0FE6/V1dN2RMfjCyVSRCnTawXZwXgWHxyvkQAiSr6w10kY17RSlQOYiypok1JR4U\n" \
"akcjMS9cmvqtmg5iUaQqqcT5NJ0hGA==\n" \
"-----END CERTIFICATE-----\n";

// Let's Encrypt R3 Root CA
const char* letsencrypt_root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFBjCCAu6gAwIBAgIRAMISMktwqbSRcdxA9+KFJjwwDQYJKoZIhvcNAQELBQAw\n" \
"TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
"cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMjQwMzEzMDAwMDAw\n" \
"WhcNMjcwMzEyMjM1OTU5WjAzMQswCQYDVQQGEwJVUzEWMBQGA1UEChMNTGV0J3Mg\n" \
"RW5jcnlwdDEMMAoGA1UEAxMDUjEyMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIB\n" \
"CgKCAQEA2pgodK2+lP474B7i5Ut1qywSf+2nAzJ+Npfs6DGPpRONC5kuHs0BUT1M\n" \
"5ShuCVUxqqUiXXL0LQfCTUA83wEjuXg39RplMjTmhnGdBO+ECFu9AhqZ66YBAJpz\n" \
"kG2Pogeg0JfT2kVhgTU9FPnEwF9q3AuWGrCf4yrqvSrWmMebcas7dA8827JgvlpL\n" \
"Thjp2ypzXIlhZZ7+7Tymy05v5J75AEaz/xlNKmOzjmbGGIVwx1Blbzt05UiDDwhY\n" \
"XS0jnV6j/ujbAKHS9OMZTfLuevYnnuXNnC2i8n+cF63vEzc50bTILEHWhsDp7CH4\n" \
"WRt/uTp8n1wBnWIEwii9Cq08yhDsGwIDAQABo4H4MIH1MA4GA1UdDwEB/wQEAwIB\n" \
"hjAdBgNVHSUEFjAUBggrBgEFBQcDAgYIKwYBBQUHAwEwEgYDVR0TAQH/BAgwBgEB\n" \
"/wIBADAdBgNVHQ4EFgQUALUp8i2ObzHom0yteD763OkM0dIwHwYDVR0jBBgwFoAU\n" \
"ebRZ5nu25eQBc4AIiMgaWPbpm24wMgYIKwYBBQUHAQEEJjAkMCIGCCsGAQUFBzAC\n" \
"hhZodHRwOi8veDEuaS5sZW5jci5vcmcvMBMGA1UdIAQMMAowCAYGZ4EMAQIBMCcG\n" \
"A1UdHwQgMB4wHKAaoBiGFmh0dHA6Ly94MS5jLmxlbmNyLm9yZy8wDQYJKoZIhvcN\n" \
"AQELBQADggIBAI910AnPanZIZTKS3rVEyIV29BWEjAK/duuz8eL5boSoVpHhkkv3\n" \
"4eoAeEiPdZLj5EZ7G2ArIK+gzhTlRQ1q4FKGpPPaFBSpqV/xbUb5UlAXQOnkHn3m\n" \
"FVj+qYv87/WeY+Bm4sN3Ox8BhyaU7UAQ3LeZ7N1X01xxQe4wIAAE3JVLUCiHmZL+\n" \
"qoCUtgYIFPgcg350QMUIWgxPXNGEncT921ne7nluI02V8pLUmClqXOsCwULw+PVO\n" \
"ZCB7qOMxxMBoCUeL2Ll4oMpOSr5pJCpLN3tRA2s6P1KLs9TSrVhOk+7LX28NMUlI\n" \
"usQ/nxLJID0RhAeFtPjyOCOscQBA53+NRjSCak7P4A5jX7ppmkcJECL+S0i3kXVU\n" \
"y5Me5BbrU8973jZNv/ax6+ZK6TM8jWmimL6of6OrX7ZU6E2WqazzsFrLG3o2kySb\n" \
"zlhSgJ81Cl4tv3SbYiYXnJExKQvzf83DYotox3f0fwv7xln1A2ZLplCb0O+l/AK0\n" \
"YE0DS2FPxSAHi0iwMfW2nNHJrXcY3LLHD77gRgje4Eveubi2xxa+Nmk/hmhLdIET\n" \
"iVDFanoCrMVIpQ59XWHkzdFmoHXHBV7oibVjGSO7ULSQ7MJ1Nz51phuDJSgAIU7A\n" \
"0zrLnOrAj/dfrlEWRhCvAgbuwLZX1A2sjNjXoPOHbsPiy+lO1KF8/XY7\n" \
"-----END CERTIFICATE-----\n";

const int FacilityCode = 16;

const char* selected_root_ca = nullptr;

PapertrailLogger::PapertrailLogger(String host, String apiToken, LogLevel level, String color, String system, String context) {
  mHost = host;
  mApiToken = apiToken;
  mLevel = level;
  mColor = color;
  mSystem = system;
  mContext = context;
  mBufferPos = 0;
  mHttpClient = new HTTPClient();
  mWifiClient = new WiFiClientSecure();

  // ESP32-specific SSL optimizations
  mWifiClient->setHandshakeTimeout(45000); // 45 seconds for SSL handshake
  mWifiClient->setTimeout(30000); // 30 second timeout

  // Test which CA works once
  String url = "https://logs.collector." + mHost.substring(mHost.indexOf("eu-01")) + "/v1/logs";
  String caTestMsg = String("CA Test - system: ") + mSystem + ", context: " + mContext + ", LogLevel: " + mLevel;
  mWifiClient->setCACert(amazon_root_ca);
  if (mHttpClient->begin(*mWifiClient, url)) {
    mHttpClient->setTimeout(30000);
    mHttpClient->setConnectTimeout(25000);
    mHttpClient->addHeader("Content-Type", "application/octet-stream");
    mHttpClient->addHeader("Authorization", "Bearer " + mApiToken);
    int testCode = mHttpClient->POST(caTestMsg);
    mHttpClient->end();
    if (testCode > 0) {
      selected_root_ca = amazon_root_ca;
  Serial.println("[CA SELECT] Using Amazon Root CA 1.");
      return;
    }
  }
  mWifiClient->setCACert(letsencrypt_root_ca);
  if (mHttpClient->begin(*mWifiClient, url)) {
    mHttpClient->setTimeout(30000);
    mHttpClient->setConnectTimeout(25000);
    mHttpClient->addHeader("Content-Type", "application/octet-stream");
    mHttpClient->addHeader("Authorization", "Bearer " + mApiToken);
    int testCode = mHttpClient->POST(caTestMsg);
    mHttpClient->end();
    if (testCode > 0) {
      selected_root_ca = letsencrypt_root_ca;
  Serial.println("[CA SELECT] Using Let's Encrypt Root CA.");
      return;
    }
  }
  // Fallback: default to Amazon if both fail
  selected_root_ca = amazon_root_ca;
  Serial.println("[CA SELECT] Fallback: Using Amazon Root CA 1.");
}

PapertrailLogger::~PapertrailLogger() {
  if (mHttpClient) {
    mHttpClient->end();
    delete mHttpClient;
  }
  if (mWifiClient) {
    mWifiClient->stop();
    delete mWifiClient;
  }
}

size_t PapertrailLogger::write(uint8_t c) {
  // check to see if we've reached the end of our buffer or received a newline character
  if (c == '\n' || mBufferPos == BUFFER_SIZE - 1) {
    // add a null terminating byte to the buffer
    mBuffer[mBufferPos] = 0;
    
    // construct the syslog message (see https://tools.ietf.org/html/rfc5424#page-19)
    String syslogMessage = "<" + String(FacilityCode*8 + mLevel) + ">1 - " + mSystem + " " + mContext + " - - - " + mColor + String((const char *) mBuffer);
    
    // Send the syslog message to SolarWinds Observability using HTTPS POST
    // Complete URL with /v1/logs endpoint
    String url = "https://logs.collector." + mHost.substring(mHost.indexOf("eu-01")) + "/v1/logs";
    
    // Check WiFi connection before HTTP request
    if (WiFi.status() != WL_CONNECTED) {
      mBufferPos = 0;
      return 1;
    }

    
    // Use the selected CA for all requests
    mWifiClient->setCACert(selected_root_ca);
    if (!mHttpClient->begin(*mWifiClient, url)) {
      mBufferPos = 0;
      return 1;
    }
    mHttpClient->setTimeout(30000); // 30 seconds
    mHttpClient->setConnectTimeout(25000); // 25 seconds for connection
    mHttpClient->addHeader("Content-Type", "application/octet-stream");
    mHttpClient->addHeader("Authorization", "Bearer " + mApiToken);
    int httpResponseCode = mHttpClient->POST(syslogMessage);
    mHttpClient->end();
    if (httpResponseCode <= 0) {
      Serial.println("Error sending log to SolarWinds: " + String(httpResponseCode));
    }
    Serial.println((const char *) mBuffer);
    mBufferPos = 0;
  } else {
    // buffer the character up for sending later
    mBuffer[mBufferPos] = c;
    mBufferPos++;
  }
  
  return 1;
}
