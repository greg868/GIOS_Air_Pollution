/**The MIT License (MIT)
 
 Copyright (c) 2020 by Chris Klinger
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <ESPWiFi.h>
#include <WiFiClient.h>
#include "GIOSAirPollution.h"

String PATH_SEPARATOR = "/";

GIOSAirPollution::GIOSAirPollution() {
}

void GIOSAirPollution::update(GIOSAirPollutionData *data, String stationId) {
  doUpdate(data, buildPath(stationId));
}

String GIOSAirPollution::buildPath(String stationId) {
  return "/pjp-api/rest/aqindex/getIndex/" + stationId;
}

void GIOSAirPollution::doUpdate(GIOSAirPollutionData *data, String path) {
  unsigned long lostTest = 10000UL;
  unsigned long lost_do = millis();
  this->data = data;
  JsonStreamingParser parser;
  parser.setListener(this);
  Serial.printf("[HTTP] Requesting resource at http://%s:%u%s\n", host.c_str(), port, path.c_str());

  WiFiClient client;
  #if defined(ESP8266)
  if (client.connect(host, port)) {
    #else
    if (client.connect(host.c_str(), port)) {
    #endif
        bool isBody = false;
        char c;
        Serial.println("[HTTP] connected, now GETting data");
        client.print("GET " + path + " HTTP/1.1\r\n"
                                    "Host: " +
                    host + "\r\n"
                            "Connection: close\r\n\r\n");

        while (client.connected() || client.available()) {
        if (client.available()) {
            if ((millis() - lost_do) > lostTest) {
            Serial.println("[HTTP] lost in client with a timeout");
            client.stop();
            ESP.restart();
            }
            c = client.read();
            if (c == '{' || c == '[') {
            isBody = true;
            }
            if (isBody) {
            parser.parse(c);
            }
        }
        // give WiFi and TCP/IP libraries a chance to handle pending events
        yield();
        }
        client.stop();
    } else {
        Serial.println("[HTTP] failed to connect to host");
    }
    this->data = nullptr;
}

void GIOSAirPollution::whitespace(char c) {
  Serial.println("whitespace");
}

void GIOSAirPollution::startDocument() {
  Serial.println("start document");
}

void GIOSAirPollution::key(String key) {
  currentKey = key;
}

void GIOSAirPollution::value(String value) {
    if (currentKey == "id") {
        this->data->id = value.toInt();
    }
    if (currentKey == "stCalcDate") {
        this->data->stCalcDate = value;
    }
    if (currentParent.startsWith("/ROOT/stIndexLevel")) {
        if (currentKey == "id") {
            this->data->summary.id = value.toInt();
        }
        if (currentKey == "indexLevelName") {
            this->data->summary.levelName = value;
        }
    }
    if (currentKey == "stSourceDataDate") {
        this->data->stSourceDataDate = value;
    }
    if (currentKey == "so2CalcDate") {
        this->data->so2CalcDate = value;
    }
    if (currentParent.startsWith("/ROOT/so2IndexLevel")) {
        if (currentKey == "id") {
            this->data->so2.id = value.toInt();
        }
        if (currentKey == "indexLevelName") {
            this->data->so2.levelName = value;
        }
    }
    if (currentKey == "so2SourceDataDate") {
        this->data->so2SourceDataDate = value;
    }
    if (currentKey == "no2CalcDate") {
        this->data->no2CalcDate = value;
    }
    if (currentParent.startsWith("/ROOT/no2IndexLevel")) {
        if (currentKey == "id") {
            this->data->no2.id = value.toInt();
        }
        if (currentKey == "indexLevelName") {
            this->data->no2.levelName = value;
        }
    }
    if (currentKey == "no2SourceDataDate") {
        this->data->no2SourceDataDate = value;
    }
    if (currentKey == "pm10CalcDate") {
        this->data->pm10CalcDate = value;
    }
    if (currentParent.startsWith("/ROOT/pm10IndexLevel")) {
        if (currentKey == "id") {
            this->data->pm10.id = value.toInt();
        }
        if (currentKey == "indexLevelName") {
            this->data->pm10.levelName = value;
        }
    }
    if (currentKey == "pm10SourceDataDate") {
        this->data->pm10SourceDataDate = value;
    }
    if (currentKey == "pm25CalcDate") {
        this->data->pm25CalcDate = value;
    }
    if (currentParent.startsWith("/ROOT/pm25IndexLevel")) {
        if (currentKey == "id") {
            this->data->pm25.id = value.toInt();
        }
        if (currentKey == "indexLevelName") {
            this->data->pm25.levelName = value;
        }
    }
    if (currentKey == "pm25SourceDataDate") {
        this->data->pm25SourceDataDate = value;
    }
    if (currentKey == "o3CalcDate") {
        this->data->o3CalcDate = value;
    }
    if (currentKey == "o3IndexLevel") {
        this->data->o3IndexLevel = value;
    }
    if (currentKey == "o3SourceDataDate") {
        this->data->o3SourceDataDate = value;
    }
    if (currentKey == "stIndexStatus") {
        this->data->stIndexStatus = value;
    }
    if (currentKey == "stIndexCrParam") {
        this->data->stIndexCrParam = value;
    }
}

void GIOSAirPollution::startArray() {
  itemCounter = 0;
  
  currentParent += PATH_SEPARATOR + currentKey + "[]";
  currentKey = "";
}

void GIOSAirPollution::startObject() {
  if(currentKey == "") {
    currentKey = "_obj";
  }
  currentParent += PATH_SEPARATOR + currentKey;
}

void GIOSAirPollution::endObject() {
  if (currentParent == "/ROOT/_obj") {
    itemCounter++;
    currentKey = "";
    currentParent= currentParent.substring(0, currentParent.lastIndexOf(PATH_SEPARATOR));
    }
}

void GIOSAirPollution::endDocument() {
}

void GIOSAirPollution::endArray() {
  currentKey = "";
  currentParent= currentParent.substring(0, currentParent.lastIndexOf(PATH_SEPARATOR));
}