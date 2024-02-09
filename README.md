# Cerber Client

## Overview

Cerber Client is an open source software that makes OTA updates possible using RAUC (https://github.com/rauc/rauc). 
The Cerber Update solution give you the possibility to realize OTA updates of your linux embedded systems. The software is directly communicating with RAUC to trigger and install the bundle (see rauc documentation for more informations on this). The Cerber Client is communicating with the Cerber Device API to detect new updates and give telemetry during the installation of the update. 

## Installation

To compile and run this project, follow these steps:

1. Clone the repository:
    ```bash
    git clone https://github.com/benjaminbaret/cerber-client
    ```

2. Navigate to the project directory:
    ```bash
    cd cerber-client
    ```

3. Install dependencies: this command is coming soon, for the moment we'll let you take a look at the project includes :)

4. Compile the sources project (repeate the same procedure for the tests):
    ```bash
    cd dev && cmake .. -Bbuild
    ```

5. Create a cerber-config.json in the folder specified in the DEFINE path declared in simple-json-parser.h and fill it with the authentication informations generated on the cerber web application

5. Install the cerber-client ON THE TARGET EMBEDDED SYSTEM and then run it:
    ```bash
    ./cerber-client
    ```


## Contributing

This project is not ready yet for production use. Contributions are welcome ! Please contact us to discuss about future implementations. As you can see, our project is very new and we need to take a few months to structure it. 

## License

We are still not sure about the license. For the moment let's say PROPRIETARY LICENSE. This will change on next weeks !