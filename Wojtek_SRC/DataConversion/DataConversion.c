

#ifdef STM32F10X_MD
 #include "stm32f10x.h"
#endif

void HEX2STR(unsigned char data, unsigned char *string)
{

 string[0]= data & 0xF0;
 string[0] >>=4;

 if (string[0]<0x0A)
	 string[0]+=0x30;
 else
	 string[0]+=0x37;

 string[1]=data & 0x0F;

 if (string[1]<0x0A)
	 string[1] +=0x30;
 else
	 string[1] +=0x37;

 string[2]=0;

}



/* int to string conversion */
void INT_dec_2STR(unsigned int data, unsigned char *string)
{


 string[0]= data/10000;

 if(string[0]<0x0A  )
 	 {
	 string[0]+=0x30;
 	 }
 else
	 {
	 string[0]+=0x37;
	 }

 string[1]= (data%10000)/1000;

 if (string[1]<0x0A)
	 {
	 string[1]+=0x30;
	 }
 else
	 {
	 string[1]+=0x37;
	 }

 string[2]= (data%1000)/100;

 if(string[2]<0x0A  )
 	 {
	 string[2]+=0x30;
 	 }
  else
 	 {
	  string[2]+=0x37;
 	 }

 string[3]= (data%100)/10;

 if (string[3]<0x0A)
 	 {
	 string[3]+=0x30;
 	 }
  else
 	 {
	  string[3]+=0x37;
 	 }

 string[4]= (data%10);

 if (string[4]<0x0A)
 	 {
	 string[4]+=0x30;
 	 }
  else
 	 {
	  string[4]+=0x37;
 	 }
 string[5]=0;


}



/****change decimal 8 bit to string ASCII ********/
uint8_t U8_DEC_TO_STRING(uint8_t value, uint8_t *string)
{
	uint8_t i;
	uint32_t mnoznik=100;
	i=0;
	if (value > 0)
	{
	string[i]= value/mnoznik;


	if (string[i]> 0)
		{
			string[i]+=0x30;
			i++;

		}

	do
	{
		string[i] = (value%mnoznik)/(mnoznik/10);
		if ((i>0) || (string[i]>0))
		{
			string[i]+=0x30;
			i++;
		}

		mnoznik=mnoznik/10;

	}
	while (mnoznik > 10);

	string[i] = value%mnoznik;
	string[i]+=0x30;
	i++;
	string[i]=0;

	}
	else
	{
		string[0]= 0x30;
		string[1]= 0x00;
		i=1;
	}
	return i;
}


/****change decimal 16 bit to string ASCII ********/

uint8_t U16_DEC_TO_STRING(uint16_t value, uint8_t *string)
{
	uint8_t i;
	uint32_t mnoznik=10000;
	i=0;
	if (value > 0)
	{

		string[i]= value/mnoznik;


		if (string[i]> 0)
		{
			i++;
			string[i]+=0x30;
		}

		do
		{
			string[i] = (value%mnoznik)/(mnoznik/10);
			if ((i>0) || (string[i]>0))
			{
				string[i]+=0x30;
				i++;
			}

			mnoznik=mnoznik/10;

		}
		while (mnoznik > 10);

		string[i] = value%mnoznik;
		string[i]+=0x30;
		i++;
		string[i]=0;
	}
	else
	{
		string[0]=0x30;
		string[1]=0x00;
		i=1;
	}
	return i;

}

/****change decimal 16 bit to string ASCII ********/

uint8_t FLOAT_DEC_TO_STRING(float value, uint8_t *string)
{
	uint8_t i;
	float temp_value;
	float mnoznik=1000.0;

	i=0;

	if (value < 0.0)
	{
		string[i]='-';
		i++;
		temp_value = value * (-1);
	}
	else if (value == 0.0)
	{
		string[0]=0x30;
		string[1]='.';
		string[2]=0x30;
		string[3]=0x00;
		i=3;
		return i;
	}
	else
	{
		temp_value = value;
	}

	string[i]=(uint8_t) temp_value/mnoznik;
//////////////////

		if (string[i]> 0)
		{
			i++;
			string[i]+=0x30;
		}

		do
		{
			string[i] =(uint8_t) ((temp_value/mnoznik));
			if ((i>0) &&(string[0]!='-') || (string[i]>0))
			{
				temp_value = temp_value - (string[i] * mnoznik);
				string[i]+=0x30;
				i++;
			}

			mnoznik=mnoznik/10.0;

		}
		while (mnoznik >= 1);
		if (i==0)
		{
			string[0]=0x30;
			i++;
		}
		else if (string[0] == '-' && i==1)
		{
			string[i]=0x30;
			i++;
		}

		string[i]='.';
		i++;
		do
		{
			string[i] =(uint8_t) ((temp_value/mnoznik));
			if ((i>0) || (string[i]>0))
			{
				temp_value = temp_value - (string[i] * mnoznik);
				string[i]+=0x30;
				i++;
			}

			mnoznik=mnoznik/10.0;

		}
		while (mnoznik > 0.01);

		string[i] = temp_value/mnoznik;
		string[i]+=0x30;
		i++;
		string[i]=0;


	return i;

}


 // **********************************
 // 	convert string to u64
 // **********************************


/**************** Decrease_table ********************/

void Decrease_table(uint8_t *table,uint8_t count )
{
 uint8_t i;
	for (i=0; i<(count-1); i++)
	{
		table[i]=table[i+1];
	}
	table[count-1]=0;
}

// ********************************************************
// ************* oblicznie wartosci sredniej **************
// ********************************************************
void AVERAGE_VALUE_16(uint16_t value, uint16_t *average)
{
	uint32_t temp32;
	temp32 = value + *average;
	temp32 = temp32/2;

	if (temp32 > (uint32_t)(*average/2))
		{
			*average = value;
		}
	else
		{
			*average = (uint16_t)temp32;
		}

}
