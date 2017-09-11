#include "Arduino.h"

#define BG_BORDER 5
#define BG_BORDER_COLOR ILI9341_WHITE
#define BG_COLOR ILI9341_BLACK

#define FONT_FREQ   DS_DIGI42pt7b
#define FONT_FREQ   digital_7__mono_38pt7b
#define FONT_DESC   verdana16pt7b
#define FONT_SMALL  verdana10pt7b

class Canvas {

	public:
		Canvas();
		~Canvas();
		void freqBg( char *type );
		void freqActive( int freq );
		void freqStandBy( int freq );
		void adfBg();
		void adf1( int adf );
		void adf2( int adf );
		void updateXPDR();

	protected:
		void drawFreqImpl( int freq, int y, uint16_t color );
		void adfImpl( int adf, int y );
		void drawBg();
		void arrowH( int x, int y, int l, int w );
		void arrowW( int x, int y, int l, int w );
	private:
		int _mhz;
		int _hz;
		unsigned long _xpdrTimer;
		bool  _xpdrTogle;
};

