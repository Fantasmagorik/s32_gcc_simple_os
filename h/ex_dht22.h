
void DHT22Init(void);
void DHT22Start(void);
unsigned int DHT22GetValue(void);
struct _dhtStructChar	{
//	char hH, hL, tH, tL;
	char tL, tH, hL, hH;
};
struct _dhtStructShort	{
//	char hH, hL, tH, tL;
	short temp, humid;

};
union DHT22Data{
		unsigned int value;
		struct _dhtStructShort structShort;
	};
