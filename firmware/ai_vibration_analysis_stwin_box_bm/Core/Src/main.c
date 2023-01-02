/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_interface.h"
#include "usb_device.h"
#include "STWIN.box.h"
#include "sensor_app.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

MDF_HandleTypeDef AdfHandle0;
MDF_FilterConfigTypeDef AdfFilterConfig0;
MDF_HandleTypeDef MdfHandle0;
MDF_FilterConfigTypeDef MdfFilterConfig0;
DMA_NodeTypeDef Node_GPDMA1_Channel5;
DMA_QListTypeDef List_GPDMA1_Channel5;
DMA_HandleTypeDef handle_GPDMA1_Channel5;
DMA_NodeTypeDef Node_GPDMA1_Channel4;
DMA_QListTypeDef List_GPDMA1_Channel4;
DMA_HandleTypeDef handle_GPDMA1_Channel4;

DAC_HandleTypeDef hdac1;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;
DMA_HandleTypeDef handle_GPDMA1_Channel3;
DMA_HandleTypeDef handle_GPDMA1_Channel2;

RTC_HandleTypeDef hrtc;

SAI_HandleTypeDef hsai_BlockA1;
SAI_HandleTypeDef hsai_BlockB1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef handle_GPDMA1_Channel1;
DMA_HandleTypeDef handle_GPDMA1_Channel0;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
CRC_HandleTypeDef hcrc;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void SystemPower_Config(void);
static void MX_GPIO_Init(void);
static void MX_GPDMA1_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADF1_Init(void);
static void MX_MDF1_Init(void);
static void MX_DAC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C3_Init(void);
static void MX_ICACHE_Init(void);
static void MX_RTC_Init(void);
static void MX_SAI1_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_SPI3_Init(void);
static void MX_TAMP_RTC_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
/* USER CODE BEGIN PFP */
static void MX_CRC_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar (int ch)
{
	(void)HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, COM_POLL_TIMEOUT);
	return ch;
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* Configure the peripherals common clocks */
	PeriphCommonClock_Config();

	/* Configure the System Power */
	SystemPower_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_GPDMA1_Init();
	MX_ADC1_Init();
	MX_ADF1_Init();
	MX_MDF1_Init();
	MX_DAC1_Init();
	MX_I2C1_Init();
	MX_I2C2_Init();
	MX_I2C3_Init();
	MX_ICACHE_Init();
	MX_RTC_Init();
	MX_SAI1_Init();
	MX_SPI1_Init();
	MX_SPI2_Init();
	MX_SPI3_Init();
	MX_TAMP_RTC_Init();
	MX_TIM3_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_USB_OTG_FS_PCD_Init();
	/* USER CODE BEGIN 2 */
	MX_USB_Device_Init();
	MX_CRC_Init();

	// CRC init needed to use AI on STM32
	HAL_CRC_Init(&hcrc);
	__HAL_RCC_CRC_CLK_ENABLE();

	// Sensor app init
	sensor_init();

	/*uint32_t size;
	char data_s[50];*/
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		/*HAL_Delay(500);
		size = sprintf(data_s, "Funziona l'USB!!!\n\r");
		CDC_Transmit_FS(( uint8_t * )data_s, size);*/
		sensor_process();
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSI
			|RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.LSIDiv = RCC_LSI_DIV1;
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
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
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
		Error_Handler();
	}
}

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the common periph clock
	 */
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_MDF1|RCC_PERIPHCLK_ADF1
			|RCC_PERIPHCLK_ADCDAC;
	PeriphClkInit.Mdf1ClockSelection = RCC_MDF1CLKSOURCE_PLL3;
	PeriphClkInit.Adf1ClockSelection = RCC_ADF1CLKSOURCE_PLL3;
	PeriphClkInit.AdcDacClockSelection = RCC_ADCDACCLKSOURCE_PLL2;
	PeriphClkInit.PLL3.PLL3Source = RCC_PLLSOURCE_HSE;
	PeriphClkInit.PLL3.PLL3M = 2;
	PeriphClkInit.PLL3.PLL3N = 48;
	PeriphClkInit.PLL3.PLL3P = 2;
	PeriphClkInit.PLL3.PLL3Q = 25;
	PeriphClkInit.PLL3.PLL3R = 2;
	PeriphClkInit.PLL3.PLL3RGE = RCC_PLLVCIRANGE_1;
	PeriphClkInit.PLL3.PLL3FRACN = 0;
	PeriphClkInit.PLL3.PLL3ClockOut = RCC_PLL3_DIVQ;
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
		Error_Handler();
	}
}

/**
 * @brief Power Configuration
 * @retval None
 */
static void SystemPower_Config(void)
{
	HAL_PWREx_EnableVddIO2();

	/*
	 * Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
	 */
	HAL_PWREx_DisableUCPDDeadBattery();
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void)
{

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */

	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.GainCompensation = 0;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
	hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
	hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_MDF;
	hadc1.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

}

/**
 * @brief ADF1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADF1_Init(void)
{

	/* USER CODE BEGIN ADF1_Init 0 */

	/* USER CODE END ADF1_Init 0 */

	/* USER CODE BEGIN ADF1_Init 1 */

	/* USER CODE END ADF1_Init 1 */

	/**
    AdfHandle0 structure initialization and HAL_MDF_Init function call
	 */
	AdfHandle0.Instance = ADF1_Filter0;
	AdfHandle0.Init.CommonParam.ProcClockDivider = 1;
	AdfHandle0.Init.CommonParam.OutputClock.Activation = ENABLE;
	AdfHandle0.Init.CommonParam.OutputClock.Pins = MDF_OUTPUT_CLOCK_0;
	AdfHandle0.Init.CommonParam.OutputClock.Divider = 5;
	AdfHandle0.Init.CommonParam.OutputClock.Trigger.Activation = ENABLE;
	AdfHandle0.Init.CommonParam.OutputClock.Trigger.Source = MDF_CLOCK_TRIG_TRGO;
	AdfHandle0.Init.CommonParam.OutputClock.Trigger.Edge = MDF_CLOCK_TRIG_FALLING_EDGE;
	AdfHandle0.Init.SerialInterface.Activation = ENABLE;
	AdfHandle0.Init.SerialInterface.Mode = MDF_SITF_NORMAL_SPI_MODE;
	AdfHandle0.Init.SerialInterface.ClockSource = MDF_SITF_CCK0_SOURCE;
	AdfHandle0.Init.SerialInterface.Threshold = 31;
	AdfHandle0.Init.FilterBistream = MDF_BITSTREAM0_FALLING;
	if (HAL_MDF_Init(&AdfHandle0) != HAL_OK)
	{
		Error_Handler();
	}

	/**
    AdfFilterConfig0 structure initialization

    WARNING : only structure is filled, no specific init function call for filter
	 */
	AdfFilterConfig0.DataSource = MDF_DATA_SOURCE_BSMX;
	AdfFilterConfig0.Delay = 0;
	AdfFilterConfig0.CicMode = MDF_ONE_FILTER_SINC5;
	AdfFilterConfig0.DecimationRatio = 16;
	AdfFilterConfig0.Gain = 1;
	AdfFilterConfig0.ReshapeFilter.Activation = ENABLE;
	AdfFilterConfig0.ReshapeFilter.DecimationRatio = MDF_RSF_DECIMATION_RATIO_4;
	AdfFilterConfig0.HighPassFilter.Activation = ENABLE;
	AdfFilterConfig0.HighPassFilter.CutOffFrequency = MDF_HPF_CUTOFF_0_000625FPCM;
	AdfFilterConfig0.SoundActivity.Activation = DISABLE;
	AdfFilterConfig0.AcquisitionMode = MDF_MODE_SYNC_CONT;
	AdfFilterConfig0.FifoThreshold = MDF_FIFO_THRESHOLD_NOT_EMPTY;
	AdfFilterConfig0.DiscardSamples = 0;
	AdfFilterConfig0.Trigger.Source = MDF_CLOCK_TRIG_TRGO;
	AdfFilterConfig0.Trigger.Edge = MDF_FILTER_TRIG_RISING_EDGE;
	/* USER CODE BEGIN ADF1_Init 2 */

	/* USER CODE END ADF1_Init 2 */

}

/**
 * @brief MDF1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_MDF1_Init(void)
{

	/* USER CODE BEGIN MDF1_Init 0 */

	/* USER CODE END MDF1_Init 0 */

	/* USER CODE BEGIN MDF1_Init 1 */

	/* USER CODE END MDF1_Init 1 */

	/**
    MdfHandle0 structure initialization and HAL_MDF_Init function call
	 */
	MdfHandle0.Instance = MDF1_Filter0;
	MdfHandle0.Init.CommonParam.InterleavedFilters = 0;
	MdfHandle0.Init.CommonParam.ProcClockDivider = 1;
	MdfHandle0.Init.CommonParam.OutputClock.Activation = DISABLE;
	MdfHandle0.Init.SerialInterface.Activation = DISABLE;
	if (HAL_MDF_Init(&MdfHandle0) != HAL_OK)
	{
		Error_Handler();
	}

	/**
    MdfFilterConfig0, MdfOldConfig0 and/or MdfScdConfig0 structures initialization

    WARNING : only structures are filled, no specific init function call for filter
	 */
	MdfFilterConfig0.DataSource = MDF_DATA_SOURCE_ADCITF1;
	MdfFilterConfig0.Delay = 0;
	MdfFilterConfig0.CicMode = MDF_ONE_FILTER_SINC5;
	MdfFilterConfig0.DecimationRatio = 4;
	MdfFilterConfig0.Offset = 0;
	MdfFilterConfig0.Gain = 4;
	MdfFilterConfig0.ReshapeFilter.Activation = DISABLE;
	MdfFilterConfig0.HighPassFilter.Activation = ENABLE;
	MdfFilterConfig0.HighPassFilter.CutOffFrequency = MDF_HPF_CUTOFF_0_000625FPCM;
	MdfFilterConfig0.Integrator.Activation = DISABLE;
	MdfFilterConfig0.SoundActivity.Activation = DISABLE;
	MdfFilterConfig0.AcquisitionMode = MDF_MODE_ASYNC_CONT;
	MdfFilterConfig0.FifoThreshold = MDF_FIFO_THRESHOLD_NOT_EMPTY;
	MdfFilterConfig0.DiscardSamples = 0;
	/* USER CODE BEGIN MDF1_Init 2 */

	/* USER CODE END MDF1_Init 2 */

}

/**
 * @brief DAC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_DAC1_Init(void)
{

	/* USER CODE BEGIN DAC1_Init 0 */

	/* USER CODE END DAC1_Init 0 */

	DAC_ChannelConfTypeDef sConfig = {0};
	DAC_AutonomousModeConfTypeDef sAutonomousMode = {0};

	/* USER CODE BEGIN DAC1_Init 1 */

	/* USER CODE END DAC1_Init 1 */

	/** DAC Initialization
	 */
	hdac1.Instance = DAC1;
	if (HAL_DAC_Init(&hdac1) != HAL_OK)
	{
		Error_Handler();
	}

	/** DAC channel OUT1 config
	 */
	sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_DISABLE;
	sConfig.DAC_DMADoubleDataMode = DISABLE;
	sConfig.DAC_SignedFormat = DISABLE;
	sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_BOTH;
	sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
	if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Autonomous Mode
	 */
	sAutonomousMode.AutonomousModeState = DAC_AUTONOMOUS_MODE_DISABLE;
	if (HAL_DACEx_SetConfigAutonomousMode(&hdac1, &sAutonomousMode) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN DAC1_Init 2 */

	/* USER CODE END DAC1_Init 2 */

}

/**
 * @brief GPDMA1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPDMA1_Init(void)
{

	/* USER CODE BEGIN GPDMA1_Init 0 */

	/* USER CODE END GPDMA1_Init 0 */

	/* Peripheral clock enable */
	__HAL_RCC_GPDMA1_CLK_ENABLE();

	/* GPDMA1 interrupt Init */
	HAL_NVIC_SetPriority(GPDMA1_Channel0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(GPDMA1_Channel0_IRQn);
	HAL_NVIC_SetPriority(GPDMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(GPDMA1_Channel1_IRQn);
	HAL_NVIC_SetPriority(GPDMA1_Channel2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(GPDMA1_Channel2_IRQn);
	HAL_NVIC_SetPriority(GPDMA1_Channel3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(GPDMA1_Channel3_IRQn);
	HAL_NVIC_SetPriority(GPDMA1_Channel4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(GPDMA1_Channel4_IRQn);
	HAL_NVIC_SetPriority(GPDMA1_Channel5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(GPDMA1_Channel5_IRQn);

	/* USER CODE BEGIN GPDMA1_Init 1 */

	/* USER CODE END GPDMA1_Init 1 */
	/* USER CODE BEGIN GPDMA1_Init 2 */

	/* USER CODE END GPDMA1_Init 2 */

}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x00F07BFF;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void)
{

	/* USER CODE BEGIN I2C2_Init 0 */

	/* USER CODE END I2C2_Init 0 */

	/* USER CODE BEGIN I2C2_Init 1 */

	/* USER CODE END I2C2_Init 1 */
	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x00F07BFF;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C2_Init 2 */

	/* USER CODE END I2C2_Init 2 */

}

/**
 * @brief I2C3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C3_Init(void)
{

	/* USER CODE BEGIN I2C3_Init 0 */

	/* USER CODE END I2C3_Init 0 */

	/* USER CODE BEGIN I2C3_Init 1 */

	/* USER CODE END I2C3_Init 1 */
	hi2c3.Instance = I2C3;
	hi2c3.Init.Timing = 0x00F07BFF;
	hi2c3.Init.OwnAddress1 = 0;
	hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c3.Init.OwnAddress2 = 0;
	hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c3) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C3_Init 2 */

	/* USER CODE END I2C3_Init 2 */

}

/**
 * @brief ICACHE Initialization Function
 * @param None
 * @retval None
 */
static void MX_ICACHE_Init(void)
{

	/* USER CODE BEGIN ICACHE_Init 0 */

	/* USER CODE END ICACHE_Init 0 */

	/* USER CODE BEGIN ICACHE_Init 1 */

	/* USER CODE END ICACHE_Init 1 */

	/** Enable instruction cache in 1-way (direct mapped cache)
	 */
	if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_ICACHE_Enable() != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN ICACHE_Init 2 */

	/* USER CODE END ICACHE_Init 2 */

}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void)
{

	/* USER CODE BEGIN RTC_Init 0 */

	/* USER CODE END RTC_Init 0 */

	RTC_PrivilegeStateTypeDef privilegeState = {0};

	/* USER CODE BEGIN RTC_Init 1 */

	/* USER CODE END RTC_Init 1 */

	/** Initialize RTC Only
	 */
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
	hrtc.Init.BinMode = RTC_BINARY_NONE;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_Handler();
	}
	privilegeState.rtcPrivilegeFull = RTC_PRIVILEGE_FULL_NO;
	privilegeState.backupRegisterPrivZone = RTC_PRIVILEGE_BKUP_ZONE_NONE;
	privilegeState.backupRegisterStartZone2 = RTC_BKP_DR0;
	privilegeState.backupRegisterStartZone3 = RTC_BKP_DR0;
	if (HAL_RTCEx_PrivilegeModeSet(&hrtc, &privilegeState) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN RTC_Init 2 */

	/* USER CODE END RTC_Init 2 */

}

/**
 * @brief SAI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SAI1_Init(void)
{

	/* USER CODE BEGIN SAI1_Init 0 */

	/* USER CODE END SAI1_Init 0 */

	/* USER CODE BEGIN SAI1_Init 1 */

	/* USER CODE END SAI1_Init 1 */
	hsai_BlockA1.Instance = SAI1_Block_A;
	hsai_BlockA1.Init.Protocol = SAI_FREE_PROTOCOL;
	hsai_BlockA1.Init.AudioMode = SAI_MODEMASTER_TX;
	hsai_BlockA1.Init.DataSize = SAI_DATASIZE_8;
	hsai_BlockA1.Init.FirstBit = SAI_FIRSTBIT_MSB;
	hsai_BlockA1.Init.ClockStrobing = SAI_CLOCKSTROBING_FALLINGEDGE;
	hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
	hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
	hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
	hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
	hsai_BlockA1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_192K;
	hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
	hsai_BlockA1.Init.MckOutput = SAI_MCK_OUTPUT_ENABLE;
	hsai_BlockA1.Init.MonoStereoMode = SAI_STEREOMODE;
	hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
	hsai_BlockA1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
	hsai_BlockA1.Init.PdmInit.Activation = DISABLE;
	hsai_BlockA1.Init.PdmInit.MicPairsNbr = 1;
	hsai_BlockA1.Init.PdmInit.ClockEnable = SAI_PDM_CLOCK1_ENABLE;
	hsai_BlockA1.FrameInit.FrameLength = 8;
	hsai_BlockA1.FrameInit.ActiveFrameLength = 1;
	hsai_BlockA1.FrameInit.FSDefinition = SAI_FS_STARTFRAME;
	hsai_BlockA1.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
	hsai_BlockA1.FrameInit.FSOffset = SAI_FS_FIRSTBIT;
	hsai_BlockA1.SlotInit.FirstBitOffset = 0;
	hsai_BlockA1.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
	hsai_BlockA1.SlotInit.SlotNumber = 1;
	hsai_BlockA1.SlotInit.SlotActive = 0x00000000;
	if (HAL_SAI_Init(&hsai_BlockA1) != HAL_OK)
	{
		Error_Handler();
	}
	hsai_BlockB1.Instance = SAI1_Block_B;
	hsai_BlockB1.Init.Protocol = SAI_FREE_PROTOCOL;
	hsai_BlockB1.Init.AudioMode = SAI_MODESLAVE_RX;
	hsai_BlockB1.Init.DataSize = SAI_DATASIZE_8;
	hsai_BlockB1.Init.FirstBit = SAI_FIRSTBIT_MSB;
	hsai_BlockB1.Init.ClockStrobing = SAI_CLOCKSTROBING_FALLINGEDGE;
	hsai_BlockB1.Init.Synchro = SAI_SYNCHRONOUS;
	hsai_BlockB1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
	hsai_BlockB1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
	hsai_BlockB1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
	hsai_BlockB1.Init.MckOutput = SAI_MCK_OUTPUT_ENABLE;
	hsai_BlockB1.Init.MonoStereoMode = SAI_STEREOMODE;
	hsai_BlockB1.Init.CompandingMode = SAI_NOCOMPANDING;
	hsai_BlockB1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
	hsai_BlockB1.Init.PdmInit.Activation = DISABLE;
	hsai_BlockB1.Init.PdmInit.MicPairsNbr = 1;
	hsai_BlockB1.Init.PdmInit.ClockEnable = SAI_PDM_CLOCK1_ENABLE;
	hsai_BlockB1.FrameInit.FrameLength = 8;
	hsai_BlockB1.FrameInit.ActiveFrameLength = 1;
	hsai_BlockB1.FrameInit.FSDefinition = SAI_FS_STARTFRAME;
	hsai_BlockB1.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
	hsai_BlockB1.FrameInit.FSOffset = SAI_FS_FIRSTBIT;
	hsai_BlockB1.SlotInit.FirstBitOffset = 0;
	hsai_BlockB1.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
	hsai_BlockB1.SlotInit.SlotNumber = 1;
	hsai_BlockB1.SlotInit.SlotActive = 0x00000000;
	if (HAL_SAI_Init(&hsai_BlockB1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SAI1_Init 2 */

	/* USER CODE END SAI1_Init 2 */

}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void)
{

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	SPI_AutonomousModeConfTypeDef HAL_SPI_AutonomousMode_Cfg_Struct = {0};

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 0x7;
	hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	hspi1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
	hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
	hspi1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
	hspi1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
	hspi1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
	hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
	hspi1.Init.IOSwap = SPI_IO_SWAP_DISABLE;
	hspi1.Init.ReadyMasterManagement = SPI_RDY_MASTER_MANAGEMENT_INTERNALLY;
	hspi1.Init.ReadyPolarity = SPI_RDY_POLARITY_HIGH;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_SPI_AutonomousMode_Cfg_Struct.TriggerState = SPI_AUTO_MODE_DISABLE;
	HAL_SPI_AutonomousMode_Cfg_Struct.TriggerSelection = SPI_GRP1_GPDMA_CH0_TCF_TRG;
	HAL_SPI_AutonomousMode_Cfg_Struct.TriggerPolarity = SPI_TRIG_POLARITY_RISING;
	if (HAL_SPIEx_SetConfigAutonomousMode(&hspi1, &HAL_SPI_AutonomousMode_Cfg_Struct) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI2_Init(void)
{

	/* USER CODE BEGIN SPI2_Init 0 */

	/* USER CODE END SPI2_Init 0 */

	SPI_AutonomousModeConfTypeDef HAL_SPI_AutonomousMode_Cfg_Struct = {0};

	/* USER CODE BEGIN SPI2_Init 1 */

	/* USER CODE END SPI2_Init 1 */
	/* SPI2 parameter configuration*/
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_2LINES;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi2.Init.CRCPolynomial = 0x7;
	hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
	hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
	hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
	hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
	hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
	hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
	hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;
	hspi2.Init.ReadyMasterManagement = SPI_RDY_MASTER_MANAGEMENT_INTERNALLY;
	hspi2.Init.ReadyPolarity = SPI_RDY_POLARITY_HIGH;
	if (HAL_SPI_Init(&hspi2) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_SPI_AutonomousMode_Cfg_Struct.TriggerState = SPI_AUTO_MODE_DISABLE;
	HAL_SPI_AutonomousMode_Cfg_Struct.TriggerSelection = SPI_GRP1_GPDMA_CH1_TCF_TRG;
	HAL_SPI_AutonomousMode_Cfg_Struct.TriggerPolarity = SPI_TRIG_POLARITY_RISING;
	if (HAL_SPIEx_SetConfigAutonomousMode(&hspi2, &HAL_SPI_AutonomousMode_Cfg_Struct) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI2_Init 2 */

	/* USER CODE END SPI2_Init 2 */

}

/**
 * @brief SPI3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI3_Init(void)
{

	/* USER CODE BEGIN SPI3_Init 0 */

	/* USER CODE END SPI3_Init 0 */

	SPI_AutonomousModeConfTypeDef HAL_SPI_AutonomousMode_Cfg_Struct = {0};

	/* USER CODE BEGIN SPI3_Init 1 */

	/* USER CODE END SPI3_Init 1 */
	/* SPI3 parameter configuration*/
	hspi3.Instance = SPI3;
	hspi3.Init.Mode = SPI_MODE_MASTER;
	hspi3.Init.Direction = SPI_DIRECTION_2LINES;
	hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi3.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi3.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi3.Init.NSS = SPI_NSS_HARD_OUTPUT;
	hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi3.Init.CRCPolynomial = 0x7;
	hspi3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	hspi3.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
	hspi3.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
	hspi3.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
	hspi3.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
	hspi3.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
	hspi3.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
	hspi3.Init.IOSwap = SPI_IO_SWAP_DISABLE;
	hspi3.Init.ReadyMasterManagement = SPI_RDY_MASTER_MANAGEMENT_INTERNALLY;
	hspi3.Init.ReadyPolarity = SPI_RDY_POLARITY_HIGH;
	if (HAL_SPI_Init(&hspi3) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_SPI_AutonomousMode_Cfg_Struct.TriggerState = SPI_AUTO_MODE_DISABLE;
	HAL_SPI_AutonomousMode_Cfg_Struct.TriggerSelection = SPI_GRP1_GPDMA_CH0_TCF_TRG;
	HAL_SPI_AutonomousMode_Cfg_Struct.TriggerPolarity = SPI_TRIG_POLARITY_RISING;
	if (HAL_SPIEx_SetConfigAutonomousMode(&hspi3, &HAL_SPI_AutonomousMode_Cfg_Struct) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI3_Init 2 */

	/* USER CODE END SPI3_Init 2 */

}

/**
 * @brief TAMP Initialization Function
 * @param None
 * @retval None
 */
static void MX_TAMP_RTC_Init(void)
{

	/* USER CODE BEGIN TAMP_Init 0 */

	/* USER CODE END TAMP_Init 0 */

	RTC_PrivilegeStateTypeDef privilegeState = {0};
	RTC_TamperTypeDef sTamper = {0};

	/* USER CODE BEGIN TAMP_Init 1 */

	/* USER CODE END TAMP_Init 1 */

	/** Enable the RTC Active Tamper
	 */
	privilegeState.tampPrivilegeFull = TAMP_PRIVILEGE_FULL_YES;
	privilegeState.MonotonicCounterPrivilege = TAMP_MONOTONIC_CNT_PRIVILEGE_NO;
	if (HAL_RTCEx_PrivilegeModeSet(&hrtc, &privilegeState) != HAL_OK)
	{
		Error_Handler();
	}

	/** Enable the RTC Tamper 1
	 */
	sTamper.Tamper = RTC_TAMPER_1;
	sTamper.Trigger = RTC_TAMPERTRIGGER_RISINGEDGE;
	sTamper.NoErase = RTC_TAMPER_ERASE_BACKUP_ENABLE;
	sTamper.MaskFlag = RTC_TAMPERMASK_FLAG_DISABLE;
	sTamper.Filter = RTC_TAMPERFILTER_DISABLE;
	sTamper.SamplingFrequency = RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV32768;
	sTamper.PrechargeDuration = RTC_TAMPERPRECHARGEDURATION_1RTCCLK;
	sTamper.TamperPullUp = RTC_TAMPER_PULLUP_ENABLE;
	sTamper.TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_ENABLE;
	if (HAL_RTCEx_SetTamper(&hrtc, &sTamper) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TAMP_Init 2 */

	/* USER CODE END TAMP_Init 2 */

}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void)
{

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 0;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 4.294967295E9;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 921600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_RTS;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART3_UART_Init(void)
{

	/* USER CODE BEGIN USART3_Init 0 */

	/* USER CODE END USART3_Init 0 */

	/* USER CODE BEGIN USART3_Init 1 */

	/* USER CODE END USART3_Init 1 */
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart3) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART3_Init 2 */

	/* USER CODE END USART3_Init 2 */

}

/**
 * @brief USB_OTG_FS Initialization Function
 * @param None
 * @retval None
 */
static void MX_USB_OTG_FS_PCD_Init(void)
{

	/* USER CODE BEGIN USB_OTG_FS_Init 0 */

	/* USER CODE END USB_OTG_FS_Init 0 */

	/* USER CODE BEGIN USB_OTG_FS_Init 1 */

	/* USER CODE END USB_OTG_FS_Init 1 */
	hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
	hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
	hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
	hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
	hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
	hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
	hpcd_USB_OTG_FS.Init.battery_charging_enable = DISABLE;
	hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
	hpcd_USB_OTG_FS.Init.vbus_sensing_enable = DISABLE;
	if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USB_OTG_FS_Init 2 */

	/* USER CODE END USB_OTG_FS_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

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
	HAL_GPIO_WritePin(GPIOI, EX_SPI_NSS_Pin|CS_ICLX_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(BLE_SPI_CS_GPIO_Port, BLE_SPI_CS_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(CS_DWB_GPIO_Port, CS_DWB_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin : INT_HTS_Pin */
	GPIO_InitStruct.Pin = INT_HTS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(INT_HTS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : WIFI_FLOW_Pin SD_DETECT_Pin INT_EX_Pin INT_EXG5_Pin */
	GPIO_InitStruct.Pin = WIFI_FLOW_Pin|SD_DETECT_Pin|INT_EX_Pin|INT_EXG5_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/*Configure GPIO pins : PC11 PC10 PC12 PC9
                           PC8 */
	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_9
			|GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : CS_DHCX_Pin CS_DLPC_Pin WIFI_CS_Pin */
	GPIO_InitStruct.Pin = CS_DHCX_Pin|CS_DLPC_Pin|WIFI_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	/*Configure GPIO pins : DCDC_2_EN_Pin SPI2_MISO_p2_Pin */
	GPIO_InitStruct.Pin = DCDC_2_EN_Pin|SPI2_MISO_p2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	/*Configure GPIO pins : C_EN_Pin INT_EXD15_Pin BLE_RST_Pin WIFI_BOOT_Pin */
	GPIO_InitStruct.Pin = C_EN_Pin|INT_EXD15_Pin|BLE_RST_Pin|WIFI_BOOT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pins : EX_SPI_NSS_Pin CS_ICLX_Pin */
	GPIO_InitStruct.Pin = EX_SPI_NSS_Pin|CS_ICLX_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	/*Configure GPIO pins : LED1_Pin BOOT0_Pin LED2_Pin STSAFE_RESET_Pin */
	GPIO_InitStruct.Pin = LED1_Pin|BOOT0_Pin|LED2_Pin|STSAFE_RESET_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	/*Configure GPIO pin : BLE_SPI_CS_Pin */
	GPIO_InitStruct.Pin = BLE_SPI_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BLE_SPI_CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : USR_BUTTON_Pin WIFI_NOTIFY_Pin */
	GPIO_InitStruct.Pin = USR_BUTTON_Pin|WIFI_NOTIFY_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pins : INT1_DHCX_Pin NFC_INT_Pin */
	GPIO_InitStruct.Pin = INT1_DHCX_Pin|NFC_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : GPIO2_EX_Pin BLE_TEST9_Pin BLE_TEST8_Pin */
	GPIO_InitStruct.Pin = GPIO2_EX_Pin|BLE_TEST9_Pin|BLE_TEST8_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/*Configure GPIO pins : INT1_DLPC_Pin INT2_DLPC_Pin INT_MAG_Pin INT_STT_Pin
                           INT1_ICLX_Pin INT2_DHCX_Pin BLE_INT_Pin INT2_ICLX_Pin
                           INT1_DWB_Pin */
	GPIO_InitStruct.Pin = INT1_DLPC_Pin|INT2_DLPC_Pin|INT_MAG_Pin|INT_STT_Pin
			|INT1_ICLX_Pin|INT2_DHCX_Pin|BLE_INT_Pin|INT2_ICLX_Pin
			|INT1_DWB_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/*Configure GPIO pin : PD2 */
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pins : SW_SEL_Pin CHRG_Pin */
	GPIO_InitStruct.Pin = SW_SEL_Pin|CHRG_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : uC_ADC_BATT_Pin ADC_EX_Pin */
	GPIO_InitStruct.Pin = uC_ADC_BATT_Pin|ADC_EX_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : CS_DWB_Pin */
	GPIO_InitStruct.Pin = CS_DWB_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(CS_DWB_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : BUTTON_PWR_Pin */
	GPIO_InitStruct.Pin = BUTTON_PWR_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(BUTTON_PWR_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : SPI2_MOSI_p2_Pin GPIO1_EX_Pin */
	GPIO_InitStruct.Pin = SPI2_MOSI_p2_Pin|GPIO1_EX_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : LDO_EN_Pin WIFI_EN_Pin */
	GPIO_InitStruct.Pin = LDO_EN_Pin|WIFI_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	HAL_NVIC_SetPriority(EXTI1_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	HAL_NVIC_SetPriority(EXTI2_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

	HAL_NVIC_SetPriority(EXTI3_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);

	HAL_NVIC_SetPriority(EXTI4_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI5_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI5_IRQn);

	HAL_NVIC_SetPriority(EXTI6_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI6_IRQn);

	HAL_NVIC_SetPriority(EXTI7_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI7_IRQn);

	HAL_NVIC_SetPriority(EXTI8_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI8_IRQn);

	HAL_NVIC_SetPriority(EXTI9_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI9_IRQn);

	HAL_NVIC_SetPriority(EXTI10_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI10_IRQn);

	HAL_NVIC_SetPriority(EXTI11_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI11_IRQn);

	HAL_NVIC_SetPriority(EXTI13_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI13_IRQn);

	HAL_NVIC_SetPriority(EXTI14_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI14_IRQn);

}

/* USER CODE BEGIN 4 */
/**
 * @brief CRC Initialization Function
 * @param None
 * @retval None
 */
static void MX_CRC_Init(void)
{

	/* USER CODE BEGIN CRC_Init 0 */

	/* USER CODE END CRC_Init 0 */

	/* USER CODE BEGIN CRC_Init 1 */

	/* USER CODE END CRC_Init 1 */
	hcrc.Instance = CRC;
	hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
	hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
	hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
	hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
	hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
	if (HAL_CRC_Init(&hcrc) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN CRC_Init 2 */

	/* USER CODE END CRC_Init 2 */

}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
