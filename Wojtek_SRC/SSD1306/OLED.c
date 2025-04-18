
#include "OLED.h"


#define OLED_WRITE_CMD(LCD_pointer , cmd)		ResWriteReg(LCD_pointer->I2Cx, 0x00, cmd, LCD_pointer->I2CAddr)
#define OLED_WRITE_DATA(LCD_pointer , DATA)		ResWriteReg(LCD_pointer->I2Cx, 0x40, DATA, LCD_pointer->I2CAddr)


// ************************************* Send LCD buffer to the device **********************************

uint8_t OLEDDisplayBufer (OLEDType *LCD)

{
	uint16_t i=0, j=0;

	uint8_t TempData=0,index=0, index2=0,TempData2=0;

	//BufPointer=LCD->Buffer;




#ifdef OLED_STM_Version


		  for (index = 0; index < (SSD1306_HEIGHT/8); index++)
		  {
			j=0;
			// ****************** write command **********************
			I2C_GenerateSTART(LCD->I2Cx, ENABLE);
			while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

			I2C_Send7bitAddress(LCD->I2Cx, (uint8_t)(LCD->I2CAddr << 1), I2C_Direction_Transmitter);
			while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

			I2C_SendData(LCD->I2Cx, (uint8_t)(0x00));
			while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	    	I2C_SendData(LCD->I2Cx, (0xB0+index));
	    	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	    	I2C_SendData(LCD->I2Cx, 0x00);
	    	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	    	I2C_SendData(LCD->I2Cx, 0x10);
	    	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));


	    	// ****************** write data for one page **********************

			I2C_GenerateSTART(LCD->I2Cx, ENABLE);
			while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

			I2C_Send7bitAddress(LCD->I2Cx, (uint8_t)(LCD->I2CAddr << 1), I2C_Direction_Transmitter);
			while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

			I2C_SendData(LCD->I2Cx, (uint8_t)(0x40));
			while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

			do{

		    	I2C_SendData(LCD->I2Cx, LCD->Buffer[(SSD1306_WIDTH * index) + j]);
		    	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		    	j++;

		    }while (j < SSD1306_WIDTH);
		    /* Write multi data */
		    //ssd1306_I2C_WriteMulti(SSD1306_I2C_ADDR, 0x40, &SSD1306_Buffer[SSD1306_WIDTH * m], SSD1306_WIDTH);
		  }
#else
			I2C_GenerateSTART(LCD->I2Cx, ENABLE);
			while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

			I2C_Send7bitAddress(LCD->I2Cx, (uint8_t)(LCD->I2CAddr << 1), I2C_Direction_Transmitter);
			while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

			I2C_SendData(LCD->I2Cx, (uint8_t)(0x40));
			while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	do {
		TempData=0;
		for (index2=0; index2<8; index2++)
			{
				TempData=0;
				for (index = 8; index > 0; index--)			// index from 8 to be able to add SHL command and coolect all data to send
				{


					TempData2 = (LCD->Buffer[i][j+index-1]) & (1<<(index2));

					if(TempData2)
						{
							TempData++;
						}
					if (index >1 )TempData <<= 1;				// don't shift at the end of data, only add

				}

 				I2C_SendData(LCD->I2Cx, TempData);
 				while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

			}

		i++;

		if (i >= (SSD1306_WIDTH/8))
		{
 			i=0;
			j+=8;
		}
	}while (j < SSD1306_HEIGHT );
#endif

	I2C_GenerateSTOP(LCD->I2Cx, ENABLE);
	//while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

}

// ****************** Draw pixel *************************************
void OLED_DrawPixel(OLEDType *LCD, uint8_t x, uint8_t y, SSD1306_COLOR_t color) {
  if ( x >= SSD1306_WIDTH ||  y >= SSD1306_HEIGHT )
  	  	  {
            /* Error */
          //  return;
          }

  // default colour is white
  //color = SSD1306_COLOR_WHITE;  /* Set color */

  //  Check if pixels are inverted
  if (LCD->sreg & OLED_TO_BE_IVERT)
  {
    color = (SSD1306_COLOR_t)!color;
  }


  if (color == SSD1306_COLOR_WHITE)
  {
    LCD->Buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y % 8));
  } else {
	  LCD->Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
  }
}


// ****************** GotoXY *************************************
void OLED_GotoXY(OLEDType *LCD, uint16_t x, uint16_t y) {
  /* Set write pointers */
  LCD->CurrentX = x;
  LCD->CurrentY = y;
}



// ************************************* clear LCD buffer **********************************
uint8_t OLEDBuferClear (OLEDType *LCD)
{
	uint16_t i=0;
#if (SSD1306_BUFFER_SIZE > 0xFFFF)
	#error"Too large buffer"
#endif

	do {
#ifdef OLED_STM_Version
		LCD->Buffer[i] = 0;
#else
		*(&(LCD->Buffer[0][0])+i) = 0;
#endif
		i++;
	}while (i < SSD1306_BUFFER_SIZE);

}



/// ************************ OLED buffer filling for test only  ***********************
/// ************************ To be removed in the future 		***********************

uint8_t OLEDTestFill(OLEDType *LCD)
{
	uint16_t z=0, j;
	uint8_t TempBuffer[2], x, y,i;

	i=1;
	j=0;
	x=0;
	y=0;

	do {

#ifdef OLED_STM_Version

			LCD->Buffer[j] |= B16_LSB(i);
			j++;

			i <<= 1;
			i += 1;

			if (i==0xff)
			{
				i=0;
			}
		}while (j < (SSD1306_BUFFER_WIDTH*(SSD1306_HEIGHT/8)));
#else
		LCD->Buffer[x][y] |= B16_LSB(i);

		y++;

		i <<= 1;
		i += 1;

		if (i==0xff)
		{
			i=0;
		}

		if ((y % 8) == 0)
		{
			y-=8;

			x++;
			if ((x > SSD1306_WIDTH/8))
			{
				z++;
				x =  0;
				y += 8;

			}
		}

	}while (z < (SSD1306_HEIGHT/8));

#endif
}
// ************************************* clear LCD buffer **********************************
uint8_t OLEDBuferPutChar(OLEDType *LCD, uint8_t x, uint8_t y, uint8_t *Char)
{
	uint16_t z=0, BitMask, b, j,i;

#ifdef OLED_STM_Version
	SSD1306_COLOR_t color=SSD1306_COLOR_WHITE;


#ifdef FONT_5X8
if (LCD->sreg & OLED_TO_BE_IVERT)
	{
		color = !color;
	}


	if ((LCD->FontWidth < (LCD->CurrentX + LCD->FontWidth)) ||
		(LCD->FontHeight < (LCD->CurrentY + LCD->FontHeight))
	    )
	  {
		  // report error
	  }
	  else{
		  LCD->CurrentX=x;
		  LCD->CurrentY=y;
		    for (i = 0; i < LCD->FontHeight; i++)
		    	{
		    		b = LCD->FontDataPointer[(*Char - 32) *  LCD->FontHeight + i];
		      for (j = 0; j <  LCD->FontWidth; j++) {
		        if ((b << j) & 0x8000) {
		          OLED_DrawPixel(LCD, LCD->CurrentX + j, (LCD->CurrentY + i), (SSD1306_COLOR_t) color);
		        } else {
		        	OLED_DrawPixel(LCD, LCD->CurrentX + j, (LCD->CurrentY + i), (SSD1306_COLOR_t)!color);
		        }
		      }
		    }

	  }
#endif

#ifdef FONT_11X18
if (LCD->sreg & OLED_TO_BE_IVERT)
	{
		color = !color;
	}


	if ((SSD1306_WIDTH < (LCD->CurrentX + LCD->FontWidth)) ||
		(SSD1306_HEIGHT < (LCD->CurrentY + LCD->FontHeight))
	    )
	  {
		  // report error
	  }
	  else{
		  LCD->CurrentX=x;
		  LCD->CurrentY=y;
		    for (i = 0; i < LCD->FontHeight; i++)
		    	{
		    		b = LCD->FontDataPointer[(*Char - 32) *  LCD->FontHeight + i];
		      for (j = 0; j <  LCD->FontWidth; j++) {
		        if ((b << j) & 0x8000) {
		          OLED_DrawPixel(LCD, LCD->CurrentX + j, (LCD->CurrentY + i), (SSD1306_COLOR_t) color);
		        } else {
		        	OLED_DrawPixel(LCD, LCD->CurrentX + j, (LCD->CurrentY + i), (SSD1306_COLOR_t)!color);
		        }
		      }
		    }

	  }
#endif
#else
		BitMask = 0x01;
		if (x%8)
		{
			x=(x/8)+1;
		}
		else
			x=(x/8) ;

	do {

		LCD->Buffer[x][y] 	&= ~(B16_LSB(CHAR_MASK));
		LCD->Buffer[(x+1)][y] &= ~(B16_MSB(CHAR_MASK));

		LCD->Buffer[x][y] 	|= B16_LSB(FONT_11x18_DATA[((*Char-0x20)*FONT_HIGH)+(z)]);
		LCD->Buffer[(x+1)][y] |= B16_MSB(FONT_11x18_DATA[((*Char-0x20)*FONT_HIGH)+(z)]);
		z++;
		y++;
		if ((x > (SSD1306_WIDTH/8)))
		{
			x=0;
			y++;
			if ((y >= SSD1306_HEIGHT))
			{
				x=0;
				y=0;
			}
		}

	}while (z < FONT_HIGH);
#endif
}

// ************************************* LCD buffer PUT STRING**********************************
// ************************************* clear LCD buffer **********************************
uint8_t OLEDBuferPutString(OLEDType *LCD, uint8_t x, uint8_t y, uint8_t *String)
{
	uint8_t i=0;

	do {
		if (x + FONT_WIDTH > SSD1306_WIDTH )
		{
			x=0;
			y+=FONT_HIGH;
			if (y > SSD1306_HEIGHT)
			{
				y=0;
			}
		}

		OLEDBuferPutChar(LCD, x, y,(String+i));
		x+= FONT_WIDTH;


	} while (String[i]!= 0);
}


// ************************************************************************************
// ********************* Write many data to the OLED at the same time *****************
// ************************************************************************************
uint8_t OLEDWriteMultiData (OLEDType *LCD, uint8_t *Data, uint16_t maxValue)
{
	uint16_t i=0;
	I2C_GenerateSTART(LCD->I2Cx, ENABLE);
	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(LCD->I2Cx, (uint8_t)(LCD->I2CAddr << 1), I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(LCD->I2Cx, (uint8_t)(0x40));
	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	i=0;

	do {
		I2C_SendData(LCD->I2Cx, Data[i]);
		while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
			i++;
	}while (i<maxValue);
	i=0;
		//while (++i<200);
}

// ************************************************************************************
/// ********************* Write many commands at the same time ************************
// ************************************************************************************

uint8_t OLEDWriteMultiCmd (OLEDType *LCD, uint8_t *Data, uint16_t maxValue)
{
	uint16_t i=0;
	I2C_GenerateSTART(LCD->I2Cx, ENABLE);
	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(LCD->I2Cx, (uint8_t)(LCD->I2CAddr << 1), I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(LCD->I2Cx, (uint8_t)(0x80));
	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	i=0;

	do {
		I2C_SendData(LCD->I2Cx, Data[i]);
		while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
			i++;
	}while (i<maxValue);
	i=0;
		//while (++i<200);
}


// ************************ display text 5x8****************************************
/*
uint8_t OLEDWriteText5x8 (OLEDType *LCD, uint8_t *text)
{
	uint16_t i=0;


	do {
		OLEDWriteMultiData (LCD, &(ssd1306_font_5x8[text[i]*8]), 8 );
		i++;
	} while (text[i] != 0x00);



//	OLEDWriteMultiData (LCD, text, sizeof(text) );


}

*/


uint8_t OLEDWriteText (OLEDType *LCD, uint8_t *text)
{
	uint16_t i=0;



	do {
#ifdef FONT_11X18
		OLEDWriteMultiData (LCD, &(FONT_11x18_DATA[((text[i]-0x20)*18)]), 36 );
#endif
#ifdef FONT_7X10
		OLEDWriteMultiData (LCD, &(FONT_7x10_DATA[((text[i]-0x20)*10)]), 20 );
#endif
#ifdef FONT_5X8
		OLEDWriteMultiData (LCD, &(ssd1306_font_5x8[text[i]*8]), 8 );
#endif
		i++;
	} while (text[i] != 0x00);




}



void DisplayCharXY(OLEDType *LCD, uint8_t *ToDisplay, uint8_t x, uint8_t y)

{
	uint8_t Command_set[]= {SSD1306_COLUMNADDR,0,16,SSD1306_PAGEADDR,0,1, SSD1306_MEMORYMODE_H};
	uint8_t tempx, tempy;
#ifdef FONT_11X18
	tempy = y/8;
	Command_set[1]= x;
	Command_set[2]= x+16;
	Command_set[4]= tempy;
	Command_set[5]= (tempy+18)/8;
	//OLEDWriteMultiCmd(LCD, Command_set,sizeof(Command_set));

	OLEDWriteMultiData (LCD, &(FONT_11x18_DATA[((*ToDisplay-0x20)*18)]), 36 );
#endif

}
// **************************************************************************
// ************** write byte in the I2C device *******************************

uint8_t ResWriteReg (I2C_TypeDef* I2Cx, uint8_t RegAddr, uint16_t value, uint8_t device_ID)
{
	static WriteRegState_t RegdWriteState = I2C_CHECK_BUSY;
	uint16_t timeout = I2C_TIMEOUT_VAL;
	uint8_t FunctionStatus=I2C_WRITE_INWORK;




	switch (RegdWriteState )
	 {
	// ************************ check if I2C is busy *************************
	 	 case I2C_WRITE_CHECK_BUSY:
	 	 {
	 		FunctionStatus=0;
	 		 if (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
	 		 {
	 			RegdWriteState++;
	 		 }
	 		 break;
	 	 }
	 	// ***************** send start marker ************************
	 	 	 case I2C_WRITE_START:
	 	 	 {
	 	 		timeout = I2C_TIMEOUT_VAL;
	 	 		 // send start bit for I2C
	 	 		I2C_GenerateSTART(I2Cx, ENABLE);
	 	 	    /* Test on EV5 and clear it */
	 	 	    do{
	 	 	    	if (timeout>0)
	 	 	    	{
	 	 	    		timeout--;

	 	 	    	}
	 	 	    	else{
	 	 	    		FunctionStatus = I2C_WRITE_INWORK;
	 	 	    		break;
	 	 	    	}
	 	 	    }
	 	 		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

	 	 		//if (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
	 	 		{
	 	 			RegdWriteState++;
	 	 		}
	 	 		//break;
	 	 	 }
	 	 // ****************** Send device address *************************
	 	  	case I2C_WRITE_SEND_ADDR:
	 	  	{
 	 	 		if (FunctionStatus == I2C_WRITE_INWORK)
 	 	 		{
 	 	 			I2C_Send7bitAddress(I2Cx, (uint8_t)(device_ID << 1), I2C_Direction_Transmitter);
 	 	 			timeout = I2C_TIMEOUT_VAL;
 	 	 	   /* Test on EV6 and clear it */
 	 	 			do{
 	 	 				if (timeout>0)
 	 	 				{
 	 	 					timeout--;

 	 	 				}
 	 	 				else
 	 	 				{
 	 	 					FunctionStatus = I2C_WRITE_ERROR;
 	 	 					break;
 	 	 				}
 	 	 				}
 	 	 			while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
 	 	 		}
 	 	 		//if (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
 	 	 	 		{
 	 	 			RegdWriteState++;
 	 	 	 		}
 	 	 	 //	break;


	 	  	}


	 	 	// ***************** send register addres inside device ******************************
	 	 	case I2C_WRITE_SEND_REG:
	 	 	{
 	 	 		if (FunctionStatus == I2C_WRITE_INWORK)
 	 	 		{
	 	 		 I2C_SendData(I2Cx, (uint8_t)(RegAddr | ((uint8_t) ((value & 0x300)>>8))));
	 	 		 timeout = I2C_TIMEOUT_VAL;
	 	 		 /* Test on EV8 and clear it */
		 	 	    do{
		 	 	    	if (timeout>0)
		 	 	    	{
		 	 	    		timeout--;

		 	 	    	}
		 	 	    	else{
		 	 	    		FunctionStatus = I2C_WRITE_ERROR;
		 	 	    		break;
		 	 	    	}
		 	 	    }
		 	 	    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
 	 	 		}
	 	 		 //if (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	 	 	 		{
	 	 			RegdWriteState++;
	 	 	 		}
	 	 	 //	break;
	 	 	 }
	 	 	case I2C_WRITE_DATA:
	 	 	{
 	 	 		if (FunctionStatus == I2C_WRITE_INWORK)
 	 	 		{
	 	 		 I2C_SendData(I2Cx, (uint8_t)(0xFF&value));
	 	 		 timeout = I2C_TIMEOUT_VAL;
	 	 		 /* Test on EV8 and clear it */
		 	 	    do{
		 	 	    	if (timeout>0)
		 	 	    	{
		 	 	    		timeout--;

		 	 	    	}
		 	 	    	else{
		 	 	    		FunctionStatus = I2C_WRITE_ERROR;
		 	 	    		break;
		 	 	    	}
		 	 	    }
	 	 		 while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

 	 	 		}
	 	 	    RegdWriteState++;
	 	 	 //	break;
	 	 	 }
	 	 	case I2C_WRITE_STOP:
	 	 	{
		 	 	/* Send STOP condition */
		 	    I2C_GenerateSTOP(I2Cx, ENABLE);
 	 			RegdWriteState++;
	 	 	 }
	 	 	default:
	 	 	{
	 	 		RegdWriteState = I2C_CHECK_BUSY;
		 	    if (FunctionStatus == I2C_WRITE_INWORK)
		 	    {
		 	    	FunctionStatus=I2C_WRITE_OK;
		 	    }


	 	 	}



	 }
return FunctionStatus;
}

/// ********* Hardware init function *************

void OLEDDisplayInit(OLEDType *LCD)
{
	int i=0;
	LCD->I2Cx = LCD_I2C;
	LCD->I2CAddr = LCD_I2C_ADDR;

	//ResWriteReg (LCD->I2Cx, uint8_t RegAddr, uint16_t value, uint8_t device_ID)
	I2C_GenerateSTART(LCD->I2Cx, ENABLE);
	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(LCD->I2Cx, (uint8_t)(LCD->I2CAddr << 1), I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(LCD->I2Cx, (uint8_t)(0x80));
	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	i=0;

	do {
		I2C_SendData(LCD->I2Cx, Init_Table[i]);
		while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
			i++;
	}while (i < sizeof(Init_Table));


	i=0;


}

void OLEDDisplayCleare(OLEDType *LCD)
{
	// define LCD range
	uint16_t i=0;
	uint8_t command_buffer[]={SSD1306_COLUMNADDR,0,127,SSD1306_PAGEADDR,0,3, SSD1306_SETPAGESTARTADDRESS,SSD1306_SETSTARTLINE, SSD1306_MEMORYMODE_H};

#ifdef FONT_11X18
//	OLEDWriteMultiCmd(LCD, command_buffer,sizeof(command_buffer));

#endif




	/*OLED_WRITE_CMD(LCD, SSD1306_SETPAGESTARTADDRESS);
	OLED_WRITE_CMD(LCD, 0);
	OLED_WRITE_CMD(LCD, 0x10);
*/








	I2C_GenerateSTART(LCD->I2Cx, ENABLE);
	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(LCD->I2Cx, (uint8_t)(LCD->I2CAddr << 1), I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(LCD->I2Cx, (uint8_t)(0x40));
	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	i=0;

	do {
		I2C_SendData(LCD->I2Cx, 0);
		while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
			i++;
			if (i ==128)
			{
				asm ("NOP");
			}
			if (i==(128*2))
			{
			asm ("NOP");
			}
			if (i==(128*3))
			{
				asm ("NOP");
			}
 	}while (i<SSD1306_FILL_BORDER);
	i=0;
	//while (++i<200);
}


void OLEDDisplayFill(OLEDType *LCD)
{
	uint16_t i=0;
	uint8_t command_buffer[8];
/*	command_buffer[0] = SSD1306_COLUMNADDR;
	command_buffer[1] = 0;
	command_buffer[2] = 127;

	OLEDWriteMultiCmd (LCD,command_buffer, 3);

	command_buffer[0] = SSD1306_PAGEADDR;
	command_buffer[2] = 3;

	OLEDWriteMultiCmd (LCD,command_buffer, 3);
*/
	I2C_GenerateSTART(LCD->I2Cx, ENABLE);
	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(LCD->I2Cx, (uint8_t)(LCD->I2CAddr << 1), I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(LCD->I2Cx, (uint8_t)(0x40));
	while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	i=0;

	do {
		I2C_SendData(LCD->I2Cx, 0xff);
		while(!I2C_CheckEvent(LCD->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
			i++;
	}while (i<SSD1306_FILL_BORDER);

	/*I2C_GenerateSTOP(LCD->I2Cx, ENABLE);
	i=0;
while (++i<200);*/
}
/// ********* Hardware init function *************

void OLEDSoftwareInit(OLEDType *LCD)
{

	LCD->I2Cx = LCD_I2C;
	LCD->I2CAddr = LCD_I2C_ADDR;
	LCD->sreg |= OLED_TO_BE_INIT;
#ifdef FONT_11X18
	LCD->FontDataPointer = FONT_11x18_DATA;
#endif
#ifdef FONT_5X8
	LCD->FontDataPointer = ssd1306_font_5x8;
#endif


	LCD->FontWidth = FONT_WIDTH;
	LCD->FontHeight = FONT_HIGH;



}

/// ********* Hardware init function *************

void OLEDHardwareInit(void)
{

	GPIO_InitTypeDef GPIOInitStructure;
	I2C_InitTypeDef I2CInitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/// clock activation for PORTB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	// Enable clock for alternative function
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);


#ifdef I2C1_ENABLE
	#ifdef STM32F103C8
	// ************ pin I/O configuration as alternative function ************
	GPIOInitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_6;//GPIO_Pin_10;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIOInitStructure);
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_7;//GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIOInitStructure);
	#endif
#endif


#ifdef I2C2_ENABLE
	#ifdef STM32F103C8

	// ************ pin I/O configuration as alternative function ************
	GPIOInitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIOInitStructure);
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIOInitStructure);

	#endif
#endif




	LCD_RCC_I2C_ENABLE;
	// ********************** I2C2 reset *********************
	LCD_I2C->CR1 |= 0x8000;
	LCD_I2C->CR1 &= (uint16_t) ~((uint16_t) 0x8000);


	// ************* I2C 2 init *******************************
	I2CInitStructure.I2C_Ack = I2C_Ack_Enable;				// -> wysylam tylko jeden bajt danych		I2C_Ack_Enable;
	I2CInitStructure.I2C_Mode = I2C_Mode_I2C;
	I2CInitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2CInitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2CInitStructure.I2C_ClockSpeed = 400000;//I2C_Speed;
    I2CInitStructure.I2C_OwnAddress1 = LCD_I2C_ADDR;		// adres odbiorcy komend.



    I2C_Cmd(LCD_I2C, ENABLE);

    /* Apply I2C configuration after enabling it */
    I2C_Init(LCD_I2C, &I2CInitStructure);


}
