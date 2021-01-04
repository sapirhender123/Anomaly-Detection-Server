#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
    CLIData data;

private:
    Command** commands;

public:
	explicit CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
    void printOptions();
    void classifyOpt(int option);
};

#endif /* CLI_H_ */
