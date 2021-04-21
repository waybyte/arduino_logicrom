# I2C Library For LOGICROM Platform
GSM module has one I2C channel and currently driver supports master mode I2C communication upto 400Khz.

## Debugging
To enable libary debug prints, just define `WIRE_DEBUG` in `wire.h` header file.
You can locate the header file in following path:
`C:\Users\{user}\AppData\Local\Arduino15\packages\logicrom\hardware\arm\{version}\libraries\Wire\`

Change line 32 from
```c++
/* #define WIRE_DEBUG */
```
to
```c++
#define WIRE_DEBUG
```

The debug print shows information of low-level LOGICROM function calls and return values. Check errno.h for details on error codes.
