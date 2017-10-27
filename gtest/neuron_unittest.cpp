#include <iostream>
#include <list>
#include "../src/neuron.hpp"
#include "../src/network.hpp"
#include "gtest/gtest.h"

TEST (NeuronTest1, MembranePotential) {
	Neuron neuron(1,1.0);
	
	//First update test
	neuron.update();
	EXPECT_EQ(20.0*(1.0-std::exp(-0.1/20.0)), neuron.getPotential());
}

TEST (NeuronTest2, SpikeArrivalTime) {
	Neuron neuron1(1,1.01);
	Neuron neuron2(1);
	
	std::list<Neuron*> neurons = { &neuron1, &neuron2};
	
	double stopTime(0.0);
	double time(92.4);
	//unsigned int t = static_cast<unsigned long>(ceil(Delay/dt));
	stopTime = time + Delay;
	
	Network network(stopTime, neurons);
	
	network.update();
	
	//just before neuron2 spike
	EXPECT_EQ(1, neuron1.getNbSpikes());
	EXPECT_EQ(0.0, neuron2.getPotential());
	neuron2.update();
	EXPECT_EQ(0, neuron2.getNbSpikes());
	EXPECT_EQ(0.1, neuron2.getPotential());

}
	
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
