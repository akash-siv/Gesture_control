#include <WiFi.h>
#include <ArduinoHA.h>
#include <Tutorial_Continuous_motion_recognition_R2_inferencing.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define FREQUENCY_HZ        EI_CLASSIFIER_FREQUENCY
#define INTERVAL_MS         (1000 / (FREQUENCY_HZ + 1))

#define INPUT_PIN       9
#define BROKER_ADDR     IPAddress(192,168,1,28)
#define WIFI_SSID       "Airtel_aabi_2066"
#define WIFI_PASSWORD   "air38981"
#define HA_USER         "akash"
#define HA_PASS         "akash"

// byte mac[] = {0x00, 0x10, 0xFA, 0x6E, 0x38, 0x4A};
// unsigned long lastReadAt = millis();
// bool lastInputState = false;

WiFiClient client;
HADevice device;
HAMqtt mqtt(client, device);

// "myInput" is unique ID of the sensor. You should define you own ID.
HABinarySensor class1("circle");
HABinarySensor class2("four");
HABinarySensor class3("go");
HABinarySensor class4("rotate");
HABinarySensor class5("waving");

Adafruit_MPU6050 mpu;
static unsigned long last_interval_ms = 0;
float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
size_t feature_ix = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");
    if (!mpu.begin()) {
      Serial.println("Failed to find MPU6050 chip");
      while (1) {
        delay(100);
      }
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    // mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);
    // mpu.setHighPassFilter(MPU6050_HIGHPASS_UNUSED);
    // mpu.setCycleRate(MPU6050_CYCLE_40_HZ);
    // mpu.setClock(MPU6050_INTR_8MHz);
    delay(100);

    pinMode(INPUT_PIN, INPUT_PULLUP);
    // lastInputState = digitalRead(INPUT_PIN);

    // Unique ID must be set!
    byte mac[] = { 0xDE, 0xBE, 0xEB, 0xFE, 0xEF, 0xF0 };
    WiFi.macAddress(mac);
    device.setUniqueId(mac, sizeof(mac));

    // connect to wifi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500); // waiting for the connection
    }
    Serial.println();
    Serial.println("Connected to the network");

    // optional device's details
    device.setName("GestureAutomation");
    device.setSoftwareVersion("1.0.0");

    // optional properties
    class1.setCurrentState(LOW);
    class1.setName("circle");
    class1.setDeviceClass("door");
    class1.setIcon("mdi:fire");

    class2.setCurrentState(LOW);
    class2.setName("four");
    class2.setDeviceClass("door");
    class2.setIcon("mdi:fire");

    class3.setCurrentState(LOW);
    class3.setName("go");
    class3.setDeviceClass("door");
    class3.setIcon("mdi:fire");

    class4.setCurrentState(LOW);
    class4.setName("rotate");
    class4.setDeviceClass("door");
    class4.setIcon("mdi:fire");

    class5.setCurrentState(LOW);
    class5.setName("waving");
    class5.setDeviceClass("door");
    class5.setIcon("mdi:fire");

    mqtt.begin(BROKER_ADDR,HA_USER,HA_PASS);
}

void loop() {
    
    mqtt.loop();

    // reading the digital input of the Arduino device
    // if ((millis() - lastReadAt) > 30) { // read in 30ms interval
    //     // library produces MQTT message if a new state is different than the previous one
    //     sensor.setState(digitalRead(INPUT_PIN));
    //     lastInputState = sensor.getCurrentState();
    //     lastReadAt = millis();
    // }

    if (millis() > last_interval_ms + INTERVAL_MS) {
    // Serial.println(FREQUENCY_HZ);
    last_interval_ms = millis();
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    features[feature_ix++] = a.acceleration.x;
    features[feature_ix++] = a.acceleration.y;
    features[feature_ix++] = a.acceleration.z;

    features[feature_ix++] = g.acceleration.x;
    features[feature_ix++] = g.acceleration.y;
    features[feature_ix++] = g.acceleration.z;

    // features buffer full? then classify!
        if (feature_ix == EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
            ei_impulse_result_t result;

            // create signal from features frame
            signal_t signal;
            numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);

            // run classifier
            EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);
            ei_printf("run_classifier returned: %d\n", res);
            if (res != 0) return;

            // print predictions
            ei_printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \n",
                result.timing.dsp, result.timing.classification, result.timing.anomaly);

            // print the predictions
            for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
                ei_printf("%s:\t%.5f\n", result.classification[ix].label, result.classification[ix].value);
            }
        #if EI_CLASSIFIER_HAS_ANOMALY == 1
            ei_printf("anomaly:\t%.3f\n", result.anomaly);
        #endif
            if (result.anomaly < 1){
              if (result.classification[0].value > 0.9) {
                // batteryLevelChar.writeValue(3);
                class1.setState(!class1.getCurrentState());
                }
              else if (result.classification[1].value > 0.9) {
                // batteryLevelChar.writeValue(1);
                class2.setState(!class2.getCurrentState());
                }
              else if (result.classification[2].value > 0.9) {
                // batteryLevelChar.writeValue(2);
                class3.setState(!class3.getCurrentState());
                }
              else if (result.classification[3].value > 0.9) {
                // batteryLevelChar.writeValue(0);
                class4.setState(!class4.getCurrentState());
                }
              else if (result.classification[4].value > 0.9) {
                // batteryLevelChar.writeValue(0);
                class5.setState(!class5.getCurrentState());
                }
            }

            // reset features frame
            feature_ix = 0;
        }
    }
}

void ei_printf(const char *format, ...) {
    static char print_buf[1024] = { 0 };

    va_list args;
    va_start(args, format);
    int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
    va_end(args);

    if (r > 0) {
        Serial.write(print_buf);
    }
}