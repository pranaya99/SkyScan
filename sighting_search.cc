#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>


class Sighting {
public:
    int speed;
    int brightness;

    Sighting(int s, int b) : speed(s), brightness(b) {}

    int getSignature() const {
        return static_cast<int>(std::ceil((speed * brightness) / 10.0));
    }

    bool operator<(const Sighting &other) const {
        if (getSignature() == other.getSignature()) {
            return speed < other.speed;
        }
        return getSignature() < other.getSignature();
    }

    friend std::ostream &operator<<(std::ostream &os, const Sighting &s) {
        os << "(" << s.speed << ", " << s.brightness << ") => " << s.getSignature();
        return os;
    }
};

class Timer {
    std::chrono::high_resolution_clock::time_point start_time;

public:
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double elapsed() const {
        auto end_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::micro>(end_time - start_time).count();
    }
};

std::vector<Sighting> readSightings(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        exit(1);
    }

    std::vector<Sighting> sightings;
    int speed, brightness;
    while (file >> speed >> brightness) {
        sightings.emplace_back(speed, brightness);
    }

    file.close();
    return sightings;
}


std::vector<int> readSignatures(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        exit(1);
    }

    std::vector<int> signatures;
    int signature;
    while (file >> signature) {
        signatures.push_back(signature);
    }

    file.close();
    return signatures;
}


int linearSearch(const std::vector<Sighting> &sightings, const std::vector<int> &signatures) {
    int count = 0;
    for (int sig : signatures) {
        for (const Sighting &s : sightings) {
            if (s.getSignature() == sig) {
                count++;
                break;
            }
        }
    }
    return count;
}

int binarySearch(std::vector<Sighting> &sightings, const std::vector<int> &signatures) {
    std::sort(sightings.begin(), sightings.end());

    int count = 0;
    for (int sig : signatures) {
        if (std::binary_search(sightings.begin(), sightings.end(), Sighting(0, sig), [](const Sighting &a, const Sighting &b) {
                return a.getSignature() < b.getSignature();
            })) {
            count++;
        }
    }
    return count;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <sighting_file.dat> <signature_file.dat> <result_file.dat>" << std::endl;
        return 1;
    }

    std::string sighting_file = argv[1];
    std::string signature_file = argv[2];
    std::string result_file = argv[3];

    // Read data
    std::vector<Sighting> sightings = readSightings(sighting_file);
    std::vector<int> signatures = readSignatures(signature_file);

    // Ask user for search method
    char choice;
    do {
        std::cout << "Choice of search method ([l]inear, [b]inary)? ";
        std::cin >> choice;
        if (choice != 'l' && choice != 'b') {
            std::cerr << "Incorrect choice" << std::endl;
        }
    } while (choice != 'l' && choice != 'b');

    Timer timer;
    int matches = 0;

    // Perform search
    timer.start();
    if (choice == 'l') {
        matches = linearSearch(sightings, signatures);
    } else {
        matches = binarySearch(sightings, signatures);
    }
    double elapsed_time = timer.elapsed();

    // Output performance and results
    std::cout << "CPU time: " << elapsed_time << " microseconds" << std::endl;

    std::ofstream output(result_file);
    if (!output.is_open()) {
        std::cerr << "Error: cannot open output file " << result_file << std::endl;
        return 1;
    }
    output << matches << std::endl;
    output.close();

    return 0;
}
