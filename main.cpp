#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

using json = nlohmann::json;

// Callback function for curl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

// Function to make HTTP request
std::string makeRequest(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string response_string;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    return response_string;
}

int main() {
    // Make request to Nominatim API
    std::string url = "https://nominatim.openstreetmap.org/search?format=json&q=Roermond";
    std::string response = makeRequest(url);

    // Parse JSON response
    json data = json::parse(response);

    if (data.empty()) {
        std::cerr << "No data returned from API" << std::endl;
        return 1;
    }

    // Extract latitude and longitude
    double lat = data[0]["lat"].get<double>();
    double lon = data[0]["lon"].get<double>();

    std::cout << "Latitude: " << lat << ", Longitude: " << lon << std::endl;

    // Create window and draw map (placeholder)
    sf::RenderWindow window(sf::VideoMode(800, 600), "Map");
    sf::CircleShape marker(5.f);
    marker.setFillColor(sf::Color::Red);

    // Normalize coordinates to window size (this is a very simple approximation)
    float x = (lon + 180) / 360 * 800;
    float y = (90 - lat) / 180 * 600;
    marker.setPosition(x, y);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        window.draw(marker);
        window.display();
    }

    return 0;
}