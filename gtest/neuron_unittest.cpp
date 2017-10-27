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
	
	double stopTime(1);
	Network network(stopTime, neurons);
	unsigned int time(1869);
	unsigned int t = static_cast<unsigned long>(floor(Delay/dt));
	
	//update du network jusqu'à ce que le premier neurone spike
	for (unsigned int i(0); i<time+t; ++i) {
		neuron1.update();
		if (neuron1.hasSpike()) {
			neuron2.setSpikesReceived(1);
			neuron2.fillBuffer();
		}
		neuron2.update();
	}
	
	//just before neuron2 spike
	EXPECT_EQ(1, neuron2.getNbSpikes());
	neuron2.update();
	EXPECT_EQ(0, neuron2.getPotential());
	EXPECT_EQ(1, neuron2.getNbSpikes());
			


	EXPECT_EQ(0.1, neuron2.getPotential());
}
	
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
