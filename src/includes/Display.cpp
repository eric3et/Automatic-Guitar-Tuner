#include "Common.h"


void DisplayOLED(int stringNum, int frequency) {
	const char notes[] = {'E','A','D','G','B','e'};
	display.clearDisplay();
	display.setCursor(0, 0);     // Start at top-left corner
	display.setTextColor(SSD1306_WHITE); // Draw white text
  
	for(int i = 0; i <= 5; i++){
		if(i == stringNum){
			display.setTextSize(4);      // Normal 1:1 pixel scale
			display.print(notes[i]);
		}else{
			display.setTextSize(1);      // Normal 1:1 pixel scale
			display.print(notes[i]);
		}
	}
	
    display.setTextSize(2);      // Normal 1:1 pixel scale
    display.setCursor(95,0);     // Start at top-left corner
    display.print("Hz");
    display.setCursor(70,18);     // Start at top-left corner
    
	if(frequency > 62 && frequency < 450) display.print(frequency);
	else{
		for(short i = 0; i < 4;i++){
			if(i==displayLoader) display.print(">");
			else display.print("-");
		}
		displayLoader++;
		if(displayLoader >= 5) displayLoader = 0;
	}
  	display.display();
}

void DisplayInit(){
  	if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    	Serial.println(F("SSD1306 allocation failed"));
    	for(;;);
  	}
}