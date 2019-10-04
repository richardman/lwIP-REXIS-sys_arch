#pragma once
/* REXIS interface to lwIP */
#include "rexis/rexis_api.h"

typedef REXIS_MAILBOX  *sys_mbox_t;
typedef REXIS_MUTEX    *sys_mutex_t;
typedef REXIS_SEMAPHORE*sys_sem_t;
typedef int             sys_thread_t;

err_t sys_mbox_new( sys_mbox_t *pxMailBox, int iSize );
void sys_mbox_free( sys_mbox_t *pxMailBox );
void sys_mbox_post( sys_mbox_t *pxMailBox, void *pxMessageToPost );
err_t sys_mbox_trypost( sys_mbox_t *pxMailBox, void *pxMessageToPost );
u32_t sys_arch_mbox_fetch( sys_mbox_t *pxMailBox, void **ppvBuffer, u32_t ulTimeOut );
u32_t sys_arch_mbox_tryfetch( sys_mbox_t *pxMailBox, void **ppvBuffer );
err_t sys_sem_new( sys_sem_t *pxSemaphore, u8_t ucCount );
u32_t sys_arch_sem_wait( sys_sem_t *pxSemaphore, u32_t ulTimeout );
err_t sys_mutex_new( sys_mutex_t *pxMutex );
void sys_mutex_lock( sys_mutex_t *pxMutex );
void sys_mutex_unlock(sys_mutex_t *pxMutex );
void sys_mutex_free( sys_mutex_t *pxMutex );
void sys_sem_signalISR( sys_sem_t *pxSemaphore );
void sys_sem_signal( sys_sem_t *pxSemaphore );
void sys_sem_free( sys_sem_t *pxSemaphore );
void sys_init(void);
u32_t sys_now(void);
sys_thread_t sys_thread_new( const char *pcName, void( *pxThread )( void *pvParameters ), void *pvArg, int iStackSize, int iPriority );
sys_prot_t sys_arch_protect( void );
void sys_arch_unprotect( sys_prot_t xValue );
void sys_assert( const char *pcMessage );
