#include "weather_client.hpp"

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
    {
        std::cerr << "Error cant open key";
        this->isSuccess = false;
    }
    return;
}

void WeatherClient::getCityUser(const std::string &city)
{
    const std::string url = "https://api.openweathermap.org/geo/1.0/direct?q=" + city + "&limit=1" + "&appid=" + apiKey;
    getAPIdata(url);
    if (!this->isSuccess)
        return;

    const double lat = data[0]["lat"];
    const double lon = data[0]["lon"];

    coords["lat"] = std::to_string(lat);
    coords["lon"] = std::to_string(lon);
}

void WeatherClient::getWeatherData(const std::string &city)
{
    getCityUser(city);
    if (!this->isSuccess)
        return;

    const std::string url = "https://api.openweathermap.org/data/2.5/weather?lat=" + coords["lat"] + "&lon=" + coords["lon"] + "&units=imperial&appid=" + apiKey;
    getAPIdata(url);

    proccessWeatherData();
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
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK)
            data = json::parse(readBuffer.c_str());
        else
            this->isSuccess = false;
    }
    else
    {
        std::cerr << "curl init error" << std::endl;
        this->isSuccess = false;
    }
}

void WeatherClient::proccessWeatherData()
{
    this->weatherInfo["temp"] = to_string(data["main"]["feels_like"]);
    this->weatherInfo["humidity"] = to_string(data["main"]["humidity"]);
    this->weatherInfo["weather"] = data["weather"][0]["description"];
    this->weatherInfo["wind"] = to_string(data["wind"]["speed"]);
}

void WeatherClient::printWeatherData(const bool &isCelsius)
{
    std::string temp;
    if (isCelsius)
    {
        double t = 0.555555556 * (stod(weatherInfo["temp"]) - 32);
        temp = std::to_string(t) + " C";
    }
    else
        temp = this->weatherInfo["temp"] + " F";

    std::cout << "Weather: " + this->weatherInfo["weather"] << std::endl;
    std::cout << "Temperature: " + temp << std::endl;
    std::cout << "Humidity: " + this->weatherInfo["humidity"] << std::endl;
    std::cout << "Wind: " + this->weatherInfo["wind"] + " m/s" << std::endl;
}

bool WeatherClient::getIsSucess() const
{
    return this->isSuccess;
}