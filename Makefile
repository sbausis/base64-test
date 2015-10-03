
TARGET=base64-test

$(TARGET): $(TARGET).c
	gcc -std=c99 -o $(TARGET) $(TARGET).c -I.

clean:
	rm -f $(TARGET)
