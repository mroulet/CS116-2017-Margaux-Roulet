#include "neuron2.hpp"

using namespace std;

//======================================================================
//constructeurs/destructeurs
Neuron::Neuron(double stopTime, double potential, unsigned int nbSpikes)
: stopTime_(stopTime), potential_(potential), nbSpikes_(nbSpikes)
{
	//on initialise pour le moment le start time à 0 par défaut (à voir pour la suite)
	startTime_ = 0.0;
	isRefractory_ = false;
	hasSpike_ = false;
	refractoryTime_ = 0.0;
	spikesReceived_ = 0;
	
	//initialisation du buffer on utilise Delay mais sous forme de pas de temps
	//notre buffer doit avoir un taille initiale avec pour valeur 0 = nb spike receive
	for (unsigned int i(0); i <= Delay; ++i) {
		buffer_.push_back(0);
	}
}	
Neuron::~Neuron()
{}
//======================================================================
//getters
double Neuron::getPotential() const
{
	return potential_;
}
//----------------------------------------------------------------------
unsigned int Neuron::getNbSpikes() const
{
	return nbSpikes_;
}
//----------------------------------------------------------------------
vector<double> Neuron::getSpikesTimes() const
{
	return spikesTimes_;
}
//----------------------------------------------------------------------
double Neuron::getRefractoryTime() const
{
	return refractoryTime_;
}
//----------------------------------------------------------------------
std::list<Neuron*> Neuron::getSynapses() const
{
	return synapses_;
}
//----------------------------------------------------------------------
std::vector<double> Neuron::getPotentials() const
{
	return potentials_;
}
bool Neuron::hasSpike() const
{
	return hasSpike_;
}
unsigned int Neuron::getSpikesReceived() const
{
	return spikesReceived_;
}
//======================================================================
//setters
void Neuron::setPotential(double potential)
{
	potential_ = potential;
}
//----------------------------------------------------------------------
void Neuron::setNbSpikes(unsigned int spikes)
{
	nbSpikes_ = spikes;
}
//----------------------------------------------------------------------
void Neuron::setIsRefractory(bool state)
{
	isRefractory_ = state;
}
//----------------------------------------------------------------------
void Neuron::setRefractoryTime(double time)
{
	refractoryTime_ = time;
}
//----------------------------------------------------------------------
void Neuron::setSpikesReceived(unsigned int spikes)
{
	spikesReceived_ = spikes;
}
//----------------------------------------------------------------------
void Neuron::addSynapse(Neuron* n)
{
	synapses_.push_back(n);
}
//======================================================================
//membrane equation: calcul de l'évolution temporel du potentiel de la membrane
double Neuron::membraneEq(double Iext)
{
	const double e = exp(-dt/tau);
	if (buffer_.empty()) {
		return e*potential_ + Iext*Resistance*(1-e);
	} else {
		cout << "here";
		cout << buffer_[0];
		return e*potential_ + Iext*Resistance*(1-e) + buffer_[0]*Amplitude;
	}	
}
//======================================================================
// Gestion du buffer
void Neuron::fillBuffer()
{
	buffer_.push_back(getSpikesReceived());
	setSpikesReceived(0);
}
//----------------------------------------------------------------------
void Neuron::emptyBuffer()
{
	if (!buffer_.empty()) {
		buffer_.erase(buffer_.begin());
	}
}
//======================================================================
//update du potentiel
void Neuron::update(double Iext)
{	

//	while (dt*time_ < stopTime_) {

		//si le potentiel est plus grand que la limite du potentiel au pas de temps précédent
		//et si le temps de réfraction est dépassé (2ms)
		//on réinitialise le potentiel et le compteur de réfraction
		if (isRefractory_ and getRefractoryTime() >= tauRp) {
			potential_= PotentialReset;
			setRefractoryTime(0.0);
			setIsRefractory(false);
		
		//si le potentiel dépasse la limite max du potentiel	
		} else if (potential_ >= Threshold or isRefractory_) {
			
			//on enregistre un spike uniquement lorsque le temps de réfraction n'a pas encore débuté
			if (getRefractoryTime() == 0.0) {
				
				//on stock le temps où le spike a lieu lorsque le potentiel atteint la limite
				spikesTimes_.push_back(dt*time_);
				
				//le neuron spike
				hasSpike_ = true;
			}
				
			//mise à jour de l'état du neurone : état réfractionnaire
			setIsRefractory(true);
			
			//incrémentation du compteur de temps de réfraction
			setRefractoryTime(getRefractoryTime() + dt);
			
			//le potentiel vaut 0 pendant les 2ms de réfraction
			setPotential(0.0);
		
		//si le potentiel est plus petit que la limite max du potentiel	
		} else if (potential_ < Threshold) {
			//mise à jour du potentiel après le calcul de son évolution temporel
			setPotential(membraneEq(Iext));
		}
		
		//Ex1: inscription des potentiel dans un tableau (pour en faire un fichier)
		potentials_.push_back(potential_);
	
		
		// incrémentation du pas de temps (steptime)
		emptyBuffer();
		time_ += n;
//	}

}
//======================================================================
