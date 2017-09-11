#include "canvas.h"

Canvas *canvas;

void setup()
{
	canvas = new Canvas();
	simOnSetup( canvas );
}

void loop(void) {
	simOnLoop( canvas );
}
