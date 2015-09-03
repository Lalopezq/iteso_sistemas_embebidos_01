#include    "pin_toggle.h"

/** Variable types and common definitions */
#include    "typedefs.h"
/** IO Indication functions */
#include    "io.h"

static UINT8 u8Counter = 0u;

void vfnPinToggle(void){                                                                   
  if(u8Counter == 5){
    u8Counter = 0;                                                          
    PORTA_PA0 = ~PORTA_PA0;                      
  }   
  u8Counter++;                                               
}