//
//  ofxLPD8806.h
//
//
//  Created by james KONG on 18/1/13.
//
//

#pragma once

#include "ofMain.h"
#ifdef TARGET_LINUX_ARM
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#endif
class ofxLPD8806
{
public:
	ofxLPD8806();
	~ofxLPD8806();
	bool connect();
	void send(const std::vector<uint8_t>& data);
	u_int8_t	toGamma(u_int8_t c);
private:
	int spi_init(int filedes);
	string device;
	int spi_device;
	bool connected;
	u_int8_t GAMMA[256];
};
