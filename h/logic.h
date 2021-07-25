
enum toCharFormat	{
	decimal = 16,
	decimal_1 ,
	decimal_2,
	decimal_3,
	decimal_4,
	hex_prefix = 32,
	hex_big = 64,
	hex_little = 128,
	hex = 256,
	hex_1,
	hex_2,
	hex_3,
	hex_4,
	hex_5,
	hex_6,
	hex_7,
	hex_8,
	prolonged = 512

};
enum serviceCmd	{
	STOP		= 0,
	START,
	RESTART
};
char *stradd(char *a, char *b);
char *int_to_string(short unsigned, char divider);
char *float_to_string(unsigned int dec, unsigned short divider, char prec);
int charToInt(char *symbol);
int strlen(char *string);
char *charToString(int value, enum toCharFormat format);
char *arrayToString(char *array, char length, char delim, enum toCharFormat format);
char stringTest(char *a, char *b);
int wc(char *string, char delim);
char *getOneWord(char *string, char number);
char strcpy (char *str1, char *str2);
