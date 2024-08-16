#include "weather_client.hpp"

int main(int argc, char const *argv[])
{
    static WeatherClient w = WeatherClient();

    std::cout << "Welcome!" << std::endl;
    std::cout << "Is the temperature output in Celsius or Fahrenheit? (C/F)" << std::endl;

    bool isCelsius = false;
    while (1)
    {
        std::string temp;
        getline(std::cin, temp);

        if (temp == "c" || temp == "C")
        {
            isCelsius = true;
            break;
        }
        else if (temp == "f" || temp == "F")
            break;

        std::cout << "Incorrect input!" << std::endl;
    }

    while (1)
    {
        std::cout << "Please, enter the name City (enter q for exit)" << std::endl;
        std::string city;
        getline(std::cin, city);

        if (city == "q")
            break;

        w.getWeatherData(city);
        if (w.getIsSucess())
            w.printWeatherData(isCelsius);
        else
            std::cout << "The city was entered incorrectly or a server error occurred" << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
