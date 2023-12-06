#pragma once
#include <JsonListener.h>
#include <JsonStreamingParser.h>

typedef struct summaryData {
    uint16_t id; // 3
    String levelName; // Umiarkowany
} summaryData;

typedef struct so2Data {
    uint16_t id; // 3
    String levelName; // Umiarkowany
} so2Data;

typedef struct no2Data {
    uint16_t id; // 3
    String levelName; // Umiarkowany
} no2Data;

typedef struct pm10Data {
    uint16_t id; // 3
    String levelName; // Umiarkowany
} pm10Data;

typedef struct pm25Data {
    uint16_t id; // 3
    String levelName; // Umiarkowany
} pm25Data;

typedef struct GIOSAirPollutionData {
    uint16_t id; // 154
    String stCalcDate; // 2023-12-06 15:20:12
    summaryData summary;
    String stSourceDataDate; // 2023-12-06 14:00:00
	String so2CalcDate; // 2023-12-06 15:20:12
    so2Data so2;
    String so2SourceDataDate; // null
    String no2CalcDate; // 2023-12-06 15:20:12
    no2Data no2;
    String no2SourceDataDate; // 2023-12-06 15:00:00
	String pm10CalcDate; // 2023-12-06 15:20:12
    pm10Data pm10;
    String pm10SourceDataDate; // null
	String pm25CalcDate; // 2023-12-06 15:20:12
    pm25Data pm25;
    String pm25SourceDataDate; // 2023-12-06 14:00:00
	String o3CalcDate; // null
	String o3IndexLevel; // null
	String o3SourceDataDate; // null
	String stIndexStatus; // true
	String stIndexCrParam; // PYL
} GIOSAirPollutionData;

class GIOSAirPollution: public JsonListener {
  private:
    const String host = "api.gios.gov.pl";
    const uint8_t port = 80;
    String currentKey = "ROOT";
    String currentParent;
    GIOSAirPollutionData *data;
    uint8_t itemCounter = 0;

    void doUpdate(GIOSAirPollutionData *data, String path);
    String buildPath(String stationId);

  public:
    GIOSAirPollution();
    void update(GIOSAirPollutionData *data, String stationId);
    virtual void whitespace(char c);
    virtual void startDocument();
    virtual void key(String key);
    virtual void value(String value);
    virtual void endArray();
    virtual void endObject();
    virtual void endDocument();
    virtual void startArray();
    virtual void startObject();
};

