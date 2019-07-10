/* Generic terminal interface for Arduino
 *  
 */
#include <arduino.h>

#ifndef TERMINAL_H
#define TERMINAL_H

#define ECHO_COMMANDS

#ifdef TERMINAL_USE_OLD_VERSION

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

#else // #ifdef USE_OLD_VERSION

#define TERMINAL_CALLBACK(f) void f(int argc, String argv[])
typedef void (*Terminal_Callback)(int, String[]);
typedef struct {
    const char * command;
    const char * description;
    Terminal_Callback callback;
} Terminal_Command_Entry;

class Terminal {
    public:
        Terminal(const char * name, Terminal_Command_Entry commands[], int numCommands) 
            : _name(name), _commands(commands), _numCommands(numCommands) {};

        void begin(int baud);
        void poll();
        void _showHelp();

    private:
        const char * _name;
        Terminal_Command_Entry * _commands;
        int _numCommands;

        void _parseLine(String command_string);

};
#endif // #ifdef USE_OLD_VERSION

#endif // #ifndef TERMINAL_H
