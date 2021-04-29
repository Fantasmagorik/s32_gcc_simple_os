#include "logic.h"
#include "common_types.h"
#define STRING_BUFFER_LENGTH	100
char string_buffer[STRING_BUFFER_LENGTH];
char *currentPosition = string_buffer;
int tempSymbol;

char *int_to_string(short unsigned dec, char  param)	{
	short unsigned scan_var= 1;
	char *returnString;
	if( (currentPosition - string_buffer + 10 /*for one transaction*/) >= STRING_BUFFER_LENGTH)
		currentPosition = string_buffer;
	returnString = currentPosition;
	//char counter;
	//if(!prolonged) uk= string_buffer; //clear string if its not a composite
	while((scan_var< param && param> 1)  || scan_var< dec)	//looking for an optimal divider
		scan_var*= 10;
	if(dec != scan_var)
		scan_var/= 10; 
	for(scan_var; scan_var; scan_var/= 10)
	{
			*currentPosition = dec/scan_var+ 48;
			dec%=	 scan_var;
			currentPosition++;
	}
	*currentPosition = 0;
	return returnString;
}


//char *int_to_stringN(short unsigned dec, char  param)	{
//	char decimal_count;
//	int current_divider;
//	for(decimal_count = 1, current_divider = 1; dec > current_divider; current_divider*= 10, decimal_count++);
//	
//}
char *float_to_string(unsigned int dec, unsigned short divider, char prec)	{
	unsigned int tempValue, tempDivider;
	char *returnString;
	if( (currentPosition - string_buffer + 10 /*for one transaction*/) >= STRING_BUFFER_LENGTH)
		currentPosition = string_buffer;
	returnString = currentPosition;
	tempValue = dec / divider;
	tempDivider = 1;
	while(tempDivider <= tempValue)
		tempDivider *= 10;
	tempDivider /= 10; //one step back
	//Integer part
	while(tempDivider)	{
		*currentPosition = tempValue / tempDivider + 48;
		tempValue %= tempDivider;
		tempDivider /= 10;
		currentPosition++;
	}
	//point
	if(prec)
		*currentPosition++ = 46;
	*currentPosition++ = 32;
	//fract part
	tempValue = dec % divider * 1000 / divider;
	tempDivider = 100;
//	while(tempDivider <= tempValue)
//		tempDivider *= 10;
//	tempDivider /= 10; //one step back
	//Fract part
	while(tempDivider && prec)	{
		*currentPosition = tempValue / tempDivider + 48;
		tempValue %= tempDivider;
		tempDivider /= 10;
		currentPosition++;
		prec--;
	}
	//null
	*currentPosition++ = 0;
	return returnString;
}




int charToInt(char *symbol)	{
	char *ukSymbol = symbol;
	int multiple;
	if( (*symbol < 48) || (*symbol > 57))
		return 0; //not digits
	while(*ukSymbol)	
		ukSymbol++;
	tempSymbol = 0;
	multiple = 1;
	while(symbol < ukSymbol)	{
		--ukSymbol;
		tempSymbol += (*ukSymbol - 48) * multiple;
		multiple *= 10;
	}
	return tempSymbol ;
}

int strlen(char *string)	{
	int i = 0;
	while(*string++)
		i++;
	return i;
}

//int strlen(char *str){
//	int i;
//	for(i = 0; *str; str++, i++);
//		return i;
//}
int bytesInChar(char symbol){
	return 0;
}