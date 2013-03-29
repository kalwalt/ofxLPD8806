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

    weConnected = client.setup(host,port);
	//optionally set the delimiter to something else.  The delimter in the client and the server have to be the same
	client.setMessageDelimiter("\n");
	
	connectTime = 0;
	deltaTime = 0;
    
    
    led = new ofxLEDsLPD8806(numLED);
    
	if(	spi.connect())
	{
		ofLogNotice()<<"connected to SPI";
	}
    
//	colors.assign(numLED,ofColor());
//	startThread(false,false);
    pix.allocate(numLED, 1, OF_IMAGE_COLOR);
}
void testApp::exit()
{
    ofLogToConsole();
//    stopThread();
	ofLogVerbose("spi")<< "close and clear led";
	led->clear(0);
    
	spi.send(led->txBuffer);
}
//void testApp::threadedFunction()
//{
//	while( isThreadRunning() != 0 ){
////		if( lock() ){
//			
////			led->setPixels(colors);
//        
//			spi.send(led->txBuffer);
//            
//			
////			unlock();
//        usleep(2);
////			usleep(10000);
////		}
//	}
//}
//--------------------------------------------------------------
void testApp::update(){
    if(weConnected){
		string cmd = client.receive();
		if(cmd!="")
		{
			ofLogVerbose("TCP Client") << "CMD " << cmd;
			int start_idx = 0;
			int end_idx = cmd.find(";");
			
        }
		if(!client.isConnected()){
			weConnected = false;
		}
	}else{
		//if we are not connected lets try and reconnect every 5 seconds
		deltaTime = ofGetElapsedTimeMillis() - connectTime;
		
		if( deltaTime > 5000 ){
			weConnected = client.setup(host,port);
			connectTime = ofGetElapsedTimeMillis();
		}
		
	}
    
    led->renderBuffer.begin();
    for( int i = 0 ; i < sequences.size() ;i++)
    {
        ofTexture *tex = sequences[i]->getFrameForTime(ofGetElapsedTimef());
        tex->draw(0,(y--)%int(tex->getHeight()));
    }
    led->renderBuffer.end();
    led->encode();
    led->encodedBuffer.readToPixels(pix);
    
    spi.send(led->txBuffer);
    for(int j = 0 ; j < pix.getHeight() ; j++)
    {
        for(int i = 0 ; i < pix.getWidth() ; i++)
        {
            ofColor c = pix.getColor(i, j);
            printf( "%i | %i | %i | ",c.r,c.g,c.b) ;
//            if((i+1)%3==0)
                cout << "\n";
        }
    }

//    for(int i = 0 ; i < led->txBuffer.size() ; i++)
//    {
//        printf( "%u | ",led->txBuffer[i]) ;
//        if((i+1)%3==0)cout << "\n";
//    }
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