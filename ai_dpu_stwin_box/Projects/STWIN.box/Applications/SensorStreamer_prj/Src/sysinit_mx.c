/**
 ******************************************************************************
 * @file    sysinit_mx.c
 * @author  STMicroelectronics - ST-Korea - MCD Team
 * @version 1.2.1
 * @date    Nov 13, 2017
 *
 * @brief
 *
 * <DESCRIPTIOM>
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

#include "services/systp.h"
#include "services/syserror.h"
#include "mx.h"

//Select the SystemClock_Config
//#define SystemClock_Config_SensorTile SystemClock_Config
#define SystemClock_Config_MX SystemClock_Config

/**
 * This type group together the components of the clock three to be modified during
 * a power mode change.
 */
typedef struct _system_clock_t{
  uint32_t latency;
  RCC_OscInitTypeDef osc;
  RCC_ClkInitTypeDef clock;
  RCC_PeriphCLKInitTypeDef periph_clock;
} system_clock_t;

/**
 * It is used to save and restore the system clock during the power mode switch.
 */
static system_clock_t system_clock;

// Private member function declaration
// ***********************************


// Public API definition
// *********************

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    sys_error_handler();
  }
  __HAL_RCC_PWR_CLK_DISABLE();
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_MSI
                              |RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMBOOST = RCC_PLLMBOOST_DIV1;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 1;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLLVCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    sys_error_handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    sys_error_handler();
  }

  __HAL_RCC_PWR_CLK_DISABLE();

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADCDAC|RCC_PERIPHCLK_DAC1
                                      |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_I2C2
                                      |RCC_PERIPHCLK_I2C3|RCC_PERIPHCLK_MDF1
                                      |RCC_PERIPHCLK_SPI1|RCC_PERIPHCLK_SPI2
                                      |RCC_PERIPHCLK_SPI3;
  PeriphClkInit.Spi1ClockSelection = RCC_SPI1CLKSOURCE_SYSCLK;
  PeriphClkInit.Spi2ClockSelection = RCC_SPI2CLKSOURCE_PCLK1;
  PeriphClkInit.Spi3ClockSelection = RCC_SPI3CLKSOURCE_SYSCLK;
  PeriphClkInit.Mdf1ClockSelection = RCC_MDF1CLKSOURCE_HCLK;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK3;
  PeriphClkInit.Dac1ClockSelection = RCC_DAC1CLKSOURCE_LSI;
  PeriphClkInit.AdcDacClockSelection = RCC_ADCDACCLKSOURCE_PLL2;
  PeriphClkInit.PLL2.PLL2Source = RCC_PLLSOURCE_HSE;
  PeriphClkInit.PLL2.PLL2M = 2;
  PeriphClkInit.PLL2.PLL2N = 48;
  PeriphClkInit.PLL2.PLL2P = 2;
  PeriphClkInit.PLL2.PLL2Q = 7;
  PeriphClkInit.PLL2.PLL2R = 25;
  PeriphClkInit.PLL2.PLL2RGE = RCC_PLLVCIRANGE_1;
  PeriphClkInit.PLL2.PLL2FRACN = 0;
  PeriphClkInit.PLL2.PLL2ClockOut = RCC_PLL2_DIVR;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    sys_error_handler();
  }


  __HAL_RCC_PWR_CLK_ENABLE();
}

void SystemClock_Backup(void)
{
  HAL_RCC_GetOscConfig(&(system_clock.osc));
  HAL_RCC_GetClockConfig(&(system_clock.clock), &(system_clock.latency));
  HAL_RCCEx_GetPeriphCLKConfig(&(system_clock.periph_clock));
}

/**
  * @brief  Restore original clock parameters
  * @retval Process result
  *         @arg SMPS_OK or SMPS_KO
  */
void SystemClock_Restore(void)
{
  /* SEQUENCE:
   *   1. Set PWR regulator to SCALE1_BOOST
   *   2. PLL ON
   *   3. Set SYSCLK source to PLL
   *
   * NOTE:
   *   Do not change or update the base-clock source (e.g. MSI and LSE)
   */

//  if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST) != HAL_OK)
  if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    sys_error_handler();
  }

//  if (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_CFGR_SWS_PLL) {
  if(__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_PLLCLK)
  {
    if(HAL_RCC_OscConfig(&(system_clock.osc)) != HAL_OK)
    {
      sys_error_handler();
    }
  }

  if(HAL_RCC_ClockConfig(&(system_clock.clock), system_clock.latency) != HAL_OK)
  {
    sys_error_handler();
  }
}

void SysPowerConfig()
{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Enable Power Clock
  __HAL_RCC_PWR_CLK_ENABLE();

  // Select MSI as system clock source after Wake Up from Stop mode
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);


  // This function is called in the early step of the system initialization.
  // All the PINs used by the application are reconfigured later by the application tasks.

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_PWREx_EnableIO2VM();
  while(!(PWR->SVMCR & PWR_SVMCR_IO2VMEN));
  HAL_PWREx_EnableVddIO2();
  while(!(PWR->SVMSR & PWR_SVMSR_VDDIO2RDY));

   /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOE_CLK_ENABLE();
   __HAL_RCC_GPIOI_CLK_ENABLE();
   __HAL_RCC_GPIOG_CLK_ENABLE();
   __HAL_RCC_GPIOC_CLK_ENABLE();
   __HAL_RCC_GPIOA_CLK_ENABLE();
   __HAL_RCC_GPIOH_CLK_ENABLE();
   __HAL_RCC_GPIOB_CLK_ENABLE();
   __HAL_RCC_GPIOD_CLK_ENABLE();
   __HAL_RCC_GPIOF_CLK_ENABLE();

   /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, CS_DHCX_Pin|CS_DLPC_Pin|WIFI_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_ICLX_GPIO_Port, CS_ICLX_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BLE_SPI_CS_GPIO_Port, BLE_SPI_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_DWB_GPIO_Port, CS_DWB_Pin, GPIO_PIN_SET);

   /*Configure GPIO pin : PtPin */
   GPIO_InitStruct.Pin = INT_HTS_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(INT_HTS_GPIO_Port, &GPIO_InitStruct);

   /*Configure GPIO pins : PGPin PG10 PGPin PGPin
                            PG0 PGPin */
   GPIO_InitStruct.Pin = WIFI_FLOW_Pin|GPIO_PIN_10|SD_DETECT_Pin|INT_EX_Pin
                           |GPIO_PIN_0|INT_EXG5_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PA15 PA10 PAPin PA9
                           PA0 PA1 PA2 PA3
                           PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_10|SW_SEL_Pin|GPIO_PIN_9
                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_6;
   GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PHPin PHPin PHPin */
  GPIO_InitStruct.Pin = CS_DHCX_Pin|CS_DLPC_Pin|WIFI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : PIPin PI4 PIPin */
  GPIO_InitStruct.Pin = DCDC_2_EN_Pin|GPIO_PIN_4|SPI2_MISO_p2_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

   /*Configure GPIO pins : PDPin PD4 PD7 PD1
                            PD14 PDPin PDPin PD8
                            PDPin */
   GPIO_InitStruct.Pin = C_EN_Pin|GPIO_PIN_4|GPIO_PIN_7|GPIO_PIN_1
                           |GPIO_PIN_14|INT_EXD15_Pin|BLE_RST_Pin|GPIO_PIN_8
                           |WIFI_BOOT_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PHPin PHPin PH14 PH13
                           PHPin PH11 PH8 PHPin
                           PH5 PH2 */
  GPIO_InitStruct.Pin = LED1_Pin|BOOT0_Pin|GPIO_PIN_14|GPIO_PIN_13
                          |LED2_Pin|GPIO_PIN_11|GPIO_PIN_8|STSAFE_RESE_Pin
                          |GPIO_PIN_5|GPIO_PIN_2;
   GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = CS_ICLX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_ICLX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = BLE_SPI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BLE_SPI_CS_GPIO_Port, &GPIO_InitStruct);

   /*Configure GPIO pins : PEPin PEPin */
   GPIO_InitStruct.Pin = USER_BUTTON_Pin|WIFI_NOTIFY_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

   /*Configure GPIO pins : PBPin PBPin */
   GPIO_InitStruct.Pin = INT1_DHCX_Pin|NFC_INT_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

   /*Configure GPIO pin : PtPin */
   GPIO_InitStruct.Pin = INT_LPS_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(INT_LPS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PFPin PFPin PFPin PF13 */
  GPIO_InitStruct.Pin = GPIO2_EX_Pin|BLE_TEST9_Pin|BLE_TEST8_Pin|GPIO_PIN_13;
   GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

   /*Configure GPIO pins : PFPin PFPin PFPin PFPin
                            PFPin PFPin PFPin PFPin
                            PFPin */
   GPIO_InitStruct.Pin = INT1_DLPC_Pin|INT2_DLPC_Pin|INT_MAG_Pin|INT_STT_Pin
                           |INT1_ICLX_Pin|INT2_DHCX_Pin|BLE_INT_Pin|INT2_ICLX_Pin
                           |INT1_DWB_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

   /*Configure GPIO pins : PB7 PB0 PB10 PB2
                            PB11 PB12 PBPin PB14
                            PBPin */
   GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_2
                           |GPIO_PIN_11|GPIO_PIN_12|SPI2_MOSI_p2_Pin|GPIO_PIN_14
                           |GPIO1_EX_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

   /*Configure GPIO pins : PC7 PCPin PCPin PC4
                            PC5 */
   GPIO_InitStruct.Pin = GPIO_PIN_7|uC_ADC_BATT_Pin|ADC_EX_Pin|GPIO_PIN_4
                           |GPIO_PIN_5;
   GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

   /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = CS_DWB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_DWB_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PE8 PE14 PE13 PE11
                           PEPin PEPin */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_14|GPIO_PIN_13|GPIO_PIN_11
                          |LDO_EN_Pin|WIFI_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
   GPIO_InitStruct.Pin = BUTTON_PWR_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(BUTTON_PWR_GPIO_Port, &GPIO_InitStruct);

   /* GPIO Ports Clock Disable */
   __HAL_RCC_GPIOE_CLK_DISABLE();
   __HAL_RCC_GPIOI_CLK_DISABLE();
   __HAL_RCC_GPIOG_CLK_DISABLE();
   __HAL_RCC_GPIOC_CLK_DISABLE();
   __HAL_RCC_GPIOA_CLK_DISABLE();
   __HAL_RCC_GPIOH_CLK_DISABLE();
   __HAL_RCC_GPIOB_CLK_DISABLE();
   __HAL_RCC_GPIOD_CLK_DISABLE();
   __HAL_RCC_GPIOF_CLK_DISABLE();
}

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  // Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
  HAL_PWREx_DisableUCPDDeadBattery();

  // System interrupt init
  // MemoryManagement_IRQn interrupt configuration
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
  // BusFault_IRQn interrupt configuration
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
  // UsageFault_IRQn interrupt configuration
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
  // SVCall_IRQn interrupt configuration
  HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
  // DebugMonitor_IRQn interrupt configuration
  HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
  // PendSV_IRQn interrupt configuration
  HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
  // SysTick_IRQn interrupt configuration
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

// Private function definition
// ***************************
