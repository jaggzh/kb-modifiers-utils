# For when Shortcuts/Macros need to wait for or otherwise manage keyboard modifier keys 

kbmods-state: Outputs modifier states. Can wait until all modifiers released

# Usage:
```
$ kbmods-state      # Outputs states
$ kbmods-state -w   # Waits until no modifiers are held down
$ kbmods-state -wv  # Waits, like ^, but keeps outputting state (a lot)
```

# Compiling
1. Clone
2. type `make`
