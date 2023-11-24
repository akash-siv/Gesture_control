Gesture based Home Assistant Trigger

What is the Project ?
Turning ON/OFF the Home Applications with Gestures with Embedded AI

About the Project.
After a while of using Home Assistant, it seems more tiring to trigger with a Mobile companion app of home assistant, so i'm planning to use gestures patterns to trigger the ON/OFF process. Thus ultimately reducing the energy consumption of the Home or Industry.

Esp32c3 is equipped with an accelerometer and a gyroscope module for real-time motion tracking. A Neural Network model (TFLiteMicro Model) is embedded in the esp32, which analyzes the data stream and sends the trigger pulse to the home assistant, hosted in the server, which toggles the appliance.

How.
Used edge impulse to create the gesture recognition model.
Iam classifying 5 gesture classes, which are used as 5 binary sensors in Home assistant.
Why Edge Impulse.
* because it is easy to 
	* collect
	* visualize
	* preprocess
	* Train
	* Deploy
* Used two deployment options and got success, both had similar performance
	* C++ library deployment with espidf in both esp32 and espc3.
	* Arduino Library in both esp32 and espc3.
Here i am showcasing Arduino esp32c3 based inference code, you can also tinker with  espidf based code links are given below.

* What does it do?
	* it collects data from an accelerometer and predicts the gesture on the edge.
	* send suitable triggers based on the prediction as mqtt signals to home assistant.
* further improvement
	* should try espDL for model creation and deployment.
	* Since it is an optimized NN, expecting lower latency in inferencing.



Components required : 
* XIAO esp32 C3 x1.
* mpu6050 module x1.
* Lipo battery 1500mah x1.

Additional Components Used : 
* Raspberry pi 3B for running Home assistant.
* Router for providing Access point.
* 3D printed enclosure for esp

Circuit Diagram :
The connection itself is pretty simple. 
esp32 c3 SDA -> MPU6050 SDA
esp32 c3 SCL -> MPU6050 SCL
esp32 c3 3.3v -> MPU6050 Vcc
esp32 c3 Gnd -> MPU6050 Gnd

Lipo battery is connected to esp32 c3 battery pads.

Code link :
https://github.com/akash-siv/Gesture_control

Youtube Link : https://youtu.be/VZjahT0N3_U

Addition References & Tutorials : 
Edge Impulse Website : https://edgeimpulse.com/
Edge Impulse Installation : https://docs.edgeimpulse.com/docs/tools/edge-impulse-cli/cli-installation
Data Forwarder (used to send data from arduino to edge impulse) : https://docs.edgeimpulse.com/docs/tools/edge-impulse-cli/cli-data-forwarder.

