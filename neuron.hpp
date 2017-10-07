#ifndef neurones_H
#define neurones_H
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

//CONSTANTES (à voir pour la suite créer un fichier res)
const double Threshold = 20.0; //limite max du potentiel
const double PotentialReset = 10.0;
const double dt = 1.0; //pas de temps de la simulation
const double Resistance = 20.0; 
const double Capacity = 1.0;
const double tau = Resistance * Capacity; // tau = 20.0 ms
const double tauRp = 2.0; //temps de réfraction du potentiel


class Neuron {
	
public:
	//constructeur/destructeur
	Neuron(double stopTime, double potential = 0.0, unsigned int nbSpikes = 0);
	~Neuron();
	
	//getter
	double getPotential() const;
	unsigned int getNbSpikes() const;
	std::vector<double> getSpikesTimes() const;
	double getRefractoryTime() const;
	
	//setter
	void setPotential(double potential);
	void setNbSpikes(unsigned int spikes);
	void setIsRefractory(bool state);
	void setRefractoryTime(double time);
	
	//membrane differential equation
	double membraneEq(double Iext);
	
	//update
	void update(double Iext);

private:
	
	//intervalle de temps
	double startTime_;
	double stopTime_;
	
	double refractoryTime_;
	
	double potential_;
	unsigned int nbSpikes_;
	std::vector<double> spikesTimes_; 
	
	// V > Threshold
	bool isRefractory_;
	
	//état d'avancement de la simulation
	double clock_;
	
};


#endif
