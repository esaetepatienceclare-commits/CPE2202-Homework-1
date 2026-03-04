#include <Arduino.h>

#define MAX_MENU_DEPTH 10

// Menu IDs
#define MENU_MAIN 0
#define MENU_SETTINGS 1
#define MENU_DISPLAY_SETTINGS 2
#define MENU_AUDIO_SETTINGS 3
#define MENU_BRIGHTNESS 4
#define MENU_CONTRAST 5

// TODO : Implement an array - based stack for integers ( menu IDs)
class MenuStack {
private :
 int stack [ MAX_MENU_DEPTH ];
 int top; // Index of the top element , -1 for empty

public :
 MenuStack () : top ( -1) {}

 // TODO : Push a menu ID onto the stack . Check for stack overflow .
 void push (int menuId ) {
   // Your code here
   if (top < MAX_MENU_DEPTH - 1) {
     stack[++top] = menuId;
   }
 }

 // TODO : Pop and return the top menu ID. Check for underflow . Return -1 if empty .
 int pop () {
   // Your code here
   if (top >= 0) {
     return stack[top--];
   }
   return -1;
 }

 // TODO : Return the top menu ID without removing it. Return -1 if empty .
 int peek () {
   // Your code here
   if (top >= 0) {
     return stack[top];
   }
   return -1;
 }

 bool isEmpty () {
   return top == -1;
 }
};

MenuStack navStack ;
int currentMenu = MENU_MAIN ;

void displayMenu (int menuId ) {
 Serial . println ("\n--- Menu ---");
 switch ( menuId ) {
   case MENU_MAIN :
     Serial . println ("1. Main Menu ");
     Serial . println (" Enter 's' for Settings ");
     break ;
   case MENU_SETTINGS :
     Serial . println ("2. Settings Menu ");
     Serial . println (" Enter 'd' for Display Settings ");
     Serial . println (" Enter 'a' for Audio Settings ");
     Serial . println (" Enter 'b' to go Back ");
     break ;
   case MENU_DISPLAY_SETTINGS :
     Serial . println ("3. Display Settings Menu ");
     Serial . println (" Enter 'r' for Brightness ");
     Serial . println (" Enter 'c' for Contrast ");
     Serial . println (" Enter 'b' to go Back ");
     break ;
   case MENU_AUDIO_SETTINGS :
     Serial . println ("3. Audio Settings Menu ");
     Serial . println (" (No sub - menus )");
     Serial . println (" Enter 'b' to go Back ");
     break ;
   case MENU_BRIGHTNESS :
     Serial . println ("4. Brightness Menu ( Setting value not implemented )");
     Serial . println (" Enter 'b' to go Back ");
     break ;
   default :
     Serial . println (" Unknown Menu ");
     break ;
 }
 Serial . print ("> ");
}

void setup () {
 Serial . begin (115200) ;
 while (! Serial );
 displayMenu ( currentMenu );
}

void loop () {
 if ( Serial . available () > 0) {
   char command = Serial . read ();

   if ( command == 'b') { // BACK command
     // TODO : Implement back navigation using the stack .
     // If the stack is not empty , pop the previous menu and set it as currentMenu .
     // If the stack is empty , you are at the main menu ; print " Already at main menu ."
     
     // Your code here
     if (!navStack.isEmpty()) {
       currentMenu = navStack.pop();
     } else {
       Serial.println("Already at main menu.");
     }

   } else { // Assume any other key is a navigation command
     // TODO : Before changing to a new menu , push the * current * menu onto the stack .
     // Then , change currentMenu based on the command .
     // For example , if at main menu (0) and 's' is pressed , currentMenu should become MENU_SETTINGS (1).
     // You 'll need a series of if/ else or a switch statement based on ( currentMenu , command ) pairs .
     
     // Your code here
     navStack.push(currentMenu);
     
     if (currentMenu == MENU_MAIN && command == 's') {
       currentMenu = MENU_SETTINGS;
     }
     else if (currentMenu == MENU_SETTINGS && command == 'd') {
       currentMenu = MENU_DISPLAY_SETTINGS;
     }
     else if (currentMenu == MENU_SETTINGS && command == 'a') {
       currentMenu = MENU_AUDIO_SETTINGS;
     }
     else if (currentMenu == MENU_DISPLAY_SETTINGS && command == 'r') {
       currentMenu = MENU_BRIGHTNESS;
     }
     else if (currentMenu == MENU_DISPLAY_SETTINGS && command == 'c') {
       currentMenu = MENU_CONTRAST;
     }
     else {
       navStack.pop(); // Invalid navigation, undo the push
     }
   }

   displayMenu ( currentMenu ); // Show the new menu
 }
}

/*
ANALYSIS:

Stack-based navigation provides an elegant solution for hierarchical menu systems:

Advantages:
1. Automatic "back" functionality - just pop to return to previous menu
2. Limited memory usage - stack depth bounded by MAX_MENU_DEPTH
3. O(1) navigation operations - push/pop are constant time
4. Natural fit for depth-first traversal of menu tree

The implementation handles:
- Stack overflow protection (preventing excessive nesting)
- Edge cases (back at main menu, leaf menus)
- Invalid command handling

Memory efficiency: The stack uses only MAX_MENU_DEPTH * sizeof(int) bytes
(typically 40 bytes for MAX_MENU_DEPTH=10), making it perfect for embedded systems.
*/