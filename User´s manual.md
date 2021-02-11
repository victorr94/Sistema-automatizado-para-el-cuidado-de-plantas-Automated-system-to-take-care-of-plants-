# Sistema-automatizado-para-el-cuidado-de-plantas-Automated-system-to-take-care-of-plants-
Diseño, construcción de un sistema domótico que se encarga del cuidado de plantas ornamentales / Design and construction of an automated plant care system 

## Funcionamiento del Sistema

  Mediante este apartado se pretende que el usuario pueda comprender el funcionamiento del sistema cuando la intención es interactuar, a través de la plataforma web, 
con el prototipo.

  El sistema funciona de manera autónoma. Sin embargo, en caso de que sea necesario manejar el prototipo de manera manual, la aplicación web ofrece esta solución. Mediante el uso de la plataforma el usuario podrá activar cualquier proceso o dispositivo.

  La aplicación web ha sido desarrollada mediante HTML.

  En primer lugar, hay que destacar que el sistema consta de una placa de Arduino UNO, unida a una placa CNC SHIELD y drivers A4988 que controlarán los movimientos de los motores, todo esto mediante el software grbl (versión 1.1h). La placa de Arduino UNO recibe los comandos G a través de una placa ESP32 (mediante Comunicación serie). Esta placa ESP32 es el núcleo del proyecto, recibe toda la información (procedente de sensores, comandos pulsados en la web...) y actúa en consecuencia, en función de las directrices que se le hayan indicado al programar o de las acciones que se manden ejecutar a través de la web. 
  
  El sistema actualmente ofrece realizar procesos de riego, eliminación de malas hierbas y medición de humedad y temperatura ambiental y humedad del terreno. 
  
  En cuanto a los actuadores, el sistema posee tres motores con los que la máquina CNC se puede mover en cualquier dirección, un panel LED, que ofrece a las plantas el espectro necesario de luz para realizar la fotosíntesis y un pequeño ventilador.


########fff
