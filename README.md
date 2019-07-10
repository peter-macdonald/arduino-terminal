A basic terminal helper for arduino.

Example sketch:
```
#include <terminal.h>

TERMINAL_CALLBACK(helloWorldFunc) {
    Serial.println("Hello world!");
    for (int i = 0; i < argc; i++) {
        Serial.print("arg: "); Serial.println(argv[i]);
    }
}

Terminal_Command_Entry menuSetup[] = {
    {"test", "Emit \"Hello world!\" message", helloWorldFunc}
};

Terminal terminal("AppName", menuSetup, sizeof(menuSetup)/sizeof(Terminal_Command_Entry));

void setup() {
    terminal.begin(115200);
}

void loop() {
    terminal.poll();
}

```