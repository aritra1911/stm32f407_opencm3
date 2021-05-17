# Programs for STM32F407VGT6 Discovery Board
My experimentation with the board and [libopencm3](https://github.com/libopencm3/libopencm3).

## Cloning
*libopencm3* is a submodule, so you might wanna do a recursive clone:
```sh
$ git clone https://github.com/aritra1911/stm32f407_opencm3.git --recurse-submodules
```
or (if you forgot to clone recursively):
```sh
$ git clone https://github.com/aritra1911/stm32f407_opencm3.git
$ cd stm32f407_opencm3
$ git submodule update --init
```

## Building
Change directory if you haven't already:
```sh
$ cd stm32f407_opencm3
```

First you'll need to build *libopencm3* itself:
```sh
$ make -C libopencm3
```
You'll only have to do this once per clone.

### Individual projects
> WARNING: Makefiles are still in progress

Every project have their respective Makefile in their respective directory. Hence in order to build project *foo*, do:
```sh
$ cd foo
$ make

```

### Flashing
In order to flash using [stlink](https://github.com/stlink-org/stlink), do:
```sh
$ make flash
```
