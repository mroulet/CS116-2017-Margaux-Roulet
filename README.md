# CS116-2017-Margaux-Roulet
# Neuron project
Margaux Roulet 
sciper: 274274 
BA3 SV EPFL

IMPLEMENT A NEURAL NETWORK SIMULATION FORM A PAPER

SOURCE:
BRUNEL Nicolas, Dynamics of Sparsely Connected Networks of Excitatory and Inhibitory Spiking Neurons, 1999

NOTE ON GITHUB:
You'll find on github 2 versions of network.xx neuron.xx test_oneNeuron.cpp test_twoNeurons.cpp. 
This is due to a change in my hierarchy during the project. The files within the folder src are the final one. 
But as I wanted to keep the old files to still have an history of my commits, I kept the older version.

CMAKE:
To cmake your program place yourself in the directory build and tap the next command lines:

$ cmake ..
$ make

You can run the main programm with the command line: $ ./neuron
You can run the unit tests with the command line: $ ./neuron_unittest

NEURON:
You can choose to run the programm with three differents main():
(You should adapt the CMakeLists.txt): 
	line 10: replace "src/test_xxxNeuron.cpp" with the test you want to use)

- test_oneNeuron.cpp:
	The programm asks you to enter the simulation duration and an external current.
	The program records the potential values of the neuron at each dt and display 
	the spikes arrival time on terminal.
	
	Try the next inputs: 
	
	stopTime: 400 ms // external current: 1.01
		You should observe 4 spikes.
		Generate a graph with Gnuplot (see below) to check your neuron potentials.
	
	stopTime: 400 ms // external current: 1.00
		You should observe no spikes.
		Generate a graph with Gnuplot (see below) to check your neuron potentials.
	
- test_twoNeurons.cpp:
	This programm test the connection between two neurons and 
	how spikes are sent from one neuron to the other.

	The programm asks you to enter the simulation duration and an external current.
	The programm will create then two neurons, one with the external current entered and the second with no external current.
	The program records the potential value of each neuron at each dt and display 
	the spikes arrival time on terminal.
	
	Try the next input: 
	
	stopTime: 400 ms // external current: 1.01
		You should observe 4 spikes for the first neuron with external current.
		You should observe no spikes for the second neuron with no current.
		
		Generate a graph with Gnuplot (see below) to check your potentials of both neurons.
		The second neuron should receive some current from the first neuron and should then depolarized and return to 0 mV.
	
- test_multipleNeurons.cpp: Main Simulation Programm
	The number of neurons should be at least 50 neurons for it to be relevant.

UNIT TESTS:

Tests with one neuron:
Test 1: Test the membrane potential value of one neuron
Test 2: Test the spikes Time arrival of one neuron 
Test 3: Test the membrane equation with a positive input current
Test 4: Test the membrane equation with an negative input current
Test 5: Test the simulation of one neuron during 400 ms

Test with two neurons:
Test 1: Test the transmission of spikes from an inhibitory source neuron to a neuron.
Test 2: Test the tranmission of spikes from a excitatory source neuron to a neuron.

Test on the network:
Test 1: Test the type definition and right number of the neurons: either inhibitory or excitatory 
Test 2: Test the right connections of the neurons within the network.


GENERATE GRAPH WITH GNUPLOT:
on terminal type the next command lines:

$ gnuplot
$ set style data lines
$ set xlabel "Time [ms]"
$ set ylabel "Potential [mV]"
$ set xrange [0:1200]
$ set yrange [0:400]
$ plot for [i=2:[NbNeurons+1]] "../res/potentials.txt" using 1:i
