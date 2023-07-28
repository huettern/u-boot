#include <init.h>

int board_init(void) {
  debug("%s: could not enable cache ways\n", __func__);
  return 0;
}

int ft_board_setup(void *fdt, struct bd_info *bd) { return 0; }

#include <miiphy.h>
#ifndef DP83867_DEVADDR
#define DP83867_DEVADDR 0x1f
#endif

int board_phy_config(struct phy_device *phydev) {
  u16 temp;

  // // enable SGMII in DP83867IS phy PHYCR register
  // temp = phy_read_mmd(phydev, DP83867_DEVADDR, 0x0010);
  // temp |= 1 << 11; // SGMII_EN
  // temp = phy_write_mmd(phydev, DP83867_DEVADDR, 0x0010, temp);
  // temp = phy_read_mmd(phydev, DP83867_DEVADDR, 0x0010);
  // if (temp & (1 << 11))
  //   printf("  OK: set SGMII_EN: %x\r\n", temp);
  // else
  //   printf("  failed to set SGMII_EN. PHYCSR: %x\r\n", temp);

  // // set 6 wire sgmii
  // temp = 1UL << 14;
  // phy_write_mmd(phydev, DP83867_DEVADDR, 0x00d3, temp);
  
  // sgmii aneg timer and clear internal test mode
  // RX_CTRL is floating on VCU128 which is not applicable
  // Clear bit 7 of conig reg 4 0x0031 (Internal test mode)
  // wrong strap configuration on the VCU128 board causes the PHY to enter
  // internal test mode and ceases to function. Clear bit 7 INT_TST_MODE_1 in
  // Configuration Register 4 (CFG4), Address 0x0031 to exit test mode. Refer to
  // datasheet of DP83867IS page 38
  temp = phy_read_mmd(phydev, DP83867_DEVADDR, 0x0031);
  temp &= ~(1U << 7); // clear INT_TST_MODE_1
  phy_write_mmd(phydev, DP83867_DEVADDR, 0x0031, temp);
  temp = phy_read_mmd(phydev, DP83867_DEVADDR, 0x0031);
  log_debug("  INFO: Set cfg4 (sgmii aneg tmr): %x\r\n", temp);

  // // En/Disable SGMII auto negotiation SGMII_AUTONEG_EN: clear bit 7 in
  // 0x0014 temp = phy_read_mmd(phydev, DP83867_DEVADDR, 0x0014);
  // temp |= (1UL << 7);
  // phy_write_mmd(phydev, DP83867_DEVADDR, 0x0014, temp);
  // temp = phy_read_mmd(phydev, DP83867_DEVADDR, 0x0014);

  // log_debug
  if (phydev->drv->config)
    return phydev->drv->config(phydev);
  return 0;
}
