#ifndef USER_MODE
#define __NO_VERSION__

//#include "rt_compat.h"

#endif


#include "cbmimo1_device.h"
#include "defs.h"
#include "extern.h"
#include "cbmimo1_pci.h"
#include "SCHED/defs.h"
#include "SCHED/extern.h"

#include "from_grlib_softconfig.h"
#include "from_grlib_softregs.h"

/*
int openair_get_adac_cnt(void) {
#ifndef NOCARD_TEST
  //  return openair_readl(bar[0]+REG_BAR+ADAC_CNT);  
#else
  return(0);
#endif
}
*/

int openair_dma(unsigned char card_id,unsigned int cmd) {

#ifndef NOCARD_TEST
  int i;
  int res;
  unsigned int val;

  if (vid != XILINX_VENDOR) {
    openair_readl(pdev[card_id],FROM_GRLIB_CFG_GRPCI_EUR_CTRL_OFFSET,&res);
    //  printk("[openair][DMA] cmd %d\n",cmd);
    
    if ((res & FROM_GRLIB_IRQ_FROM_PCI) != 0) {
      printk("[openair][DMA] Error: cmd %x, Leon IRQ active\n", cmd);
      return -1;
    }
    //printk("[openair][DMA] cmd %x on card %d\n",cmd,card_id);
    
    //openair_writel(cmd,bar[0]+REG_BAR+DMA_CMD);  // arms DMA
    openair_writel(pdev[card_id], FROM_GRLIB_CFG_GRPCI_EUR_CTRL_OFFSET, ((cmd & FROM_GRLIB_IRQ_FROM_PCI_MASK) | FROM_GRLIB_IRQ_FROM_PCI));
    //  openair_writel(PCI_INTR_VAL,bar[0]+REG_BAR+PCI_INTR_OFF); // trigger interrupt
    //  openair_writel(PCI_INTR_VAL,PCI_INTR_OFF);
    
    //  printk("[openair][DMA] cmd %d done \n",cmd);
    return 0;
  }
  else {  // ExpressMIMO
    printk("Sending command to ExpressMIMO : %x\n",cmd);
    //write cmd to be executed by
    iowrite32(cmd,(bar[0]+0x04));
    printk("Readback of control1 %x\n",ioread32(bar[0]+0x4));
    val = ioread32(bar[0]);
    // set interrupt bit to trigger LEON interrupt
    iowrite32(val|0x1,bar[0]);
    printk("Readback of control0 %x\n",ioread32(bar[0]));
  }
#endif //NOCARD_TEST
}
