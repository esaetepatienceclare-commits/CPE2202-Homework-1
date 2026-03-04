#include <Arduino.h>

struct SensorData {
 float temperature ;
 float humidity ;
 int light ;
};

// TODO : Step 1 - Identify and fix the memory leak in this function .
// TODO : Step 3 - Modify this function to accept an existing SensorData struct by reference .
SensorData * readSensors () {
 SensorData * data = new SensorData ;
 data -> temperature = random (200 , 300) / 10.0;
 data -> humidity = random (300 , 600) / 10.0;
 data -> light = random (0, 1024) ;
 return data ;
}

void setup () {
 Serial . begin (115200) ;
 while (! Serial );
}

void loop () {
 SensorData * sensor = readSensors ();

 Serial . print (" Free Heap : ");
 Serial . print (ESP. getFreeHeap ());
 Serial . print (" | Temp : ");
 Serial . print (sensor -> temperature );
 Serial . print (", Hum: ");
 Serial . print (sensor -> humidity );
 Serial . print (", Light : ");
 Serial . println (sensor -> light );

 // TODO : Step 1 - Add the missing code here to prevent the memory leak .
 delete sensor;

 delay (1000) ;
}

/*
DESIGN EVALUATION:

Even after fixing the memory leak, the original design (constant new/delete) is NOT appropriate for a 
long-running embedded system because:

1. Overhead of Dynamic Allocation:
   - Each new/delete operation takes processing time (microseconds to milliseconds)
   - Over 86,400 seconds in a day, with 1 reading/second = 86,400 allocations!
   - This wastes CPU cycles that could be used for actual sensor processing

2. Memory Fragmentation:
   - Repeated allocations of same size can still fragment the heap
   - Fragmentation can cause allocation failures even when enough total memory exists
   - ESP32 may crash when it can't allocate memory due to fragmentation

3. Determinism Issues:
   - Dynamic allocation time is not predictable
   - Real-time systems require predictable timing
   - Could cause missed deadlines in time-critical applications

4. Power Consumption:
   - Dynamic memory operations consume more power
   - Important for battery-powered embedded devices

The improved stack-based approach is superior because:
- No dynamic allocation overhead
- No fragmentation risk
- Deterministic execution time
- Lower power consumption
- Impossible to have memory leaks
*/