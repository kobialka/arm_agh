/* keyboard.h */



typedef enum tKeyboardState{RELASED, BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4} tKeyboardState;

void KeyboardInit(void);
tKeyboardState eKeyboard_Read(void);



