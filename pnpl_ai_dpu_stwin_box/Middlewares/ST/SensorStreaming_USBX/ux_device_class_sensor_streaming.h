

/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** USBX Component                                                        */ 
/**                                                                       */
/**   Device Sensor Streaming Class                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


#ifndef UX_DEVICE_CLASS_SENSOR_STREAMING_H
#define UX_DEVICE_CLASS_SENSOR_STREAMING_H

#include "ux_api.h"
#include <stdint.h>

extern UCHAR _ux_system_slave_class_sensor_streaming_name[];
#ifndef SS_CLASS_THREAD_PRIO
#define SS_CLASS_THREAD_PRIO 1
#endif

#ifndef SS_CLASS_THREAD_STACK_SIZE
#define SS_CLASS_THREAD_STACK_SIZE 4096
#endif

#ifndef SS_BULKIN_THREAD_PRIO
#define SS_BULKIN_THREAD_PRIO 3
#endif

#ifndef SS_BULKIN_THREAD_STACK_SIZE
#define SS_BULKIN_THREAD_STACK_SIZE 4096
#endif

#ifndef N_CHANNELS_MAX
#define N_CHANNELS_MAX                                13U
#endif

/* Define Storage Class USB Class constants.  */

#define N_IN_ENDPOINTS                                5U
#define N_OUT_ENDPOINTS                               1U


#define DATA_IN_EP1                                   0x81  /* EP1 for data IN */
#define DATA_IN_EP2                                   0x82  /* EP2 for data IN */
#define DATA_IN_EP3                                   0x83  /* EP3 for data IN */
#define DATA_IN_EP4                                   0x84  /* EP4 for data IN */
#define DATA_IN_EP5                                   0x85  /* EP5 for data IN */

#define DATA_OUT_EP1                                  0x06  /* EP1 for data OUT */

#define SS_WCID_DATA_FS_MAX_PACKET_SIZE               64U  /* Endpoint IN & OUT Packet size */

#define UX_SLAVE_CLASS_SENSOR_STREAMING_CLASS         0U
#define UX_SLAVE_CLASS_SENSOR_STREAMING_SUBCLASS      0U
#define UX_SLAVE_CLASS_SENSOR_STREAMING_PROTOCOL      0U

#define UX_DEVICE_CLASS_SENSOR_STREAMING_PACKET_SIZE  64U

#define STREAMING_STATUS_IDLE                         0x00U
#define STREAMING_STATUS_STARTED                      0x01U
#define STREAMING_STATUS_STOPPING                     0x02U

#define UX_DEVICE_CLASS_SENSOR_STREAMING_WRITE_EVENT  0x01U

/* Define Slave SENSOR_STREAMING Class Calling Parameter structure */

typedef struct UX_SLAVE_CLASS_SENSOR_STREAMING_PARAMETER_STRUCT
{
	VOID      (*ux_slave_class_sensor_streaming_instance_activate)(VOID *, VOID *);
	VOID      (*ux_slave_class_sensor_streaming_instance_deactivate)(VOID *, VOID *);
	int8_t    (*ux_slave_class_sensor_streaming_instance_control)(VOID *, uint8_t isHostToDevice, uint8_t cmd, uint16_t wValue, uint16_t wIndex, uint8_t *pbuf, uint16_t length);
	VOID       *app_obj_ctrl;
} UX_SLAVE_CLASS_SENSOR_STREAMING_PARAMETER;

/* Define Slave Data Pump Class structure.  */

typedef struct
{
	uint8_t           *rx_buffer;
	uint32_t          rx_length;
	volatile uint32_t rx_state;
	uint8_t           *tx_buffer[N_CHANNELS_MAX];
	uint32_t          tx_buff_idx[N_CHANNELS_MAX];
	volatile uint8_t  tx_buff_status[N_CHANNELS_MAX];
	volatile uint8_t  tx_buff_reset[N_CHANNELS_MAX];
	uint16_t          ch_data_size[N_CHANNELS_MAX];
	uint8_t           ep_map[N_CHANNELS_MAX];
	uint8_t           streaming_status;
}STREAMING_HandleTypeDef;

typedef struct UX_THREAD_PARAM_BULKIN_STRUCT
{
	UX_SLAVE_ENDPOINT *endpoint;
	TX_SEMAPHORE      semaphore;
	volatile uint8_t  last_packet_sent;
	volatile uint32_t tx_state;
	uint8_t           *tx_buffer;
	uint32_t          tx_size;
} UX_THREAD_EP_BULKIN_PARAM;

typedef struct UX_SLAVE_CLASS_SENSOR_STREAMING_BULKIN_STRUCT
{
	UX_THREAD                    thread;
	UCHAR                        *thread_stack;
	UX_THREAD_EP_BULKIN_PARAM    ep_param;
} UX_SLAVE_CLASS_SENSOR_STREAMING_BULKIN;


typedef struct UX_SLAVE_CLASS_SENSOR_STREAMING_STRUCT
{
	UX_SLAVE_INTERFACE                              *ux_slave_class_sensor_streaming_interface;
	UX_SLAVE_CLASS_SENSOR_STREAMING_PARAMETER       *ux_slave_class_sensor_streaming_parameter;
	UX_SLAVE_CLASS_SENSOR_STREAMING_BULKIN           ux_slave_class_sensor_streaming_bulkin[N_IN_ENDPOINTS];
	UX_SLAVE_ENDPOINT                               *ux_slave_class_sensor_streaming_bulkout_endpoint;
	ULONG                                            ux_slave_class_sensor_streaming_alternate_setting;
	UX_EVENT_FLAGS_GROUP                             ux_slave_class_sensor_streaming_event_flags_group;
	STREAMING_HandleTypeDef                         *hwcid;
} UX_SLAVE_CLASS_SENSOR_STREAMING;




UINT _ux_device_class_sensor_streaming_initialize(UX_SLAVE_CLASS_COMMAND *command);
UINT _ux_device_class_sensor_streaming_activate(UX_SLAVE_CLASS_COMMAND *command);
UINT _ux_device_class_sensor_streaming_deactivate(UX_SLAVE_CLASS_COMMAND *command);
UINT _ux_device_class_sensor_streaming_entry(UX_SLAVE_CLASS_COMMAND *command);
UINT _ux_device_class_sensor_streaming_read(UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, UCHAR *buffer,
                                            ULONG requested_length, ULONG *actual_length);
UINT _ux_device_class_sensor_streaming_write(UX_SLAVE_ENDPOINT *ep, UCHAR *buffer,
                                             ULONG requested_length, ULONG *actual_length);
UINT _ux_device_class_sensor_streaming_change(UX_SLAVE_CLASS_COMMAND *command);
VOID _ux_device_class_sensor_streaming_thread(ULONG sensor_streaming_class);
UINT _ux_device_class_sensor_streaming_control_request(UX_SLAVE_CLASS_COMMAND *command);
VOID _ux_device_class_sensor_streaming_bulkin_entry(ULONG ss);


/* Define Device SENSOR_STREAMING Class API prototypes.  */
UINT ux_device_class_sensor_streaming_SetTransmissionEP        (UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, uint8_t id, uint8_t ep);
UINT ux_device_class_sensor_streaming_SetRxDataBuffer          (UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, uint8_t  *ptr);
UINT ux_device_class_sensor_streaming_SetTxDataBuffer          (UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, uint8_t ch_number, uint8_t * ptr, uint16_t size);
UINT ux_device_class_sensor_streaming_CleanTxDataBuffer        (UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, uint8_t ch_number);
UINT ux_device_class_sensor_streaming_FillTxDataBuffer         (UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming, uint8_t ch_number, uint8_t * buf, uint32_t size);
UINT ux_device_class_sensor_streaming_StartStreaming           (UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming);
UINT ux_device_class_sensor_streaming_StopStreaming            (UX_SLAVE_CLASS_SENSOR_STREAMING *sensor_streaming);
UINT ux_device_class_sensor_streaming_stack_class_register     (void *ss_class_parameter);

#endif
