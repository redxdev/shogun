# Sholan Symbol Standard

Shasm doesn't define any specifics as to how symbols should be defined (and as such is compiler
implementation specific), but sholan keeps to the following standard for symbol naming.

## External Symbols

All external symbols (functions, variables, etc) are named as-is. That means that if you have an
external function called "print", then the symbol name sholan will use will be "print".

## Internal Functions

Internal functions are named "sl_f_{argcount}_{name}".