#include <Arduino.h>

#define BUFFER_SIZE 64

// TODO : Implement this templated CircularBuffer class
template <typename T>
class CircularBuffer {
private :
 T buffer [ BUFFER_SIZE ];
 int head ; // Points to the next location to write to
 int tail ; // Points to the next location to read from
 int count ; // Number of elements currently in the buffer

public :
 CircularBuffer () : head (0) , tail (0) , count (0) {}

 // TODO : Add an item to the buffer . Return true if successful .
 bool enqueue (T item ) {
   // Your code here ( check for full , then add , update head and count )
   if (count == BUFFER_SIZE) {
     return false;
   }
   buffer[head] = item;
   head = (head + 1) % BUFFER_SIZE;
   count++;
   return true;
 }

 // TODO : Remove an item and store it in ’item ’. Return true if successful .
 bool dequeue (T& item ) {
   // Your code here ( check for empty , then remove , update tail and count )
   if (count == 0) {
     return false;
   }
   item = buffer[tail];
   tail = (tail + 1) % BUFFER_SIZE;
   count--;
   return true;
 }

 bool isEmpty () {
   return count == 0;
 }

 bool isFull () {
   return count == BUFFER_SIZE ;
 }
};

CircularBuffer <int > sensorDataBuffer ;
unsigned long lastProducerTime = 0;
unsigned long lastConsumerTime = 0;

void setup () {
 Serial . begin (115200) ;
 while (! Serial );
 Serial . println (" Starting Circular Buffer Demo ...");
}

void loop () {
 unsigned long currentMillis = millis ();

 // --- Producer : Simulate a high - speed sensor (e.g., 1000 Hz -> every 1 ms) ---
 if ( currentMillis - lastProducerTime >= 1) {
   lastProducerTime = currentMillis ;
   int simulatedSensorValue = random (0, 1000) ;

   // TODO : Try to add the sensor value to the buffer .
   // If the buffer is full , print a warning message " Buffer Overflow !".
   // Your code here
   if (!sensorDataBuffer.enqueue(simulatedSensorValue)) {
     Serial.println("Buffer Overflow!");
   }

 }

 // --- Consumer : Simulate a slow processing loop (e.g., 10 Hz -> every 100 ms) ---
 if ( currentMillis - lastConsumerTime >= 100) {
   lastConsumerTime = currentMillis ;
   int processedValue = 0;
   int itemsProcessedThisCycle = 0;

   // TODO : Process all available data in the buffer
   // While the buffer is not empty , dequeue an item and print it.
   // Keep track of how many items were processed in this cycle .
   // Your code here
   while (sensorDataBuffer.dequeue(processedValue)) {
     Serial.println(processedValue);
     itemsProcessedThisCycle++;
   }

   Serial . print (" --- Processed ");
   Serial . print ( itemsProcessedThisCycle );
   Serial . println (" items this cycle ---");
 }
}

/*
ANALYSIS:

The circular buffer effectively decouples the high-speed producer (1000 Hz) from the 
slow consumer (10 Hz). With 100ms consumer intervals, the consumer processes ~100 items 
each cycle (since producer generates 100 items in 100ms). The BUFFER_SIZE of 64 is actually 
insufficient for this scenario because:

- In 100ms, producer generates 100 items
- Consumer only processes items once every 100ms
- Buffer needs to hold at least 100 items to prevent overflow

If the consumer is delayed, the buffer will overflow. This demonstrates the importance of:
1. Proper buffer sizing based on production/consumption rates
2. Handling overflow conditions gracefully
3. Monitoring buffer status in real-time systems
*/