
#ifndef QMTABLE_H_
#define QMTABLE_H_
#include "global.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;
class Qmtable {
	int state;
	unsigned int	Qc;
	int	incS;
	int	decS;
public:
	Qmtable();
	virtual ~Qmtable();
	int getDecS() const;
	void setDecS(int decS);
	int getIncS() const;
	void setIncS(int incS);
	unsigned int getQc() const;
	void setQc(unsigned int qc);
	int getState() const;
	void setState(int state);
	void printTable();
	static void loadQMtable(char[]);
};

#endif /* QMTABLE_H_ */
