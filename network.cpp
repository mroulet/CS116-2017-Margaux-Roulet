#include "network.hpp"

using namespace std;

//======================================================================
//constructeurs/destructeurs
Network::Network(double networkStopTime, std::list<Neuron*> neurons)
: networkStopTime_(networkStopTime), neurons_(neurons)
{
	connect();
}
Network::~Network()
{}
//======================================================================
//méthode connect
void Network::connect()
{
	// Initialisation de la liste des neurons connectés
	for (auto n : neurons_) {
		for (auto neuronConnected : neurons_) {
			if (n != neuronConnected) {
				n->addSynapse(neuronConnected);
			}
		}
	}
}
//======================================================================
//update du network
void Network::update()
{
	while(dt*clock_ < networkStopTime_)	{
		
		for (auto neuron : neurons_) {
			neuron->update();
			//si le neuron spike
			if (neuron->hasSpike()) {
				//on transmet aux neurones connectés le spike à un temps tpre
				for (auto n : neuron->getSynapses()) {
					cout << "nb neurons connectés: " << neuron->getSynapses().size() << " ";
					n->setSpikesReceived(n->getSpikesReceived() + 1);
					cout << n->getSpikesReceived() << " " << endl;
				}
			}		
		}
		
		for (auto neuron : neurons_) {
			neuron->fillBuffer();
		}
		
	// incrémentation du pas de temps (steptime)
	clock_ += n;	
	}
}
//======================================================================

