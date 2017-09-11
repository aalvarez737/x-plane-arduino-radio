#define ADF1		1
#define ADF2		2
#define COM1_ACT	3
#define COM1_STB	4
#define COM2_ACT	5
#define COM2_STB	6
#define NAV1_ACT	7
#define NAV1_STB	8
#define NAV2_ACT	9
#define NAV2_STB	10
#define XPDR		11

int _current1;
int _current2;
int _encInc;
int _encDec;
bool _encMode;
int _switchFreq;
long oldPosition  = -999;

void simOnSetup( Canvas *canvas ) {
	_current1 = COM1_ACT;
	_current2 = COM1_STB;
	_encInc		= 10;
	_encDec		= 11;
	_switchFreq = 15;
	canvas->freqBg("COM1" );
	canvas->freqActive( 12590 );
	canvas->freqStandBy( 11885 );
	pinMode(7,INPUT_PULLUP);
}

void simOnLoop( Canvas *canvas ) {
	if (digitalRead(7) == LOW) {
		incMode( canvas );
		updateCurrent1( canvas );
		updateCurrent2( canvas );
	}
	canvas->updateXPDR();
}

void incMode( Canvas *canvas ) {
	_encMode	= true;
	switch (_current1) {
		case ADF1:
			_current1	= COM1_ACT;
			_current2	= COM1_STB;
			_encInc		= 10;
			_encDec		= 11;
			_switchFreq = 14;
			canvas->freqBg( "COM1" );
			break;
		case COM1_ACT:
			_current1 = COM2_ACT;
			_current2 = COM2_STB;
			_encInc		= 20;
			_encDec		= 21;
			_switchFreq = 24;
			canvas->freqBg( "COM2" );
			break;
		case COM2_ACT:
			_current1 = NAV1_ACT;
			_current2 = NAV1_STB;
			_encInc		= 30;
			_encDec		= 31;
			_switchFreq = 34;
			canvas->freqBg( "NAV1" );
			break;
		case NAV1_ACT:
			_current1 = NAV2_ACT;
			_current2 = NAV2_STB;
			_encInc		= 40;
			_encDec		= 41;
			_switchFreq = 44;
			canvas->freqBg( "NAV2" );
			break;
		case NAV2_ACT:
			_current1 = ADF1;
			_current2 = ADF2;
			canvas->adfBg();
			break;
		default:
			break;
	}
}

void updateCurrent1( Canvas *canvas ) {
	switch (_current1) {
		case ADF1:
			canvas->adf1( 290 );
			break;
		case COM1_ACT:
		case COM2_ACT:
			canvas->freqActive( 11885 );
			break;
		case NAV1_ACT:
		case NAV2_ACT:
			canvas->freqActive( 11320 );
			break;
		default:
			break;
	}
}

void updateCurrent2( Canvas *canvas ) {
	switch (_current2) {
		case ADF2:
			canvas->adf2( 195 );
			break;
		case COM1_STB:
		case COM2_STB:
			canvas->freqStandBy( 12590 );
			break;
		case NAV1_STB:
		case NAV2_STB:
			canvas->freqStandBy( 11440 );
			break;
		default:
			break;
	}
}
