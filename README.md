🌌 Project Overview

SkyScan is a command-line C++ application designed to help analyze night sky observations and distinguish conventional aircraft from potential unidentified aerial phenomena.

Given a file of sighting reports (with apparent speed and brightness data) and a file of known aircraft signature values, the program identifies how many of those sightings match known aircraft.

Users can choose between a linear search (for smaller datasets or fewer checks) or a binary search (after sorting the data) for efficiency. The program reports how long the operation took in microseconds and outputs the total number of matches to a results file.
