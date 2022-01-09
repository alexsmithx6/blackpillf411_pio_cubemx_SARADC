## Brief:
* SAR ADC project, building upon the core of [this example](https://github.com/alexsmithx6/blackpillf411_pio_cubemx_usb_example).

## Todo:
* Schematic of hardware setup.
* Re-organise code in **src/** and **inc/**.
* Add sample + hold circuit at SAR ADC input.

# Dependencies:
* [VSCode](https://code.visualstudio.com/)
* [platformio](https://platformio.org/) - follow the installation guide which is integrated into VSCode
* [Python 3.6+](https://www.python.org/) - setup is known to have worked with Python 3.8.3
* [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html)
* [stm32pio](https://github.com/ussserrr/stm32pio) - may use ```pip install stm32pio```


## Setup:
Currently the setup is a bit finicky but worth it if you want to work in vscode rather than the other bloated IDE's, AND use STM32CubeMX for some code generation.

1. Setup .ioc file (in this project it is blackpill_cubemx.ioc) with STM32CubeMX (no need to generate the code), following steps 1-3 of [this guide](https://github.com/ussserrr/stm32pio/tree/master/examples/cli)
2. Run the code in terminal
```
stm32pio init
stm32pio new -b blackpill_f411ce --start-editor=code --with-build
```
3. Create new file called ```platformio.ini``` and put configurations related to environment in. For this project, mine were:
```
[env:blackpill_f411ce]
platform = ststm32
board = blackpill_f411ce
framework = stm32cube

debug_tool = jlink

; SWD interface
upload_protocol = jlink
```
4. ```PlatformIO: Build``` and ```PlatformIO: Upload``` should work now assuming that your code contains no compilation errors.

## Setup Notes:
* before running ```stm32pio init```, I had to change the cubemx_cmd and java_cmd path in stm32pio.ini.
* .h files are stored in ```include``` and .c files are stored in ```src```.