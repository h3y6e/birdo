# Birdo

## Usage (macOS)

1. install `arduino-cli`

```sh
$ brew update
$ brew install arduino-cli
$ arduino-cli core install arduino:avr
```

2. clone this repo

```sh
$ cd ~/Documents/Arduino/libraries/
$ git clone https://github.com/5ebec/birdo
```

3. add `boards.local.txt` file to `arduino/hardware/avr/`
   (ex. `~/Library/Arduino15/packages/arduino/hardware/avr/1.8.3/`)

```txt
leonardo.vid.1=0x0f0d
leonardo.pid.1=0x0092

leonardo.build.vid=0x0f0d
leonardo.build.pid=0x0092
```

4. compile and upload

```sh
# check connected boards
$ arduino-cli board list
# compile
$ arduino-cli compile -b arduino:avr:leonardo [sketch/path]
# upload
$ arduino-cli upload -p [port] -b arduino:avr:leonardo [sketch/path]
```
