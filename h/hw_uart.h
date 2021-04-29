void UART3Init(void);
void UART1SendByte(char byte);
void UART3SendString(char *string);
char *getOneWord(char *string, char number);
int wc(char *string, char delim);