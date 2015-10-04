
TARGET=base64-test

HFILES=$(TARGET).h
HFILES+=base64-function.h
HFILES+=base64-function-original.h
HFILES+=base64-function-fixed.h

CFILES=$(TARGET).c
CFILES+=base64-function-original.c
CFILES+=base64-function-fixed.c

$(TARGET): $(HFILES) $(CFILES)
	gcc -std=c99 -o $(TARGET) $(CFILES) -I.

clean:
	rm -f $(TARGET)
