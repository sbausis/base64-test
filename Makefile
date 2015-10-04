
TARGET=base64-test

HFILES=base64-function.h
HFILES+=base64-funcset.h
HFILES+=base64-function-original.h
HFILES+=base64-function-fixed.h
HFILES+=$(TARGET).h

CFILES=base64-funcset.c
CFILES+=base64-function-original.c
CFILES+=base64-function-fixed.c
CFILES+=$(TARGET).c

$(TARGET): $(HFILES) $(CFILES)
	gcc -std=c99 -o $(TARGET) $(CFILES) -I.

clean:
	rm -f $(TARGET)
