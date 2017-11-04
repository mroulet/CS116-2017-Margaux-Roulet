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
	 * @brief Get neurons
	 * 
	 */
	 std::vector<Neuron*> getNeurons() const;
	
	/**
	 * @brief Get the number of neurons inside the network.
	 */
	unsigned int getNbNeurons() const;
	
	/**
	 * @brief Matrix of neurons
	 * 
	 */
	std::vector<std::vector< unsigned int> > getIndex() const;
	
	/**
	 * @brief Set the end time of the simulation.
	 * 
	 * @param time interval [ms]
	 */
	void setNetworkStopTime(double t);

	/**
	 * @brief Get the number of excitatory connections
	 * 
	 * @return an integer
	 */
	unsigned int getNbExcitatoryConnections();
	
	/**
	 * @brief Get th number of inhibitory connections
	 *
	 * @return an integer
	 */
	unsigned int getNbInhibitoryConnections();
	
	/**
	 * @brief Get the number of external connections
	 * 
	 * @return an integer
	 */
	unsigned int getNbExternalConnections();
	 
	/**
	 * @brief Get the frequency of threshold
	 * 
	 * @return the value of the frequency
	 */
	double getFrequencyThr();
	
	/**
	 * @brief Get the external frequency
	 * 
	 * @return the value of the external frequency
	 */ 
	double getExternalFrequency();
	
	/**
	 * @brief Record the total number of spikes at each dt in the file "spikes2.txt"
	 */
	void writeSpikeToFile();
	
	/**
	 * @brief Poisson distribution of external Spike
	 * 
	 * @return a random integer that will define the number of external spike receive
	 */ 
	unsigned int poisson();
	
	/**
	 * @brief Uniform distribution of connection
	 */ 
	unsigned int uniform(unsigned int size);
	
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
	
	//à commenter
//	std::vector<unsigned int> getSpikesTable() const;

private:

	double networkStartTime_; //!< Start time of the simulation
	
	double networkStopTime_; //!< End time of the simulation
	
	std::vector<Neuron*> neurons_; //!< Table of neurons of the network

	unsigned int neuronIndex_; //!< Index of the neuron list

	std::mt19937 generator; //!< Mester Twyster engine for random distribution

	std::poisson_distribution<unsigned int> distributionPoisson; //!< Poisson distribution for external spikes
	
	std::vector<unsigned int> spikesTable_; //!< Table in which spikes are recorded by dt
	
	unsigned int nbSpikesTotal_; //!< Number of spikes of all neurons that happen each step time.
	
	/**
	 * @brief Matrix of index corresponding to neurons. 
	 * 
	 * The first vector contains all the neurons of the network.
	 * The second vector contains all the target of each neurons.
	 */
	std::vector<std::vector< unsigned int> > index_;

	double clock_; //!< Global clock of the simulation
	
	/**
	 * @brief File for gnuplot graph
	 * 
	 * Record of the total number of spikes at each dt
	 * Gnuplot graph: xrange: time in ms / yrange: number of spikes
	 */
	std::ofstream* spikesFile_;
	
	/**
	 * @brief File for Jupyter graphs
	 * 
	 * Record of the each neuron index that spikes and its corresponding time spike
	 * First jupyter graph:  xrange: time in ms / yrange: index of the first 50 neurons
	 * Second jupyter graph: xrange: time in ms / yrange: spikes count
	 */
	std::ofstream* spikesIndexFile_;
};

#endif
