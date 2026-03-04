#include <Arduino.h>

#define MAX_PLAYLIST_SIZE 10

// ==================== TYPE DEFINITIONS ====================
struct Song {
  char title[50];
  char artist[50];
  int duration; // in seconds
};

// --- Array-based Playlist Implementation ---
struct ArrayList {
  Song songs[MAX_PLAYLIST_SIZE];
  int size = 0;
};

// --- Linked List Node Structure ---
struct SongNode {
  Song song;
  SongNode* next;
};

struct LinkedList {
  SongNode* head = nullptr;
};

// ==================== FUNCTION PROTOTYPES ====================
// Array list functions
void addSong(ArrayList* list, const Song& newSong);
void removeSong(ArrayList* list, int index);
void displayPlaylist(const ArrayList* list);

// Linked list functions
void addSong(LinkedList* list, const Song& newSong);
void removeSong(LinkedList* list, int index);
void displayPlaylist(const LinkedList* list);

// TODO: Add a song to the end of the array list. Check for overflow.
void addSong(ArrayList* list, const Song& newSong) {
  // Check if playlist is full
  if (list->size >= MAX_PLAYLIST_SIZE) {
    Serial.println("Error: Playlist is full! Cannot add more songs.");
    return;
  }
  
  // Add the new song at the end
  list->songs[list->size] = newSong;
  list->size++;
  Serial.println("Song added to array playlist successfully!");
}

// TODO: Remove a song at a given index (0-based). Shift elements left.
void removeSong(ArrayList* list, int index) {
  // Check if index is valid
  if (index < 0 || index >= list->size) {
    Serial.println("Error: Invalid index!");
    return;
  }
  
  // Shift all elements after index to the left
  for (int i = index; i < list->size - 1; i++) {
    list->songs[i] = list->songs[i + 1];
  }
  
  list->size--;
  Serial.println("Song removed from array playlist successfully!");
}

// TODO: Print all songs in the list.
void displayPlaylist(const ArrayList* list) {
  if (list->size == 0) {
    Serial.println("Playlist is empty!");
    return;
  }
  
  Serial.println("\n--- Array Playlist ---");
  for (int i = 0; i < list->size; i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.print(list->songs[i].title);
    Serial.print(" by ");
    Serial.print(list->songs[i].artist);
    Serial.print(" [");
    Serial.print(list->songs[i].duration);
    Serial.println(" sec]");
  }
  Serial.println("----------------------");
}

// --- Linked List-based Playlist Implementation ---

// TODO: Add a song to the end of the linked list.
void addSong(LinkedList* list, const Song& newSong) {
  // Create new node
  SongNode* newNode = new SongNode;
  newNode->song = newSong;
  newNode->next = nullptr;
  
  // If list is empty, new node becomes head
  if (list->head == nullptr) {
    list->head = newNode;
  } else {
    // Traverse to the end and add new node
    SongNode* current = list->head;
    while (current->next != nullptr) {
      current = current->next;
    }
    current->next = newNode;
  }
  Serial.println("Song added to linked list playlist successfully!");
}

// TODO: Remove a song at a given index (0-based).
void removeSong(LinkedList* list, int index) {
  if (list->head == nullptr) {
    Serial.println("Error: Playlist is empty!");
    return;
  }
  
  if (index < 0) {
    Serial.println("Error: Invalid index!");
    return;
  }
  
  SongNode* temp = nullptr;
  
  // Special case: removing the head
  if (index == 0) {
    temp = list->head;
    list->head = list->head->next;
    delete temp;
    Serial.println("Song removed from linked list successfully!");
    return;
  }
  
  // Find the node before the one to delete
  SongNode* current = list->head;
  int currentIndex = 0;
  
  while (current != nullptr && currentIndex < index - 1) {
    current = current->next;
    currentIndex++;
  }
  
  // Check if index is valid
  if (current == nullptr || current->next == nullptr) {
    Serial.println("Error: Index out of bounds!");
    return;
  }
  
  // Remove the node
  temp = current->next;
  current->next = temp->next;
  delete temp;
  Serial.println("Song removed from linked list successfully!");
}

// TODO: Print all songs in the list.
void displayPlaylist(const LinkedList* list) {
  if (list->head == nullptr) {
    Serial.println("Playlist is empty!");
    return;
  }
  
  Serial.println("\n--- Linked List Playlist ---");
  SongNode* current = list->head;
  int index = 0;
  
  while (current != nullptr) {
    Serial.print(index);
    Serial.print(": ");
    Serial.print(current->song.title);
    Serial.print(" by ");
    Serial.print(current->song.artist);
    Serial.print(" [");
    Serial.print(current->song.duration);
    Serial.println(" sec]");
    
    current = current->next;
    index++;
  }
  Serial.println("----------------------------");
}

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  // Test Array-based Playlist
  Serial.println("=== Testing Array Playlist ===");
  ArrayList myArrayPlaylist;
  addSong(&myArrayPlaylist, {"Song A1", "Artist 1", 180});
  addSong(&myArrayPlaylist, {"Song B2", "Artist 2", 240});
  addSong(&myArrayPlaylist, {"Song C3", "Artist 1", 200});
  displayPlaylist(&myArrayPlaylist);
  removeSong(&myArrayPlaylist, 1);
  Serial.println(" After removing index 1:");
  displayPlaylist(&myArrayPlaylist);
  
  // Test Linked List-based Playlist
  Serial.println("\n=== Testing Linked List Playlist ===");
  LinkedList myLinkedListPlaylist;
  addSong(&myLinkedListPlaylist, {"Song X1", "Artist X", 195});
  addSong(&myLinkedListPlaylist, {"Song Y2", "Artist Y", 225});
  addSong(&myLinkedListPlaylist, {"Song Z3", "Artist Z", 215});
  displayPlaylist(&myLinkedListPlaylist);
  removeSong(&myLinkedListPlaylist, 1);
  Serial.println(" After removing index 1:");
  displayPlaylist(&myLinkedListPlaylist);
  
  // Clean up linked list nodes to avoid memory leak
  SongNode* current = myLinkedListPlaylist.head;
  while (current != nullptr) {
    SongNode* temp = current;
    current = current->next;
    delete temp;
  }
}

void loop() {}

/*
ANALYSIS:

1. Memory Efficiency Comparison:
   For a playlist that rarely changes, the array-based implementation is more memory-efficient because:
   - Arrays have no overhead per element (no pointer storage)
   - Arrays store data in contiguous memory with no fragmentation
   - Linked lists require an extra pointer (4-8 bytes) per node for the 'next' reference
   - Example: For 100 songs, linked list uses 400-800 extra bytes just for pointers

2. Time Efficiency for Frequent Modifications:
   For constant adds/removes from the middle, linked lists are more time-efficient because:
   - Arrays require shifting O(n) elements (costly for large playlists)
   - Linked lists only need pointer updates O(1) after finding the position
   - Example: Removing song at position 50 in 100-song playlist:
     * Array: Must shift 50 elements → 50 operations
     * Linked List: Only need to update 2 pointers → constant time

3. Risks of Linked List on ESP32 Over Long Period:
   - Memory fragmentation: Frequent new/delete operations can fragment the heap
   - Memory leaks: Forgetting to delete nodes causes gradual memory depletion
   - Slower access times: Traversing pointers is slower than array indexing
   - Stack overflow risk: Recursive operations on long lists can overflow stack
   - Non-deterministic behavior: Dynamic allocation timing varies
*/