#include "network.hpp"
#include "neuron.hpp"
#include <iostream>

using namespace std;

int main()
{
//initialisation du neuron
	double stopTime;
	unsigned int NbNeurons;
	
	//vérif erreurs entrée !!! catch throw
	
	cout << "Entrez le nombre de neurones: "; 
	cin >> NbNeurons;
	
	cout << "Entrez la durée de la simulation: ";
	cin >> stopTime;
	
	vector<Neuron*> neurons;
	
	for (unsigned int i(0); i < NbNeurons; ++i) {
		Neuron* n = new Neuron(1);
		neurons.push_back(n);
	}
	
	Network network(stopTime, neurons);

	network.update();

	unsigned int compteur(1);
	unsigned int k(1);

		string fileName("../res/potentials.txt");
		ofstream file(fileName.c_str());
		
		// stockage du potentiel membranaire pour chaque pas de temps
			if (file.fail ()) {
			cerr << "Error opening file " << fileName << endl;
		
			} else {
				
				while(k < stopTime/dt) {
				
					file << k*dt << " ";
					
					for (auto neuron : neurons) {
						file << neuron->getPotentials()[k-1] << " ";
					}
					
					file << endl;
					++k;
				}
			}
		
		//fermeture du fichier
		file.close();	

/*	for (auto neuron : neurons) {
		//création et ouverture d'un file pour chaque neurone
		string fileName("../res/neuron" + to_string(compteur) + ".txt");
		ofstream file(fileName.c_str());
		
		// stockage du potentiel membranaire pour chaque pas de temps
			if (file.fail ()) {
			cerr << "Error opening file " << fileName << endl;
		
			} else {
				for (auto potential : neuron->getPotentials()) {
					file << k*dt << " " << potential << endl;
					++k;
				}
			}
		
		//fermeture du fichier
		file.close();	
	
		++compteur;
		k = 1;

	}
*/

/*	for (auto neuron : neurons) {		
		cout << "Display of times when spikes occur: " << endl;
		for (auto spikes : neuron->getSpikesTimes()) {
			cout << spikes << " ";
		}
		cout << endl;
	}
*/			
			
	return 0;
}


