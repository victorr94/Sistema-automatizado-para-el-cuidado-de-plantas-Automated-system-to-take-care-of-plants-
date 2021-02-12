# Sistema-automatizado-para-el-cuidado-de-plantas-Automated-system-to-take-care-of-plants-
Diseño, construcción de un sistema domótico que se encarga del cuidado de plantas ornamentales / Design and construction of an automated plant care system 

## Funcionamiento del Sistema

  Mediante este apartado se pretende que el usuario pueda comprender el funcionamiento del sistema cuando la intención es interactuar, a través de la plataforma web, 
con el prototipo.

  El sistema funciona de manera autónoma. Sin embargo, en caso de que sea necesario, la aplicación web permite manejar el prototipo de manera manual. Mediante el uso de la plataforma el usuario podrá activar cualquier proceso o dispositivo. La aplicación web ha sido desarrollada mediante HTML.

  En primer lugar, hay que destacar que el sistema consta de una placa de Arduino UNO, unida a una placa CNC SHIELD y drivers A4988 que controlarán los movimientos de los motores (NEMA 17), todo esto mediante el software grbl (versión 1.1h). La placa de Arduino UNO recibe los comandos G a través de una placa ESP32 (mediante Comunicación UART o serie). Esta placa ESP32 es el núcleo del proyecto, recibe toda la información (procedente de sensores, comandos pulsados en la web...) y actúa en consecuencia, en función de las directrices que se le hayan indicado al programar o de las acciones que se manden ejecutar a través de la web. 
  
  El sistema actualmente ofrece realizar procesos de riego, eliminación de malas hierbas y medición de humedad y temperatura ambiental y humedad del terreno. 
  
  En cuanto a los actuadores, el sistema posee tres motores con los que la máquina CNC se puede mover en cualquier dirección, un panel LED (desmontable), que ofrece a las plantas el espectro necesario de luz para realizar la fotosíntesis, un pequeño ventilador y una bomba peristáltica con la que se proporciona agua a los vegetales.
  
  

## System Operation

Through this section it is pretended so that the final user can get a general knowledge of the system operation, when the intention is to interactuate, throughout the web platform, with the prototipe.

The systems works automatically. However if it is necesary the web application lets us to work in a manually mode. Using the web platform the user can activate any proccess, gadget or device. All the web platform has been developed using HTML.

First of all, it is necesary to explain that the system includes an Arduino UNO board, a CNC SHIELD board, and drivers A4988. This boards are used to control the movements of the motors (NEMA 17), programmed with the software grbl (1.1h version). The Arduino UNO board receives the G commands trough a ESP32 board (by serial or UART communication). This ESP32 board is the center of the project, it receives all of the information (coming from sensors, pressed commands of the web...) and it acts accordingly, based on the guidelines that have been indicated when programming or on the actions that are ordered to be carried out through the web.

The system currently offers to carry out processes of watering, removing weeds or measurement of ambient humidity and temperature and soil humidity.

Taking into account the actuators, the system has three motors that move the CNC machine in any direction, a LED panel (removable), which offers the plants the necessary spectrum of light to carry out photosynthesis, a small fan and a peristaltic pump that is used to provide water to the vegetables during the watering proccess. 



### Uso de la aplicación web

Una vez cargados los diferentes softwares en las placas de Arduino UNO y en la placa ESP32, el sistema trabajará de manera autónoma. Sin embargo, también se puede acceder a  la aplicación web y de esta manera ordenar al sistema que realice diferentes acciones. A continuación, se muestran el entorno de la aplicación web y las posibilidades que ofrece.

Para conectarse a la plataforma, será necesario el uso de cualquier dispositivo con conexión a internet. Al iniciar el programa el dispositivo ESP32 ofrecerá la dirección IP a la que está asociada la plataforma. Es necesario copiar esta dirección e introducirla en el buscador de cualquier navegador disponible en el dispositivo.

![](https://github.com/victorr94/Sistema-automatizado-para-el-cuidado-de-plantas-Automated-system-to-take-care-of-plants-/blob/main/fotos/Información%20ESP32.JPG)

Al ingresar en la aplicación web el sistema nos lleva a la página inicial (frontpage.html), donde se ofrece información visual sobre el sistema y se invita a entrar en la plataforma.

![](https://github.com/victorr94/Sistema-automatizado-para-el-cuidado-de-plantas-Automated-system-to-take-care-of-plants-/blob/main/fotos/frontpage.html.JPG)

Una vez pulsado el botón de ingreso (GO IN), el sistema lleva al usuario a la página index.html donde se despliegan las diferentes opciones que se pueden realizar.

![](https://github.com/victorr94/Sistema-automatizado-para-el-cuidado-de-plantas-Automated-system-to-take-care-of-plants-/blob/main/fotos/index.html.JPG)

El sistema ofrece entrar en modo manual a través del apartado options del menu desplegable, desde donde se puede mover la máquina en cualquier dirección o encender/apagar el ventilador y/o el panel LED.

![](https://github.com/victorr94/Sistema-automatizado-para-el-cuidado-de-plantas-Automated-system-to-take-care-of-plants-/blob/main/fotos/manual.html.JPG)




### Use of the web application 

Once the different softwares have been loaded to the Arduino UNO board and to the ESP32 board the system will work autonomously. However, the final user also has access to a web platform and in this way can order the system to perform different processes. In the next part of this document the environment of the web aplication will be shown and the posibilities that it offers. 

To enter to the platform it is necessary to use any devece with an internet connection. When the ESP32 board stars running it shows the IP adress associated to the platform. This adress should be copied to the browser of the device with internet connection.

![](https://github.com/victorr94/Sistema-automatizado-para-el-cuidado-de-plantas-Automated-system-to-take-care-of-plants-/blob/main/fotos/Información%20ESP32.JPG)

The first page that will be shown is frontpage.html. It is offered general information of the system and guests are invited to enter the platform.

![](https://github.com/victorr94/Sistema-automatizado-para-el-cuidado-de-plantas-Automated-system-to-take-care-of-plants-/blob/main/fotos/frontpage.html.JPG)

Once the enter button (GO IN) has been pressed, the system shows the user the index.html page where different options that can be performed are displayed.

![](https://github.com/victorr94/Sistema-automatizado-para-el-cuidado-de-plantas-Automated-system-to-take-care-of-plants-/blob/main/fotos/index.html.JPG)

The system offers to enter manual mode through the options section of the drop-down menu. In this new page (manual.html) the final user can move the machine in any direction or turn on / off the fan and / or the LED panel.

![](https://github.com/victorr94/Sistema-automatizado-para-el-cuidado-de-plantas-Automated-system-to-take-care-of-plants-/blob/main/fotos/manual.html.JPG)



