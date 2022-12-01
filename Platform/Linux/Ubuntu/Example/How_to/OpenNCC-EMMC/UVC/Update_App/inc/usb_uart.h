
#ifndef USB_UART_H_
#define USB_UART_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#ifdef __cplusplus
extern "C" {
#endif

enum{
    UART_WRITE_READY = 0,
    EMMC_CTRL_UPDATE_BOOT,
    EMMC_CTRL_UPDATE_APP,
    EMMC_CTRL_UPDATE_BLOB,
    EMMC_CTRL_UPDATE_BLOB2,
    CAM_CTRL_AI_RUN,
};

#define MAX_UART_EP      (10)
#define USB_UART_HDR     (0xabcd5a5a)
#define UART_MAX_SIZE    (8*1024*1024)
#define MAX_UART_BLOCK_SIZE   (4000)


typedef struct
{
    int  flag;
    int  ep;
    int  size;
} UartHeader_t;

typedef struct
{
    int cmd;
    int val;
} UartCmd_t;

typedef void(*uartCb)(unsigned char *data, int size);

/**
 * @brief Register uart device
 */
void registerUartCb(uartCb cb, unsigned int ep);

/**
 * @brief Initialize uart channel
 */
void uart_init(void);

/**
 * @brief Update main application(firmware)
 */
void update_app();

/**
 * @brief Update first AI model
 */
void update_blob();

/**
 * @brief Update second AI model
 */
void update_blob2();

/**
 * @brief Control AI function
 * @param status 0 disable
 *               1 enable
 */
void AI_Run(int status);

#ifdef __cplusplus
}
#endif

#endif
