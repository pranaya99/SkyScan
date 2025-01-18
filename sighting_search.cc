#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <chrono> // NOLINT (build/c++11)

/* Timer class to measure elapsed time in microseconds */
class Timer
{
public:
  /* Resets the timer to the current time and this method records the current time and sets it as the start time for measuring elapsed time. */
  void Reset()
  {
    start_time = std::chrono::high_resolution_clock::now();
  }
  /* Returns the elapsed time in microseconds. */
  double Elapsedtime() const
  {
    auto end_time = std::chrono::high_resolution_clock::now();
    // Calculates the time difference between current time and start time in microseconds
    return std::chrono::duration<double, std::micro>(end_time - start_time).count();
  }

  /* Overloads the << operator to print elapsed time in a nice format */
  friend std::ostream &operator<<(std::ostream &os, const Timer &timer)
  {
    os << timer.Elapsedtime() << " microseconds";
    return os;
  }

private:
  std::chrono::high_resolution_clock::time_point start_time; // Stores the starting time for measuring elapsed time
};

/* Class to represent a sighting with speed and brightness */
class Sighting
{
public:
  int speed;      // Speed of the sighting
  int brightness; // Brightness of the sighting
  /* Constructor to initialize speed and brightness */
  Sighting(int s, int b) : speed(s), brightness(b) {}

  /* Computes the signature of a sighting based on its speed and brightness
  Formula: signature = ceil((speed * brightness) / 10.0) */
  int Signature() const
  {
    return static_cast<int>(std::ceil((speed * brightness) / 10.0));
  }

  /* Comparison operator to sort sightings by tsdheir signatures, and then by speed */
  bool operator<(const Sighting &other) const
  {
    int thisSignature = this->Signature();  // Compute this object's signature
    int otherSignature = other.Signature(); // Compute other object's signature
    return (thisSignature < otherSignature) || (thisSignature == otherSignature && this->speed < other.speed);
  }
  friend std::ostream &operator<<(std::ostream &os, const Sighting &s)
  {
    os << "Speed: " << s.speed << ", Brightness: " << s.brightness;
    return os;
  }
};


std::vector<int> Read_int_from_File(const std::string &filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Error: cannot open file " + filename);
  }

  std::vector<int> data; // Vector to store the read integers
  int value;
  while (file >> value) // Read integers one by one until EOF
  {
    data.push_back(value);
  }
  return data; // Return the vector of integers
}


std::vector<Sighting> Read_sightings(const std::string &filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Error: cannot open file " + filename);
  }

  std::vector<Sighting> sightings; // Vector to store the sightings
  int speed, brightness;
  while (file >> speed >> brightness) // Read pairs of speed and brightness
  {
    sightings.push_back(Sighting(speed, brightness)); // Create a Sighting object and store it in the vector
  }
  return sightings;
}

int L_Search(const std::vector<Sighting> &sightings, const std::vector<int> &signatures)
{
  int ctr = 0;
  for (size_t i = 0; i < signatures.size(); i++)
  {
    int signature = signatures[i];
    for (size_t j = 0; j < sightings.size(); j++)
    {
      const Sighting &sighting = sightings[j];
      if (sighting.Signature() == signature)
      {
        ctr++;
        break;
      }
    }
  }
  return ctr;
}


int B_Search(const std::vector<Sighting> &sightings, const std::vector<int> &signatures)
{
  int ctr = 0;
  for (const auto &sig : signatures)
  {
    int left = 0;
    int right = sightings.size() - 1;
    bool found = false; 

    while (left <= right)
    {
      int mid = left + (right - left) / 2;
      if (sightings[mid].Signature() == sig)
      {
        ctr++;
        found = true;
        break;
      }
      else if (sightings[mid].Signature() < sig)
      {
        left = mid + 1;
      }
      else
      {
        right = mid - 1;
      }
    }

    if (!found)
    {
      continue;
    }
  }
  return ctr;
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
    std::vector<Sighting> sightings = Read_sightings(argv[1]);
    std::vector<int> signatures = Read_int_from_File(argv[2]);

    char method;

    std::cout << "Choice of search method ([l]inear, [b]inary)?\n";
    while (true)
    {
      std::cin >> method;
      if (method == 'l' || method == 'b')
      {
        break;
      }
      std::cerr << "Incorrect choice" << std::endl;
    }
    if (method == 'b')
    {
      std::sort(sightings.begin(), sightings.end());
    }
    
    Timer timer;
    timer.Reset(); // Reset timer
    int ctr;
    if (method == 'l')
    {
      ctr = L_Search(sightings, signatures);
    }
    else
    {
      ctr = B_Search(sightings, signatures);
    }
    double elapsedTime = timer.Elapsedtime();
    std::cout << "CPU time: " << elapsedTime << " microseconds" << std::endl;
    std::ofstream resultFile(argv[3]);
    if (!resultFile.is_open())
    {
      throw std::runtime_error("Error: cannot open file " + std::string(argv[3]));
    }
    resultFile << ctr << std::endl;
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}