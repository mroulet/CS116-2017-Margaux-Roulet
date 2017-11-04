#include <iostream>
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
	
	std::vector<Neuron*> neurons = { &neuron1, &neuron2};

	double stopTime(0.0);
	double time(92.4);
	stopTime = time + Delay -0.1;
	
	Network network(stopTime, neurons);
	
	network.update();

	//just before neuron2 spike
	EXPECT_EQ(1, neuron1.getNbSpikes());
	EXPECT_EQ(0.0, neuron2.getPotential());
	neuron2.update();
	EXPECT_EQ(0, neuron2.getNbSpikes());
	EXPECT_EQ(0.1, neuron2.getPotential());

}

TEST (NeuronTest3, PositiveInput) {
	Neuron neuron(1);
	neuron.setIext(1.0);
	
	//First update test
	neuron.update();
	EXPECT_EQ(20.0*(1.0-std::exp(-0.1/20.0)), neuron.getPotential());
	
	//Test after numerous update
	unsigned int i(0);
	while (i < 10000) {
		neuron.update();
		++i;
	}
	
	
	EXPECT_EQ(0,neuron.getNbSpikes());
	EXPECT_GT(1E-3, std::fabs(19.999 - neuron.getPotential()));
	
	neuron.setIext(0.0);
	
	unsigned int j(0);
	while (j < 2000) {
		neuron.update();
		++j;
	}
	
	EXPECT_NEAR(0, neuron.getPotential(), 1e-3);	
	
}

TEST (NeuronTest4, NegativeInput) {
	Neuron neuron(1);
	neuron.setIext(-1.0);
	
	//First update test
	neuron.update();
	EXPECT_EQ(-20.0*1.0*(1-std::exp(-0.1/20.0)), neuron.getPotential());
	
	//Test after numerous update
	unsigned int i(0);
	while (i < 1000) {
		neuron.update();
		++i;
	}
	
	EXPECT_EQ(0,neuron.getNbSpikes());
	EXPECT_GT(-20.0*1.0*(1-std::exp(-0.1/20.0)), neuron.getPotential());
	
	neuron.setIext(0.0);
	
	unsigned int j(0);
	while (j < 2000) {
		neuron.update();
		++j;
	}
	
	EXPECT_NEAR(0, neuron.getPotential(), 1e-3);	
	
}

TEST (NeuronTest5, oneNeuronSimulationSpikes) {
	Neuron neuron(1, 1.01);
	//We know that spikes imes arrive at 92.4ms 186.9ms 281.3ms 376.5;
	
	unsigned int j(0);
	while (j < 4000) {
		neuron.update();
		++j;
	}
	
	EXPECT_EQ(4, neuron.getNbSpikes());
}

TEST (TwoNeuronsTest1, InhibitatoryConnection_Delay) {
	Neuron neuron1(1,1.01);
	neuron1.setIsInhibiter(true);
	Neuron neuron2(1);
	
	std::vector<Neuron*> neurons = { &neuron1, &neuron2};

	double stopTime(0.0);
	double time(92.4);
	stopTime = time + Delay - 0.1;
	
	Network network(stopTime, neurons);
	
	network.update();

	//just before neuron2 spike
	EXPECT_EQ(1, neuron1.getNbSpikes());
	EXPECT_EQ(0.0, neuron2.getPotential());
	neuron2.update();
	EXPECT_EQ(0, neuron2.getNbSpikes());
	EXPECT_EQ(-g*0.1, neuron2.getPotential());

}

TEST (TwoNeuronsTest2, ExcitatoryConnection_Delay) {
	Neuron neuron1(1,1.01);
	Neuron neuron2(1);
	
	std::vector<Neuron*> neurons = { &neuron1, &neuron2};

	double stopTime(0.0);
	double time(92.4);
	stopTime = time + Delay - 0.1;
	
	Network network(stopTime, neurons);
	
	
	
	network.update();

	//just before neuron2 spike
	EXPECT_EQ(1, neuron1.getNbSpikes());
	EXPECT_EQ(0.0, neuron2.getPotential());
	neuron2.update();
	EXPECT_EQ(0, neuron2.getNbSpikes());
	EXPECT_EQ(0.1, neuron2.getPotential());

}

TEST (NetworkTest1, typeDefinition) {
	
	std::vector<Neuron*> neurons;
	
	// The network is composed of 100 neurons
	for (unsigned int i(0); i < 100; ++i) {
		Neuron* n = new Neuron(1);
		neurons.push_back(n);
	}
	Network network(1, neurons);
	unsigned int nbInhibitory(0);
	unsigned int nbExcitatory(0);

	for (auto neuron : network.getNeurons()) {

		if (neuron->isInhibiter()) {
			++nbInhibitory;
		} else { ++nbExcitatory; }
	}
	
	EXPECT_EQ(20, nbInhibitory);
	EXPECT_EQ(80, nbExcitatory);
}

TEST (NetworkTest2, connections) {
	
	std::vector<Neuron*> neurons;
	
	// The network is composed of 100 neurons
	for (unsigned int i(0); i < 1000; ++i) {
		Neuron* n = new Neuron(1);
		neurons.push_back(n);
	}
	
	Network network(1, neurons);
	
	EXPECT_EQ(20, network.getNbInhibitoryConnections());
	EXPECT_EQ(80, network.getNbExcitatoryConnections());
}	
/*
TEST (optimisation, test1) {

	std::vector<Neuron*> neurons;
	
	// The network is composed of 100 neurons
	for (unsigned int i(0); i < 12500; ++i) {
		Neuron* n = new Neuron(1);
		neurons.push_back(n);
	}
	
	Network network(100, neurons);
	std::cout << "nbNeurons: " << network.getNbNeurons() << std::endl;

	unsigned int k(0);
	while(k < 100)	{
		//cout << "update: " << k << endl;
///		for (size_t i(0); i < index_.size(); ++i) {
		for (auto neuron : network.getNeurons()) {
			
///			neuron[i]->update();			
			neuron->update();			
						
			if (network.getNbNeurons() >= 50) {
			
				//randomly distributed external spike from outside network
				unsigned int backgroundNoise(network.poisson());
				
				// si le poisson process active les synapses externe	
				//alors pour chaque synapses externe activées (random) un spike est distribué au neurone
///				neuron[i]->setSpikesReceived(index_[i]->getSpikesReceived() + backgroundNoise);
				neuron->setSpikesReceived(neuron->getSpikesReceived() + backgroundNoise);
			}		
			
			//notre neurone est connecté à plusieurs neurones sources	
///			for (auto neuronSource : index_[i])	{
			for (auto neuronSource : neuron->getSynapses()) {					
				
				//si un neurone source spike, on ajoute un spike à notre neuron:
				if (neuronSource->hasSpike()) {
					
					//négatif si le neurone source est inhibiteur
					if (neuronSource->isInhibiter()) {
///						neuron[i]->setSpikesReceived(neuron[i]->getSpikesReceived() - g);
						neuron->setSpikesReceived(neuron->getSpikesReceived() - g);
					
					//positif si le neurone source est excitateur
					} else {
///						neuron[i]->setSpikesReceived(neuron[i]->getSpikesReceived() + 1);
						neuron->setSpikesReceived(neuron->getSpikesReceived() + 1);
					}	
				}
			}	
///			if (neuron[i]->hasSpike()) {
			if (neuron->hasSpike()) {
//				++netwok.getnbSpnbSpikesTotal_;
			}	
			
///			neuron->fillBuffer();

		} 
			
		for (auto neuron : network.getNeurons()) {
			neuron->fillBuffer();
		}
	
//	cout << "update: " << k << endl;
//	cout << "nbSpike: " << nbSpikesTotal_ << endl;	
//	spikesTable_.push_back(nbSpikesTotal_);
/// writeSpikeToFile(nbSpikesTotal_);
	*spikesFile_ << k*dt << " ";
	*spikesFile_ << nbSpikesTotal_ << endl;
	nbSpikesTotal_ = 0;
		
	// incrémentation du pas de temps (steptime)
	clock_ += h;	
	++k; //cout incrémenteur
	
	}
		++k;
			}
		EXPECT_EQ(1, 1);
	
}
*/
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
