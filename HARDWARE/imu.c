	/**
  ****************************(C) COPYRIGHT 2021 SHUIMUJIEMING****************************
  * @file       imu.c/h
  * @brief      陀螺仪初始化以及角度计算等函数
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     7-10-2021     shuimujieming              1. 完成
  *
  ****************************(C) COPYRIGHT 2021 SHUIMUJIEMING****************************
  */
#include "imu.h"

#define Kp 2.0f     //陀螺仪角度计算比例                                
	
#define Ki 0.01f    //陀螺仪角度计算积分


//#define IST8310  //定义磁力传感器是否使用

#define BOARD_DOWN 0 //板子是否朝下

//四元数初始化定义，使用volatile定义其为易变内存
volatile float        q0 = 1.0f;
volatile float        q1 = 0.0f;
volatile float        q2 = 0.0f;
volatile float        q3 = 0.0f;

volatile float        exInt, eyInt, ezInt;                   /* error integral */

static volatile float gx, gy, gz, ax, ay, az, mx, my, mz;  
volatile uint32_t     last_update=0, now_update=0;               /* Sampling cycle count, ubit ms */
static uint8_t        tx, rx;
static uint8_t        tx_buff[14] = { 0xff };
uint8_t               mpu_buff[14];                          /* buffer to save imu raw data */
uint8_t               ist_buff[6];                           /* buffer to save IST8310 raw data */
mpu_data_t            mpu_data;
imu_t                 imu={0};



/*
*@title：MPU写入一个字节
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/

uint8_t MPU_Write_Byte(uint8_t const reg,uint8_t const data)
{
	MPU_NSS_LOW;
	SPI_ReadWriteByte(reg&0x7f);//0x 0111 1111
	SPI_ReadWriteByte(data);
	MPU_NSS_HIGH;
	return 0;
}
/*
*@title：MPU读取一个字节
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
uint8_t MPU_Read_Byte(uint8_t const reg)
{
	uint8_t MPU_Rx;
	MPU_NSS_LOW;
	SPI_ReadWriteByte(reg|0x80);
	MPU_Rx=SPI_ReadWriteByte(0xff);//此处与官方不同，注意！
	MPU_NSS_HIGH;
	
	return MPU_Rx;
}

/*
*@title：MPU读取多个字节
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
uint8_t MPU_Read_Bytes(uint8_t const regAddr,uint8_t *pData,uint8_t len)
{
	int delay_count;
	MPU_NSS_LOW;
	for(int i=0;i<len;i++)
	{
		*pData = MPU_Read_Byte(regAddr+i);
		pData++;
		delay_count=10;
		while(delay_count--);
	}
	return 0;
	
}
/*
*@title：磁力计写入一个字节
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void IST8310_Write_By_MPU(uint8_t addr,uint8_t data)
{
	MPU_Write_Byte(MPU6500_I2C_SLV1_CTRL,0X00);
	delay_ms(2);
	MPU_Write_Byte(MPU6500_I2C_SLV1_REG,addr);
	delay_ms(2);
	MPU_Write_Byte(MPU6500_I2C_SLV1_DO,data);
	delay_ms(2);
	
	MPU_Write_Byte(MPU6500_I2C_SLV1_CTRL,0x80|0x01);
	delay_ms(10);
}
/*
*@title：磁力计读取一个字节
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/

uint8_t IST8310_Read_By_MPU(uint8_t addr)
{
	uint8_t returnval;
	MPU_Write_Byte(MPU6500_I2C_SLV4_REG,addr);
	delay_ms(10);
	MPU_Write_Byte(MPU6500_I2C_SLV4_CTRL,0x80);
	delay_ms(10);
	returnval=MPU_Read_Byte(MPU6500_I2C_SLV4_DI);
	
	MPU_Write_Byte(MPU6500_I2C_SLV4_CTRL,0x00);
	delay_ms(10);
	return returnval;
}
/*
*@title：
*@description：获取磁力计的数据
*@param 1：	
*@param 2：	
*@return:：	
*/
void IST8310_Get_Data(uint8_t *buff)
{
	MPU_Read_Bytes(MPU6500_EXT_SENS_DATA_00,buff,6);
}

/*
*@title：MPU获取偏移值
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void MPU_Offset_Cali()
{
	for(int i=0;i<300;i++)
	{
		MPU_Read_Bytes(MPU6500_ACCEL_XOUT_H,mpu_buff,14);
		mpu_data.ax_offset += mpu_buff[0] << 8 | mpu_buff[1];
		mpu_data.ay_offset += mpu_buff[2] << 8 | mpu_buff[3];
		mpu_data.az_offset += mpu_buff[4] << 8 | mpu_buff[5];
	
		mpu_data.gx_offset += mpu_buff[8]  << 8 | mpu_buff[9];
		mpu_data.gy_offset += mpu_buff[10] << 8 | mpu_buff[11];
		mpu_data.gz_offset += mpu_buff[12] << 8 | mpu_buff[13];
	
		delay_xms(5);
	}
	
	mpu_data.ax_offset=mpu_data.ax_offset / 300;
	mpu_data.ay_offset=mpu_data.ay_offset / 300;
	mpu_data.az_offset=mpu_data.az_offset / 300;
	mpu_data.gx_offset=mpu_data.gx_offset / 300;
	mpu_data.gy_offset=mpu_data.gy_offset / 300;
	mpu_data.gz_offset=mpu_data.gz_offset / 300;
}


/*
*@title：IMU数据获取
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void IMU_Get_Data()
{
	
	MPU_Read_Bytes(MPU6500_ACCEL_XOUT_H,mpu_buff,14);
	
	//MPU6500数据读入
	mpu_data.ax   = mpu_buff[0] << 8 | mpu_buff[1];
  mpu_data.ay   = mpu_buff[2] << 8 | mpu_buff[3];
  mpu_data.az   = mpu_buff[4] << 8 | mpu_buff[5];
	
  mpu_data.temp = mpu_buff[6] << 8 | mpu_buff[7];
	
  mpu_data.gx = ((mpu_buff[8]  << 8 | mpu_buff[9])  - mpu_data.gx_offset);
  mpu_data.gy = ((mpu_buff[10] << 8 | mpu_buff[11]) - mpu_data.gy_offset);
  mpu_data.gz = ((mpu_buff[12] << 8 | mpu_buff[13]) - mpu_data.gz_offset);
	
	if(abs(mpu_data.gz )< 3)
	{
		mpu_data.gz = 0;
	}
	
	memcpy(&imu.ax,&mpu_data.ax,6*sizeof(int16_t));
	
	imu.temp=21+mpu_data.temp/333.87f;
	
	/* 2000dps -> rad/s */
	
	imu.wx   = mpu_data.gx / 16.384f / 57.3f; 
  imu.wy   = mpu_data.gy / 16.384f / 57.3f; 
  imu.wz   = mpu_data.gz / 16.384f / 57.3f;

}


/*
*@title：
*@description：I2C初始化
*@param 1：	
*@param 2：	
*@return:：	
*/
void MPU_MASTER_I2C_AUTO_READ_CONFIG(uint8_t device_address,uint8_t reg_base_addr,uint8_t data_num)
{
	MPU_Write_Byte(MPU6500_I2C_SLV1_ADDR,device_address);
	delay_ms(2);
	MPU_Write_Byte(MPU6500_I2C_SLV1_REG,IST8310_R_CONFA);
	delay_ms(2);
	MPU_Write_Byte(MPU6500_I2C_SLV1_DO,IST8310_ODR_MODE);
	delay_ms(2);
	
	
	MPU_Write_Byte(MPU6500_I2C_SLV0_ADDR,0x80|device_address);
	delay_ms(2);
	MPU_Write_Byte(MPU6500_I2C_SLV0_REG,reg_base_addr);
	delay_ms(2);
	
	MPU_Write_Byte(MPU6500_I2C_SLV4_CTRL,0x03);
	delay_ms(2);
	
	MPU_Write_Byte(MPU6500_I2C_MST_DELAY_CTRL,0x01|0x02);
	delay_ms(2);
	
	MPU_Write_Byte(MPU6500_I2C_SLV1_CTRL,0x80|0x01);
	
	delay_ms(6);
	
	MPU_Write_Byte(MPU6500_I2C_SLV0_CTRL,0x80|data_num);
	delay_ms(2);
	
}
/*
*@title：初始化磁力计
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
uint8_t IST8310_Init()
{
	MPU_Write_Byte(MPU6500_USER_CTRL,0x30);
	delay_ms(10);
	
	MPU_Write_Byte(MPU6500_I2C_MST_CTRL,0x0d);
	delay_ms(10);
	
	MPU_Write_Byte(MPU6500_I2C_SLV1_ADDR,IST8310_ADDRESS);
	delay_ms(10);
	MPU_Write_Byte(MPU6500_I2C_SLV4_ADDR,0x80|IST8310_ADDRESS);
	delay_ms(10);
	
	IST8310_Write_By_MPU(IST8310_R_CONFB,0X01);
	delay_ms(10);
	if(IST8310_DEVICE_ID_A!=IST8310_Read_By_MPU(IST8310_WHO_AM_I))
		return 1;
	
	IST8310_Write_By_MPU(IST8310_R_CONFB,0x01);
	delay_ms(10);
	
	IST8310_Write_By_MPU(IST8310_R_CONFA,0x00);
	if(IST8310_Read_By_MPU(IST8310_R_CONFA)!=0x00)
		return 2;
	delay_ms(10);
	
	IST8310_Write_By_MPU(IST8310_R_CONFB,0x00);
	if(IST8310_Read_By_MPU(IST8310_R_CONFB!=0x00))
		return 3;
	delay_ms(10);
	
	IST8310_Write_By_MPU(IST8310_AVGCNTL,0x24);
	if(IST8310_Read_By_MPU(IST8310_AVGCNTL)!=0x24)
		return 4;
	delay_ms(10);
	
	IST8310_Write_By_MPU(IST8310_PDCNTL,0xc0);
	if(IST8310_Read_By_MPU(IST8310_PDCNTL)!=0xc0)
		return 5;
	
	MPU_Write_Byte(MPU6500_I2C_SLV1_CTRL,0x00);
	delay_ms(10);
	MPU_Write_Byte(MPU6500_I2C_SLV4_CTRL,0x00);
	delay_ms(10);
	
	MPU_MASTER_I2C_AUTO_READ_CONFIG(IST8310_ADDRESS,IST8310_R_XL,0x06);
	delay_ms(100);
	return 0;
	
}
/*
*@title：设置陀螺仪角速度
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
uint8_t MPU_SET_GYRO_FSR(uint8_t fsr)
{
	return MPU_Write_Byte(MPU6500_GYRO_CONFIG,fsr<<3);
}
/*
*@title：设置陀螺仪加速度
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
uint8_t MPU_SET_ACCEL_FSR(uint8_t fsr)
{
	return MPU_Write_Byte(MPU6500_ACCEL_CONFIG,fsr<<3);
}

/*
*@title：初始化MPU
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
uint8_t MPU_Device_Init()
{
	
	//获取陀螺仪对应ID
	while(MPU_Read_Byte(MPU6500_WHO_AM_I)!=MPU6500_ID)
	{
		LED_RED_ON;
	}
	LED_RED_ON;
	LED_GREEN_ON;//绿灯亮
	
	//陀螺仪配置项
	uint8_t MPU_Init_Data[8][2]={
		{MPU6500_PWR_MGMT_1,0X80},
		{MPU6500_PWR_MGMT_1,0x03},  
		{MPU6500_PWR_MGMT_2,0x00},
		{MPU6500_CONFIG,0x04} , 
		{MPU6500_GYRO_CONFIG,0x18},  
		{MPU6500_ACCEL_CONFIG,0x10},
		{MPU6500_ACCEL_CONFIG_2,0x02}, 
		{MPU6500_USER_CTRL,0x20}
	};
	//配置陀螺仪选项
	for(int i=0;i<8;i++)
	{
		MPU_Write_Byte(MPU_Init_Data[i][0],MPU_Init_Data[i][1]);
		delay_ms(1);
	}
	
//	IST8310_Init();//磁力计初始化
	
	MPU_Offset_Cali();//偏移值初始化
	
	return 0;
}
/*
*@title：四元数初始化
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void Quaternion_Init()
{
	int16_t hx,hy;
	
	hx=imu.mx;
	hy=imu.my;
	
	//判断板子是否朝下
	#ifdef BOARD_DOWN
	if (hx < 0 && hy < 0) 
	{
		if (abs(hx / hy) >= 1)
		{
			q0 = -0.005;
			q1 = -0.199;
			q2 = 0.979;
			q3 = -0.0089;
		}
		else
		{
			q0 = -0.008;
			q1 = -0.555;
			q2 = 0.83;
			q3 = -0.002;
		}
		
	}
	else if (hx < 0 && hy > 0)
	{
		if (abs(hx / hy)>=1)   
		{
			q0 = 0.005;
			q1 = -0.199;
			q2 = -0.978;
			q3 = 0.012;
		}
		else
		{
			q0 = 0.005;
			q1 = -0.553;
			q2 = -0.83;
			q3 = -0.0023;
		}
		
	}
	else if (hx > 0 && hy > 0)
	{
		if (abs(hx / hy) >= 1)
		{
			q0 = 0.0012;
			q1 = -0.978;
			q2 = -0.199;
			q3 = -0.005;
		}
		else
		{
			q0 = 0.0023;
			q1 = -0.83;
			q2 = -0.553;
			q3 = 0.0023;
		}
		
	}
	else if (hx > 0 && hy < 0)
	{
		if (abs(hx / hy) >= 1)
		{
			q0 = 0.0025;
			q1 = 0.978;
			q2 = -0.199;
			q3 = 0.008;			
		}
		else
		{
			q0 = 0.0025;
			q1 = 0.83;
			q2 = -0.56;
			q3 = 0.0045;
		}		
	}
	#else
		if (hx < 0 && hy < 0)
	{
		if (abs(hx / hy) >= 1)
		{
			q0 = 0.195;
			q1 = -0.015;
			q2 = 0.0043;
			q3 = 0.979;
		}
		else
		{
			q0 = 0.555;
			q1 = -0.015;
			q2 = 0.006;
			q3 = 0.829;
		}
		
	}
	else if (hx < 0 && hy > 0)
	{
		if(abs(hx / hy) >= 1)
		{
			q0 = -0.193;
			q1 = -0.009;
			q2 = -0.006;
			q3 = 0.979;
		}
		else
		{
			q0 = -0.552;
			q1 = -0.0048;
			q2 = -0.0115;
			q3 = 0.8313;
		}
		
	}
	else if (hx > 0 && hy > 0)
	{
		if(abs(hx / hy) >= 1)
		{
			q0 = -0.9785;
			q1 = 0.008;
			q2 = -0.02;
			q3 = 0.195;
		}
		else
		{
			q0 = -0.9828;
			q1 = 0.002;
			q2 = -0.0167;
			q3 = 0.5557;
		}
		
	}
	else if (hx > 0 && hy < 0)
	{
		if(abs(hx / hy) >= 1)
		{
			q0 = -0.979;
			q1 = 0.0116;
			q2 = -0.0167;
			q3 = -0.195;			
		}
		else
		{
			q0 = -0.83;
			q1 = 0.014;
			q2 = -0.012;
			q3 = -0.556;
		}		
	}
	#endif
}
/*shuimujieming
*@title：求解平方根倒数
*@description：1/Sqrt(x)
*@param 1：	
*@param 2：	
*@return:：	
*/

float inv_sqrt(float x)
{
    float xhalf = 0.5f*x;		 // get half of x
    int i = *(int*)&x;       // get bits for floating value 此处使用的数据类型位int，而源程序为long。注意！
    i = 0x5f3759df - (i>>1); // gives initial guess y0
    x = *(float*)&i;         // convert bits back to float
    x = x*(1.5f-xhalf*x*x);  // Newton step, repeating increases accuracy
	
    return x;
}
	
/*
*@title：IMU姿态更新
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void IMU_Ahrs_Update()
{
	float norm;//归一化数
	float hx, hy, hz, bx, bz;
	float vx, vy, vz, wx, wy, wz;
	float ex, ey, ez, halfT;
	float tempq0,tempq1,tempq2,tempq3;

	//四元数乘积
	float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
	float q0q3 = q0*q3;
	float q1q1 = q1*q1;
	float q1q2 = q1*q2;
	float q1q3 = q1*q3;
	float q2q2 = q2*q2;   
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;   

	//读取陀螺仪9轴数据
	gx = imu.wx;
	gy = imu.wy;
	gz = imu.wz;
	ax = imu.ax;
	ay = imu.ay;
	az = imu.az;
	mx = imu.mx;
	my = imu.my;
	mz = imu.mz;

	//时间获取，用于积分
	now_update  = imu_tick_counts;//1ms
	halfT       = ((float)(now_update - last_update) / 2000.0f);
	last_update = now_update;
	
	
	//对加速度进行归一化
	norm = inv_sqrt(ax*ax + ay*ay + az*az);       
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;
	
	//磁力计引入
	#ifdef IST8310
		norm = inv_sqrt(mx*mx + my*my + mz*mz);          
		mx = mx * norm;
		my = my * norm;
		mz = mz * norm; 
	#else
		mx = 0;
		my = 0;
		mz = 0;		
	#endif
	
	
	/* compute reference direction of flux */
	hx = 2.0f*mx*(0.5f - q2q2 - q3q3) + 2.0f*my*(q1q2 - q0q3) + 2.0f*mz*(q1q3 + q0q2);
	hy = 2.0f*mx*(q1q2 + q0q3) + 2.0f*my*(0.5f - q1q1 - q3q3) + 2.0f*mz*(q2q3 - q0q1);
	hz = 2.0f*mx*(q1q3 - q0q2) + 2.0f*my*(q2q3 + q0q1) + 2.0f*mz*(0.5f - q1q1 - q2q2);         
	bx = sqrt((hx*hx) + (hy*hy));
	bz = hz; 
	
	
	/* estimated direction of gravity and flux (v and w) */
	vx = 2.0f*(q1q3 - q0q2);
	vy = 2.0f*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	wx = 2.0f*bx*(0.5f - q2q2 - q3q3) + 2.0f*bz*(q1q3 - q0q2);
	wy = 2.0f*bx*(q1q2 - q0q3) + 2.0f*bz*(q0q1 + q2q3);
	wz = 2.0f*bx*(q0q2 + q1q3) + 2.0f*bz*(0.5f - q1q1 - q2q2);  
	
	
	/* 
	 * error is sum of cross product between reference direction 
	 * of fields and direction measured by sensors 
	 */
	ex = (ay*vz - az*vy) + (my*wz - mz*wy);
	ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
	ez = (ax*vy - ay*vx) + (mx*wy - my*wx);

	//比例积分
	/* PI */
	if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
	{
		exInt = exInt + ex * Ki * halfT;
		eyInt = eyInt + ey * Ki * halfT;	
		ezInt = ezInt + ez * Ki * halfT;
		
		gx = gx + Kp*ex + exInt;
		gy = gy + Kp*ey + eyInt;
		gz = gz + Kp*ez + ezInt;
	}

	tempq0 = q0 + (-q1*gx - q2*gy - q3*gz) * halfT;
	tempq1 = q1 + (q0*gx + q2*gz - q3*gy) * halfT;
	tempq2 = q2 + (q0*gy - q1*gz + q3*gx) * halfT;
	tempq3 = q3 + (q0*gz + q1*gy - q2*gx) * halfT;  

	//四元数归一化
	norm = inv_sqrt(tempq0*tempq0 + tempq1*tempq1 + tempq2*tempq2 + tempq3*tempq3);
	q0 = tempq0 * norm;
	q1 = tempq1 * norm;
	q2 = tempq2 * norm;
	q3 = tempq3 * norm;
	
}
/*
*@title：IMU欧拉角更新
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void IMU_Attitude_Update()
{
	//四元数转化为欧拉角
		/* roll   -pi----pi  */	
	imu.rol =  atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1)* 57.3;

	/* pitch  -pi/2----pi/2 */
	imu.pit = asin(-2*q1*q3 + 2*q0*q2)* 57.3;   

	/* yaw    -pi----pi */
	imu.yaw = atan2(2*q1*q2 + 2*q0*q3, -2*q2*q2 - 2*q3*q3 + 1)* 57.3; 
}
/*
*@title：IMU数据更新
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void IMU_Data_Update()
{
		IMU_Get_Data();//读取陀螺仪数据
		IMU_Ahrs_Update();//姿态解算
		IMU_Attitude_Update();//欧拉角转化
//		USART3_Send_NUC();

}
/*
*@title：IMU外部中断初始化
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void IMU_Interrupt_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXIT_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE );
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE );

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init (GPIOB,&GPIO_InitStruct);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB ,GPIO_PinSource8);

	EXIT_InitStruct.EXTI_Line = EXTI_Line8;
	EXIT_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXIT_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXIT_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXIT_InitStruct);

	//外部中断配置优先级
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct); 
}
/*
*@title：IMU外部中断处理函数
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void EXTI9_5_IRQHandler(void)
{
   if (EXTI_GetITStatus (EXTI_Line8 )!=RESET)
   {
//			IMU_DATA_UPDATE();
   }
   EXTI_ClearITPendingBit(EXTI_Line8);
}
/*
*@title：IMU初始化
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void IMU_Init()
{

	SPI_IMU_Init();	//SPI外设初始化
	
	MPU_Device_Init();//陀螺仪设备初始化
	
	Quaternion_Init();//四元数初始化
		
	IMU_Interrupt_Init();//陀螺仪外接中断初始化
	
	
}


