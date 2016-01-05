/* Generic terminal interface for Arduino
 *  
 */
#include <arduino.h>

#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#define ECHO_COMMANDS

// Container for command metadata
typedef void (*TERM_CALLBACK)(int, String*);
struct TERM_COMMAND {
  String command;
  const char * description;
  TERM_CALLBACK callback;
};

// Initialization. Includes setup of the serial output and setting metadata variables
void term_initialize(const char * name_set, const char * version_set, int num_of_commands);

// Set a string to be displayed at the beginning of the help message.
void term_register_help_message(const char * message);

// Display the help message
void term_show_help();

// Add a command to the list of all possible commands
void term_register_command(const char * command, const char * description, TERM_CALLBACK callback);

// Poll the input and execute any commands sent
void term_poll_input();

void parse_command_string(String command_string);

void execute_command(String command, int argc, String * argv);

#endif
