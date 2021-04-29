//font
#define PRINTBUFFERLENGTH	1960 

#ifndef FONT_STRUCT
#define FONT_STRUCT
//		struct protoFontInformation	{
//		char height;
//		char firstCharCode;
//		const char *addrFontDataStart;
//		const struct protoFontDescriptors *addrFontDescriptor;
//	};

struct coordinates{
	short xS, yS, xE, yE;
};
		//struct _fontType{
			#define fontTypeNumeric  				1;
			 #define fontTypeEngLetters 			2;
			 #define fontTypeSmallEngLetters  4;
			 #define fontTypeRusLetters 			8;
			 #define fontTypeSmallRusLetters  16;
		//};
	struct _fontInformation{
		const char 		*fontDataStart;					//address of font data
		int				  	fontType;			//how many character does the font have?
		const struct	_lettersInfo *lettersInfoStart;
		char 					height;
		char letterOffset;
		
	};
	
struct _terminalWindow{
  struct coordinates position;
	struct _fontInformation *font;
	short colorA;
	short colorB;
	unsigned short posX;
	unsigned short posY;
};


//struct protoFontInformation* Arial16Init(void);
//struct protoFontInformation* Arial24Init(void);

		struct _lettersInfo	{
		unsigned width : 8;
		unsigned position : 18;
	};
	
	enum	_bufferState	{
		free, blocked,  ready
	};
//		state
	struct _bufferInfo{
		enum _bufferState bufferState;
		//char letterHeight;
		char letterWide;
		char bufferDataStart[PRINTBUFFERLENGTH];
	};


#endif

#ifndef CTPS
#define CTPS


struct taskPrint	{
	char *data;
	short unsigned transmitted;
	char currentX, currentY,
			 limitX	 , limitY;
	struct protoFontInformation *font;
	
	
};

#endif

//struct protoFontInformation *arial24Info(void);

