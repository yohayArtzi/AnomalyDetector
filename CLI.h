

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
    TimeSeries *ts;
    vector<Command*> cmds;

public:
	CLI(DefaultIO* defaultIO);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
