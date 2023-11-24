// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define FREQUENCY_HZ        100
#define INTERVAL_MS         (1000 / (FREQUENCY_HZ + 1))

Adafruit_MPU6050 mpu;
static unsigned long last_interval_ms = 0;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
 
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  // mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);
  // mpu.setHighPassFilter(MPU6050_HIGHPASS_UNUSED);
  // mpu.setCycleRate(MPU6050_CYCLE_40_HZ);
  // mpu.setClock(MPU6050_INTR_8MHz);

  delay(100);
}

void loop() {
  /* Get new sensor events with the readings */

  if (millis() > last_interval_ms + INTERVAL_MS) {
    last_interval_ms = millis();
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  // Serial.print("Acceleration X: ");
    Serial.print(a.acceleration.x);Serial.print(",");
  // Serial.print(", Y: ");
    Serial.print(a.acceleration.y);Serial.print(",");
  // Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
  // Serial.println(" m/s^2");

    Serial.println("");
    // delay(9);
  }
}