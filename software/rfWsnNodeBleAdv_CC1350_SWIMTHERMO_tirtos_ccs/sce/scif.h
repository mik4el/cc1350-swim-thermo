/** \mainpage Driver Overview
  *
  * \section section_drv_info Driver Information
  * This Sensor Controller Interface driver has been generated by the Texas Instruments Sensor Controller
  * Studio tool:
  * - <b>Project name</b>:     LMT70 simple ADC
  * - <b>Project file</b>:     C:/Users/mik4el/Documents/GitHub/cc1350-swim-thermo/software/rfWsnNodeBleAdv_CC1350_SWIMTHERMO_tirtos_ccs/sce/adc_sample.scp
  * - <b>Code prefix</b>:      -
  * - <b>Operating system</b>: TI-RTOS
  * - <b>Tool version</b>:     2.4.0.793
  * - <b>Tool patches</b>:     None
  * - <b>Target chip</b>:      CC1350, package QFN32 4x4 RSM, revision -
  * - <b>Created</b>:          2019-05-16 12:18:20.259
  * - <b>Computer</b>:         DESKTOP-SB0S8NL
  * - <b>User</b>:             mik4el
  *
  * No user-provided resource definitions were used to generate this driver.
  *
  * No user-provided procedure definitions were used to generate this driver.
  *
  * Do not edit the generated source code files other than temporarily for debug purposes. Any
  * modifications will be overwritten by the Sensor Controller Studio when generating new output.
  *
  * \section section_drv_modules Driver Modules
  * The driver is divided into three modules:
  * - \ref module_scif_generic_interface, providing the API for:
  *     - Initializing and uninitializing the driver
  *     - Task control (for starting, stopping and executing Sensor Controller tasks)
  *     - Task data exchange (for producing input data to and consume output data from Sensor Controller
  *       tasks)
  * - \ref module_scif_driver_setup, containing:
  *     - The AUX RAM image (Sensor Controller code and data)
  *     - I/O mapping information
  *     - Task data structure information
  *     - Driver setup data, to be used in the driver initialization
  *     - Project-specific functionality
  * - \ref module_scif_osal, for flexible OS support:
  *     - Interfaces with the selected operating system
  *
  * It is possible to use output from multiple Sensor Controller Studio projects in one application. Only
  * one driver setup may be active at a time, but it is possible to switch between these setups. When
  * using this option, there is one instance of the \ref module_scif_generic_interface and
  * \ref module_scif_osal modules, and multiple instances of the \ref module_scif_driver_setup module.
  * This requires that:
  * - The outputs must be generated using the same version of Sensor Controller Studio
  * - The outputs must use the same operating system
  * - The outputs must use different source code prefixes (inserted into all globals of the
  *   \ref module_scif_driver_setup)
  *
  *
  * \section section_project_info Project Description
  * Sampling the LMT70s on DIO7 and toggle between T_ON1 and T_ON2 on the CC1350 SWIMTHERMO according to
  * TI application note.
  *
  *
  * \subsection section_io_mapping I/O Mapping
  * Task I/O functions are mapped to the following pins:
  * - Simple LMT70 ADC:
  *     - <b>A: Input for LMT70</b>: DIO7
  *     - <b>O: Toggle LMT70 2</b>: DIO6
  *     - <b>O: Toggle LMT70 1</b>: DIO5
  *
  *
  * \section section_task_info Task Description(s)
  * This driver supports the following task(s):
  *
  *
  * \subsection section_task_desc_simple_lmt70_adc Simple LMT70 ADC
  * Sampling the LMT70s on DIO7 and toggle between T_ON1 and T_ON2 on the CC1350 SWIMTHERMO according to
  * TI application note.
  *
  */




/** \addtogroup module_scif_driver_setup Driver Setup
  *
  * \section section_driver_setup_overview Overview
  *
  * This driver setup instance has been generated for:
  * - <b>Project name</b>:     LMT70 simple ADC
  * - <b>Code prefix</b>:      -
  *
  * The driver setup module contains the generated output from the Sensor Controller Studio project:
  * - Location of task control and scheduling data structures in AUX RAM
  * - The AUX RAM image, and the size the image
  * - Task data structure information (location, size and buffer count)
  * - I/O pin mapping translation table
  * - Task resource initialization and uninitialization functions
  * - Hooks for run-time logging
  *
  * @{
  */
#ifndef SCIF_H
#define SCIF_H

#include <stdint.h>
#include <stdbool.h>
#include "scif_framework.h"
#include "scif_osal_tirtos.h"


/// Target chip name
#define SCIF_TARGET_CHIP_NAME_CC1350
/// Target chip package
#define SCIF_TARGET_CHIP_PACKAGE_QFN32_4X4_RSM

/// Number of tasks implemented by this driver
#define SCIF_TASK_COUNT 1

/// Simple LMT70 ADC: Task ID
#define SCIF_SIMPLE_LMT70_ADC_TASK_ID 0


/// Simple LMT70 ADC I/O mapping: Input for LMT70
#define SCIF_SIMPLE_LMT70_ADC_DIO_A_LMT70_IN 7
/// Simple LMT70 ADC I/O mapping: Toggle LMT70 2
#define SCIF_SIMPLE_LMT70_ADC_DIO_O_T_ON2 6
/// Simple LMT70 ADC I/O mapping: Toggle LMT70 1
#define SCIF_SIMPLE_LMT70_ADC_DIO_O_T_ON1 5


// All shared data structures in AUX RAM need to be packed
#pragma pack(push, 2)


/// Simple LMT70 ADC: Task output data structure
typedef struct {
    uint16_t adcValue1; ///< 
    uint16_t adcValue2; ///< 
} SCIF_SIMPLE_LMT70_ADC_OUTPUT_T;


/// Sensor Controller task data (configuration, input buffer(s), output buffer(s) and internal state)
typedef struct {
    struct {
        SCIF_SIMPLE_LMT70_ADC_OUTPUT_T output;
    } simpleLmt70Adc;
} SCIF_TASK_DATA_T;

/// Sensor Controller task generic control (located in AUX RAM)
#define scifTaskData    (*((volatile SCIF_TASK_DATA_T*) 0x400E00EA))


// Initialized internal driver data, to be used in the call to \ref scifInit()
extern const SCIF_DATA_T scifDriverSetup;


// Restore previous struct packing setting
#pragma pack(pop)


// AUX I/O re-initialization functions
void scifReinitTaskIo(uint32_t bvTaskIds);


// RTC-based tick generation control
void scifStartRtcTicks(uint32_t tickStart, uint32_t tickPeriod);
void scifStartRtcTicksNow(uint32_t tickPeriod);
void scifStopRtcTicks(void);


#endif
//@}


// Generated by DESKTOP-SB0S8NL at 2019-05-16 12:18:20.259
