/*******************************************************************************
  PIC32MZW Wireless Driver Header File

  Company:
    Microchip Technology Inc.

  File Name:
    wdrv_pic32mzw.h

  Summary:
    PIC32MZW Wireless Driver Header File

  Description:
    The PIC32MZW device driver provides an interface to manage the PIC32MZW.
    This file contains the main driver descriptor structure.

    Other API's are provided in other header files, specifically:
      bssfind     - BSS scan functionality.
      softap      - Soft-AP mode.
      sta         - Infrastructure stations mode.

      authctx     - Authentication context management.
      bssctx      - BSS context management.
      httpprovctx - HTTP provisioning context management.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2019 released Microchip Technology Inc. All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END

#ifndef _WDRV_PIC32MZW_H
#define _WDRV_PIC32MZW_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "system_definitions.h"
#include "drv_pic32mzw1.h"
#include "tcpip/src/link_list.h"
#include "wdrv_pic32mzw_bssfind.h"
#include "wdrv_pic32mzw_assoc.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility
    extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************

#define WDRV_PIC32MZW_NUM_ASSOCS    1

typedef bool (*WDRV_PIC32MZW_REG_DOMAIN_CALLBACK)
(
	uint8_t *buf,
	size_t size
);


// *****************************************************************************
/*  PIC32MZW Control Driver Descriptor

  Summary:
    Driver descriptor for the PIC32MZW Control driver.

  Description:
    Structure containing the system level descriptor for the PIC32MZW driver.
    This structure is initialized by a call to WDRV_PIC32MZW_MACInitialize.

  Remarks:
    None.
*/

typedef struct _WDRV_PIC32MZW_CTRLDCPT
{
    /* Primary driver handle. */
    DRV_HANDLE handle;

    /* Flag indicating if this instance is operating as s station or soft-AP. */
    bool isAP;

    /* Flag indicating if a connection has been established. */
    bool isConnected;

    /* Flag indicating if a connection attempt is in progress. */
    bool isConnecting;

    /* Flag indicating if a BSS scan is currently in progress. */
    bool scanInProgress;

    /* Current index of the BSS scan results. */
    uint8_t scanIndex;

    /* Time spent on each active channel probing for BSS's. */
    uint16_t scanActiveScanTime;

    /* Time spent on each passive channel listening for beacons. */
    uint16_t scanPassiveListenTime;

    /* Access semaphore for MAC firmware library. */
    OSAL_SEM_HANDLE_TYPE drvAccessSemaphore;

    /* Bitmap of enabled 2.4GHz channels for scanning. */
    WDRV_PIC32MZW_CHANNEL24_MASK scanChannelMask24;

    /* Current operating channel. */
    WDRV_PIC32MZW_CHANNEL_ID opChannel;

    /* Association information. */
    WDRV_PIC32MZW_ASSOC_INFO assocInfo[WDRV_PIC32MZW_NUM_ASSOCS];

    /* Callback to use for BSS find operations. */
    WDRV_PIC32MZW_BSSFIND_NOTIFY_CALLBACK pfBSSFindNotifyCB;

    /* Callback to use for notifying changes in connection state. */
    WDRV_PIC32MZW_BSSCON_NOTIFY_CALLBACK pfConnectNotifyCB;

    /* Callback to use for retrieving association RSSI information from the PIC32MZW. */
    WDRV_PIC32MZW_ASSOC_RSSI_CALLBACK pfAssociationRSSICB;

	/* Callback to use for retrieving association RSSI information from the PIC32MZW. */
    WDRV_PIC32MZW_REG_DOMAIN_CALLBACK pfRegulatoryDomainCB;
		
} WDRV_PIC32MZW_CTRLDCPT;

// *****************************************************************************
/*  PIC32MZW MAC Driver Descriptor

  Summary:
    Driver descriptor for the PIC32MZW MAC driver.

  Description:
    Structure containing the system level descriptor for the PIC32MZW driver.
    This structure is initialized by a call to WDRV_PIC32MZW_MACInitialize.

  Remarks:
    None.
*/

typedef struct _WDRV_PIC32MZW_MACDCPT
{
    /* Primary driver handle. */
    DRV_HANDLE handle;

    /* Event function pointer for signalling TCP/IP stack. */
    TCPIP_MAC_EventF eventF;

    /* Event function parameters to pass to TCP/IP stack. */
    const void *eventParam;

    /* Mask of currently enabled events to signal. */
    TCPIP_MAC_EVENT eventMask;

    /* Current events to be signalled to stack. */
    TCPIP_MAC_EVENT events;

    /* Access semaphore to protect updates to event state. */
    OSAL_SEM_HANDLE_TYPE eventSemaphore;

    /* Linked list of receive Ethernet packets. */
    PROTECTED_SINGLE_LIST ethRxPktList;
} WDRV_PIC32MZW_MACDCPT;

// *****************************************************************************
/*  PIC32MZW Driver Descriptor

  Summary:
    The defines the PIC32MZW driver descriptor.

  Description:
    This data type defines the system level descriptor for the PIC32MZW driver.
    This structure is initialized by a call to WDRV_PIC32MZW_Initialize.

  Remarks:
    None.
*/

typedef struct _WDRV_PIC32MZW_DCPT
{
    /* Flag indicating if the driver has been initialized. */
    bool isInit;

    /* Driver status which can be queried via WDRV_PIC32MZW_Status. */
    SYS_STATUS sysStat;

    /* Flag indicating if this instance of the driver has been opened by
        a call to WDRV_PIC32MZW_Open. */
    bool isOpen;

    /* Pointer to instance specific descriptor (Control). */
    WDRV_PIC32MZW_CTRLDCPT  *pCtrl;

    /* Pointer to instance specific descriptor (MAC). */
    WDRV_PIC32MZW_MACDCPT   *pMac;
} WDRV_PIC32MZW_DCPT;

// *****************************************************************************
/*  

  Summary:
    

  Description:
    

  Remarks:
    None.
*/

typedef struct _WDRV_PIC32MZW_MAC_ACCESS_COUNTER
{
    /* Number of memory allocation function calls. */
    uint32_t alloc;
    
    /* Number of memory allocation free function call. */
    uint32_t free;
    
    /* Currently allocated memory size. */
    uint32_t allocSize;
    
    /* Currently unallocated memory size. */
    uint32_t freeSize;
} WDRV_PIC32MZW_MAC_ACCESS_COUNTER;

// *****************************************************************************
/*  

  Summary:
    

  Description:
    

  Remarks:
    None.
*/

typedef struct _WDRV_PIC32MZW_MAC_MEM_STATISTICS
{
    /* Number of transmit and receive frames handled to/from the driver layer. */
    struct
    {
        uint32_t tx;
        uint32_t rx;
    } pkt;

    /* Packet memory allocation counters.
     * Index 0 to 4 are the order of PRIORITY_LEVEL. */
    WDRV_PIC32MZW_MAC_ACCESS_COUNTER pri[5];

    /* Memory allocation counters. */
    WDRV_PIC32MZW_MAC_ACCESS_COUNTER mem;
} WDRV_PIC32MZW_MAC_MEM_STATISTICS;

// *****************************************************************************
// *****************************************************************************
// Section: PIC32MZW Debugging Routines
// *****************************************************************************
// *****************************************************************************

//*******************************************************************************
/*
  Function:
    void WDRV_PIC32MZW_DebugRegisterCallback
    (
        WDRV_PIC32MZW_DEBUG_PRINT_CALLBACK const pfDebugPrintCallback
    )

  Summary:
    Register callback for debug serial stream.

  Description:
    The debug serial stream provides a printf-like stream of messages from within
    the PIC32MZW driver. The caller can provide a function to be called when
    output is available.

  Precondition:
    None.

  Parameters:
    pfDebugPrintCallback - Function pointer to printf-like function.

  Returns:
    None.

  Remarks:
    None.

 */

void WDRV_PIC32MZW_DebugRegisterCallback(WDRV_PIC32MZW_DEBUG_PRINT_CALLBACK const pfDebugPrintCallback);

// *****************************************************************************
// *****************************************************************************
// Section: PIC32MZW Driver Client Routines
// *****************************************************************************
// *****************************************************************************

//*******************************************************************************
/*
  Function:
    DRV_HANDLE WDRV_PIC32MZW_Open(const SYS_MODULE_INDEX index, const DRV_IO_INTENT intent)

  Summary:
    Opens an instance of the PIC32MZW driver.

  Description:
    Opens an instance of the PIC32MZW driver and returns a handle which must be
    used when calling other driver functions.

  Precondition:
    WDRV_PIC32MZW_Initialize should have been called.

  Parameters:
    index   - Identifier for the driver instance to be opened.
    intent  - Zero or more of the values from the enumeration
                DRV_IO_INTENT ORed together to indicate the intended use
                of the driver

  Returns:
    - Valid handle - if the open function succeeded
    - DRV_HANDLE_INVALID - if an error occurs

  Remarks:
    The index and intent parameters are not used in the current implementation
    and are maintained only for compatibility with the generic driver Open
    function signature.

*/

DRV_HANDLE WDRV_PIC32MZW_Open(const SYS_MODULE_INDEX index, const DRV_IO_INTENT intent);

//*******************************************************************************
/*
  Function:
    void WDRV_PIC32MZW_Close(DRV_HANDLE handle)

  Summary:
    Closes an instance of the PIC32MZW driver.

  Description:
    This is the function that closes an instance of the MAC.
    All per client data is released and the handle can no longer be used
    after this function is called.

  Precondition:
    WDRV_PIC32MZW_Initialize should have been called.
    WDRV_PIC32MZW_Open should have been called to obtain a valid handle.

  Parameters:
    handle  - Client handle obtained by a call to WDRV_PIC32MZW_Open.

  Returns:
    None.

  Remarks:
    None.

*/

void WDRV_PIC32MZW_Close(DRV_HANDLE handle);

// *****************************************************************************
// *****************************************************************************
// Section: PIC32MZW Information Routines
// *****************************************************************************
// *****************************************************************************

//*******************************************************************************
/*
  Function:
    WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_InfoDeviceMACAddressGet
    (
        DRV_HANDLE handle,
        uint8_t *const pMACAddress
    )

  Summary:
    Retrieves the MAC address of the PIC32MZW.

  Description:
    Retrieves the current working MAC address.

  Precondition:
    WDRV_PIC32MZW_Initialize should have been called.
    WDRV_PIC32MZW_Open should have been called to obtain a valid handle.

  Parameters:
    handle      - Client handle obtained by a call to WDRV_PIC32MZW_Open.
    pMACAddress - Pointer to buffer to receive the MAC address.

  Returns:
    WDRV_PIC32MZW_STATUS_OK             - The information has been returned.
    WDRV_PIC32MZW_STATUS_NOT_OPEN       - The driver instance is not open.
    WDRV_PIC32MZW_STATUS_INVALID_ARG    - The parameters were incorrect.

  Remarks:
    None.

*/

WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_InfoDeviceMACAddressGet
(
    DRV_HANDLE handle,
    uint8_t *const pMACAddress
);

//*******************************************************************************
/*
  Function:
    WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_InfoOpChanGet
    (
        DRV_HANDLE handle,
        uint8_t *const pMACAddress
    )

  Summary:
    Retrieves the operating channel of the PIC32MZW.

  Description:
    Retrieves the current operating channel.

  Precondition:
    WDRV_PIC32MZW_Initialize should have been called.
    WDRV_PIC32MZW_Open should have been called to obtain a valid handle.

  Parameters:
    handle  - Client handle obtained by a call to WDRV_PIC32MZW_Open.
    pOpChan - Pointer to variable to receive the operating channel.

  Returns:
    WDRV_PIC32MZW_STATUS_OK             - The information has been returned.
    WDRV_PIC32MZW_STATUS_NOT_OPEN       - The driver instance is not open.
    WDRV_PIC32MZW_STATUS_INVALID_ARG    - The parameters were incorrect.
    WDRV_PIC32MZW_STATUS_NOT_CONNECTED  - No current connection.

  Remarks:
    None.

*/

WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_InfoOpChanGet
(
    DRV_HANDLE handle,
    WDRV_PIC32MZW_CHANNEL_ID *const pOpChan
);

WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_ListRegDomain(DRV_HANDLE handle, const WDRV_PIC32MZW_REG_DOMAIN_CALLBACK pfNotifyCallback); 

WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_GetRegDomain(DRV_HANDLE handle, const WDRV_PIC32MZW_REG_DOMAIN_CALLBACK pfNotifyCallback);

WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_SetRegDomain(DRV_HANDLE handle, const WDRV_PIC32MZW_REG_DOMAIN_CALLBACK pfNotifyCallback, uint8_t *pRDName);

//*******************************************************************************
/*
  Function:
    void WDRV_PIC32MZW_GetStatistics
    (
        DRV_HANDLE handle,
        WDRV_PIC32MZW_MAC_MEM_STATISTICS *pStats
    );

  Summary:
    Retrieves the static data of the PIC32MZW.

  Description:
    Retrieves the static data of the PIC32MZW..

  Precondition:
    WDRV_PIC32MZW_Initialize should have been called.
    WDRV_PIC32MZW_Open should have been called to obtain a valid handle.

  Parameters:
    handle - Client handle obtained by a call to WDRV_PIC32MZW_Open.
    pStats - Pointer to buffer to receive the statistic data.

  Returns:
    WDRV_PIC32MZW_STATUS_OK             - The information has been returned.
    WDRV_PIC32MZW_STATUS_INVALID_ARG    - The parameters were incorrect.

  Remarks:
    None.
*/
#ifdef WDRV_PIC32MZW_STATS_ENABLE
WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_GetStatistics
(
    DRV_HANDLE handle,
    WDRV_PIC32MZW_MAC_MEM_STATISTICS *pStats
);
#endif

    // DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END

#endif /* _WDRV_PIC32MZW_H */