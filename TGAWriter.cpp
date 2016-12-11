//
//	TGAWriter.cpp
//	RaXaR
//	Based on code from http://www.codermind.com/
//	Lewis Christie


#include "TGAWriter.h"

TGAWriter::TGAWriter(int nWidth, int nHeight){

	width = nWidth;
	height = nHeight;
	data = new float[width * height * 3];
	currentPixel = 0;

}

void TGAWriter::putNextPixel(float red, float green, float blue){

	data[currentPixel] = blue;
	data[currentPixel + 1] = green;
	data[currentPixel + 2] = red;
	currentPixel += 3;

}

void TGAWriter::putNextPixel(Colour c){
	putNextPixel((float)c.red(),(float)c.green(),(float)c.blue());
}

bool TGAWriter::writeImage(){
	if(currentPixel < width * height * 3){
		return false;
	}

	string fileName = "output.tga";

	ofstream imageFile(fileName.c_str(), ios_base::binary);

	if(!imageFile){
		return false;
	}

	// Addition of the TGA header (source: http://www.codermind.com/)
	imageFile.put(0).put(0);
	imageFile.put(2);

	imageFile.put(0).put(0);
	imageFile.put(0).put(0);
	imageFile.put(0);

	imageFile.put(0).put(0);
	imageFile.put(0).put(0);

	imageFile.put((unsigned char)(width & 0x00FF)).put((unsigned char)((width & 0xFF00) / 256));
	imageFile.put((unsigned char)(height & 0x00FF)).put((unsigned char)((height & 0xFF00) / 256));
	imageFile.put(24);
	imageFile.put(0);
	// end of the TGA header


	for(int i = 0; i < (width * height * 3); i++){
		imageFile.put((unsigned char)min(data[i]*255.0f,255.0f));
	}

	return true;

}

TGAWriter::~TGAWriter(){
	delete data;
}
