# Fall-Detection-System

This system proposes a wearable fall detection device which involves various processes working together, from data acquisition, data transmission using MQTT protocols, ML based fall prediction and alerts using the notification mechanism. The sensor readings from the MPU6050 is processed by the ESP32 and the threshold algorithm is applied on this data. In
case a fall is detected, the sensor readings are published to the MQTT broker via Wi-Fi and the ML model subscribes to this data and predicts if a fall has occurred or not. If the ML model predicts a fall, a notification is sent to the caregivers phone via the Blynk App. Figure below illustrates the process flow from the detection of a fall from the MPU6050 sensor, the transfer of data via the ESP32 over Wi-Fi to the MQTT broker, processing in Google Colab and the sending of the alert via the Blynk app.

![Screenshot 192626](./Screenshot%202025-03-22%20192626.png)

The acceleration and gyroscope readings are obtained using the getEvent() function. This function returns the acceleration values in terms of m/s2 and the angular rotation in terms of °/s. These values are converted to raw sensor values as the ML model is trained on the Sisfall dataset which contains the sensor readings as raw values.

The image below illustrates the circuit connections of the ESP32 with the MPU6050 sensor.

![Screenshot 192653](./Screenshot%202025-03-22%20192653.png)

The Machine Learning model implemented uses the decision tree algorithm to predict if a fall has occured or not. The model is trained on the Sisfall ddataset that The dataset contains orientation data collected from 38 individuals across various age brackets. It consists of data collected from 23 young adults performing 19 activities of daily living (ADL) and 15 fall activities, 15 ADL types performed by 14 elderly subjects aged over 62 and data from one elderly subject aged 60 who performed ADLs as well as falls. The ML model resulted in an accuracy of 87.91%. The confuaion matrix of the model is given below:

![Screenshot 192735](./Screenshot%202025-03-22%20192735.png)
