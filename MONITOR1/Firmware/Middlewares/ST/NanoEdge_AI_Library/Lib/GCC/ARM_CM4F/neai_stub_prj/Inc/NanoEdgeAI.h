/**
 * NanoEdge AI header file
 */

/* Includes */
#include <stdint.h>

/* Define */
#define NEAI_ID "st_stub21212121212121212"
#define AXIS_NUMBER 3
#define DATA_INPUT_USER 1024

/* Function prototypes */
#ifdef __cplusplus
extern "C" {
#endif
	void NanoEdgeAI_initialize(void);
	uint8_t NanoEdgeAI_learn(float data_input[]);
	uint8_t NanoEdgeAI_detect(float data_input[]);
	void NanoEdgeAI_set_sensitivity(float sensitivity);
	float NanoEdgeAI_get_sensitivity(void);
	void NanoEdgeAI_forecasting_set_limit(float min_value, float max_value);
	void NanoEdgeAI_forecasting_push(float value);
	void NanoEdgeAI_forecasting_get(float forecast[], uint32_t n_step);
#ifdef __cplusplus
}
#endif
