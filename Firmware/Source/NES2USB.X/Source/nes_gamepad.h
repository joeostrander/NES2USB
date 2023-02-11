#ifndef NES_GAMEPAD_H
#define NES_GAMEPAD_H

#define BUTTON_A        (1<<0)
#define BUTTON_B        (1<<1)
#define BUTTON_SELECT   (1<<2)
#define BUTTON_START    (1<<3)
#define BUTTON_UP       (1<<4)
#define BUTTON_DOWN     (1<<5)
#define BUTTON_LEFT     (1<<6)
#define BUTTON_RIGHT    (1<<7)

//        NES Word Mapping
//        x x x x x x x x
//        | | | | | | | |_  A
//        | | | | | | |___  B
//        | | | | | |_____  SELECT
//        | | | | |_______  START
//        | | | |_________  UP
//        | | |___________  DOWN
//        | |_____________  LEFT
//        |_______________  RIGHT

void NES_GPIO_Initialize(void);
uint8_t NES_read_pad(void);

#endif /* NES_GAMEPAD_H */