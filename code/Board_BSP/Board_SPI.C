/*
//SPI�ļ� ������6500��SPI ͨ��SPI��ȡ6500���ݵķ�����
//SJ 2018.1.18
*/ 
#include "Board_SPI.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
/*
 * ��������SPI1_Init
 * ����  ��SPI1��ʼ��
 * ����  ����
 * ���  ����
 */ 
 /* Statics */

 //![Simple SPI Config]
/* SPI Master Configuration Parameter */
const eUSCI_SPI_MasterConfig spiMasterConfig =
{
        EUSCI_B_SPI_CLOCKSOURCE_SMCLK,             // SMCLK Clock Source
        12000000,                                   // SMCLK = DCO = 12MHZ
        1000000,                                    // SPICLK = 500khz
        EUSCI_B_SPI_MSB_FIRST,                     // MSB First
        EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, // HIGH polarity
        EUSCI_B_SPI_3PIN                           // 3Wire SPI Mode
};
//![Simple SPI Config]
void SPI1_Init(void)
{
	 GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P10,
            GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
	
	 GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
										 
									
	 MPU6500_CS(1); 			 																											 
	 									
	
	/* Configuring SPI in 3wire master mode */
  SPI_initMaster(EUSCI_B3_BASE, &spiMasterConfig);			
 /* Enable SPI module */
  SPI_enableModule(EUSCI_B3_BASE);	
	//SPI_enableInterrupt(EUSCI_B0_BASE, EUSCI_B_SPI_RECEIVE_INTERRUPT);
  //Interrupt_enableInterrupt(INT_EUSCIB0);	

}
/*
 * ��������MPU6500_Write_Reg
 * ����  ������SPIд��Ĵ��� Ϊ��6500����ָ���װ�ӿ�
 * ����  ��reg:ָ���ļĴ�����ַ��value��д���ֵ
 * ���  ��status������״ֵ̬
 */ 
void MPU6500_Write_Reg(uint8_t reg,uint8_t value)
{
	MPU6500_CS(0);  										//ʹ��6500Ƭѡ
	SPI1_Read_Write_Byte(reg);           //����д����+�Ĵ�����
	SPI1_Read_Write_Byte(value);				  //д��Ĵ���ֵ
	MPU6500_CS(1);  										//��ֹMPU9500
}
/*
 * ��������MPU6500_Read_Reg
 * ����  ������SPI��ȡ�Ĵ��� Ϊ6500��ȡ���ݷ�װ�ӿ�
 * ����  ��reg:ָ���ļĴ�����ַ
 * ���  ��reg_val��reg�Ĵ�����ַ��Ӧ��ֵ
 */ 
uint8_t MPU6500_Read_Reg(uint8_t reg)
{
	uint8_t reg_val=0;
	reg = reg | 0x80;
	MPU6500_CS(0);  										//ʹ��6500Ƭѡ
	SPI1_Read_Write_Byte(reg); 	      //���Ͷ�����+�Ĵ�����
	reg_val=SPI1_Read_Write_Byte(0xff); //��ȡ�Ĵ���ֵ
	MPU6500_CS(1);  									  //��ֹMPU9250
	return(reg_val);
}
/*
 * ��������SPI1_Read_Write_Byte
 * ����  ��SPI������д���� ��дһ���ֽ�
 * ����  ��TxData:Ҫд����ֽ�
 * ���  ����ȡ�����ֽ�
 */ 
uint8_t SPI1_Read_Write_Byte(uint8_t TxData)
{				 	
	uint32_t StartTick=RTE_RoundRobin_GetTick();
	while (!(EUSCI_B3->IFG & EUSCI_B_IFG_TXIFG))
	{
		if(RTE_RoundRobin_GetTick()-StartTick>=500)
			return 0;
	}//���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	SPI_transmitData(EUSCI_B3_BASE, TxData); 																//ͨ������SPIx����һ������
	StartTick=RTE_RoundRobin_GetTick();
	while (!(SPI_getInterruptStatus(EUSCI_B3_BASE, EUSCI_B_SPI_RECEIVE_INTERRUPT)))
	{
		if(RTE_RoundRobin_GetTick()-StartTick>=500)
			return 0;
	} //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ		
	return EUSCI_B3->RXBUF;			
//		static volatile uint8_t RXData = 0;	//����ͨ��SPIx������յ�����					    
//   while (!(SPI_getInterruptStatus(EUSCI_B0_BASE, EUSCI_B_SPI_TRANSMIT_INTERRUPT)));
//		SPI_transmitData(EUSCI_B0_BASE, TxData);
//	 RXData = SPI_receiveData(EUSCI_B0_BASE);
//	 
//	 return RXData;
}

