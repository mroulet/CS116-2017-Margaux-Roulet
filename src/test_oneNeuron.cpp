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
	assert(stopTime > 0);
	
	Neuron neuron(stopTime);
	
	neuron.update(Iext);
	
	// TEST : affichage du pas de temps où les spikes ont lieu
	
	cout << "Display of times when spikes occur: " << endl;
	for (auto spikes : neuron.getSpikesTimes()) {
		cout << spikes << " ";
	}
	
	if (neuron.geSpikesTimes().size() = 0) {
		cout << "no spikes" << endl;
	
	cout << endl;
	
	return 0;
}
