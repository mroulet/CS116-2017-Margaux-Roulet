#include "network.cpp"
#include "neuron.cpp"
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
	
	Neuron neuron1(n);
	Neuron neuron2(n);

	list<Neuron*> neurons = { &neuron1, &neuron2};

	Network network(stopTime, neurons);
	
	network.update(Iext);
	
	unsigned int compteur(1);
	for (auto neuron : neurons) {
		
		//TEST1 : stockage du potentiel membranaire 
		
		//création et ouverture d'un file
		string fileName;
		cout << "Name the file in which you want to record the potential of neuron: " << compteur << " "
			<< flush;
		cin >> ws;
		getline(cin, fileName);
		
		ofstream file(fileName.c_str());
		
		// stockage du potentiel membranaire pour chaque pas de temps
			if (file.fail ()) {
			cerr << "Error opening file " << fileName << endl;
		
			} else {
				for (auto potential : neuron->getPotentials()) {
					file << potential << endl;
				}
			}
		
		//fermeture du fichier
		file.close();	
		
		// TEST2 : affichage du pas de temps où les spikes ont lieu
		
		cout << "Display of times when spikes occur: " << endl;
		for (auto spikes : neuron->getSpikesTimes()) {
			cout << spikes << " ";
		}
		cout << endl;
		
		++compteur;
	}
			
	return 0;
}

