# My own lily58 keymap with 2 oled screen

## Keymap can found in the json file
The json file was generated with the online configurator, because I had no idea what keymap did I want.
The json file was converted to .c file with json2c, and than simply copied the keymaps variable to the keymap.c file. (That is why it looks odd.)

## OLED Features
My main goal was to set up RAW-HID, and send realtime messaged from the PC to the keyboard. The current implementation uses the master side to display CPU/RAM/VOL statuses coming from the pc, and the slave side just showing an animation [https://github.com/marekpiechut/qmk-animations/tree/master](Link to animation)

### Future improvements
- That would be good to have multiple screens and switch back and forth from the pc side or creating some hotkeys
- Hotkey for brightness adjustment
- Auto variable screens with different informations
- VIM Mode, as I use nvim as my main editor, it would be great to integrate some features with my keyboard (lsp infos, etc.)

## How to install?
My keyboard (lily58/rev1) uses a promicro rp2040 microcontroller, so for this you need to do the following:
- For the first time add the udev rules!!
- Than in the qmk_firmware folder:
```
qmk setup
```
- Put the controller into boot mode by clicking the reset button twice
- Once it appeared issue the following command:

:warning: **DISCONNECT THE TTRS CABLE WHILE FLASHING IT CAN HARM YOUR BOARD**
```
qmk flash -kb lily58/rev1 -km lnxx-56 -e CONVERT_TO=promicro_rp2040
```

## Software for the PC side can be found on
:point_right: **UNDER CONSTRUCTION** :point_left
