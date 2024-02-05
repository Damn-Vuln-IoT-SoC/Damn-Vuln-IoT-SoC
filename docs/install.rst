Install the SoC generator
=========================

In this part we will explain how to install the SoC generator and all the dependencies needed to run the platform.

.. warning::

   During the development of the platform we used a Digilent Basys3 board, to be able to use the SoC on the board it is necessary to use Xilinx Vivado, here we don't show you how to install the tools necessary to deploy the SoC on your FPGA board, we advise you to look at the LiteX documentation (`here <https://github.com/enjoy-digital/litex/wiki>`_) for more information.

Installation of necessary tools
-------------------------------

To use the platform it is necessary to install some tools before.

.. warning::

    Here we assume that you already have python and python virtual environment installed.

..  code-block:: console

    $ sudo apt install make gcc g++ libz-dev

Creation of a Python virtual environment
----------------------------------------

We create a virtual python environment to be more secure, and not to break existing installations.

.. code-block:: console

    $ python3 -m venv LiteX
    $ source LiteX/bin/activate
    $ cd LiteX

Clone the project
-----------------

.. code-block:: console

    $ git clone https://forgens.univ-ubs.fr/gitlab/adamhlt/damn-vuln-iot-soc.git

Install LiteX with the setup
----------------------------

We come to install the platform, the version of LiteX used is: ``2022.08``, it should not be changed, to develop new modules for the platform it is necessary to use this version.
Since we use the VexRiscV processor the standard LiteX configuration is sufficient.

.. code-block:: console

    $ pip3 install -r requirements.txt 
    $ cd damn-vuln-iot-soc/Damn-Vuln-IoT-SoC
    $ chmod +x litex_setup.py
    $ ./litex_setup.py --init --install --config=standard --tag=2022.08

Install the toolchain
---------------------

In order to compile the firmware for the VexRiscV it is necessary to install the RiscV toolchain, it may be necessary to use the ``sudo`` command to install it.

.. code-block:: console

    $ ./litex_setup.py --gcc=riscv

Install the setup for the simulation and test it
------------------------------------------------

.. code-block:: console

    $ sudo apt install libevent-dev libjson-c-dev verilator
    $ litex_sim --cpu-type=vexriscv

Build the SoC for the challenges
--------------------------------

.. note::

    If you have already installed the tools to generate the bitstream for your FPGA board you can generate it with LiteX and load it on the card with the ``--load`` command.

Before generating an SoC it is necessary to choose which vulnerabilities you want to add to the SoC. For that I invite you to read the section of each vulnerability to know which ones to choose, some require additional hardware like a flash memory, a JTAG programmer... 
Then you can edit the ``conf.ini`` file to fill in the vulnerabilities to add and their parameters if needed. Once all this is done you can use the following command in the folder ``Damn-Vuln-IoT-SoC/litex-boards/litex_boards/targets`` to generate the SoC :

.. code-block:: console

    $ ./digilent_basys3_vul.py --cpu-type=vexriscv --cpu-variant=lite+vul --integrated-main-ram-size=0x5000 --bios-console=disable --build

After you can add ``--load`` parameter to load the bitstream on the FPGA.

.. code-block:: console

    $ ./digilent_basys3_vul.py --cpu-type=vexriscv --cpu-variant=lite+vul --integrated-main-ram-size=0x5000 --bios-console=disable --load

Now you can use the platform to generate vulnerable SoCs and train on them.

.. note::

    When building you can add the ``--jtag-debug`` command to enbale JTAG interface for debugging without limitation.

.. warning::

    If you want to integrate challenges using SPI flash memory, it is necessary to perform additional steps.

Build the firmware
------------------

.. note::

    To load the firmware on the base please refer to the LiteX `documentation <https://github.com/enjoy-digital/litex/wiki/Load-Application-Code-To-CPU#serial-boot>`_ using ``litex_term``.

The firmware is made to be modular and include only the code needed for your challenges. In order to generate the firmware, please generate the bitstream first and choose the challenges you want.
To generate the firmware, please go to the ``Damn-Vuln-IoT-SoC/litex/litex/soc/software/firmware`` folder and run the ``build.sh`` script.

.. code-block:: console

    $ cd Damn-Vuln-IoT-SoC/litex/litex/soc/software/firmware
    $ ./build.sh
    $ litex_term <device> --kernel=firmware.bin

.. note::

    To load the firmware on the external SPI flash memory you have to use the ``firmware.fbi`` which is generated with the ``build.sh`` script.

Build OpenOCD for RiscV
-----------------------

If you want to use the JTAG interface and the GDB debugger you will need to build OpenOCD for RiscV.

.. code-block:: console
    
    $ git clone https://github.com/SpinalHDL/openocd_riscv.git
    $ cd openocd_riscv/
    $ ./bootstrap
    $ ./configure --enable-ftdi --enable-dummy
    $ make
    $ make install
