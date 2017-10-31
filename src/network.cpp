#include "network.hpp"

using namespace std;

//======================================================================
//constructeurs/destructeurs
Network::Network(double networkStopTime, vector<Neuron*> neurons)
: networkStopTime_(networkStopTime), neurons_(neurons)
{
	nbSpikesTotal_ = 0;
	
	if(getNbNeurons() >= 50) {
		
		defineTypeNeuron();
	}
	connect();
}
//----------------------------------------------------------------------
Network::~Network()
{}
//======================================================================
//getter/setter
vector<Neuron*> Network::getNeurons() const
{
	return neurons_;
}
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
std::vector<unsigned int> Network::getSpikesTable() const
{
	return spikesTable_;
}
//----------------------------------------------------------------------
double Network::getFrequencyThr()
{
	return Threshold / (getNbExcitatoryConnections() * Amplitude * tau);
}
//----------------------------------------------------------------------
double Network::getExternalFrequency()
{	// cest/vtzhr * ce * vthr -> la multiplication des ce se fait dans poisson
	return getNbExcitatoryConnections()*Eta*getFrequencyThr();
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
	unsigned int n = getNbNeurons()/5;
	
/*	for (auto neuron : neurons_) {
		neuronsExcitatory_.push_back(neuron);
	}
	
	// les N/5 premier neurones de la liste de neurones sont inhibiteurs
	for (unsigned int i(0); i < n; ++i) {
		neurons_[i]->setIsInhibiter(true);
		neuronsExcitatory_.erase(neuronsExcitatory_.begin()+i);
		neuronsInhibitory_.push_back(neurons_[i]);
		++i;
	}
*/
	for (unsigned int i(0); i < n; ++i) {
			neurons_[i]->setIsInhibiter(true);
	}
}
//======================================================================
//méthode connect
void Network::connect()
{	
	//First method connect (for test_twoneurons and google test)
	if (getNbNeurons() < 50) {
		// Initialisation de la liste des neurons connectés
		for (auto n : neurons_) {
			for (auto neuronConnected : neurons_) {
				if (n != neuronConnected) {
					n->addSynapse(neuronConnected);
				}
			}
		}

	} else {
		unsigned int nbInhibitory = getNbNeurons()/5;
		unsigned int nbExcitatory = getNbNeurons() - nbInhibitory;
		
		for (auto neuron : neurons_) {
			for (unsigned int i(0); i < getNbInhibitoryConnections(); ++i) {
				unsigned int index(uniform(nbInhibitory));
				neuron->addSynapse(neurons_[index]);
			}
			for (unsigned int j(0); j < getNbExcitatoryConnections(); ++j) {
				unsigned int index(uniform(nbExcitatory));
				neuron->addSynapse(neurons_[index+nbInhibitory]);
			}
		}
	}
	
/*	} else {
		//on parcours les neurones receveurs qui doivent recevoir ne*0.1 connections excitatrices
		//et ni*0.1 connectios inhibitrices
		for (auto neuron : neurons_) {

			for (unsigned int i(0); i < getNbExcitatoryConnections(); ++i) {
				//on choisit uniformément un neuron source excitateur
				unsigned int index(uniform(neuronsExcitatory_.size()));
				neuron->addSynapse(neuronsExcitatory_[index]);
			}	
			for (unsigned int j(0); j < getNbInhibitoryConnections(); ++j) {
				//on choisit uniformément un neuron source inhibiteur
				unsigned int index(uniform(neuronsInhibitory_.size()));
				neuron->addSynapse(neuronsInhibitory_[index]);
			}
		}
*/											
}
//======================================================================
//Poisson distribution of randomly external spike
unsigned int Network::poisson()
{
	random_device device;
	default_random_engine generator(device());
	poisson_distribution<unsigned int> distribution(dt*Vext);
	
	return distribution(generator);
}
//----------------------------------------------------------------------
//To generate the connection we need uniformly distributed random numbers
unsigned int Network::uniform(unsigned int size)
{	//static
	random_device device;
	default_random_engine generator(device());
	uniform_int_distribution<unsigned int> distribution(0, size-1);
	
	return distribution(generator);
}
//======================================================================
//update du network
void Network::update()
{
	unsigned int k(1);
	while(dt*clock_ < networkStopTime_)	{
		
		for (auto neuron : neurons_) {
			
			neuron->update();
						
			if (getNbNeurons() >= 50) {
			
				//randomly distributed external spike from outside network
				unsigned int backgroundNoise(poisson());
				
				// si le poisson process active les synapses externe	
				//alors pour chaque synapses externe activées (random) un spike est distribué au neurone
				neuron->setSpikesReceived(neuron->getSpikesReceived() + backgroundNoise);
			}		

			//notre neurone est connecté à plusieurs neurones sources		
			for (auto neuronSource : neuron->getSynapses()) {					
				
				//si un neurone source spike, on ajoute un spike
				if (neuronSource->hasSpike()) {
					
					//négatif s'il est inhibiteur
					if (neuronSource->isInhibiter()) {
							neuron->setSpikesReceived(neuron->getSpikesReceived() - g);
					
					//positif s'il est excitateur
					} else {
						neuron->setSpikesReceived(neuron->getSpikesReceived() + 1);
					}	
				}
			}	
			
			if (neuron->hasSpike()) {
				++nbSpikesTotal_;
			}	
		}
			
		for (auto neuron : neurons_) {
			neuron->fillBuffer();
		}
	
//	cout << "update: " << k << endl;
//	cout << "nbSpike: " << nbSpikesTotal_ << endl;	
	spikesTable_.push_back(nbSpikesTotal_);
	nbSpikesTotal_ = 0;
		
	// incrémentation du pas de temps (steptime)
	clock_ += h;
	++k;	
	}
}
//======================================================================
