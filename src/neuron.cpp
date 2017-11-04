#include "neuron.hpp"

using namespace std;

//======================================================================
//constructeurs/destructeurs
Neuron::Neuron(double stopTime, double iext, double potential)
: stopTime_(stopTime), iext_(iext), potential_(potential)
{
	//on initialise pour le moment le start time à 0 par défaut (à voir pour la suite)
	startTime_ = 0.0;
	isRefractory_ = false;
	hasSpike_ = false;
	isInhibiter_ = false;
	refractoryTime_ = 0.0;
	spikesReceived_ = 0;
	nbSpikes_ = 0;
	
	writeBox_ = 14;
	readBox_ = 0;

	//la taille du buffer initial est 13 (soit 14 cases).
	//de ce fait, il y aura un délai de 15 cases pour qu'un spike s'y inscrive, soit 1.5 ms
	for (unsigned int i(0); i < DelayStep ; ++i) {
		buffer_.push_back(0);
	}
}	
Neuron::~Neuron()
{}
//======================================================================
//Getters
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
std::vector<Neuron*> Neuron::getSynapses() const
{
	return synapses_;
}
//----------------------------------------------------------------------
std::vector<double> Neuron::getPotentials() const
{
	return potentials_;
}
//----------------------------------------------------------------------
bool Neuron::hasSpike() const
{
	return hasSpike_;
}
//----------------------------------------------------------------------
bool Neuron::isInhibiter() const
{
	return isInhibiter_;
}
//----------------------------------------------------------------------
int Neuron::getSpikesReceived() const
{
	return spikesReceived_;
}
//======================================================================
//Setters
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
void Neuron::setSpikesReceived(int spikes)
{
	spikesReceived_ = spikes;
}
//----------------------------------------------------------------------
void Neuron::setIext(double Iext)
{
	iext_ = Iext;
}
//----------------------------------------------------------------------
void Neuron::addSynapse(Neuron* const n)
{
	synapses_.push_back(n);
}
//----------------------------------------------------------------------
void Neuron::setIsInhibiter(bool type)
{
	isInhibiter_ = type;
}
//======================================================================
//Membrane equation : temporal evolution of the membrane potential
double Neuron::membraneEq()
{
	assert(!buffer_.empty());
	return e*potential_ + iext_*Resistance*(1-e) + buffer_[readBox_]*Amplitude;	
}
//======================================================================
// Gestion du buffer
void Neuron::fillBuffer()
{
	buffer_[writeBox_] = getSpikesReceived();
	setSpikesReceived(0);
}
//----------------------------------------------------------------------
void Neuron::updateReadBox()
{			
	if (readBox_+1 > 14) {
		readBox_ = 0;
	
	} else { ++readBox_;
	}
}
//----------------------------------------------------------------------
void Neuron::updateWriteBox()
{
	if (writeBox_+1 > 14) {
		writeBox_ = 0;
	
	} else { ++writeBox_; 
	}
}
//OLD VERSION OF BUFFER : all slower than the actual one
//----------------------------------------------------------------------
/*void Neuron::fillBuffer()
{
	buffer_.push_back(getSpikesReceived());
	setSpikesReceived(0);
}
//----------------------------------------------------------------------
void Neuron::emptyBuffer()
{
	assert(!buffer_.empty());
	buffer_.erase(buffer_.begin());
}
//----------------------------------------------------------------------
int Neuron::getBuffer(unsigned int idx) 
{
	//invalid operant type -> static cast
	return buffer_[idx%(DelayStep+1)];
}
//----------------------------------------------------------------------
void Neuron::setBuffer(unsigned int idx, int value)
{
	buffer_[idx%(DelayStep+1)] = value;
}
//----------------------------------------------------------------------
void Neuron::fillBuffer(unsigned int const& clock)
{
	setBuffer(clock + DelayStep, getBuffer(clock +DelayStep) + getSpikesReceived());
}*/
//======================================================================
//update du potentiel
void Neuron::update()
{	
		hasSpike_ = false;
		
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
//				spikesTimes_.push_back(dt*time_);
				
				//le neuron spike
				hasSpike_ = true;
				++nbSpikes_;
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
			setPotential(membraneEq());
		}
		
		updateReadBox();

		//Ex1: inscription des potentiel dans un tableau (pour en faire un fichier) à modifier, créer une méthode print
//		potentials_.push_back(potential_);
}
//======================================================================
