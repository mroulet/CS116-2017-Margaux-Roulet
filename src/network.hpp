#ifndef network_H
#define network_H
#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <fstream>
#include "neuron.hpp"

class Network {

public:
	Network(double networkStopTime, std::list<Neuron*> neurons);
	~Network();
	
	void update();
	void connect();

private:
	double networkStartTime_;
	double networkStopTime_;
	
	std::list<Neuron*> neurons_;
	
	double clock_;
};

#endif
