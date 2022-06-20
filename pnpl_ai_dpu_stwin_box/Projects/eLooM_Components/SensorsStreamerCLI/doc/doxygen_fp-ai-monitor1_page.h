/**
 * \page page4 FP-AI-MONITOR1
 *
 * \tableofcontents
 *
 * \section intro1 Introduction
 * This is a placeorder for teh SensorManager API documentation.
 * This section gives a look to the simple demo application delivered with the framework. It has been developed on a
 * [NUCLEO-L452RE](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-l452re.html) board.
 * Despite its simplicity it is a good example to understand how to implement an application with the \ref overview "ODeV framework".
 *
 * The application implements two managed tasks, one to blink the LED with a regular interval (::HelloWorldTask), and another to put the system in low power mode and wake-up it when the user
 * press the push button (::PushButtonTask). The demo uses also a driver to operate the GPIO IP (::NucleoDriver).
 *
 * \ref fig22 "Fig.22" displays how the demo application extends the firmware architecture of the framework. The new objects are highlighted in yellow.
 *
 * \anchor fig22 \image html 22_demo_architecture.png "Fig.22 - Demo firmware architecture" width=900px
 *
 * When the user power on the Nucleo board, the user LED start blinking with a period of 1 second and a duty cycle of 50%. The first time the user press the push button the system is armed.
 * This means that from now on every successive press of the push button will trigger a transaction from RUN to SLEEP_1 or vice versa. When the system is in SLEEP_1 the LED stops
 * blinking. When the system is in RUN the LED starts blinking again. By default in SLEEP_1 the APMH put the MCU in STOP2, but the user can change the low power mode
 * to STANDBY by modifying the SYS_CFG_DEF_PM_HELPER_STANDBY parameters in the sysconfig.h file.
 *
 * \section app_tasks Application Tasks
 * This section gives a look to the features exported by the application managed tasks. There are two application tasks plus one system task and two FreeRTOS daemon tasks.
 *
 * | Index | Task name           | Task priority                | Note                    |
 * | :---: | :------------------ | ---------------------------: | :---------------------- |
 * | 1     | HelloWorld task     | tskIDLE_PRIORITY+1       = 1 | Managed task            |
 * | 2     | PushButton task     | tskIDLE_PRIORITY+2       = 2 | Managed task Ex         |
 * |       |                     |                              |                         |
 * | 3     | INIT task           | configMAX_PRIORITIES - 1 = 4 | system task             |
 * | 4     | Idle task           | tskIDLE_PRIORITY         = 0 | FreeRTOS task           |
 * | 5     | Timer service task  | configTIMER_TASK_PRIORITY= 3 | FreeRTOS task           |
 *
 * The scheduler executes the ready task with highest priority. The system is configured with 5 priority levels, from the lowest (0) to the highest (4).
 * The highest priority level (4) is not used by the managed tasks, because it is reserved to a special system task, the INIT task.
 * Each task is designed to respond to events generated by the MCU peripherals IRQ or by software events, for example messages.
 *
 * | Index | IRQ line       | IRQ  priority  | Note                                                               |
 * | :---: | :------------- | :------------: | :----------------------------------------------------------------- |
 * | 1     | EXTI15_10_IRQn | 13             | EXTI 13 used to detect the push button events                      |
 * |       |                |                |                                                                    |
 * | 2     | TIM6_IRQn      | 14             | High frequency tick to generate the runtime statistics in DEBUG    |
 * | 3     | TIM3_IRQn      | 15             | generates the HAL 1ms tick                                         |
 * | 4     | SysTick_IRQn   | 15             | generate the scheduler 1ms tick                                    |
 * | 5     | PendSV_IRQn    | 15             | Used by FreeTOS                                                    |
 *
 * Note that, despite the task priorities, the Cortex-M defines the IRQ priorities from the lowest 15 to the highest 0.
 *
 * \subsection HelloWorldTask Hello World Task
 * This managed task is responsible to blink the user LED with a fixed period of 1 second and a duty cycle of 50%.
 *
 * \subsubsection HelloWorldTask_LL Low Level Driver
 * In the Low Level API layer this demo introduce one driver object, the ::NucleoDriver. This driver allow the application tasks to
 * operate the hardware (push button and LED). Only one driver is implemented for this demo and it is shared between the managed tasks.
 *
 * \subsubsection HelloWorldTask_SL Service Layer
 * This demo, due to its simplicity, does not provide any service in this layer.
 *
 * \subsubsection HelloWorldTask_AL Application Layer
 * The ::HelloWorldTask extends the ::AManagedTask interface. This is because the task feature is well modeled by this base class a control loop that implements a short step
 * in order to keep the system responsive.
 *
 * This managed task allocate the driver (::NucleoDriver) in the HelloWorldTask_vtblHardwareInit() virtual function and it exports the API to share the driver object
 * with the other managed tasks - HelloWorldTaskGetDriver().
 *
 * \subsection PushButtonTask Push Button Task
 * This managed task is responsible to handle the user button and to generate the ::SysEvent in order to trigger the power mode transactions.
 *
 * \subsubsection PushButtonTask_LL Low Level Driver
 * This managed task uses the driver allocated by the ::HelloWorldTask. The link between the driver and the task is done in the SysOnStartApplication()
 * function, to be sure that all the resources of the demo application have been allocated and initialized.
 *
 * \subsubsection PushButtonTask_SL Service Layer
 * This demo, due to its simplicity, does not provide any service in this layer.
 *
 * \subsubsection PushButtonTask_AL Application Layer'
 * The ::PushButtonTask extends the ::AManagedTaskEx interface. This interface, that in turn extends the ::AManagedTask, allows a managed task to block its
 * execution for an indefinite period of time (portMAX_DELAY). This exactly matches the semantic of the task, that has to wait for the button pressed and the
 * button released events.
 *
 * \section build_configs Build Configurations
 * There are four build configurations for the demo than can be selected from the _Manage Configuration_ dialog of Atollic TrueSTUDIO
 *
 * \anchor fig23 \image html 23_demo_build_configs.png "Fig.23 - Builds Configuration"
 *
 * - **Debug**: this configuration enables the Debug log via UART. For more information see the SYS_DEBUGF3() and SysDebugInit() functions.
 * It also allow to debug the application in low power mode.
 * - **Release**: this configuration optimize the final binary file by using the -O2 option and by removing the debug features.
 * - **TEST_R**: this configuration is like the Release but keep enabled the Debug log.
 * - **Trace**: this configuration enable the [Tracealyzer 4](https://percepio.com/tracealyzer/) support. The trace is configured in streaming mode, so the user has to start a
 * streaming trace after lunching this debug configuration. To use TA4 in streaming mode a J-Link probe is needed. It is possible to convert
 * the embedded ST-Link of the NUCLEO board to emulate the J-Link. For more information
 * look at this [web page](https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/).
 */
