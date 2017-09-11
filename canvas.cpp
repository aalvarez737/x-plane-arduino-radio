#include "canvas.h"

#include "SPI.h"
#include "PDQ_ILI9341_config.h"
#include "PDQ_GFX.h"
#include "PDQ_ILI9341.h"

#include "fonts/verdana.ttf16.h"
#include "fonts/verdana.ttf10.h"
#include "fonts/digital-7__mono_.ttf38.h"

PDQ_ILI9341 _sur;

Canvas::Canvas() {
	#if defined(ILI9341_RST_PIN)	// reset like Adafruit does
	FastPin<ILI9341_RST_PIN>::setOutput();
	FastPin<ILI9341_RST_PIN>::hi();
	FastPin<ILI9341_RST_PIN>::lo();
	delay(1);
	FastPin<ILI9341_RST_PIN>::hi();
	#endif
	_sur.begin();
	_sur.setRotation( 3 );
	_mhz = 0;
	_hz = 0;
	_xpdrTimer = millis();
	_xpdrTogle = true;
}

Canvas::~Canvas() {
}

void Canvas::freqBg( char *type ) {
	_sur.fillScreen(BG_COLOR);
	drawBg();
	_sur.setTextColor(ILI9341_WHITE);
	_sur.setFont(&FONT_DESC);
	_sur.setCursor( 20, 75 );
	_sur.print( type );
	_sur.setCursor( 20, 195 );
	_sur.print("STBY");
	arrowH( 50, 105, 30, 12 );
	_sur.setFont(&FONT_FREQ);
	_sur.setTextColor(ILI9341_GREEN);
	_sur.setCursor( 210, 90 );
	_sur.print(".");
	_sur.setTextColor(ILI9341_WHITE);
	_sur.setCursor( 210, 210 );
	_sur.print(".");
}

void Canvas::freqActive( int freq ) {
	drawFreqImpl( freq, 90, ILI9341_GREEN );
}

void Canvas::freqStandBy( int freq ) {
	drawFreqImpl( freq, 210, ILI9341_WHITE );
}

void Canvas::drawFreqImpl( int freq, int y, uint16_t color ) {
	_sur.setFont(&FONT_FREQ);
	if (freq > 10000) {
		int hz = (int)freq %100;
		int mhz = (freq - hz ) / 100;
		if ( mhz != _mhz ) {
			int x = 115;
			_sur.setCursor( x, y );
			_sur.setTextColor(BG_COLOR);
			_sur.print( _mhz );
			_mhz = mhz;
			_sur.setCursor( x, y );
			_sur.setTextColor(color);
			_sur.print( _mhz );
		}
		if ( hz != _hz ) {
			int x = 235;
			_sur.setCursor( x, y );
			_sur.setTextColor(BG_COLOR);
			if( _hz < 10)
				_sur.print( 0 );
			_sur.print( _hz );
			_hz = hz;
			_sur.setCursor( x, y );
			_sur.setTextColor(color);
			if( _hz < 10)
				_sur.print( 0 );
			_sur.print( _hz );
		}
	}
}

void Canvas::adfBg() {
	_sur.fillScreen(BG_COLOR);
	drawBg();
	_sur.setTextColor(ILI9341_WHITE);
	_sur.setFont(&FONT_DESC);
	_sur.setCursor( 20, 75 );
	_sur.print( "ADF1" );
	_sur.setCursor( 20, 195 );
	_sur.print("ADF2");
}

void Canvas::adf1( int adf ) {
	adfImpl( adf, 40 );}

void Canvas::adf2( int adf ) {
	adfImpl( adf, 160 );
}

void Canvas::adfImpl( int adf, int y ) {
	_sur.setTextColor(ILI9341_WHITE);
	_sur.setFont(&FONT_FREQ);
	_sur.fillRect( 200, y, 100, 55, BG_COLOR);
	_sur.setCursor( 200, y+50 );
	_sur.print( adf );
}

void Canvas::drawBg() {
	int middleW = _sur.width()/2+BG_BORDER/2;
	int middleH = _sur.height()*3/4+BG_BORDER/2;
	for(int i=1; i<BG_BORDER; i++){
		_sur.drawRect( i, i, _sur.width()-i*2, _sur.height()-i*2, BG_BORDER_COLOR);
//		_sur.drawLine( middle+i, i, middle+i, _sur.height()-i*2, BG_BORDER_COLOR);
//		_sur.drawLine( i,  middleH+i, _sur.width()-i*2, middleH+i, BG_BORDER_COLOR);
//		_sur.drawLine( i,  middleH+i, _sur.width()-i*2, middleH+i, BG_BORDER_COLOR);
	}
	_sur.setFont(&FONT_SMALL);
	_sur.setCursor( 220, 20 );
	_sur.setTextColor(ILI9341_WHITE);
	_sur.print("XPDR");
/*	sur->setCursor( 10, 20 );
	sur->setTextColor(ILI9341_WHITE);
	sur->print("ADF");
	sur->setTextColor(ILI9341_GREEN);
	sur->print("COM1");
	sur->setTextColor(ILI9341_WHITE);
	sur->print("COM2");
	sur->setTextColor(ILI9341_CYAN);
	sur->print("NAV1");
	sur->setTextColor(ILI9341_WHITE);
	sur->print("NAV2");*/
}

//	Aux functions
void Canvas::arrowW( int x, int y, int l, int w ) {
	_sur.fillRect( x,  y-5, l, 10, BG_BORDER_COLOR );
	_sur.fillTriangle( x-w, y, x, y + w, x, y - w,	BG_BORDER_COLOR );
	_sur.fillTriangle( x + l + w, y, x + l, y + w, x + l, y - w, BG_BORDER_COLOR );
}

void Canvas::arrowH( int x, int y, int l, int w ) {
	_sur.fillTriangle( x, y-w, x-w, y, x+w, y, BG_BORDER_COLOR );
	_sur.fillRect( x-5, y, 11, l, BG_BORDER_COLOR);
	_sur.fillTriangle( x, y+w+l, x-w, y+l, x+w, y+l, BG_BORDER_COLOR );
}

void Canvas::updateXPDR() {
	if (millis() - _xpdrTimer > 500) {
		_xpdrTimer = millis();
		if (_xpdrTogle) {
			_sur.setTextColor(ILI9341_GREEN);
		} else {
			_sur.setTextColor(ILI9341_BLACK);
		}
		_xpdrTogle = !_xpdrTogle;
		_sur.setFont(&FONT_SMALL);
		_sur.setCursor( 280, 20 );
		_sur.print("ON");
	}
}
