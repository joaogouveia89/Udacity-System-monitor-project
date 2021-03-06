#include "ncurses_display.h"
#include "system.h"

//GOALS:

/*BASIC REQUIREMENTS:
      RUNNING
      NO WARNINGS
      RUN CONTINUOUSLY WITHOU ERROR UNTIL USER TERMINATES THE PROGRAM
      ORGANIZED INTO APPROPRIATE CLASSES
      
  SYSTEM REQUIREMENTS:
      THE SYSTEM MONITOR PROGRAM SHOULD LIST AT LEAST THE OPERATING SYSTEM, KERNEL VERSION, TOTAL NUMBER OF PROCESSES, NUMBER OF RUNNING PROCESSES, AND UP TIME
      THE SYSTEM CLASS SHOULD BE COMPOSED OF AT LEAST ONE OTHER CLASS.

  PROCESSOR REQUIREMENTS:
      THE SYSTEM MONITOR SHOULD DISPLAY THE CPU UTILIZATION

  PROCESS REQUIREMENTS:
      THE SYSTEM MONITOR SHOULD DISPLAY A PARTIAL LIST OF PROCESSES RUNNING ON THE SYSTEM
      THE SYSTEM MONITOR SHOULD DISPLAY THE PID, USER, CPU UTILIZATION, MEMORY UTILIZATION, UP TIME, AND COMMAND FOR EACH PROCESS
  
  SUGGESTIONS:
    CALCULATE CPU UTILIZATION DYNAMICALLY, BASED ON RECENT UTILIZATION
    SORT PROCESSES BASED ON CPU OR MEMORY UTILIZATION
    MAKE THE DISPLAY INTERACTIVE
    RESTRUCTURE THE PROGRAM TO USE ABSTRACT CLASSES (INTERFACES) AND PURE VIRTUAL FUNCTIONS
    PORT THE PROGRAM TO ANOTHER OPERATING SYSTEM
*/


int main() {
  System system;
  NCursesDisplay::Display(system);
}