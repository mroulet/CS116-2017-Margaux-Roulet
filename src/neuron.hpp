#ifndef neuron_H
#define neuron_H
#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>
#include <fstream>
#include <list>
#include <math.h>

///Constants
const double Threshold = 20.0; 					//!< Maximum potential limit [mV]
const double PotentialReset = 0.0; 				//!< Potential reset  [mV]
const double dt = 0.1; 							//!< Time variation [ms]
const unsigned int h = 1; 						//!< Steptime (will be converted en ms)
const double Resistance = 20.0; 				//!< Membrane Resistance [Ohm]
const double Capacity = 1.0; 					//!< Capacity [Farrad]
const double tau = Resistance * Capacity; 		//!< Tau excitatory
const double e = exp(-dt/tau);					//!< needed for the membrane equation 
const double tauRp = 2.0; 						//!< Refractory time period [ms]
const double Amplitude = 0.1; 					//!< Spike Amplitude received from excitatory neurons
const double Delay = 1.5; 						//!< Transmission delay [ms]
const double g = 4.5;							//!< Relative strength of inhibitory synapses
const double ConnectionPercent = 0.1;			//!< Connections pourcentage in the whole network
const double Eta = 0.9;							//!< Eta function
const double Vext = Threshold * Eta /(Amplitude*tau); //!< External frequency
const unsigned int DelayStep = static_cast<unsigned long>(floor(Delay/dt)); //!< Delay in steps

/*! 
 * @class Neuron
 * 
 * @brief Managing the neuron simulation.
 * 
 * Temporal evolution of the neuron: its potential, state and number of spikes.
 * Each steptime the membrane potential is updated and the spike condition checked.
 * Each neuron receives activity (spikes) from its local network (excitatory and inhibitory)
 * and from the rest of the brain (only excitatory).
 */
class Neuron {
	
public:
	/**
     * @brief Constructor
     *
     * @param stopTime of the simuation of the neuron
     * @param iext is the external current. Default value = 0.0
     * @param membrane potential. Default value = 0.0
     * 
     * @note Initialisation of the time buffer necessary for the right implemtation of the transmission delay
     */
	Neuron(double stopTime, double iext = 0.0, double potential = 0.0);
	
	/**
	 * @brief Destructor
	 */
	~Neuron();
	
	/**
	 * @brief Get the potential.
	 * 
	 * @return current value of membrane potential.
	 */
	double getPotential() const;
	
	/**
	 * @brief Get the number of spikes.
	 * 
	 * @return current value of number of spike that occured.
	 */
	unsigned int getNbSpikes() const;
	
	/**
	 * @brief Get spikes time arrivals.
	 * 
	 * @return current times when spikes occured, stored in a vector.
	 */
	std::vector<double> getSpikesTimes() const;
	
	/**
	 * @brief Get the refractory time.
	 * 
	 * @return current value of refractory time.
	 */
	double getRefractoryTime() const;
	
	/**
	 * @brief Get synapses.
	 * 
	 * @return current connections of this neuron with all other neurons from network connected with it.
	 */
	std::vector<Neuron*> getSynapses() const;
	
	/**
	 * @brief Get potentials.
	 * 
	 * @return current values of membrane potential for each dt.
	 */
	std::vector<double> getPotentials() const;
	
	/**
	 * @brief Get the potential state.
	 * 
	 * @return one neuron has a spike if its membrane potential is greater than threshold.
	 */	
	bool hasSpike() const;
	
	/**
	 * @brief Get the type of the neuron
	 * 
	 * @return is inhibiter or is excitatory
	 */
	bool isInhibiter() const;
	
	/**
	 * @brief Get spikes received.
	 * 
	 * @return current numbers of spike received from connected neurons.
	 */
	int getSpikesReceived() const;
	
	/**
	 * @brief Set potential.
	 * 
	 * Set current value of membrane potential.
	 */
	void setPotential(double potential);
	
	/**
	 * @brief Set number of spikes.
	 * 
	 * Set current value of number of spikes that occured.
	 */
	void setNbSpikes(unsigned int spikes);
	
	/**
	 * @brief Set the refractory state.
	 * 
	 * Set the refractory state of the neuron. 
	 * One neuron is refractory during 2ms after passing the threshold.
	 * When refractory, the membrane potential is set to 0.0.
	 */
	void setIsRefractory(bool state);
	
	/**
	 * @brief Set the neuron type.
	 * 
	 * Whether the neuron is Excitatory or Inhibitory.
	 */
	void setIsInhibiter(bool type);
	
	/**
	 * @brief Set refractory time.
	 * 
	 * Set current value of the refractory timer.
	 */
	void setRefractoryTime(double time);
	
	/**
	 * @brief Set number of spikes recevied.
	 * 
	 * Set current value of number of spikes received from each neuron connected every dt.
	 */
	void setSpikesReceived(int spikes);
	
	/**
	 * @brief Set the external current.
	 * 
	 * Set current value of the external current.
	 */
	void setIext(double iext);
	
	/**
	 * @brief Add synapse.
	 * 
	 * Create a connection between two neurons.
	 */
	void addSynapse(Neuron* const n);
	
	/**
	 * @brief Calculate the membrane potential of the Neuron.
	 * 
	 * General solution of the Brunel linear differential equation.
	 * Approximated for a constant current input Iext and a constant amplitude J.
	 * 
	 * @return the updated value of potential
	 */
	double membraneEq();
	
	/**
	 * @brief Run the simulation of one neuron.
	 * 
	 * This function is the main loop of the class Neuron
	 * 
	 * For a time simulation given:
	 * if the potential is greater than the threshold we set the neuron refractory
	 * if the neuron is refractory the potential is reset
	 * otherwise solve the membrane equation to update the potential
	 */
	void update();
	
	/**
	 * @brief Record the spikes received by a connected neuron into the time buffer at time t + Delay.
	 * 	
	 * If the potential is greater than the threshold, 
	 * a spike is signal at time t to the post synaptic neuron. 
	 * The post synaptic neuron responds at t + Delay
	 * 
	 * @note The buffer size is equal to the number of steptime needed to accomplish the time transmission delay
	 * Each step time of the simulation, to fill the buffer, we push_back
	 */
	void fillBuffer();
//	/**
//	 * @brief Read the spikes received by a connected neuron at time t.
//	 * 
//	 * @note Each step time of the simulation, to empty the buffer, we read the first value of the buffer and erase it.
//	 */
//	void emptyBuffer();
	
	/**
	 * @brief Update the buffer index in which you record spikes
	 */
	void updateWriteBox();
	
	/**
	 * @brief Update the buffer index in which your read the spikes
	 */
	void updateReadBox();

//	void fillBuffer(unsigned int const& clock);
//	int getBuffer(unsigned int idx);
//	void setBuffer(unsigned int idx, int value);


private:
	
	double startTime_; //!< Start time of the neuron simulation [ms]
	
	double stopTime_; //!< End time of the neuron simulation [ms]		
	
	std::vector<Neuron*> synapses_; //!< List of connected neurons		
	
	double refractoryTime_; //!< Refractory timer
	
	std::vector<double> spikesTimes_; //!< Record of times when the neuron spiked
	
	double iext_; //!< External Current
				
	double potential_; //!< Membrane potential
	
	std::vector<double> potentials_; //!< Record of membrane potential at each dt, to create a file
	
	unsigned int nbSpikes_; //!< Number of spikes
	
	/**
	 * @brief Number of spike received from source neuron
	 * 
	 * Number of spikes received from other connected neuron, updated each dt.
	 * The neuron receives 1 from excitatory neurons and external neurons and -g from inhibitory neurons.
	 */
	int spikesReceived_;
	
	bool isRefractory_; //!< Neuron refractory state
	
	bool hasSpike_; //!< Neuron spike state
	
	bool isInhibiter_; //!< Neuron type: return true if is inhibiter return false if is excitatory
	
	std::vector<int> buffer_; //!< Time buffer containing each spike received at each dt
	
	/**
	 * @brief Buffer index in which your record file
	 * 
	 * Index in which your record spike taking into account the Delay. 
	 * Is incremented each dt, when index 14 is reached, writeBox is set to 0
	 */
	size_t writeBox_;
	
	/**
	 * @brief Buffer index in which your read file
	 * 
	 * Index in which your read spike taking into account the Delay. 
	 * Is incremented each dt, when index 14 is reached, readBox is set to 0
	 */
	size_t readBox_;
	
	double time_; //!< Simulation time state
	
};


#endif
