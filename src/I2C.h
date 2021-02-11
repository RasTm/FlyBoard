#include "stm32f4xx.h"

void write_byte(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t data);
void multi_byte_write(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t *data, uint8_t byte_count);

void read_byte(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t *data);
void multi_byte_read(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t *data, uint8_t byte_count);

void write_byte_small(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t data);
void multi_byte_write_small(uint8_t dev_addr, uint8_t *data, uint8_t byte_count);

void read_byte_small(uint8_t dev_addr, uint8_t *data);
void multi_byte_read_small(uint8_t dev_addr, uint8_t *data, uint8_t byte_count);

void multi_byte_read_extend(uint8_t dev_addr, uint16_t dev_reg_addr, uint8_t *data, uint8_t byte_count);
