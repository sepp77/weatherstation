#ifndef WEATHERSTATION_HPP
#define WEATHERSTATION_HPP

// ------------- class -----------------------
class WeatherStation{
    public:
        WeatherStation();
        ~WeatherStation();
        void begin();
        void update();
        
    private:
        // Add private members and methods as needed
        float temperature;
        float humidity;       
};

#endif // WEATHERSTATION_HPP