#include "testApp.h"
int y = 0;
//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofxXmlSettings xml;
    host = "127.0.0.1";
    port = 2838;
    numLED = 160;
    ofLogToFile("log/"+ofGetTimestampString()+".log", true);
    ofLogVerbose()<< "Going to load settings";
	if(xml.loadFile("configs.xml"))
	{
		string str;
		xml.copyXmlToString(str);
		
		if(xml.pushTag("DATA"))
		{
			if (xml.getValue("FULLSCREEN", 0)>0) {
				
				ofSetFullscreen(true);
			}
			ofSetLogLevel((ofLogLevel)xml.getValue("LOG_LEVEL", 0));
			host = xml.getValue("IP", "127.0.0.1");
            port = xml.getValue("PORT", 2838);
            numLED = xml.getValue("NUM_LED", 160);
            
            if(xml.pushTag("SEQUENCES"))
            {
                int numTag = xml.getNumTags("SEQUENCE");
                ofLogVerbose()<< "numTag "<< numTag;
                for(int i= 0 ; i < numTag ; i++){
                    sequences.push_back(new ofxImageSequence());
                    sequences.back()->loadSequence(xml.getValue("SEQUENCE", "./sequences",i));
                    sequences.back()->setFrameRate(xml.getValue("FRAMERATE",12));
                }
                xml.popTag();
            }
        }
    }
    else
    {
        ofLogError("config.xml")<< "error load config";
    }

    
    
    led = new ofxLEDsLPD8806(numLED);
    
	if(	spi.connect())
	{
		ofLogNotice()<<"connected to SPI";
	}

}
void testApp::exit()
{

    ofLogToConsole();

	ofLogVerbose("spi")<< "close and clear led";
	led->clear(0);
    
	spi.send(led->txBuffer);
}

//--------------------------------------------------------------
void testApp::update(){
    
    led->renderBuffer.begin();
    for( int i = 0 ; i < sequences.size() ;i++)
    {
        ofTexture *tex = sequences[i]->getFrameForTime(ofGetElapsedTimef());
        tex->draw(0,(y--)%int(tex->getHeight()));
    }
    led->renderBuffer.end();
    led->encode();
    
    spi.send(led->txBuffer);


}

//--------------------------------------------------------------
void testApp::draw(){
   
    ofPushMatrix();
    ofScale(5,5);
    led->encodedBuffer.draw(20,40);
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}