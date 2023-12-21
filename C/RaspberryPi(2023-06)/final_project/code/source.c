#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <softPwm.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <wiringPi.h>

// LED pin number
#define LED 1
#define SERVO 2

// Structure to store sunrise/sunset data
struct SunData {
    char sunrise[20];
    char sunset[20];
};

// Callback function to retrieve sunrise/sunset data
size_t write_callback(void* contents, size_t size, size_t nmemb, char* buffer) {
    strcat(buffer, (char*)contents);
    return size * nmemb;
}

// Function to convert UNIX time to standard time format
void convertUnixTimeToTimeFormat(time_t unixTime, char* timeString) {
    struct tm* timeInfo = localtime(&unixTime);
    strftime(timeString, 20, "%Y-%m-%d %H:%M:%S", timeInfo);
}

// Function to retrieve sunrise/sunset time
void getSunriseSunset(struct SunData* data) {
    // API key
    char apiKey[] = "bb7f9a44c36456c4990bc7e5dd487cab";

    // Latitude and longitude
    float latitude = 37.5665; // Latitude of Seoul (Modify according to your location)
    float longitude = 126.9780; // Longitude of Seoul (Modify according to your location)

    // Create API URL
    char url[200];
    snprintf(url, sizeof(url), "https://api.openweathermap.org/data/2.5/weather?lat=%f&lon=%f&appid=%s", latitude, longitude, apiKey);

    // Use libcurl to request API data
    CURL* curl = curl_easy_init();
    if (curl) {
        char apiResponse[10000] = "";
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, apiResponse);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        // JSON parsing
        struct json_object* root, * sys, * sunrise, * sunset;
        root = json_tokener_parse(apiResponse);
        json_object_object_get_ex(root, "sys", &sys);
        json_object_object_get_ex(sys, "sunrise", &sunrise);
        json_object_object_get_ex(sys, "sunset", &sunset);
        time_t sunriseUnixTime = json_object_get_int64(sunrise);
        time_t sunsetUnixTime = json_object_get_int64(sunset);
        convertUnixTimeToTimeFormat(sunriseUnixTime, data->sunrise);
        convertUnixTimeToTimeFormat(sunsetUnixTime, data->sunset);

        // Release JSON object
        printf("API Response: %s\n", apiResponse);
        json_object_put(root);
    }
}

int main() {
    // Initialize GPIO
    if (wiringPiSetup() == -1)
    {
        return -1;
    }

    // Set LED pin mode
    pinMode(LED, OUTPUT);
    pinMode(SERVO, OUTPUT);
    softPwmCreate(SERVO, 0, 200);

    // Create a structure to store sunrise/sunset data
    struct SunData sunData;

    // Retrieve sunrise/sunset time
    getSunriseSunset(&sunData);

    while (1) {
        // Change currenttime
        struct tm currentTime;
        time_t now = time(NULL);
        currentTime = *localtime(&now);
        currentTime.tm_hour = 5;
        currentTime.tm_min = 0;
        currentTime.tm_sec = 0;
        now = mktime(&currentTime);
        
        char currentTimeString[20];
        convertUnixTimeToTimeFormat(now, currentTimeString);
        printf("CurrentTime : %s\n",currentTimeString);

        // Control LED
        if (strcmp(currentTimeString, sunData.sunrise) >= 0 && strcmp(currentTimeString, sunData.sunset) <= 0) 
        {
            digitalWrite(LED, HIGH);  // Turn on the LED
            printf("sunrise time : %s \n", sunData.sunrise);
            softPwmWrite(SERVO, 15);
            delay(1000);
        }
        else {
            digitalWrite(LED, LOW);  // Turn off the LED
            printf("sunset time : %s \n", sunData.sunset);
            softPwmWrite(SERVO, 24);
            delay(1000);
        }
    }

    return 0;
}
