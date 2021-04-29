#include "logic.h"
#include "common_types.h"
#define STRING_BUFFER_LENGTH	400
//char string_buffer[STRING_BUFFER_LENGTH];
char stringBuffer[STRING_BUFFER_LENGTH], toCharBuffer[12];
char *currentPosition = stringBuffer;
int tempSymbol;
//need refactoring	//diff styles
//char *int_to_string(short unsigned dec, char  param)	{
//	short unsigned scan_var= 1;
//	char *returnString;
//	if( (currentPosition - stringBuffer + 10 /*for one transaction*/) >= STRING_BUFFER_LENGTH)
//		currentPosition = stringBuffer;
//	returnString = currentPosition;
//	//char counter;
//	//if(!prolonged) uk= string_buffer; //clear string if its not a composite
//	if(dec == 0)
//		*currentPosition++ = 48;
//	else	{
//		while((scan_var< param && param> 1)  || scan_var< dec)	//looking for an optimal divider
//			scan_var*= 10;
//		if(dec != scan_var)
//			scan_var/= 10; 
//		for(scan_var; scan_var; scan_var/= 10)
//		{
//				*currentPosition = dec/scan_var+ 48;
//				dec%=	 scan_var;
//				currentPosition++;
//		}
//	}
//	*currentPosition = 0;
//	return returnString;
//}

char *float_to_string(unsigned int dec, unsigned short divider, char prec)	{
	unsigned int tempValue, tempDivider;
	char *returnString;
		
	if( (currentPosition - stringBuffer + 10 /*for one transaction*/) >= STRING_BUFFER_LENGTH)
		currentPosition = stringBuffer;
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
	//*currentPosition++ = 32;
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
	char *str = string;
	while(*str++)
		i++;
	//string -= i;
	return i;
}



char *charToString(int value, enum toCharFormat format)	{
	char dividerMask, digitsCount, *returnString;// = toCharBuffer;
	int restValue = 1, div = 1;	
	if( (format & decimal) )	{
		dividerMask = 10;
		digitsCount = (format & ~prolonged) - decimal;
	}
	else if (format & hex) 	{
		dividerMask = 16;
		digitsCount = (format & ~(hex_prefix + hex_little + hex_big)) - hex;  
	}
	while( ((div *= dividerMask) < value) || (digitsCount > restValue) )	//looking for divider
		restValue++;								//count of digits
	if(div != value)	{
		div /= dividerMask;
		restValue--;
	}
	if(!(format & prolonged))
		if( (currentPosition - stringBuffer + restValue + 1 /*for one transaction*/) > STRING_BUFFER_LENGTH)
			currentPosition = stringBuffer;
	returnString = currentPosition;
	if(format & hex_prefix)	{
		*currentPosition++ = 48;	//0
		*currentPosition++ = 120;	//x
		}
		for(restValue = value; div; restValue %= div, div /= dividerMask, ++currentPosition)	{
			value = restValue / div;
			if(value > 9)	{
				if(format & hex_big)
					value += 7;
				else if(format & hex_little)
					value += 39;
				}
			*currentPosition = value + 48;				
	}
	if(!(format & prolonged))
		*currentPosition = 0;
	return returnString;
}

char *arrayToString(char *array, char length, char delim, enum toCharFormat format)	{
	//delim is a symbol for print between values, format is
	//1 - 3 : decimal, x- necessary digits. Example digit 10: 1- 10, 2- 10, 3- 010; 4: 1- 4, 2- 04, 3- 004
				//16 	-  hex
				//160 -  hex with prefix "0x"
	char *string;
	int totalChars = length * 4 + 1;
	//int i;
	if(format & hex)	
		totalChars +=  1 + (format & hex_prefix)? 2 : 0; 
	if( (currentPosition - stringBuffer + totalChars + 1) > STRING_BUFFER_LENGTH)
			currentPosition = stringBuffer;
	string = currentPosition;
	format += prolonged;
	if(format & hex_prefix)	{
		charToString((char)*array, format);
		array++;
		format -= hex_prefix;
		length--;
		*currentPosition++ = delim;
	}
	while(length--){
		charToString((int) *array++, format);
		*currentPosition++ = delim;
	}
	*(currentPosition - 1) = 0;
	
	currentPosition++;
	return  string;
}

int bytesInChar(char symbol){
	return 0;
}



char stringTest(char *a, char *b)	{
	int counter = 0;
	while(*b > 32)	{
		if(*a != *b)
			return 1;
		a++;
		b++;
		counter++;
	}
	if(*a != *b)
			return 1;
	//a -=	counter;
	return(0);
	
}
int wc(char *string, char delim)	{
	int i = 0, x;	// 
	for(; *string < 33 && *string != 0; string++); //find a symbol
	
	for(; *string != 0; string++)
		if( *string == delim) {
			for(; *string < 33 && *string != 0; string++);
			if(*string != 0)
				i++;
			else
				return i;
		}

	return i;		
}
char *getOneWord(char *string, char number)	{
	int i;
	char *ukStartWord;// = stringBuffer;
	char *ukString = string;
	#define WORD_LENGTH	20
//	
//	for(i = 0; i < STRING_BUFFER_LENGTH; i++)	 //clear buffer
//		*ukStartWord++ = 0;
	
	if((currentPosition + WORD_LENGTH) > (stringBuffer + STRING_BUFFER_LENGTH))
		currentPosition = stringBuffer;
	ukStartWord = currentPosition;
	for(i = 0; i != number; i++)	{
		while(*ukString != 32)	{		//move forward for looking a SPACE
				ukString++;
		}
		for(; *ukString < 33 && *ukString != 0; ukString++);
	}
//	if(number)
//		ukString++;
	i = 0;
	while( (*(ukString + i) >  32) && (i < STRING_BUFFER_LENGTH) )	{
		*currentPosition++ = *(ukString + i);
		i++;
	}
	*currentPosition++ = 0;
	return ukStartWord;
}
char strcpy (char *str1, char *str2){
	char i;
	for(i = 0; *str1 != 0; i++, str1++, str2++) 
		*str2 = *str1;
	return i;
}
int stradd(char *a, char *b)	{
	int counter = 0;
	while(*a++)
		if(*a <= 32)
			break;
	while(*b){
		*a++ = *b++;
		counter++;
	}
	*a = 0;
	return counter;
}
