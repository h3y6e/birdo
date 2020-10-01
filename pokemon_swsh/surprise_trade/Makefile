PORT = /dev/cu.usbmodemHIDGF1
BOARD = arduino:avr:leonardo
SKETCH = surprise_trade_ja

build: compile upload

compile: $(SKETCH)/$(SKETCH).ino
	arduino-cli compile -v -b $(BOARD) $(SKETCH)/$(SKETCH).ino
	touch compile

upload: compile
	arduino-cli upload -v -p $(PORT) -b $(BOARD) $(SKETCH)
	touch upload

.PHONY: clean
clean:
	rm -r $(SKETCH)/build