/*
    Основной объект, реализующий получения данных из api, из обработку и вывод
    
    Конструктор класса присваивает api ключ к переменной apiKey

    Используй метод getWeatherData(city) для извлечения данных о погоде из api
        так как для извлечения данных о погоде, необходимо передавать коорд. города
        данный метод в своём теле первостепенно вызывает метод getCityUser(city)
        который извлекает из api коорд. юзера, после чего отправляется второй запрос по api
        для получения данных о погоде и записи их в атрибут data
*/

#pragma once

#include "main.hpp"

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