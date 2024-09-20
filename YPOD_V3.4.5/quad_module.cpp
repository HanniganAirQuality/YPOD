/*******************************************************************************
 * @file    quad_module.cpp
 * @brief   Splits quadstat code from .ino & updates to MCP242x.h library
 *
 * @cite    XPOD >> quad_module.cpp by Ajay Kandagal, ajka9053@colorado.edu
 *
 * @author  Percy Smith, percy.smith@colorado.edu
 * @date    July 4, 2024
 * @log     Formatting & adding comments 
 * @TBD     Add an option to disable (not default) unused channels
******************************************************************************/
#include "quad_module.h"

/**************************************************************************/
 /*!
 *    @brief  ADS_Module object; Assigns addresses & channels to ADS1115 modules
 */
/**************************************************************************/
QUAD_Module::QUAD_Module()
{
  status = true;
}

bool QUAD_Module::begin()
{
  alpha_one = MCP342x(ALPHA_ONE_ADDR);
  alpha_two = MCP342x(ALPHA_TWO_ADDR);

  MCP342x::generalCallReset();
  delay(1);

  return status;
}

quad_data QUAD_Module::return_data()
{
  MCP342x::Config status;
  long value = 0;

  // Initiate a conversion; convertAndRead() will wait until it can be read
  alpha_one.convertAndRead(MCP342x::channel1, MCP342x::oneShot, MCP342x::resolution16, 
                          MCP342x::gain1, 1000000, value, status);
  data.a1C1 = value;

   alpha_one.convertAndRead(MCP342x::channel2, MCP342x::oneShot, MCP342x::resolution16, 
                          MCP342x::gain1, 1000000, value, status);
  data.a1C2 = value;

  alpha_one.convertAndRead(MCP342x::channel3, MCP342x::oneShot, MCP342x::resolution16, 
                          MCP342x::gain1, 1000000, value, status);
  data.a2C1 = value;

  alpha_one.convertAndRead(MCP342x::channel4, MCP342x::oneShot, MCP342x::resolution16, 
                          MCP342x::gain1, 1000000, value, status);
  data.a2C2 = value;

  alpha_two.convertAndRead(MCP342x::channel1, MCP342x::oneShot, MCP342x::resolution16, 
                          MCP342x::gain1, 1000000, value, status);
  data.a3C1 = value;

  alpha_two.convertAndRead(MCP342x::channel2, MCP342x::oneShot, MCP342x::resolution16, 
                          MCP342x::gain1, 1000000, value, status);
  data.a3C2 = value;

  alpha_two.convertAndRead(MCP342x::channel3, MCP342x::oneShot, MCP342x::resolution16, 
                          MCP342x::gain1, 1000000, value, status);
  data.a4C1 = value;

  alpha_two.convertAndRead(MCP342x::channel4, MCP342x::oneShot, MCP342x::resolution16, 
                          MCP342x::gain1, 1000000, value, status);
  data.a4C2 = value;

  return data;
}