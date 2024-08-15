#include "weather_client.hpp"

int main(int argc, char const *argv[])
{
    WeatherClient w = WeatherClient();

    w.getWeatherData("London");

    return 0;
}
