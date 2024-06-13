# PanamaCityBeach

Arduino Project to drive a robotic car around a track for a 2-minute timed run. The car will autonomously drive around the track
using camera input from a HUSKYLENS camera and apply a controller to the car to adjust its speed and angle of wheels to 
drive around the track for as many laps as possible.

The controller will also take power measurements of the power supply every half second and generate an output file.
The output file will be parsed with the Python file Ouput_power_data.py to output a csv with headers and formatted data.
