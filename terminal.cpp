 /* Generic terminal interface for arduino
  *  
  */
#include "terminal.h"
#include <arduino.h>

#ifdef TERMINAL_USE_OLD_VERSION

const char * term_name;
const char * term_version;
const char * help_message_pretext;

int num_of_commands;
TERM_COMMAND * registered_commands;
int num_of_registered_commands;

bool enable_input = false;;

void term_initialize(const char * name_set, const char * version_set, int num_of_commands_set){
  
  Serial.begin(9600);
  
  term_name = name_set;
  term_version = version_set;
  Serial.print("Starting "); Serial.print(term_name); Serial.print(" "); Serial.println(term_version);
  
  num_of_commands = num_of_commands_set;
  registered_commands = new TERM_COMMAND[num_of_commands];
  if ( registered_commands == NULL ) 
    Serial.println("NOT ENOUGH MEMORY FOR ALLOCATING COMMANDS!");
  num_of_registered_commands = 0;
  enable_input = true;
  return;
}

void term_register_help_message(const char * message){
  help_message_pretext = message;
  return;
}

void term_show_help(){
  Serial.println(help_message_pretext);
  Serial.println("Commands:");
  Serial.println("\thelp - show this help message");
  for ( int i = 0; i < num_of_registered_commands; i++ ){
    Serial.print("\t"); Serial.print(registered_commands[i].command); 
    Serial.print(" - "); Serial.println(registered_commands[i].description);
  }
  return;
}

void term_register_command(const char * command, const char * description, TERM_CALLBACK callback){
  registered_commands[num_of_registered_commands].command = command;
  registered_commands[num_of_registered_commands].description = description;
  registered_commands[num_of_registered_commands].callback = callback;
  num_of_registered_commands ++;
  return;
}

void term_poll_input(){
  if ( ! enable_input ) return;
  
  // Get as much of a line as possible from serial. If the line finishes, exit and execute that command.
  static String input_line = "";
  static bool line_done = false;
  char c;
  
  while(Serial.available()>0) {
    c = Serial.read();
    Serial << c;
    if ( c == '\n' ){
      line_done = true;
      break;
    }
    input_line += c;
  }
  
  if ( line_done ){
#ifdef ECHO_COMMANDS
    Serial.println(input_line);
#endif
    parse_command_string(input_line);
    input_line = "";
    line_done = false;
  }
  
  return;
}

int take_first_word(String & out, String & in, char delim = ' '){
  if ( in.indexOf(delim) == -1 ){
    out = in;
    return 1;
  } else {
    out = in.substring(0,in.indexOf(delim));
    in = in.substring(in.indexOf(' ')+1, in.length());
    return 0;
  }
}

void parse_command_string(String command_string){
  int argc = 0;
  String * argv = NULL;
  String command;
  String cmd_str_first_pass = command_string;
  String buff;

  // Determine the number of arguments
  while ( take_first_word(buff,cmd_str_first_pass) == 0 ) argc ++;
  
  // Fill data with commands and arguments
  take_first_word(command,command_string);
  if ( argc > 0 ){
    argv = new String[argc];
    if ( argv == NULL ){
      Serial.println("[parse_command_string]Out of memory error");
    }
    for ( int i = 0; i < argc; i++ ){
      take_first_word(argv[i],command_string);
    }
  }
  execute_command(command,argc,argv);
  return;
}

void execute_command(String command, int argc, String * argv){
  for ( int i = 0; i < num_of_registered_commands; i++ ){
    if ( command == registered_commands[i].command ){
      // Perform callback with arguments
      registered_commands[i].callback(argc, argv);
      return;
    }
  }
  term_show_help();
  return;
}

#else //  #ifdef USE_OLD_VERSION

//**********************************************************************************
void Terminal::_showHelp() {
    Serial.println("Commands:");
    Serial.println("\t? - show this help message");
    for (int i = 0; i < _numCommands; i++){
        Serial.print("\t"); Serial.print(_commands[i].command); 
        Serial.print(" - "); Serial.println(_commands[i].description);
    }
}

//**********************************************************************************
int _popFirstWord(String & out, String & in, char delim = ' '){
    if (in.indexOf(delim) == -1){
        out = in;
        return 1;
    } else {
        out = in.substring(0, in.indexOf(delim));
        in = in.substring(in.indexOf(' ')+1, in.length());
        return 0;
    }
}

void Terminal::_parseLine(String command_string){
    int argc = 0;
    String * argv = NULL;
    String command;
    String cmd_str_first_pass = command_string;
    String buff;

    // Count the number of arguments
    while (_popFirstWord(buff, cmd_str_first_pass) == 0) argc ++;

    // Fill data with commands and arguments
    _popFirstWord(command, command_string);

    if (argc > 0){
        argv = new String[argc];
        if (argv == NULL){
            Serial.println("[parse_command_string]Out of memory error");
        }
        for (int i = 0; i < argc; i++){
            _popFirstWord(argv[i], command_string);
        }
    }

    for (int i = 0; i < _numCommands; i++){
        if (command == _commands[i].command){
            // Perform callback with arguments
            _commands[i].callback(argc, argv);
            return;
        }
    }

    _showHelp();
}

//**********************************************************************************
void Terminal::begin(int baud) {
    Serial.begin(baud);
    while (!Serial);

    Serial.print("Starting "); Serial.println(_name);
}

//**********************************************************************************
void Terminal::poll() {
    // Get as much of a line as possible from serial. If the line finishes, exit and execute that command.
    static String input_line = "";
    static bool line_done = false;
    char c;

    while (Serial.available() > 0) {
        c = Serial.read();
        Serial << c;
        if (c == '\n'){
            line_done = true;
            break;
        }
        input_line += c;
    }

    if (line_done){
#ifdef ECHO_COMMANDS
        Serial.println(input_line);
#endif
        _parseLine(input_line);
        input_line = "";
        line_done = false;
    }
}

//**********************************************************************************
#endif // #else of #ifdef USE_OLD_VERSION 