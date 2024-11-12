#ifndef CompilerMacros_h
#define CompilerMacros_h
#include <Definitions.h>

#if SPS_UART_NUM == 0
#define SPS_UART_EVENT serialEvent
#define SPS_UART Serial
#elif SPS_UART_NUM == 1
#define SPS_UART_EVENT serialEvent1
#define SPS_UART Serial1
#endif

//Strings verketten
#define STR_HELPER(x) #x 
#define STR(x) STR_HELPER(x) 



//Serial Periods
#define SPS_UART_Period 1000000/SPS_UART_Frequency


#endif