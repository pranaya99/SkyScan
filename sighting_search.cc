#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <chrono> // NOLINT (build/c++11)

// Timer class to measure elapsed time
class Timer
{
public:
  void Reset()
  {
    start_time = std::chrono::high_resolution_clock::now();
  }

  double ElapsedMicroseconds() const
  {
    auto end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::micro>(end_time - start_time).count();
  }

  friend std::ostream &operator<<(std::ostream &os, const Timer &timer)
  {
    os << timer.ElapsedMicroseconds() << " microseconds";
    return os;
  }

private:
  std::chrono::high_resolution_clock::time_point start_time;
};

// Class to represent a sighting
class Sighting
{
public:
  int speed;
  int brightness;

  Sighting(int s, int b) : speed(s), brightness(b) {}

  int Signature() const
  {
    return static_cast<int>(std::ceil((speed * brightness) / 10.0));
  }

  bool operator<(const Sighting &other) const
  {
    int thisSignature = this->Signature();
    int otherSignature = other.Signature();
    return (thisSignature < otherSignature) ||
           (thisSignature == otherSignature && this->speed < other.speed);
  }

  friend std::ostream &operator<<(std::ostream &os, const Sighting &s)
  {
    os << "Speed: " << s.speed << ", Brightness: " << s.brightness;
    return os;
  }
};

// Generic function to read integers from a file into a vector
std::vector<int> ReadIntegersFromFile(const std::string &filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Error: cannot open file " + filename);
  }

  std::vector<int> data;
  int value;
  while (file >> value)
  {
    data.push_back(value);
  }
  return data;
}

// Function to read sightings from a file into a vector of Sighting objects
std::vector<Sighting> ReadSightingsFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Error: cannot open file " + filename);
    }

    std::vector<Sighting> sightings;
    int speed, brightness;
    while (file >> speed >> brightness)
    {
        sightings.push_back(Sighting(speed, brightness));
    }
    return sightings;
}

// Perform a linear search for a signature in the sightings
bool LinearSearch(const std::vector<Sighting> &sightings, int signature)
{
  for (const auto &s : sightings)
  {
    if (s.Signature() == signature)
    {
      return true;
    }
  }
  return false;
}

// Perform a binary search for a signature in the sightings
bool BinarySearch(const std::vector<Sighting> &sightings, int signature)
{
  int left = 0;
  int right = sightings.size() - 1;

  while (left <= right)
  {
    int mid = left + (right - left) / 2;

    // Compare the signature at mid position with the target signature
    if (sightings[mid].Signature() == signature)
    {
      return true; // Found the signature
    }
    else if (sightings[mid].Signature() < signature)
    {
      left = mid + 1; // Search the right half
    }
    else
    {
      right = mid - 1; // Search the left half
    }
  }

  return false; // Signature not found
}

// Count matches between sightings and signatures using the specified search method
int CountMatches(const std::vector<Sighting> &sightings, const std::vector<int> &signatures, char method)
{
  int count = 0;
  for (const auto &sig : signatures)
  {
    if (method == 'l')
    {
      if (LinearSearch(sightings, sig))
      {
        count++;
      }
    }
    else if (method == 'b')
    {
      if (BinarySearch(sightings, sig))
      {
        count++;
      }
    }
  }
  return count;
}

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << " <sighting_file.dat> <signature_file.dat> <result_file.dat>" << std::endl;
    return 1;
  }

  try
  {
    std::vector<Sighting> sightings = ReadSightingsFromFile(argv[1]);
    std::vector<int> signatures = ReadIntegersFromFile(argv[2]);

    char method;
    // Handle invalid choice input with correct error message
    std::cout << "Choice of search method ([l]inear, [b]inary)? ";
    while (true)
    {

      std::cin >> method;
      if (method == 'l' || method == 'b')
      {
        break;
      }
      std::cout << "Incorrect choice" << std::endl;
    }

    if (method == 'b')
    {
      std::sort(sightings.begin(), sightings.end());
    }

    Timer timer;
    timer.Reset();
    int matchCount = CountMatches(sightings, signatures, method);
    double elapsedTime = timer.ElapsedMicroseconds();
    // Print the CPU time in the expected format
    std::cout << "CPU time: " << elapsedTime << " microseconds" << std::endl;
    std::ofstream resultFile(argv[3]);
    if (!resultFile.is_open())
    {
      throw std::runtime_error("Error: cannot open file " + std::string(argv[3]));
    }
    resultFile << matchCount << std::endl;
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
