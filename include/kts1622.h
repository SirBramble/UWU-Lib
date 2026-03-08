#ifndef KTS1622_H
#define KTS1622_H

#include <cstdint>

#if IS_MCU_VERSION != 0
#include <Arduino.h>
#include <Wire.h>
#else
typedef enum {
  INPUT            = 0x0,
  OUTPUT           = 0x1,
  INPUT_PULLUP     = 0x2,
  INPUT_PULLDOWN   = 0x3,
  OUTPUT_OPENDRAIN = 0x4,

  OUTPUT_2MA       = 0x5,
  OUTPUT_4MA       = 0x6,
  OUTPUT_8MA       = 0x7,
  OUTPUT_12MA      = 0x8,
} PinMode;

typedef enum {
  LOW     = 0,
  HIGH    = 1,
  CHANGE  = 2,
  FALLING = 3,
  RISING  = 4,
} PinStatus;

#define TwoWire void

#endif

#define IS_EXPANDER_PIN 0x80

#define EGP0 0x80
#define EGP1 0xC0

#define EGP0_0 0|EGP0
#define EGP0_1 1|EGP0
#define EGP0_2 2|EGP0
#define EGP0_3 3|EGP0
#define EGP0_4 4|EGP0
#define EGP0_5 5|EGP0
#define EGP0_6 6|EGP0
#define EGP0_7 7|EGP0

#define EGP1_0 0|EGP1
#define EGP1_1 1|EGP1
#define EGP1_2 2|EGP1
#define EGP1_3 3|EGP1
#define EGP1_4 4|EGP1
#define EGP1_5 5|EGP1
#define EGP1_6 6|EGP1
#define EGP1_7 7|EGP1

/**
 * @brief The Input Port Registers (registers 00h, 01h) reflect the incoming logic levels of the pins. The Input port registers
 *        are read only, writes to these registers have no effect and the transaction will be acknowledged (ACK). The
 *        default value ‘X’ is determined by the externally applied logic level. If a pin is configured as an output (registers
 *        04h, 05h), the port value is equal to the actual voltage level on that pin. If the output is configured as open-drain
 *        (register 4Fh and registers 58h, 59h), the input port value is forced to 0.
 *
 *        After reading input port registers, all interrupts will be cleared.
 *
 */
const uint8_t c_addr_reg_input_0            = 0x00;
const uint8_t c_addr_reg_input_1            = 0x01;

/**
 * @brief The Output Port Registers (registers 02h, 03h) show the outgoing logic levels of the pins defined as outputs by
 *        the Configuration Registers. Bit values in these registers have no effect on pins defined as inputs. In turn, reads
 *        from these registers reflect the value that was written to these registers, not the actual pin value.
 *
 */
const uint8_t c_addr_reg_output_0           = 0x02;
const uint8_t c_addr_reg_output_1           = 0x03;

/**
 * @brief The Polarity Inversion Registers (registers 04h, 05h) allow polarity inversion of pins defined as inputs by the
 *        Configuration Registers. If a bit in these registers is set (written with ‘1’), the corresponding port pin’s polarity is
 *        inverted in the input register. If a bit in this register is cleared (written with a ‘0’), the corresponding port pin’s
 *        polarity is retained.
 *
 */
const uint8_t c_addr_reg_pol_inv_0          = 0x04;
const uint8_t c_addr_reg_pol_inv_1          = 0x05;

/**
 * @brief The Configuration Registers (registers 06h, 07h) configure the direction of the I/O pins. If a bit in these registers
 *        is set to 1, the corresponding port pin is enabled as an input. If a bit in these registers is cleared to 0, the
 *        corresponding port pin is enabled as an output.
 *
 */
const uint8_t c_addr_reg_config_0           = 0x06;
const uint8_t c_addr_reg_config_1           = 0x07;


/**
 * @brief The Output Drive Strength Registers (registers 40h, 41h, 42h, 43h) control the output drive level of the GPIO.
 *        Each GPIO can be configured independently to a certain output current level by two register control bits. For
 *        example Port 0.7 is controlled by register 41h CC0.7 (bits [7:6]), Port 0.6 is controlled by register 41h CC0.6
 *        (bits [5:4]). The output drive level of the GPIO is programmed 00b = 0.25x, 01b = 0.5x, 10b = 0.75x or 11b = 1x
 *        of the drive capability of the I/O.
 *
 */
const uint8_t c_addr_reg_output_drive_0A    = 0x40;
const uint8_t c_addr_reg_output_drive_0B    = 0x41;
const uint8_t c_addr_reg_output_drive_1A    = 0x42;
const uint8_t c_addr_reg_output_drive_1B    = 0x43;

/**
 * @brief The Input Latch Registers (registers 44h, 45h) enable and disable the input latch of the I/O pins. These registers
 *        are effective only when the pin is configured as an input port. When an input latch register bit is 0, the
 *        corresponding input pin state is not latched. A state change in the corresponding input pin generates an interrupt.
 *        A read of the input register clears the interrupt. If the input goes back to its initial logic state before the input port
 *        register is read, then the interrupt is cleared.
 *
 *        When an input latch register bit is 1, the corresponding input pin state is latched. A change of state of the input
 *        generates an interrupt and the input logic value is loaded into the corresponding bit of the input port register
 *        (registers 00h, 01h). A read of the input port register clears the interrupt. If the input pin returns to its initial logic
 *        state before the input port register is read, then the interrupt is not cleared and the corresponding bit of the input
 *        port register keeps the logic value that initiated the interrupt.
 *
 *        For example, if the P0_4 input was as logic 0 and the input goes to logic 1 then back to logic 0, the input port 0
 *        register will capture this change and an interrupt is generated (if unmasked). When the read is performed on the
 *        input port 0 register, the interrupt is cleared, assuming there were no additional input(s) that have changed, and
 *        bit 4 of the input port 0 register will read ‘1’. The next read of the input port register bit 4 register should now
 *        read ‘0’.
 *
 *        An interrupt remains active when a non-latched input simultaneously switches state with a latched input and
 *        then returns to its original state. A read of the input register reflects only the change of state of the latched input
 *        and also clears the interrupt. The interrupt is cleared if the input latch register changes from latched to non-
 *        latched configuration and I/O pin returns to its original state.
 *
 *        If the input pin is changed from latched to non-latched input, a read from the input port register reflects the
 *        current port logic level. If the input pin is changed from non-latched to latched input, the read from the input
 *        register reflects the latched logic level.
 *
 */
const uint8_t c_addr_reg_input_latch_0      = 0x44;
const uint8_t c_addr_reg_input_latch_1      = 0x45;

/**
 * @brief The Pull-up and Pull-down Enable Registers allow the user to enable or disable pull-up/pull-down resistors on
 *        the I/O pins. Setting the bit to logic 1 enables the selection of pull-up/pull-down resistors. Setting the bit to logic
 *        0 disconnects the pull-up/pull-down resistors from the I/O pins. Also, the resistors will be disconnected when the
 *        outputs are configured as open-drain outputs. Use the pull-up/pull-down registers to select either a pull-up or
 *        pull-down resistor.
 *
 */
const uint8_t c_addr_reg_pu_pd_enable_0     = 0x46;
const uint8_t c_addr_reg_pu_pd_enable_1     = 0x47;

/**
 * @brief The I/O port can be configured to have pull-up or pull-down resistor by programming the pull-up/pull-down
 *        selection register. Setting a bit to logic 1 selects a 100kΩ pull-up resistor for that I/O pin. Setting a bit to logic 0
 *        selects a 100kΩ pull-down resistor for that I/O pin. If the pull-up/down feature is disconnected, writing to this
 *        register will have no effect on I/O pin. Typical value is 100kΩ with minimum of 50kΩ and maximum of 150kΩ.
 *
 */
const uint8_t c_addr_reg_pu_pd_select_0     = 0x48;
const uint8_t c_addr_reg_pu_pd_select_1     = 0x49;

/**
 * @brief Interrupt Mask Registers are set to logic 1 upon power-on, disabling interrupts during system start-up. Interrupts
 *        may be enabled by setting corresponding mask bits to logic 0. If an input changes state and the corresponding
 *        bit in the Interrupt mask register is set to 1, the interrupt is masked and the interrupt pin will not be asserted. If
 *        the corresponding bit in the Interrupt mask register is set to 0, the interrupt pin will be asserted. When an input
 *        changes state and the resulting interrupt is masked (interrupt mask bit is 1), setting the input mask register bit
 *        to 0 will cause the interrupt pin to be asserted. If the interrupt mask bit of an input that is currently the source of
 *        an interrupt is set to 1, the interrupt pin will be de-asserted.
 *
 */
const uint8_t c_addr_reg_irq_mask_0         = 0x4A;
const uint8_t c_addr_reg_irq_mask_1         = 0x4B;

/**
 * @brief The read-only interrupt status registers are used to identify the source of an interrupt. When read, a logic 1
 *        indicates that the corresponding input pin was the source of the interrupt. A logic 0 indicates that the input pin is
 *        not the source of an interrupt. When a corresponding bit in the interrupt mask register is set to 1 (masked), the
 *        interrupt status bit will return logic 0.
 *
 */
const uint8_t c_addr_reg_irq_status_0       = 0x4C;
const uint8_t c_addr_reg_irq_status_1       = 0x4D;


/**
 * @brief The output port configuration register selects port-wise push-pull or open-drain I/O stage. A logic 0 configures
 *        the I/O as push-pull. A logic 1 configures the I/O as open-drain and the recommended command sequence is to
 *        program this register (4Fh) before the Configuration Register (06h, 07h) sets the port pins as outputs.
 *        ODEN0 configures P0_x, ODEN1 configures P1_x.
 *
 *        Individual pins may be programmed as open-drain or push-pull by programming Individual Pin Output
 *        Configuration registers (58h, 59h).
 *
 *        A register group read or write operation is not allowed on this register. Successive read or write accesses will
 *        remain at this register address.
 *
 */
const uint8_t c_addr_reg_irq_out_port_conf  = 0x4F;

/**
 * @brief The Interrupt Edge Registers determine what action on an input pin will cause an interrupt along with the Interrupt
 *        Mask registers (4Ah, 4Bh). If the Interrupt is enabled (set ‘0’ in the Mask register) and the action at the
 *        corresponding pin matches the required activity, the INT̅̅̅̅̅ output will become active. The default value for each
 *        pin is 00b or level triggered, meaning a level change on the pin will cause an interrupt event. A level triggered
 *        action means a change in logic state (HIGH-to-LOW or LOW-to-HIGH), since the last read of the Input Port
 *        Register (00h, 01h) which can be latched with a corresponding ‘1’ set in the Input Latch Register (44h, 45h). If
 *        the Interrupt Edge Register entry is set to 11b, any edge, positive or negative, causes an interrupt event. If an
 *        entry is 01b, only a positive-going edge will cause an interrupt event, while a 10b will require a negative edge to
 *        cause an interrupt event. These edge interrupt events are latched, regardless of the status of the Input Latch
 *        Register (44h, 45h). These edged interrupts can be cleared in a number of ways: Reading Input Port Registers
 *        (00h, 01h); setting the Interrupt Mask Register (4Ah, 4Bh) to 1 (masked); setting the Interrupt Clear Register
 *        (54h, 55h) to 1 (this is a write-only register); resetting the Interrupt Edge Register (50h to 53h) back to 0.
 *
 */
const uint8_t c_addr_reg_irq_edge_0A        = 0x50;
const uint8_t c_addr_reg_irq_edge_0B        = 0x51;
const uint8_t c_addr_reg_irq_edge_1A        = 0x52;
const uint8_t c_addr_reg_irq_edge_1B        = 0x53;

/**
 * @brief The write-only interrupt clear registers clear individual interrupt sources (status bit). Setting an individual bit or
 *        any combination of bits to logic 1 will reset the corresponding interrupt source, so if that source was the only
 *        event causing an interrupt, the INT̅̅̅̅̅ will be cleared. After writing a logic 1 the bit returns to logic 0.
 *
 */
const uint8_t c_addr_reg_irq_clear_0        = 0x54;
const uint8_t c_addr_reg_irq_clear_1        = 0x55;

/**
 * @brief The read-only input status registers function exactly like Input Port 0, 1 (00h, 01h) without resetting the interrupt
 *        logic. This allows inspection of the actual state of the input pins without upsetting internal logic. If the pin is
 *        configured as an input, the port read is unaffected by input latch logic or other features, the state of the register
 *        is simply a reflection of the current state of the input pins. If a pin is configured as an output by the
 *        Configuration Register (06h, 07h), and is also configured as open-drain (register 4Fh and 58h, 59h), the read
 *        for that pin will always return 0, otherwise that state of that pin is returned.
 *
 */
const uint8_t c_addr_reg_input_status_0     = 0x56;
const uint8_t c_addr_reg_input_status_1     = 0x57;

/**
 * @brief The Individual Pin Output Configuration Registers modify output configuration (push-pull or open-drain) set by
 *        the Output Port Configuration Register (4Fh).
 *
 *        If the ODENx bit is set at logic 0 (push-pull), any bit set to logic 1 in the IOCRx register will reverse the output
 *        state of that pin only to open-drain. When ODENx bit is set at logic 1 (open-drain), a logic 1 in IOCRx will set
 *        that pin to push-pull.
 *
 *        The recommended command sequence to program the output pin is to program ODENx (4Fh), the IOCRx, and
 *        finally the Configuration Register (06h, 07h) to set the pins as outputs.
 *
 */
const uint8_t c_addr_reg_individual_out_0   = 0x58;
const uint8_t c_addr_reg_individual_out_1   = 0x59;

/**
 * @brief The Switch Debounce Enable Registers enable the switch debounce function for Port 0 and Port 1 pins. If a pin
 *        on Port 0 or Port 1 is designated as an input, a logic 1 in the switch debounce enable register will connect
 *        debounce logic to that pin. If a pin is assigned as an output (via Configuration Port 0 or Port 1 register) the
 *        debounce logic is not connected to that pin and it will function as a normal output. The switch debounce logic
 *        requires an oscillator time base input and if this function is used, P0_0 is designated as the oscillator input. If
 *        P0_0 is not configured as input and if SD0.0 is not set to logic 1, then switch debounce logic is not connected to
 *        any pin.
 *
 */
const uint8_t c_addr_reg_sw_db_0            = 0x5A;
const uint8_t c_addr_reg_sw_db_1            = 0x5B;

/**
 * @brief The Switch Debounce Count Register is used to count the debounce time that the switch debounce logic uses
 *        to determine if a switch connected to one of the Port 0 or Port 1 pins finally stays open (logic 1) or closed (logic
 *        0). This number, together with the oscillator frequency supplied to P0_0, determines the debounce time (for
 *        example, the debounce time will be 10μs if this register is set to 0Ah and external oscillator frequency is 1MHz).
 *        The switch debounce logic is disabled if this register is set to 00h.
 *
 */
const uint8_t c_addr_reg_sw_db_count        = 0x5C;

class kts1622
{
public:
  kts1622(uint8_t address, uint8_t int_pin, TwoWire* interface);

  void use_irq();

  void pin_mode(uint8_t pin, PinMode mode);
  void digital_write(uint8_t pin, PinStatus status);
  bool digital_read(uint8_t pin);

private:

  void update_input_reg();

  void write_bit(uint8_t reg, uint8_t bit, bool value);
  bool read_bit(uint8_t reg, uint8_t bit);

  void write_register(uint8_t reg, uint8_t value);
  uint8_t read_register(uint8_t reg);

  bool is_irq_set();

  uint8_t m_address = 0x00;
  uint8_t m_irq_pin = -1;
  TwoWire* m_wire = nullptr;

  bool m_using_irq = false;

  uint8_t m_input_reg[2] = {0,0};


};

#endif
