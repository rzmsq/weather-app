#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

class WeatherClient
{
private:
    std::string apiKey;

    using json = nlohmann::json;
    json data;

    std::map<std::string, std::string> coords;

    void getCityUser(const std::string &);
    void getAPIdata(const std::string &);
    static size_t writeCallback(void *, size_t, size_t, void *);
    const void getAPIkey();

public:
    WeatherClient(/* args */);
    ~WeatherClient();

    void getWeatherData(const std::string &city);
};

WeatherClient::WeatherClient()
{
    getAPIkey();
}

WeatherClient::~WeatherClient()
{
}

const void WeatherClient::getAPIkey()
{
    std::ifstream f("api.key");

    if (f.is_open())
    {
        getline(f, apiKey);
        f.close();
    }
    else
        std::cerr << "Error cant open key";
    return;
}

void WeatherClient::getCityUser(const std::string &city)
{
    const std::string url = "http://api.openweathermap.org/geo/1.0/direct?q=" + city + "&limit=5" + "&appid=" + apiKey;
    getAPIdata(url);

    const double lat = data[0]["lat"];
    const double lon = data[0]["lon"];

    coords["lat"] = std::to_string(lat);
    coords["lon"] = std::to_string(lon);
}

void WeatherClient::getWeatherData(const std::string &city)
{
    getCityUser(city);

    const std::string url = "https://api.openweathermap.org/data/2.5/weather?lat=" + coords["lat"] + "&lon=" + coords["lon"] + "&appid=" + apiKey;
    getAPIdata(url);

    std::cerr << data["weather"]["description"];
}

size_t WeatherClient::writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

void WeatherClient::getAPIdata(const std::string &url)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl)
    {
        while (res != CURLE_OK)
        {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
        }
        curl_easy_cleanup(curl);
        data = json::parse(readBuffer.c_str());
    }
}

int main(int argc, char const *argv[])
{
    WeatherClient w = WeatherClient();

    w.getWeatherData("London");

    return 0;
}
