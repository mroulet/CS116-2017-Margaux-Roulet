#ifndef network_H
#define network_H
#include <iostream>
#include "neuron.hpp"
#include <cmath>
#include <vector>
#include <fstream>

class Network {

public:
	Network(double networkStopTime, std::list<Neuron*> neurons);
	~Network();
	
	void update(double Iext);

private:
	double networkStartTime_;
	double networkStopTime_;
	
	std::list<Neuron*> neurons_;
	
	double clock_;
};

#endif
