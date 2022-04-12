/** *****************************************************************************************
  * @file    kalman.c                                                                                                                                      *
  * @author  Liu heng                                                                                                                                    *
  * @version V1.0.0                                                                                                                                      *
  * @date    27-August-2013                                                                                                                         *
  * @brief   һά�������˲����ľ���ʵ�֡�ʵ�ֹ�����ȫ��Ӳ���޹أ�   *
  *   ��ֱ�ӵ��ã�������ֲ��                                                                                               *
  *   ʹ��ʱ�ȶ���һ��kalmanָ�룬Ȼ�����kalmanCreate()����һ���˲���*
  *   ÿ�ζ�ȡ�����������ݺ󼴿ɵ���KalmanFilter()�������ݽ����˲���               *
  *****************************************************************************************
  *                          ʹ��ʾ��                                                     *
  *          kalman p;                                                                   *
  *          float SersorData;                                                            *
  *          kalmanCreate(&p,20,200);                                                  *
  *          while(1)                                                                     *
  *          {                                                                            *
  *             SersorData = sersor();                                                    *
  *             SersorData = KalmanFilter(&p,SersorData);                                  *
  *             printf("%2.2f",SersorData);                                               *
  *          }                                                                            *
  *****************************************************************************************
  *          MPU6050�Ŀ������˲����ο����� Q��0.005 R��0.01                                  *
  *****************************************************************************************/

#include "kalman.h"

/**
  * @name   kalmanCreate
  * @brief  ����һ���������˲���
  * @param  p:  �˲���
  *         T_Q:ϵͳ����Э����
  *         T_R:��������Э����
  *         
  * @retval none
  */
void kalmanCreate(kalman *p,float T_Q,float T_R)
{
    //kalman* p = ( kalman*)malloc(sizeof( kalman));
    p->X_last = (float)0;
    p->P_last = 0;
    p->Q = T_Q;
    p->R = T_R;
    p->A = 1;
    p->H = 1;
    p->X_mid = p->X_last;
    //return p;
}

/**
  * @name   KalmanFilter
  * @brief  �������˲���
  * @param  p:  �˲���
  *         dat:���˲�����
  * @retval �˲��������
  */

float KalmanFilter(kalman* p,float dat)
{
    p->X_mid =p->A*p->X_last;                     //x(k|k-1) = AX(k-1|k-1)+BU(k)
    p->P_mid = p->A*p->P_last+p->Q;               //p(k|k-1) = Ap(k-1|k-1)A'+Q
    p->kg = p->P_mid/(p->P_mid+p->R);             //kg(k) = p(k|k-1)H'/(Hp(k|k-1)'+R)
    p->X_now = p->X_mid+p->kg*(dat-p->X_mid);     //x(k|k) = X(k|k-1)+kg(k)(Z(k)-HX(k|k-1))
    p->P_now = (1-p->kg)*p->P_mid;                //p(k|k) = (I-kg(k)H)P(k|k-1)
    p->P_last = p->P_now;                         //״̬����
    p->X_last = p->X_now;
    return p->X_now;
}
