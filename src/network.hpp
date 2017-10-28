#ifndef network_H
#define network_H
#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <fstream>
#include <random>
#include "neuron.hpp"


/*! 
 * @class Network
 * 
 * @brief Managing the network simulation of several neurons.
 * 
 * Temporal evolution of a network of numerous neurons randomly connected.
 * The network define the neuron type. 
 * According to Brunel's model, if N is the number of neuron,
 * there are then Ni = 0.2 N inhibitory neurons and Ne = 0.8 N excitatory neurons.
 * Each neuron receives input from 10% of the other neurons.
 * Connection strength depends on J = Amplitude and g = Ji / Je
 * 
 * @note Brunel's balanced cortex model contains 10 000 excitatory neurons and 2500 inhibitory neurons
 */
class Network {

public:
	/**
	 * @brief Constructor
	 * 
	 * @param network stop time determine the end time of the simulation
	 * @param neurons is the list of local neurons connected
	 */
	Network(double networkStopTime, std::vector<Neuron*> neurons);
	
	/**
	 * @brief Destructor
	 */
	~Network();
	
	/**
	 * @brief Get the number of neurons inside the network.
	 */
	unsigned int getNbNeurons() const;
	
	/**
	 * @brief Set the end time of the simulation.
	 * 
	 * @param time interval [ms]
	 */
	void setNetworkStopTime(double t);
	 
	//.......
	unsigned int getNbExcitatoryConnections();
	unsigned int getNbInhibitoryConnections();
	unsigned int getNbExternalConnections();
	 
	double getFrequencyThr();
	double getExternalFrequency();
	 
	unsigned int poisson();
	unsigned int uniform();
	 
	
	
	/**
	 * @brief Define whether the neuron is excitatory or inhibitory.
	 * 
	 * There are Ne excitatory neurons and Ni inhibitory neurons integrate and fire neurons
	 */
	void defineTypeNeuron();
	
	/**
	 * @brief Connect the local neurons inside the network.
	 * 
	 * Each neuron receives input from 10% of other neurons. 
	 * Hence, there are 0.1*Ne excitatory connections and 0.1*Ni inhibitory neurons.
	 * Each neuron receives randomly chosen connections. 
	 * To generate the connection we need uniformly distributed random numbers.
	 * @note One neuron can connect several times to the same neuron and it can connect to itself.
	 */
	void connect();
	
	/**
	 * @brief Run the simulation of the network
	 * 
	 * Main simulation loop.
	 * The network updates each neurons. 
	 * If a spike occurs in a neuron, the spike is transmitted to the connected neurons at current time t + delay.
	 * @note The network handles the recording into the time buffer of each neuron.
	 */
	void update();

private:
	/**
	 * @brief Start time of the simulation
	 */
	double networkStartTime_;
	
	/**
	 * @brief End time of the simulation
	 */
	double networkStopTime_;
	
	/**
	 * @brief Table of neurons
	 */
	std::vector<Neuron*> neurons_;
	
	/**
	 * Global clock of the simulation
	 */
	double clock_;
};

#endif
