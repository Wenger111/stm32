int main(void)
{

  
  HAL_Init();

  SystemClock_Config();

  
  MX_GPIO_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
HAL_TIM_PWM_Start (&htim3 ,TIM_CHANNEL_1 );
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		for (int i =0;i<99;i++)
		{
			__HAL_TIM_SET_COMPARE (&htim3 ,TIM_CHANNEL_1 ,i);
			HAL_Delay (10);
		}
		for (int i =99;i>=0;i--)
		{
			__HAL_TIM_SET_COMPARE (&htim3 ,TIM_CHANNEL_1 ,i);
			HAL_Delay (10);
		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
