PORT = /dev/cu.usbmodemHIDGF1
BOARD = arduino:avr:leonardo
GAME = pokemon_bdsp
SKETCH = hatching

build: compile upload

compile: $(GAME)/$(SKETCH)/$(SKETCH).ino
	arduino-cli compile -v -b $(BOARD) $(GAME)/$(SKETCH)/$(SKETCH).ino
	touch compile

upload: compile
	arduino-cli upload -v -p $(PORT) -b $(BOARD) $(GAME)/$(SKETCH)
	touch upload

.PHONY: clean
clean:
	rm -r $(GAME)/$(SKETCH)/build
