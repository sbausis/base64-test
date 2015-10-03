
TARGET=base64-test

$(TARGET): $(TARGET).c
	gcc -o $(TARGET) $(TARGET).c -I.

clean:
	rm -f $(TARGET)
