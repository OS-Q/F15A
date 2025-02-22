#include "soc/dport_reg.h"
#include "sdkconfig.h"
#include "esp_err.h"
#include "eri.h"
#include "xtensa-debug-module.h"


typedef enum {
    TRAX_DOWNCOUNT_WORDS,
    TRAX_DOWNCOUNT_INSTRUCTIONS
} trax_downcount_unit_t;

typedef enum {
    TRAX_ENA_NONE = 0,
    TRAX_ENA_PRO,
    TRAX_ENA_APP,
    TRAX_ENA_PRO_APP,
    TRAX_ENA_PRO_APP_SWAP
} trax_ena_select_t;


/**
 * @brief  Enable the trax memory blocks to be used as Trax memory.
 *
 * @param  pro_cpu_enable : true if Trax needs to be enabled for the pro CPU
 * @param  app_cpu_enable : true if Trax needs to be enabled for the pro CPU
 * @param  swap_regions : Normally, the pro CPU writes to Trax mem block 0 while
 *                        the app cpu writes to block 1. Setting this to true
 *                        inverts this.
 *
 * @return esp_err_t. Fails with ESP_ERR_NO_MEM if Trax enable is requested for 2 CPUs 
 *                    but memmap only has room for 1, or if Trax memmap is disabled 
 *                    entirely.
 */
int esp_trax_enable(trax_ena_select_t ena);

/**
 * @brief  Start a Trax trace on the current CPU
 *
 * @param  units_until_stop : Set the units of the delay that gets passed to
 *              esp_trax_trigger_traceend_after_delay. One of TRAX_DOWNCOUNT_WORDS
 *              or TRAX_DOWNCOUNT_INSTRUCTIONS.
 *
 * @return esp_err_t. Fails with ESP_ERR_NO_MEM if Trax is disabled.
 */
int esp_trax_start_trace(trax_downcount_unit_t units_until_stop);


/**
 * @brief  Trigger a Trax trace stop after the indicated delay. If this is called
 *         before and the previous delay hasn't ended yet, this will overwrite
 *         that delay with the new value. The delay will always start at the time
 *         the function is called.
 *
 * @param  delay : The delay to stop the trace in, in the unit indicated to 
 *              esp_trax_start_trace. Note: the trace memory has 4K words available.
 *
 * @return esp_err_t
 */
int esp_trax_trigger_traceend_after_delay(int delay);


