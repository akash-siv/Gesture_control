Used edge impulse to create the gesture recognition model.

* because it is easy to 
	* collect
	* visualize
	* preprocess
	* Train
	* Deploy
* Used two deployment options and got success, both had similar performance
	* C++ library deployment with espidf in both esp32 and espc3.
	* Arduino Library in both esp32 and espc3.

* What does it do?
	* it collects data from accelerometer and predicts the gesture on the edge.
	* send suitable triggers based on the prediction as mqtt signals to home assistant.
* further improvement
	* should try espDL for model creation and deployment.
	* Since it is a optimised NN, expecting lower latency in inferencing.