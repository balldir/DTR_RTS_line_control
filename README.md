# DTR/RTS line control
A simple application to control DTR/RTS lines from linux shell

# Compile
```
gcc dtr_rts_line_control.c -o dtr_rts_line_control
```

# Example
Set DTR line of /dev/ttyUSB0
```
dtr_rts_line_control /dev/ttyUSB0 DTR SET
```

Clear RTS line of /dev/ttyUSB0
```
dtr_rts_line_control /dev/ttyUSB0 RTS CLEAR
```
