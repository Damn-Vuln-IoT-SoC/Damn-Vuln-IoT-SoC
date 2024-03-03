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

    $ python3 -m venv DVIS
    $ source DVIS/bin/activate
    $ cd DVIS

Clone the project
-----------------

.. code-block:: console

    $ git clone https://github.com/Damn-Vuln-IoT-SoC/Damn-Vuln-IoT-SoC.git

Install LiteX with the setup
----------------------------

We come to install the platform, the version of LiteX used is: ``2022.08``, it should not be changed, to develop new modules for the platform it is necessary to use this version.
Since we use the VexRiscV processor the standard LiteX configuration is sufficient.

.. code-block:: console

    $ pip3 install -r requirements.txt 
    $ cd Damn-Vuln-IoT-SoC/Damn-Vuln-IoT-SoC
    $ chmod +x litex_setup.py
    $ ./litex_setup.py --init --install --config=standard --tag=2022.08

Install the toolchain
---------------------

In order to compile the firmware for the VexRiscV it is necessary to install the RiscV toolchain, it may be necessary to use the ``sudo`` command to install it.

.. code-block:: console

    $ ./litex_setup.py --gcc=riscv

Build OpenOCD for RiscV
-----------------------

If you want to use the JTAG interface and the GDB debugger you will need to build OpenOCD for RiscV.

.. code-block:: console

    $ sudo apt-get install libtool automake libusb-1.0.0-dev texinfo libusb-dev libyaml-dev pkg-config
    $ git clone https://github.com/SpinalHDL/openocd_riscv.git
    $ cd openocd_riscv/
    $ ./bootstrap
    $ ./configure --enable-ftdi --enable-dummy
    $ make
    $ make install

Build the Demo
--------------

Before generating an SoC it is necessary to choose which vulnerabilities you want to add to the SoC. For that I invite you to read the section of each vulnerability to know which ones to choose, some require additional hardware like a flash memory, a JTAG programmer... 
Then you can edit the ``config/config.ini`` file to fill in the vulnerabilities to add and their parameters if needed.

.. code-block:: console

   $ git clone https://github.com/Damn-Vuln-IoT-SoC/Damn-Vuln-IoT-SoC-Demo.git
   $ pip3 install Damn-Vuln-IoT-SoC-Demo/
   $ cd Damn-Vuln-IoT-SoC-Demo/Damn-Vuln-IoT-SoC-Demo
   $ chmod +x build.py
   $ ./build.py --cpu-type=vexriscv --cpu-variant=lite+vul --integrated-main-ram-size=0x5000 --no-compile-gateware --build
   $ cd firmware
   $ ./firmware.py --build-path=../build/board/ --mem=rom
   $ cd ..
   $ ./build.py --cpu-type=vexriscv --cpu-variant=lite+vul --integrated-rom-init=firmware/firmware.bin --bios-console=disable --build --load
   $ screen <device> 115200

.. note::

    When building you can add the ``--jtag-debug`` command to enbale JTAG interface for debugging without limitation.

.. warning::

    If you want to integrate challenges using SPI flash memory, it is necessary to perform additional steps.

Install Verilator for simulation
--------------------------------

If you want to use the simulation mode provided by LiteX, you will need to install OpenOCD. The version provided by Ubuntu is too old, so you will need to install it from source.

.. code-block:: console

      $ sudo apt-get install git help2man perl python3 make autoconf g++ flex bison ccache
      $ sudo apt-get install libgoogle-perftools-dev numactl perl-doc
      $ sudo apt-get install libfl2
      $ sudo apt-get install libfl-dev
      $ sudo apt-get install zlibc zlib1g zlib1g-dev
      
      $ git clone https://github.com/verilator/verilator

      $ cd verilator    
      $ autoconf
      $ ./configure
      $ make -j `nproc`
      $ sudo make install

Test the Demo in simulation mode
--------------------------------

Alternatively, you can try the simulation, which is a downgraded version of the challenges and won't allow you to do them all.

.. code-block:: console

   $ sudo apt install libevent-dev libjson-c-dev
   $ git clone https://github.com/Damn-Vuln-IoT-SoC/Damn-Vuln-IoT-SoC-Demo.git
   $ pip3 install Damn-Vuln-IoT-SoC-Demo/
   $ cd Damn-Vuln-IoT-SoC-Demo/Damn-Vuln-IoT-SoC-Demo
   $ chmod +x sim.py
   $ ./sim.py --cpu-type=vexriscv --cpu-variant=lite+vul --integrated-main-ram-size=0x5000 --no-compile-gateware
   $ cd firmware
   $ ./firmware.py --build-path=../build/sim/ --mem=rom
   $ cd ..
   $ ./sim.py --cpu-type=vexriscv --cpu-variant=lite+vul --integrated-rom-init=firmware/firmware.bin --bios-console=disable
