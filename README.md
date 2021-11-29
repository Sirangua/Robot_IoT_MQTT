# Robot IoT usando MQTT
Un robot que creado en una ESP32 y que se controla a distancia mediante una web utilizando el protocolo MQTT

## Componentes del robot
- ESP32
- Sensor de distancia HC_SR04
- Servomotor
- Controlador de motores L298N
- 2 Motorreductores
- 2 Llantas
- Resistencias: 1kΩ y 2kΩ
- Alimentación independiente para la ESP32 y motores
- Cables para conectar los componentes
- Una base para montarlo

## Conexiones 

![Robot_IoT_bb](https://user-images.githubusercontent.com/72269114/143927146-ac148fac-307d-43cb-b1c9-299b1092bbc3.png)

## Broker MQTT
Para el Broker MQTT se usó EMQX un broker open source. Este broker se montó en un máquina virtual de Azure, sin embargo EMQX puede ser montado en un entorno local o en Docker. Es el ejemplo de Azure se siguieron los siguientes pasos
- Montar la máquyina virtual con Linux en Azure
- Abrir los puertos necesarios del Firewall de Azure
- Nos conectamos a la maquina usando SSH en PowerShell
- Abrimos los puertos necesarios del Firewall de la Máquina Virtual
- Descargamos e iniciamos EMQX
- Usar el Dashboard de EMQX para administrar los dispositivos

A continuación documentación que puede resultal útil para los pasos anteriores
### Documentación de EMQX
https://github.com/emqx/emqx

### Montar máquinas virtuales en Azure
https://docs.microsoft.com/en-us/azure/virtual-machines/windows/quick-create-portal

### Abrir los puertos de Azure
https://docs.microsoft.com/en-us/azure/virtual-machines/windows/nsg-quickstart-powershell

### Conectarse mediante SSH
https://docs.microsoft.com/en-us/azure/virtual-machines/linux/ssh-from-windows
