#ifndef neuron_H
#define neuron_H
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <list>
#include <math.h>

//CONSTANTES (à voir pour la suite créer un fichier res)
const double Threshold = 20.0; //limite max du potentiel en mV
const double PotentialReset = 0.0; //en mV
const double dt = 0.1; //pas de temps en ms
//unsigned int h = 1; //steptime (sera converti en ms)
const double Resistance = 20.0; //resistance membranaire Ohm
const double Capacity = 1.0; //Capacité Farrad
const double tau = Resistance * Capacity; // tau = 20.0 ms
const double tauRp = 2.0; //temps de réfraction du potentiel en ms
const double Amplitude(0.1); // Constante J d'un spike reçu
const double Delay(1.5); // transmission delay (en ms)


class Neuron {
	
public:
	//constructeur/destructeur
	Neuron(double stopTime, double iext = 0.0, double potential = 0.0, unsigned int nbSpikes = 0); //TEST 1NEURON
	~Neuron();
	
	//getter
	double getPotential() const;
	unsigned int getNbSpikes() const;
	std::vector<double> getSpikesTimes() const;
	double getRefractoryTime() const;
	std::list<Neuron*> getSynapses() const;
	std::vector<double> getPotentials() const;	
	bool hasSpike() const;
	unsigned int getSpikesReceived() const;
	
	//setter
	void setPotential(double potential);
	void setNbSpikes(unsigned int spikes);
	void setIsRefractory(bool state);
	void setRefractoryTime(double time);
	void setSpikesReceived(unsigned int spikes);
	void setIext(double iext);
	void addSynapse(Neuron* n);
	
	//membrane differential equation
	double membraneEq();
	
	//update
	void update();
	
	//gestion du buffer
	void fillBuffer();
	void emptyBuffer();

private:
	
	//intervalle de temps en steptime
	double startTime_;
	double stopTime_;
	
	//liste des neurones connectés
	std::list<Neuron*> synapses_;
	
	double refractoryTime_;
	std::vector<double> spikesTimes_; 
	
	double iext_;
	double potential_;
	std::vector<double> potentials_; // ex1: créer un fichier contenant les potentiels
	unsigned int nbSpikes_;
	unsigned int spikesReceived_; // spike recu des autre neurones connectés à chaque pas de temps dt
	
	
	// lorsque la limite de potentiel est atteint alors:
	bool isRefractory_;
	bool hasSpike_;
	
	//tableau de spikes reçu à chaque pas de temps
	std::vector<unsigned int> buffer_;
	
	//état d'avancement de la simulation
	double time_;
	
};


#endif
