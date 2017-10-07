#include "neuron.hpp"

using namespace std;

//======================================================================
//constructeurs/destructeurs
Neuron::Neuron(double stopTime, double potential, unsigned int nbSpikes)
: stopTime_(stopTime), potential_(potential), nbSpikes_(nbSpikes)
{
	//on initialise pour le moment le start time à 0 par défaut (à voir pour la suite)
	startTime_ = 0.0;
	isRefractory_ = false;
	refractoryTime_ = 0.0;
}	
//----------------------------------------------------------------------
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
//======================================================================
//membrane equation: calcul de l'évolution temporel du potentiel de la membrane
double Neuron::membraneEq(double Iext)
{
	const double e = exp(-dt/tau);
	
	return e*potential_ + Iext*(Resistance/tau)*(1-e);
}
//======================================================================
//update du potentiel
void Neuron::update(double Iext)
{	
	//création et ouverture d'un file
	string fileName;
	cout << "Name the file in which you want to record your potential : "
		<< flush;
	cin >> ws;
	getline(cin, fileName);
	
	ofstream file(fileName.c_str());
	
	while (clock_ < stopTime_) {
		
		//si le potentiel est plus grand que la limite du potentiel au pas de temps précédent
		//et si le temps de réfraction est dépassé (2ms)
		//on réinitialise le potentiel et le compteur de réfraction
		if (isRefractory_ and getRefractoryTime() >= tauRp) {
			potential_= PotentialReset;
			setRefractoryTime(0.0);
			setIsRefractory(false);
		
		//si le potentiel est plus petit que la limite max du potentiel	
		} else if (potential_ < Threshold) {
						
			//mise à jour du potentiel après le calcul de son évolution temporel
			setPotential(membraneEq(Iext));
		
		//si le potentiel dépasse la limite max du potentiel	
		} else if (potential_ >= Threshold) {
			
			//on incrémente le compteur de spike
			++nbSpikes_;
			
			//on enregistre un spike uniquement lorsque le temps de réfraction n'a pas encore débuté
			// (à changer selon l'équation du réfractory spike)
			if (getRefractoryTime() == 0.0) {
				
				//on stock le temps où le spike a lieu lorsque le potentiel atteint la limite
				spikesTimes_.push_back(clock_);
			}
				
			//mise à jour de l'état du neurone : état réfractionnaire
			setIsRefractory(true);
			
			//incrémentation du compteur de temps de réfraction
			setRefractoryTime(getRefractoryTime() + dt);

		} 
		
		// storage of the membrane potential for each dt in a file
		if (file.fail ()) {
		cerr << "Error opening file " << fileName << endl;
	
		} else {
			file << potential_ << endl;
		}
		
		// incrémentation du pas de temps
		clock_ += dt;
	}
	
	//fermeture du fichier
	file.close();	
}
//======================================================================
