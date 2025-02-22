// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _ROM_CACHE_H_
#define _ROM_CACHE_H_

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup uart_apis, uart configuration and communication related apis
  * @brief uart apis
  */

/** @addtogroup uart_apis
  * @{
  */

/**
  * @brief Initialise cache mmu, mark all entries as invalid.
  *        Please do not call this function in your SDK application.
  *
  * @param  int cpu_no : 0 for PRO cpu, 1 for APP cpu.
  *
  * @return None
  */
void mmu_init(int cpu_no);

/**
  * @brief Set Flash-Cache mmu mapping.
  *        Please do not call this function in your SDK application.
  *
  * @param  int cpu_no : CPU number, 0 for PRO cpu, 1 for APP cpu.
  *
  * @param  int pod : process identifier. Range 0~7.
  *
  * @param  unsigned int vaddr : virtual address in CPU address space.
  *                              Can be IRam0, IRam1, IRom0 and DRom0 memory address.
  *                              Should be aligned by psize.
  *
  * @param  unsigned int paddr : physical address in Flash.
  *                              Should be aligned by psize.
  *
  * @param  int psize : page size of flash, in kilobytes. Should be 64 here.
  *
  * @param  int num : pages to be set.
  *
  * @return unsigned int: esp_error status
  *                   0 : mmu set success
  *                   1 : vaddr or paddr is not aligned
  *                   2 : pid esp_error
  *                   3 : psize esp_error
  *                   4 : mmu table to be written is out of range
  *                   5 : vaddr is out of range
  */
unsigned int cache_flash_mmu_set(int cpu_no, int pid, unsigned int vaddr, unsigned int paddr,  int psize, int num);

/**
  * @brief Set Ext-SRAM-Cache mmu mapping.
  *        Please do not call this function in your SDK application.
  *
  * @param  int cpu_no : CPU number, 0 for PRO cpu, 1 for APP cpu.
  *
  * @param  int pod : process identifier. Range 0~7.
  *
  * @param  unsigned int vaddr : virtual address in CPU address space.
  *                              Can be IRam0, IRam1, IRom0 and DRom0 memory address.
  *                              Should be aligned by psize.
  *
  * @param  unsigned int paddr : physical address in Ext-SRAM.
  *                              Should be aligned by psize.
  *
  * @param  int psize : page size of flash, in kilobytes. Should be 32 here.
  *
  * @param  int num : pages to be set.
  *
  * @return unsigned int: esp_error status
  *                   0 : mmu set success
  *                   1 : vaddr or paddr is not aligned
  *                   2 : pid esp_error
  *                   3 : psize esp_error
  *                   4 : mmu table to be written is out of range
  *                   5 : vaddr is out of range
  */
unsigned int cache_sram_mmu_set(int cpu_no, int pid, unsigned int vaddr, unsigned int paddr, int psize, int num);

/**
  * @brief Initialise cache access for the cpu.
  *        Please do not call this function in your SDK application.
  *
  * @param  int cpu_no : 0 for PRO cpu, 1 for APP cpu.
  *
  * @return None
  */
void Cache_Read_Init(int cpu_no);

/**
  * @brief Flush the cache value for the cpu.
  *        Please do not call this function in your SDK application.
  *
  * @param  int cpu_no : 0 for PRO cpu, 1 for APP cpu.
  *
  * @return None
  */
void Cache_Flush(int cpu_no);

/**
  * @brief Disable Cache access for the cpu.
  *        Please do not call this function in your SDK application.
  *
  * @param  int cpu_no : 0 for PRO cpu, 1 for APP cpu.
  *
  * @return None
  */
void Cache_Read_Disable(int cpu_no);

/**
  * @brief Enable Cache access for the cpu.
  *        Please do not call this function in your SDK application.
  *
  * @param  int cpu_no : 0 for PRO cpu, 1 for APP cpu.
  *
  * @return None
  */
void Cache_Read_Enable(int cpu_no);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* _ROM_CACHE_H_ */
