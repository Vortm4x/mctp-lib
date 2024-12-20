#ifndef HOST_H
#define HOST_H

typedef enum __attribute__ ((__packed__))
{
    MCTP_BINDING_TYPE_RESERVED  = 0x00,
    MCTP_BINDING_TYPE_SMBUS     = 0x01,
    MCTP_BINDING_TYPE_PCIE_VDM  = 0x02,
    MCTP_BINDING_TYPE_USB       = 0x03,
    MCTP_BINDING_TYPE_KCS       = 0x04,
    MCTP_BINDING_TYPE_SERIAL    = 0x05,
}
mctp_binding_type_t;

typedef enum __attribute__ ((__packed__))
{
    MCTP_PHYSICAL_MEDIUM_UNSPEC                 = 0x00,
    MCTP_PHYSICAL_MEDIUM_SMBUS_20_100KHZ        = 0x01,
    MCTP_PHYSICAL_MEDIUM_SMBUS_20_I2C_100KHZ    = 0x02,
    MCTP_PHYSICAL_MEDIUM_I2C_STANDARD_MODE      = 0x03,
    MCTP_PHYSICAL_MEDIUM_SMBUS30_400KHZ         = 0x04,
    MCTP_PHYSICAL_MEDIUM_I2C_FAST_MODE          = 0x04,
    MCTP_PHYSICAL_MEDIUM_SMBUS30_1000KHZ        = 0x05,
    MCTP_PHYSICAL_MEDIUM_I2C_FAST_MODE_PLUS     = 0x05,    
    MCTP_PHYSICAL_MEDIUM_I2C_HIGH_SPEED_MODE    = 0x06,
    MCTP_PHYSICAL_MEDIUM_PCIE_REV11             = 0x08,
    MCTP_PHYSICAL_MEDIUM_PCIE_REV20             = 0x09,
    MCTP_PHYSICAL_MEDIUM_PCIE_REV21             = 0x0A,
    MCTP_PHYSICAL_MEDIUM_PCIE_REV3X             = 0x0B,
    MCTP_PHYSICAL_MEDIUM_PCIE_REV4X             = 0x0C,
    MCTP_PHYSICAL_MEDIUM_PCIE_REV5X             = 0x0D,
    MCTP_PHYSICAL_MEDIUM_CXL_1X                 = 0x0D,
    MCTP_PHYSICAL_MEDIUM_CXL_2X                 = 0x0D,
    MCTP_PHYSICAL_MEDIUM_PCI                    = 0x0F,
    MCTP_PHYSICAL_MEDIUM_USB_10                 = 0x10,
    MCTP_PHYSICAL_MEDIUM_USB_20                 = 0x11,
    MCTP_PHYSICAL_MEDIUM_USB_30                 = 0x12,
    MCTP_PHYSICAL_MEDIUM_NC_SI_OVER_RBT         = 0x18,
    MCTP_PHYSICAL_MEDIUM_KCS_LEGACY             = 0x20,
    MCTP_PHYSICAL_MEDIUM_KCS_PCI                = 0x21,
    MCTP_PHYSICAL_MEDIUM_SERIAL_HOST_LEGACY     = 0x22,
    MCTP_PHYSICAL_MEDIUM_SERIAL_HOST_PCI        = 0x23,
    MCTP_PHYSICAL_MEDIUM_ASYNC_SERIAL           = 0x24,
    MCTP_PHYSICAL_MEDIUM_I3C                    = 0x30,
}
mctp_physical_medium_t;

typedef enum __attribute__ ((__packed__))
{
    MCTP_HOST_IF_KCS        = 0x02,
    MCTP_HOST_UART_8250     = 0x03,
    MCTP_HOST_UART_16450    = 0x04,
    MCTP_HOST_UART_16550    = 0x05,
    MCTP_HOST_UART_16550A   = 0x05,
    MCTP_HOST_UART_16650    = 0x06,
    MCTP_HOST_UART_16650A   = 0x06,
    MCTP_HOST_UART_16750    = 0x07,
    MCTP_HOST_UART_16750A   = 0x07,
    MCTP_HOST_UART_16850    = 0x08,
    MCTP_HOST_UART_16850A   = 0x08,
    MCTP_HOST_I2C           = 0x09,
    MCTP_HOST_SMBUS         = 0x09,
    MCTP_HOST_I3C           = 0x0A,
    MCTP_HOST_PCIe_VDM      = 0x0B,
    MCTP_HOST_MMBI          = 0x0C,
}
mctp_hif_type_t;

#endif //HOST_H