/*  keyboard.h  */

enum eKeyboardState {RELASED, BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};



void Keyboard_Init(void);
enum eKeyboardState eKeyboard_Read(void);


