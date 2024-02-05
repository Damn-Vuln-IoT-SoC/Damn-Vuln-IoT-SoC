<div align="center">

# Damn Vuln IoT SoC

[![Ubtuntu](https://img.shields.io/badge/platform-Ubuntu%2020.04-0078d7.svg?style=for-the-badge&logo=appveyor)](https://www.ubuntu-fr.org) 
[![Python](https://img.shields.io/badge/language-Python3-%23f34b7d.svg?style=for-the-badge&logo=appveyor)](https://www.python.org) 
[![LiteX](https://img.shields.io/badge/Library-LiteX-red.svg?style=for-the-badge&logo=appveyor)](https://github.com/enjoy-digital/litex) 
[![Migen](https://img.shields.io/badge/Library-Migen-green.svg?style=for-the-badge&logo=appveyor)](https://m-labs.hk/gateware/migen/)

</div>

# :book: Project Overview

This project is an educational platform for hardware security demonstrations and challenges. The goal is to have a modular platform with several exercises, to train on FPGA boards. It is very easy to make new boards compatible with the project or to add new challenges.

# :rocket: Getting Started

> ⚠️ Before installation, we assume that your development tools for your FPGA board are already installed, for example, if you are using a Digilent Basys3 board you need to install Xilinx Vivado. 
> These tools are not necessary if you plan to use only simulation.

## Build the documentation

### Generate the documentation

```bash
cd damn-vuln-iot-soc
pip3 install -r requirements.txt
cd docs
make html
cd _build/html
```

# Contributors

### Founder of the project

- [Philippe TANGUY](https://labsticc.fr/en/directory/tanguy-philippe)

### Developers

- Mohamed AFASSI
- [Adam HENAULT](https://github.com/adamhlt)
- Seydina Oumar NIANG
- Rébecca SZABO

# Credits

- [LiteX](https://github.com/enjoy-digital/litex) : This SoC generator is the basis of this project, it is a powerful tool.
- [Migen](https://m-labs.hk/gateware/migen/) : Migen is a Python-based tool that automates further the VLSI design process. It is used in LiteX and it is a very interesting tool.
