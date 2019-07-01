#include "board_mpu.h"
#include "BSP_Systick.h"
/*��ͨ�˲�����*/
#define GYRO_LPF_CUTOFF_FREQ  50
#define ACCEL_LPF_CUTOFF_FREQ 30
#define MAG_LPF_CUTOFF_FREQ 30

lpf2pData accLpf[3];
lpf2pData gyroLpf[3];
lpf2pData magLpf[3];

bool MPU6500Init(void)
{
	RTE_Printf("%d\r\n",MPU6500_Read_Reg(WHO_AM_I));
	RTE_RoundRobin_DelayMS(100);
	MPU6500_Write_Reg(PWR_MGMT_1,0X80);   		//��Դ����,��λMPU6500
	RTE_RoundRobin_DelayMS(100);
	if(MPU6500_Read_Reg(WHO_AM_I) == 112)	//��ȷ��ȡ��6500��ID
	{		
		RTE_Printf("hello mpu6500\r\n");
		MPU6500_Write_Reg(PWR_MGMT_1,0X80);   		//��Դ����,��λMPU6500
		RTE_RoundRobin_DelayMS(100);
		MPU6500_Write_Reg(SIGNAL_PATH_RESET,0X07);//�����ǡ����ٶȼơ��¶ȼƸ�λ
		RTE_RoundRobin_DelayMS(100);
		MPU6500_Write_Reg(PWR_MGMT_1,0X01);   //ѡ��ʱ��Դ
		MPU6500_Write_Reg(PWR_MGMT_2,0X00);   //ʹ�ܼ��ٶȼƺ�������
		MPU6500_Write_Reg(CONFIG,0X03);				//��ͨ�˲��� 0x03 41hz MPU6500_ACCEL_DLPF_BW_41
		MPU6500_Write_Reg(SMPLRT_DIV,0X00);		//������1000/(1+0)=1000HZ
		MPU6500_Write_Reg(GYRO_CONFIG,0X18);  //�����ǲ�����Χ 0X18 ����2000�� 
		MPU6500_Write_Reg(ACCEL_CONFIG,0x10); //���ٶȼƲ�����Χ 0X10 ����8g
		MPU6500_Write_Reg(ACCEL_CONFIG2,0x02);//��ͨ�˲���98hz MPU6500_DLPF_BW_98
		
		for (uint8_t i = 0; i < 3; i++)// ��ʼ�����ټƺ����ݶ��׵�ͨ�˲�
		{
			lpf2pInit(&gyroLpf[i], 1000, GYRO_LPF_CUTOFF_FREQ);
			lpf2pInit(&accLpf[i],  1000, ACCEL_LPF_CUTOFF_FREQ);
			lpf2pInit(&magLpf[i],  1000, MAG_LPF_CUTOFF_FREQ);
		}
		return true;
	}
	else 
		return false;
}
