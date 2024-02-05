.. Damn Vuln IoT SoC documentation master file, created by
   sphinx-quickstart on Thu Feb  9 17:47:19 2023.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Damn Vuln IoT SoC
=================

This project is educative plateform that aims at developing a generator of vulnerable, configurable and easily usable Systems-on-Chip (SoC). It is intended for users wishing to train themselves to exploit or correct vulnerabilities in the context of hardware security, this platform can be used as course material or for CTF.

The following documentation will guide you through the installation and configuration of the development environment required for this project.

Hardware and software used for the project
------------------------------------------

.. warning::

   Since it's an educational platform we decided to use a Digilent Basys3 board since it's a popular and inexpensive board, but the project can easily be adapted to many other FPGA boards.

- OS : Ubuntu 20.04
- LiteX (Tag : 2022.08)
- Python 3.6 or higher
- Digilent Basys3 FPGA board (can be another one)

Content
-------

.. toctree::

   install
   keypad_challenge
   jtag_activation_overlap
   jtag_password_hardcoded
   jtag_password_random
   jtag_activation_firmware
   loop_back_signal
   memory_overlap


Contributors
------------

**Founder of the project :**

- `Philippe TANGUY <https://labsticc.fr/en/directory/tanguy-philippe>`_


**Developers :**

- Mohamed AFASSI
- `Adam HENAULT <https://github.com/adamhlt>`_
- Seydina Oumar NIANG
- Rébecca SZABO

Credits
-------

- `LiteX <https://github.com/enjoy-digital/litex>`_ : This SoC generator is the basis of this project, it is a powerful tool.
- `Migen <https://m-labs.hk/gateware/migen/>`_ : Migen is a Python-based tool that automates further the VLSI design process. It is used in LiteX and it is a very interesting tool.