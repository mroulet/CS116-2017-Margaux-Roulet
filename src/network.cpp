#include "network.hpp"

using namespace std;

//======================================================================
//constructeurs/destructeurs
Network::Network(double networkStopTime, vector<Neuron*> neurons)
: networkStopTime_(networkStopTime), neurons_(neurons)
{
	defineTypeNeuron();
	connect();
}
//----------------------------------------------------------------------
Network::~Network()
{}
//======================================================================
//getter/setter
unsigned int Network::getNbNeurons() const
{
	return neurons_.size();
}
//----------------------------------------------------------------------
// Initialisation de la liste des neurons connectés pour chaque neurones !
// Nous devons créé Ce = Ne*0.1 connections excitatrices et  Ci = Ni*0.1 connections inhibitrices
// Ni / Ne = 0.25 selon Brunel's model
unsigned int Network::getNbExcitatoryConnections()
{
	double n = getNbNeurons()*ConnectionPercent*0.8;
	unsigned int connections = static_cast<unsigned long>(n);
	return connections;
}
//----------------------------------------------------------------------
unsigned int Network::getNbInhibitoryConnections()
{
	double n = getNbNeurons()*ConnectionPercent*0.2;
	unsigned int connections = static_cast<unsigned long>(n);
	return connections;
}
//----------------------------------------------------------------------
unsigned int Network::getNbExternalConnections()
{
	return getNbExcitatoryConnections();
}
//----------------------------------------------------------------------
double Network::getFrequencyThr()
{
	return Threshold / (getNbExcitatoryConnections() * Amplitude * tau);
}
//----------------------------------------------------------------------
double Network::getExternalFrequency()
{
	return Vext_Vthr*getFrequencyThr();
}
//----------------------------------------------------------------------
void Network::setNetworkStopTime(double t)
{
	networkStopTime_ = t;
}
//======================================================================
//determine si le neurone est excitatory or inhibitory
void Network::defineTypeNeuron()
{
	//note: la liste des neurones doit être multiple de 5 : Ni = 0.2 N , Ne = 0.8N
	unsigned int n = 10;
	
	for (unsigned int i(0); i < n; ++i) {
		neurons_[i]->setIsInhibiter(true);
		++i;
	}
}
//======================================================================
//méthode connect
void Network::connect()
{	
	unsigned int k(1);
	for (auto neuron : neurons_) {
		
		unsigned int i(0); // compteur des connections inhibitrices
		unsigned int j(0); // compteur des connections excitatrices
		
		// Je parcours tous les neurones receveurs et 
		// crée aléatoirement des connections inhibitrice et excitatrice
		while (i < getNbInhibitoryConnections() or j < getNbExcitatoryConnections()) {
			
			unsigned int index(uniform());
						
			// Je crée Ci connections inhibitrice avec des neurones inhibiteurs
			if (neurons_[index]->isInhibiter() == true and i < getNbInhibitoryConnections()) {
				neuron->addSynapse(neurons_[index]);

				++i;
		
			//Je crée Ce connections excitatrices avec des neurones exciateurs
			} else if (neurons_[index]->isInhibiter() == false and j < getNbExcitatoryConnections()) {
				
				neuron->addSynapse(neurons_[index]);
				++j;
			}
		}
	++k;
	}
}
//======================================================================
//Poisson distribution of randomly external spike
unsigned int Network::poisson()
{
	random_device device;
	default_random_engine generator(device());
	poisson_distribution<unsigned int> distribution(getNbExcitatoryConnections()*getExternalFrequency());
	
	return distribution(generator);
}
//----------------------------------------------------------------------
//To generate the connection we need uniformly distributed random numbers
unsigned int Network::uniform()
{
	random_device device;
	default_random_engine generator(device());
	uniform_int_distribution<unsigned int> distribution(0, neurons_.size()-1);
	
	return distribution(generator);
}
//======================================================================
//update du network
void Network::update()
{
	while(dt*clock_ < networkStopTime_)	{
		for (auto neuron : neurons_) {
			
			neuron->update();
			
			//randomly distributed external spike from outside network
			unsigned int backgroundNoise(poisson());
			cout << "Poisson: " << backgroundNoise << endl;
			
			// si le poisson process active les synapses externe	
			//alors pour chaque synapses externe activées (random) un spike est distribué au neurone
			for (unsigned int i(0); i < backgroundNoise; ++i) {
				neuron->setSpikesReceived(neuron->getSpikesReceived() + 1);
			}
			
			//si le neuron spike
			if (neuron->hasSpike()) {
				
				//on transmet aux neurones connectés le spike à un temps tpre
				for (auto n : neuron->getSynapses()) {
					
					if (neuron->isInhibiter()) {
						n->setSpikesReceived(n->getSpikesReceived() - g);
					}
					else {
						n->setSpikesReceived(n->getSpikesReceived() + 1);
					}	
				}
			}		
		}
		
		for (auto neuron : neurons_) {
			neuron->fillBuffer();
		}
		
	// incrémentation du pas de temps (steptime)
	clock_ += h;	
	}
}
//======================================================================
/*int k(1);
for (auto neuron : neurons_) {
		cout << "neuron" << k << endl;
		cout << "Uniform: " << index << endl;	
		cout << "nb neurons: " << getNbNeurons() << " ";
		cout << "Ce: " << getNbExcitatoryConnections() << " Ci: " << getNbInhibitoryConnections() << endl;
		cout << "synapses: " << neuron->getSynapses().size() << endl;
		if (neurons_[index]->isInhibiter() == true) { cout << "inhibiteur" << endl; }
		if (neurons_[index]->isInhibiter() == false) { cout << "excitatory" << endl; }
		cout << "nb connections inhibitrices: "<< i << endl;
		cout << "nb connections excitatrices: "<< j << endl;
		++k;
}
			
*/
