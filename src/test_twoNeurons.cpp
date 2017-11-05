#include "network.hpp"
#include "neuron.hpp"
#include <iostream>

using namespace std;

int main()
{
//initialisation du neuron
	double Iext;
	double stopTime;
	
	//vérif erreurs entrée
	cout << "Entrez le courant I(t):" << endl;
	cin >> Iext;
	
	cout << "Entrez la durée de la simulation:" << endl;
	cin >> stopTime;
	
	Neuron neuron1(h, Iext);
	Neuron neuron2(h);

	vector<Neuron*> neurons = { &neuron1, &neuron2};

	Network network(stopTime, neurons);
	
	network.update();
	
	unsigned int compteur(1);
	unsigned int k(1);
	for (auto neuron : neurons) {	
		
		// TEST : affichage du pas de temps où les spikes ont lieu
		
		cout << "Display of times when spikes occur: " << endl;
		for (auto spikes : neuron->getSpikesTimes()) {
			cout << spikes << " ";
		}
		cout << endl;
		
		
		++compteur;
		k = 0;
	}
			
	return 0;
}

