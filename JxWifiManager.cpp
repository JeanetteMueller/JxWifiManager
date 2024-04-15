#include "JxWifiManager.h"

JxWifiManager::JxWifiManager()
{
    currentMode = WifiModeNetwork;

    subnetMask = IPAddress(255, 255, 255, 0);
}

bool JxWifiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void JxWifiManager::setup()
{
    Serial.println("");
    if (currentMode == WifiModeHotspot)
    {
        Serial.print("Setup Hotspot: ");
        Serial.println(hotspot_Ssid);
        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(host, host, subnetMask);
        WiFi.softAP(hotspot_Ssid, hotspot_Password, 1, false);
    }
    else
    {
        Serial.println("Connecting to WiFi...");
        WiFi.mode(WIFI_STA);
        WiFi.begin(network_Ssid, network_Password);
    }
}

void JxWifiManager::loop()
{
    if (currentMode == WifiModeHotspot)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            if (_lastTry + 5000 < millis())
            {
                _lastTry = millis();
                Serial.print("Wifi Adress: ");
                Serial.println(WiFi.localIP());
            }
        }
    }
    else
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            if (_lastTry + 100 < millis())
            {
                _lastTry = millis();

                if (_tryCount <= 120)
                {

                    Serial.print(_tryCount);
                    Serial.print('.');

                    _tryCount++;

                    if (_tryCount % 20 == 1)
                    {
                        Serial.println("");
                        Serial.println("Connecting to WiFi...");
                    }
                }
                else
                {
                    currentMode = WifiModeHotspot;
                    _lastTry = 0;
                    setup();
                }
            }
        }
        else
        {
            if (_lastTry + 5000 < millis())
            {
                _lastTry = millis();
                Serial.print("Wifi Adress: ");
                Serial.println(WiFi.localIP());
            }
        }
    }
}