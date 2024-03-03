Challenge 2 : Enable JTAG (Overlap version)
===========================================

.. note:: 

    This challenge can be disable, to change the configuration edit the file ``config.ini`` and modify the field ``jtag_lock_overlap``. This challenge is part of the challenges that revolves around the JTAG, this implies that if other challenges are activated it will change the Migen code used as well as the Verilog code generated, however the principle of the challenge remains the same. For more details I invite you to look at the : ``ip/jtag_lock.py``.

.. warning::

    You have to know that this challenge is automaticly desactivate if the challenge ``jtag_lock_firmware`` is enable.

Introduction
------------

This challenge is about the JTAG debug port, at the startup of the FPGA board the JTAG port is disabled, to know the status of the JTAG port a command is available in the administration panel. The status command allows to display if the JTAG is activated or not and if it is blocked by a password or unblocked (this is only displayed if a JTAG port locking challenge has been activated). Here the goal is to make an audit of the headers present in the board package support and to discover an overlap of the CSRs (mapped memory registers) for the LEDs control and the JTAG activation. This challenge is of medium difficulty, but the exploitation of the bug is very simple.

Analysis of the Board Support Package headers
---------------------------------------------

In this part we will quickly analyze the header file ``csr.h``, this header file writes all the necessary functions to write to the CSRs from the firmware. It is an essential file for the development of application for our SoC.  We can see that 2 functions write to the same place in memory, the functions ``jtag_jtag_lock_write`` and ``leds_out_write``. The function ``leds_out_write`` is used in the ``leds`` command and allows to control the leds of the board. After having seen this we can imagine that we can activate the JTAG interface using the ``led`` command.

.. code-block:: c 

    ...

    /* jtag */
    #define CSR_JTAG_BASE (CSR_BASE + 0x5000L)
    #define CSR_JTAG_JTAG_LOCK_ADDR (CSR_BASE + 0x5000L)
    #define CSR_JTAG_JTAG_LOCK_SIZE 1
    static inline uint32_t jtag_jtag_lock_read(void) {
        return csr_read_simple((CSR_BASE + 0x5000L));
    }
    static inline void jtag_jtag_lock_write(uint32_t v) {
        csr_write_simple(v, (CSR_BASE + 0x5000L)); // Write to the address CSR_BASE + 0x5000
    }
    #define CSR_JTAG_JTAG_PASSWORD_ADDR (CSR_BASE + 0x5004L)
    #define CSR_JTAG_JTAG_PASSWORD_SIZE 1
    static inline uint32_t jtag_jtag_password_read(void) {
        return csr_read_simple((CSR_BASE + 0x5004L));
    }
    static inline void jtag_jtag_password_write(uint32_t v) {
        csr_write_simple(v, (CSR_BASE + 0x5004L));
    }
    #define CSR_JTAG_JTAG_PASSWORD_SOFT_ADDR (CSR_BASE + 0x5008L)
    #define CSR_JTAG_JTAG_PASSWORD_SOFT_SIZE 1
    static inline uint32_t jtag_jtag_password_soft_read(void) {
        return csr_read_simple((CSR_BASE + 0x5008L));
    }
    static inline void jtag_jtag_password_soft_write(uint32_t v) {
        csr_write_simple(v, (CSR_BASE + 0x5008L));
    }
    #define CSR_JTAG_JTAG_STATUS_ADDR (CSR_BASE + 0x500cL)
    #define CSR_JTAG_JTAG_STATUS_SIZE 1
    static inline uint32_t jtag_jtag_status_read(void) {
        return csr_read_simple((CSR_BASE + 0x500cL));
    }
    #define CSR_JTAG_PASSWORD_STATUS_ADDR (CSR_BASE + 0x5010L)
    #define CSR_JTAG_PASSWORD_STATUS_SIZE 1
    static inline uint32_t jtag_password_status_read(void) {
        return csr_read_simple((CSR_BASE + 0x5010L));
    }

    /* leds */
    #define CSR_LEDS_BASE (CSR_BASE + 0x5000L)
    #define CSR_LEDS_OUT_ADDR (CSR_BASE + 0x5000L)
    #define CSR_LEDS_OUT_SIZE 1
    static inline uint32_t leds_out_read(void) {
        return csr_read_simple((CSR_BASE + 0x5000L));
    }
    static inline void leds_out_write(uint32_t v) {
        csr_write_simple(v, (CSR_BASE + 0x5000L)); // Write to the address CSR_BASE + 0x5000
    }


Verilog code of the challenge
-----------------------------

To confirm our hypothesis we can analyze the Verilog code, as you can see when the CSR value is equal to 1 the signals are connected to the CPU JTAG interface.

.. code-block:: verilog

    ...

    if ((main_jtag_lock_storage == 1'd1)) begin // If the CSR is equal to 1 the signals are connected
		main_basesoc_i_jtag_tdi <= jtag_tdi;
		main_basesoc_i_jtag_tms <= jtag_tms;
		main_basesoc_i_jtag_tck <= jtag_tck;
		jtag_tdo <= main_basesoc_o_jtag_tdo;
	end else begin
		main_dummy_tdi <= jtag_tdi;
		main_dummy_tms <= jtag_tms;
		main_dummy_tck <= jtag_tck;
		jtag_tdo <= 1'd0;
	end

Resolve the challenge
---------------------

Since we find the overlap between the 2 CSRs, we can now try to exploit the bug to resolve the challenge. At the beginning of the challenge we can check that the JTAG is not activated and all the leds are blinking.

.. image:: images/disable.png
  :width: 750
  :alt: The JTAG interface is disable

Then we use the command ``leds`` to control the LEDs and set it to 1, we can see that only the first LED is on. We can now retrieve the status of the JTAG interface.

.. image:: images/enable.png
  :width: 750
  :alt: The JTAG interface is enable