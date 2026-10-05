#include "userFunctions.hpp"
#include "main.hpp"

void debugMsg(const char *msg, msgType type)
{
    if (DEBUG)
    {
        switch (type)
        {
        case MSG_INFO:
            Serial.print("[INFO]\t");
            break;
        case MSG_WARNING:
            Serial.print("[WARNING]\t");
            break;
        case MSG_ERROR:
            Serial.print("[ERROR]\t");
            break;
        }
        Serial.println(msg);
    }
};

String httpGETRequest(const char *serverName)
{
    HTTPClient http;

    // Your IP address with path or Domain name with URL path
    http.begin(serverName);

    // Send HTTP POST request
    int httpResponseCode = http.GET();

    String payload = "{}";

    if (httpResponseCode > 0)
    {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();

    return payload;
}

const char *getWlanStatus(long rssi)
{
    int quality = 2 * (rssi + 100);
    quality = constrain(quality, 0, 100);

    if (quality > 75)
    {
        return "\ue21f";
    }
    else if (quality > 50)
    {
        return "\ue220";
    }
    else if (quality > 25)
    {
        return "\ue221";
    }
    else
    {
        return "\ue222";
    }
}

const char *getTimeString()
{
    static char buf[6]; // "HH:MM" + '\0'
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
        snprintf(buf, sizeof(buf),
                 "%02d:%02d",
                 timeinfo.tm_hour,
                 timeinfo.tm_min);
    }
    else
    {
        strcpy(buf, "--:--");
    }
    return buf;
}

uint8_t drawWeatherIcon(const String &icon)
{
    debugMsg(icon.c_str(), MSG_INFO);
    if (icon == "01d" || icon == "01n")
        return (69); // Sonne
    else if (icon == "02d" || icon == "02n")
        return (65); // Sonne + Wolke
    else if (icon == "03d" || icon == "03n" ||
             icon == "04d" || icon == "04n")
        return (64); // Wolken
    else if (icon == "09d" || icon == "09n" ||
             icon == "10d" || icon == "10n" || icon == "11d" || icon == "11n" || icon == "13d" || icon == "13n")
        return (67); // Regen
    else
        return (0); // Default icon (no icon)F
}

float getTemperature()
{
    float temperature = dht.readTemperature();

    while (isnan(temperature))
    {
        debugMsg("Failed to read from DHT sensor!", MSG_ERROR);
        return NAN; // Return NaN to indicate an error
    }

    debugMsg(
        ("DHT sensor read successful: " + String(temperature) + "°C").c_str(),
        MSG_INFO);

    return temperature;
}

struct TempRange
{
    float min;
    float max;
};

int *getTempRange(const int temps[], size_t count)
{
    static int range[2];

    range[0] = temps[0]; // min
    range[1] = temps[0]; // max

    for (size_t i = 1; i < count; i++)
    {
        if (temps[i] < range[0])
            range[0] = temps[i];

        if (temps[i] > range[1])
            range[1] = temps[i];
    }

    return range;
}