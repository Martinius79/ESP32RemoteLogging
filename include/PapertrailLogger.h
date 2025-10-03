#ifndef __papertrail_logger_h__
#define __papertrail_logger_h__

#include <HTTPClient.h>
#include <WiFiClientSecure.h>

enum LogLevel {
  Error = 3,
  Warning = 4,
  Notice = 5,
  Info = 6,
  Debug = 7
};

#define BUFFER_SIZE 200

class PapertrailLogger: public Print  {
  private:
    String mHost;
    String mApiToken;
    HTTPClient *mHttpClient;
    WiFiClientSecure *mWifiClient;
    LogLevel mLevel;
    String mSystem;
    String mContext;
    String mColor;

    uint8_t mBuffer[BUFFER_SIZE];
    int mBufferPos;
    
  public:
    PapertrailLogger(String host, String apiToken, LogLevel level, String color, String system, String context);
    size_t write(uint8_t c);
    ~PapertrailLogger();
};

#endif
