# Client-Server Model

Welcome to the client-server model repository. This repository contains a client and server model designed and programmed to demonstrate basic client-server interactions.

## Table of Contents

- [Introduction](#introduction)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction

This repository includes a basic client-server model implemented in C for Linux systems. The client and server communicate over a network, demonstrating fundamental networking concepts and operations such as socket programming, data exchange, and process handling.

## Prerequisites

Before you begin, ensure you have met the following requirements:

- You have a Linux-based operating system installed (e.g., Ubuntu, Fedora, Debian).
- You have GCC (GNU Compiler Collection) installed on your system.
- Basic knowledge of C programming and socket programming.

## Installation

1. Clone the repository to your local machine using the following command:
   ```bash
   https://github.com/divyesh255/client-server-model.git
   ```

2. Navigate to the cloned directory:
   ```bash
   cd client-server-model
   ```

## Usage

To compile and run the client and server programs, follow these steps:

1. Open a terminal and navigate to the directory containing the makefile.
2. Compile the program using Makefile:
   ```bash
   make
   ```
3. Navigate to the bin dorectory.

4. Run the server program:
   ```bash
   ./server <port number>
   ```

5. In a separate terminal, run the client program:
   ```bash
   ./client <IP address> <PORT> <input file>
   ```
6. Server will receive the file. Convert it to upper case and send it back to the client.


## Contributing

Contributions are always welcome! If you have suggestions or improvements, feel free to fork the repository and create a pull request. Please follow these steps:

1. Fork the repository.
2. Create a new branch:
   ```bash
   git checkout -b feature_branch
   ```
3. Make your changes and commit them:
   ```bash
   git commit -m "Description of changes"
   ```
4. Push to the branch:
   ```bash
   git push origin feature_branch
   ```
5. Create a pull request on GitHub.

## License

This repository is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

---

Happy coding! If you have any questions or need further assistance, feel free to open an issue.
Contact: divyeshkapadiy25@gmail.com

---

