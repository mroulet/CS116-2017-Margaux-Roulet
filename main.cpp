#include "neuron.hpp"
#include "neuron.cpp"
#include <iostream>

using namespace std;

int main()
{
//initialisation du neuron
	double Iext;
	double stopTime;
	
	cout << "Entrez le courant I(t):" << endl;
	cin >> Iext;
	
	cout << "Entrez la durée de la simulation:" << endl;
	cin >> stopTime;
	
	Neuron neuron(stopTime);
	
	neuron.update(Iext);
	
	cout << "Number of spikes that occured: " << neuron.getNbSpikes() << endl;
	
	cout << "Display of times when spikes occur: " << endl;
	for (auto spikes : neuron.getSpikesTimes()) {
		cout << spikes << " ";
	}
	cout << endl;
	
	return 0;
}
