
#include <iostream>
#include <vector>
#include "File.cpp"
#include "QM_Table.cpp"
#include "global.h"

void initialize();
void changeQe(int val);
void checkChangeQ_c();
void renormalize();
void printFileSize(char[],char[],int, int);
using namespace std;
char *memblock;

namespace QM{
	unsigned int A;
	unsigned short int C;
	int currentState;
	unsigned int Q_c;
	bool carry;
	bool MPS;
	bool LPS;
	int scrapBits;
}


int main() {

	//User Interaction to enter filename
		cout << "Input the complete FileName : \n";
		char filename[512];
		cin.getline(filename,512);

//	char filename[] = "resources/binary.dat";

	//load the QM table in the global vector qmcodetable
	char tablefile[] = "QM_state_transition_table.txt";
	Qmtable::loadQMtable(tablefile);

	char opFile[] = "QM_Encoded.qm";
	char *outputFile = opFile;
	writePrepare(outputFile);

	//read the contents of the file and store it in memblock
	memblock = readFileByBytes(filename);
	int originalFileSize = FileSizeinBytes;
	//initialize the QM coder
	initialize();
	cout << "Start Encoding\n" ;
	//read the bits from the file till end of file
	while (!checkEOF()) {
		if (ReadBit() == QM::MPS) {
//			cout << "0";
			QM::A = QM::A - QM::Q_c;
			if(QM::A < 0x8000){
				if(QM::A < QM::Q_c){
					if((QM::C + QM::A) > 0xFFFF) QM::carry = true;
					QM::C += QM::A;
					QM::A = QM::Q_c;
				}
				changeQe(1);
				renormalize();
			}
		}
		else{
//			cout << "1";
			QM::A = QM::A - QM::Q_c;
			if(QM::A >= QM::Q_c){
				if((QM::C + QM::A) > 0xFFFF) QM::carry = true;
					QM::C += QM::A;
					QM::A = QM::Q_c;
			}
			changeQe(0);
			renormalize();
		}
	}

	cout << "Done\n" ;
	cout << "Encoded data stored in [QM_Encoded.qm]\n" ;

	checkStatusOfLastBit();
	closeFile();

	int encodedFileSize = getFileSize("QM_Encoded.qm");
	printFileSize(filename,outputFile,encodedFileSize,originalFileSize);

	system("pause");
	return 0;
}


void initialize(){
	QM::A = 0x10000;
	QM::C = 0x0000;
	QM::currentState = 10;
	QM::Q_c = qmcodetable[QM::currentState]->getQc();
	QM::carry = false;
	QM::MPS = 0;
	QM::LPS = 1;
	QM::scrapBits = 0;
}


  //val = 1 if MPS
  //val = 0 for LPS

void changeQe(int val){
	//MPS received, increment the state
	if(val == 1){
		int temp = qmcodetable[QM::currentState]->getIncS();
		QM::currentState += temp;
	}
	//LPS received, decrement the state
	else{
		int temp = qmcodetable[QM::currentState]->getDecS();
		QM::currentState -= temp;
		//Swap the MSB and LSB
		if(qmcodetable[QM::currentState]->getDecS() == -1){
			temp = QM::LPS;
			QM::LPS = QM::MPS;
			QM::MPS = temp;
		}

	}
	QM::Q_c = qmcodetable[QM::currentState]->getQc();
}

void renormalize(){

	if(QM::carry){
		WriteBit(1);
		QM::scrapBits++;
		//cout << "Carry generated" << endl;
		QM::carry = false;
	}
	//output until the value of A >= 0x8000
	while(QM::A < 0x8000){
		QM::A = QM::A << 1;
		//check if MSB is 0
		if((QM::C & (0x8000)) == 0) {
			//cout << 0;
			WriteBit(0);
			QM::scrapBits++;
		}
		else {
			//cout << 1;
			WriteBit(1);
			QM::scrapBits++;
		}
		//cout << hex << QM::C << endl;
		QM::C = QM::C << 1;
	}
}

//checks whether q_c is changing properly or not.

void checkChangeQ_c(){
	cout << "current state: " << std::dec << QM::currentState << endl;
	cout << "current Qc: " << std::hex << QM::Q_c << endl;
	changeQe(0);
	cout << "next state: " << std::dec << QM::currentState << endl;
	cout << "next Qc: " << std::hex << QM::Q_c << endl;
}

void printFileSize(char inputFileName[], char outputFileName[], int encodedFileSize, int originalFileSize){
	cout << endl;
	cout << "----------------------------------\n\n" ;
	cout << "Input FileName      : " <<inputFileName ;
	cout << "\nOutput FileName     : " << outputFileName;
	cout << "\nOriginal FileSize   : " << originalFileSize << " bytes" ;
	cout << "\nCompressed FileSize : " << encodedFileSize << " bytes" ;
	cout << "\nCompression Ratio   : " << (float)encodedFileSize/originalFileSize*100 << "%\n";
	cout << "\n----------------------------------\n" ;
}


