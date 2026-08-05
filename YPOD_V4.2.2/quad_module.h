/*******************************************************************************
 * @file    quad_module.cpp
 * @brief   
 *
 * @author 	Ajay Kandagal, ajka9053@colorado.edu
 *
 * @editor  Percy Smith, percy.smith@colorado.edu
 *
 * @date 	  July 3, 2024
 ******************************************************************************/
#ifndef _QUAD_Module_H
#define _QUAD_Module_H

#include <Arduino.h>
#include <Wire.h>
#include <MCP342x.h>

#define ALPHA_ONE_ADDR        (0x69)
#define ALPHA_TWO_ADDR        (0x6E)

struct quad_data 
{
  long a1C1, a1C2, a2C1, a2C2, a3C1, a3C2, a4C1, a4C2;
};

class QUAD_Module
{
  public:
    QUAD_Module();
    bool begin();
    quad_data return_data();
  private:
    MCP342x alpha_one;
    MCP342x alpha_two;
    quad_data data;
    bool status;
};

#endif  //_QUAD_Module_H