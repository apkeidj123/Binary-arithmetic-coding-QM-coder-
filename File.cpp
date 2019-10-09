

#include "File.h"

// stores file Size
int FileSizeinBytes;
std::ofstream myFile;

using namespace std;

int getFileSize(char *filename){
	streampos size;
	ifstream file (filename, ios::in|ios::binary|ios::ate);
	if(file.is_open()) {
	    size = file.tellg();
	    file.close();
	}else{
		cout << "Unable to open the output file\n";
	}
	return size;
}

char* readFileByBytes(char* fileName){
		streampos size;
		  char * memblock;

		  ifstream file (fileName, ios::in|ios::binary|ios::ate);
		  if (file.is_open())
		  {
		    size = file.tellg();
		    memblock = new char [size];
		    file.seekg (0, ios::beg);
		    file.read (memblock, size);
		    file.close();

		   // cout << "The entire file content is in memory\n" ;
		    FileSizeinBytes = size;

//		    cout << "File Size : " << FileSizeinBytes << endl;
		    //delete[] memblock;
		  }
		  else {
			  cout << "Error: Unable to open the file";

		  }

		  return memblock;
}

void WriteByte(unsigned char byte){
	cout << " " << (int)byte ;
}

void writeFileByBytes(unsigned char data){
	//cout << "data " << (int)data << endl;
			  myFile << data;
}

unsigned char b;
int s;

void WriteBit(bool x)
{
//    b |= (x ? 1 : 0) << s;
    b |= (x ? 1 : 0) << (7-s);
    s++;

    if (s == 8)
    {
    	writeFileByBytes(b);
        b = 0;
        s = 0;
    }
}


void writeSingleCode(unsigned long code, char size)	{

	for(int i = 0; i < size; i++){
				bool x = ((code & (1 << i))?1:0);
					WriteBit(x);
		}
}

 //Check the last bits status

void checkStatusOfLastBit(){
	if(s <= 8){
		for (int i = 0; i < s; i++)
			b |= 0 << s;
		writeFileByBytes(b);
	}
}


 //fileName : name of the file to write

void writePrepare(char *fileName){

	  myFile.open(fileName, ios::out|ios::binary);
}

//for read bits
namespace RB {
	int pointer;
	unsigned char b1;
	int s1;
}

// reads the file bit by bit (starting from MSB)

//pointer -> stores the current location in the memory block.
// b1 -> stores the current symbol.
// s1 -> stores the current count from the 8 bits.

bool ReadBit() {
	if (RB::s1 == 0) {
		RB::b1 = memblock[RB::pointer++];
//		cout << (char)b1 << endl;
		RB::s1 = 8;
	}
//	cout << "s: " << RB::s1 << endl;

//	bool bit = 1;
//	if(((RB::b1 >> (RB::s1-1)) & 1) == 0){
//		bit = 0;
//	}
	bool bit = (RB::b1 >> (RB::s1-1)) & 1;
	RB::s1--;
	return bit;
}

bool checkEOF() {
	bool bit = 0;
	//IF the count is equal to FileSize and s1-> points to LSB.
	if (RB::pointer == (FileSizeinBytes) && RB::s1 == 0)
		bit = 1;
//	cout << "pointer: " << RB::pointer << endl;
	return bit;
}

void closeFile(){
	myFile.close();
}


