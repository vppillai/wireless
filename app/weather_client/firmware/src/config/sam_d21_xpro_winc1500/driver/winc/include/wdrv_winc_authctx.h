/*******************************************************************************
  WINC Driver Authentication Context Header File

  Company:
    Microchip Technology Inc.

  File Name:
    wdrv_winc_authctx.h

  Summary:
    WINC wireless driver authentication context header file.

  Description:
    This interface manages the authentication contexts which 'wrap' the state
      associated with authentication schemes.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017 released Microchip Technology Inc. All rights reserved.

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

#ifndef _WDRV_WINC_AUTHCTX_H
#define _WDRV_WINC_AUTHCTX_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>

#include "wdrv_winc_common.h"

// *****************************************************************************
// *****************************************************************************
// Section: WINC Driver Authentication Context Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/*  Authentication Types

  Summary:
    List of possible authentication types.

  Description:
    This type defines the possible authentication types.

  Remarks:
    None.
*/

typedef enum
{
    /* Invalid or unknown type. */
    WDRV_WINC_AUTH_TYPE_INVALID = /*DOM-IGNORE-BEGIN*/ M2M_WIFI_SEC_INVALID /*DOM-IGNORE-END*/,

    /* Open authentication. */
    WDRV_WINC_AUTH_TYPE_OPEN = /*DOM-IGNORE-BEGIN*/ M2M_WIFI_SEC_OPEN /*DOM-IGNORE-END*/,

    /* WPA-PSK authentication. */
    WDRV_WINC_AUTH_TYPE_WPA_PSK = /*DOM-IGNORE-BEGIN*/ M2M_WIFI_SEC_WPA_PSK /*DOM-IGNORE-END*/,

    /* WEP authentication. */
    WDRV_WINC_AUTH_TYPE_WEP = /*DOM-IGNORE-BEGIN*/ M2M_WIFI_SEC_WEP /*DOM-IGNORE-END*/,

    /* 802.1x authentication. */
    WDRV_WINC_AUTH_TYPE_802_1X = /*DOM-IGNORE-BEGIN*/ M2M_WIFI_SEC_802_1X /*DOM-IGNORE-END*/,

#ifdef WDRV_WINC_DEVICE_ENTERPRISE_CONNECT
    /* 802.1x authentication (MS-CHAPv2). */
    WDRV_WINC_AUTH_TYPE_802_1X_MSCHAPV2,

    /* 802.1x authentication (TLS). */
    WDRV_WINC_AUTH_TYPE_802_1X_TLS
#endif
} WDRV_WINC_AUTH_TYPE;

// *****************************************************************************
/*  Authentication Context

  Summary:
    Context structure containing information about authentication types.

  Description:
    The context contains the type of authentication as well as any state
      information.

  Remarks:
    None.
*/

typedef struct
{
    /* Authentication type of context. */
    WDRV_WINC_AUTH_TYPE authType;

    /* Union of data structures for each authentication type. */
    union
    {
        /* WPA-PSK authentication state. */
        uint8_t PSK[M2M_MAX_PSK_LEN];

        /* WEP authentication state. */
        struct
        {
            uint8_t idx;
            uint8_t size;
            uint8_t key[WEP_104_KEY_STRING_SIZE+1];
        } WEP;

#ifdef WDRV_WINC_DEVICE_ENTERPRISE_CONNECT
        /* 802.1x (MS-CHAPv2) authentication state. */
        struct
        {
            char domainUserName[M2M_AUTH_1X_USER_LEN_MAX+1];
            uint8_t password[M2M_AUTH_1X_PASSWORD_LEN_MAX];
            uint16_t passwordLength;
            bool visibleUserName;
            bool domainIsFirst;
        } WPAEntMSCHAPv2;

        /* 802.1x (TLS) authentication state. */
        struct
        {
            char domainUserName[M2M_AUTH_1X_USER_LEN_MAX+1];
            const uint8_t *pRSAPrivateKeyModulus;
            uint32_t RSAPrivateKeyModulusLength;
            const uint8_t *pRSAPrivateKeyExponent;
            const uint8_t *pRSACertificate;
            uint32_t RSACertificateLength;
            bool visibleUserName;
            bool domainIsFirst;
        } WPAEntTLS;
#endif
    } authInfo;
} WDRV_WINC_AUTH_CONTEXT;

// *****************************************************************************
// *****************************************************************************
// Section: WINC Driver Authentication Context Routines
// *****************************************************************************
// *****************************************************************************

//*******************************************************************************
/*
  Function:
    bool WDRV_WINC_AuthCtxIsValid(const WDRV_WINC_AUTH_CONTEXT *const pAuthCtx)

  Summary:
    Tests if an authentication context is valid.

  Description:
    Tests the elements of the authentication context to judge if their values are legal.

  Precondition:
    None.

  Parameters:
    pAuthCtx  - Pointer to an authentication context.

  Returns:
    true or false indicating if context is valid.

  Remarks:
    None.

*/

bool WDRV_WINC_AuthCtxIsValid(const WDRV_WINC_AUTH_CONTEXT *const pAuthCtx);

//*******************************************************************************
/*
  Function:
    WDRV_WINC_STATUS WDRV_WINC_AuthCtxSetDefaults
    (
        WDRV_WINC_AUTH_CONTEXT *const pAuthCtx
    )

  Summary:
    Configures an authentication context into a default state.

  Description:
    Ensures that each element of the structure is configured into a default state.

  Precondition:
    None.

  Parameters:
    pAuthCtx - Pointer to an authentication context.

  Returns:
    WDRV_WINC_STATUS_OK          - The context has been configured.
    WDRV_WINC_STATUS_INVALID_ARG - The parameters were incorrect.

  Remarks:
    A default context is not valid until it is configured.

*/

WDRV_WINC_STATUS WDRV_WINC_AuthCtxSetDefaults
(
    WDRV_WINC_AUTH_CONTEXT *const pAuthCtx
);

//*******************************************************************************
/*
  Function:
    WDRV_WINC_STATUS WDRV_WINC_AuthCtxSetOpen
    (
        WDRV_WINC_AUTH_CONTEXT *const pAuthCtx
    )

  Summary:
    Configure an authentication context for Open authentication.

  Description:
    The type and state information are configured appropriately for Open
      authentication.

  Precondition:
    None.

  Parameters:
    pAuthCtx - Pointer to an authentication context.

  Returns:
    WDRV_WINC_STATUS_OK             - The context has been configured.
    WDRV_WINC_STATUS_INVALID_ARG    - The parameters were incorrect.

  Remarks:
    None.

*/

WDRV_WINC_STATUS WDRV_WINC_AuthCtxSetOpen
(
    WDRV_WINC_AUTH_CONTEXT *const pAuthCtx
);

//*******************************************************************************
/*
  Function:
    WDRV_WINC_STATUS WDRV_WINC_AuthCtxSetWEP
    (
        WDRV_WINC_AUTH_CONTEXT *const pAuthCtx,
        uint8_t idx,
        uint8_t *pKey,
        uint8_t size
    )

  Summary:
    Configure an authentication context for WEP authentication.

  Description:
    The type and state information are configured appropriately for WEP
      authentication.

  Precondition:
    None.

  Parameters:
    pAuthCtx - Pointer to an authentication context.
    idx      - WEP index.
    pKey     - Pointer to WEP key.
    size     - Size of WEP key.

  Returns:
    WDRV_WINC_STATUS_OK             - The context has been configured.
    WDRV_WINC_STATUS_INVALID_ARG    - The parameters were incorrect.

  Remarks:
    None.

*/

WDRV_WINC_STATUS WDRV_WINC_AuthCtxSetWEP
(
    WDRV_WINC_AUTH_CONTEXT *const pAuthCtx,
    uint8_t idx,
    uint8_t *const pKey,
    uint8_t size
);

//*******************************************************************************
/*
  Function:
    WDRV_WINC_STATUS WDRV_WINC_AuthCtxSetWPA
    (
        WDRV_WINC_AUTH_CONTEXT *const pAuthCtx,
        uint8_t *pPSK,
        uint8_t size
    )

  Summary:
    Configure an authentication context for WPA-PSK authentication.

  Description:
    The type and state information are configured appropriately for WPA-PSK
      authentication.

  Precondition:
    None.

  Parameters:
    pAuthCtx - Pointer to an authentication context.
    pPSK     - Pointer to WPA-PSK key.
    size     - Size of WPA-PSK key.

  Returns:
    WDRV_WINC_STATUS_OK             - The context has been configured.
    WDRV_WINC_STATUS_INVALID_ARG    - The parameters were incorrect.

  Remarks:
    None.

*/

WDRV_WINC_STATUS WDRV_WINC_AuthCtxSetWPA
(
    WDRV_WINC_AUTH_CONTEXT *const pAuthCtx,
    uint8_t *const pPSK,
    uint8_t size
);

//*******************************************************************************
/*
  Function:
    WDRV_WINC_STATUS WDRV_WINC_AuthCtxSetWPAEnterpriseMSCHAPv2
    (
        WDRV_WINC_AUTH_CONTEXT *const pAuthCtx,
        const char *const pUserDomain,
        const uint8_t *const pPassword,
        const uint16_t passwordLength,
        const bool visibleUserName
    )

  Summary:
    Configure an authentication context for WPA(2)-Enterprise authentication
    using MS-CHAPv2.

  Description:
    The type and state information are configured appropriately for WPA-Enterprise
      authentication.

  Precondition:
    None.

  Parameters:
    pAuthCtx        - Pointer to an authentication context.
    pUserDomain     - Pointer to user and domain name.
    pPassword       - Pointer to password.
    passwordLength  - Length of password.
    visibleUserName - Flag indicating if usernames should be visible in initial
                        EAP exchanges.

  Returns:
    WDRV_WINC_STATUS_OK             - The context has been configured.
    WDRV_WINC_STATUS_INVALID_ARG    - The parameters were incorrect.

  Remarks:
    None.

*/

#ifdef WDRV_WINC_DEVICE_ENTERPRISE_CONNECT
WDRV_WINC_STATUS WDRV_WINC_AuthCtxSetWPAEnterpriseMSCHAPv2
(
    WDRV_WINC_AUTH_CONTEXT *const pAuthCtx,
    const char *const pUserDomain,
    const uint8_t *const pPassword,
    const uint16_t passwordLength,
    const bool visibleUserName
);
#endif

//*******************************************************************************
/*
  Function:
    WDRV_WINC_STATUS WDRV_WINC_AuthCtxSetWPAEnterpriseTLS
    (
        WDRV_WINC_AUTH_CONTEXT *const pAuthCtx,
        const char *const pUserDomain,
        const uint8_t *const pPrivateKey,
        const uint16_t privateKeyLength,
        const uint8_t *const pCert,
        const uint16_t certLength,
        const bool visibleUserName
    )

  Summary:
    Configure an authentication context for WPA(2)-Enterprise authentication
    using TLS.

  Description:
    The type and state information are configured appropriately for WPA-Enterprise
      authentication.

  Precondition:
    None.

  Parameters:
    pAuthCtx         - Pointer to an authentication context.
    pUserDomain      - Pointer to user and domain name.
    pPrivateKey      - Pointer to RSA private key in DER binary format.
    privateKeyLength - Length of RSA private key.
    pCert            - Pointer to RSA certificate in DER binary format.
    certLength       - Length of RSA certificate.
    visibleUserName  - Flag indicating if usernames should be visible in initial
                       EAP exchanges.

  Returns:
    WDRV_WINC_STATUS_OK             - The context has been configured.
    WDRV_WINC_STATUS_INVALID_ARG    - The parameters were incorrect.

  Remarks:
    None.

*/

#ifdef WDRV_WINC_DEVICE_ENTERPRISE_CONNECT
WDRV_WINC_STATUS WDRV_WINC_AuthCtxSetWPAEnterpriseTLS
(
    WDRV_WINC_AUTH_CONTEXT *const pAuthCtx,
    const char *const pUserDomain,
    const uint8_t *const pPrivateKey,
    const uint16_t privateKeyLength,
    const uint8_t *const pCert,
    const uint16_t certLength,
    const bool visibleUserName
);
#endif

#endif /* _WDRV_WINC_AUTHCTX_H */
