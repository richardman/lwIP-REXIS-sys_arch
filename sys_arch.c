#include "lwip/err.h"
#include "lwip/sys.h"
#include "sys_arch.h"

extern int pending_context_switch;

REXIS_MUTEX *lwIP_system_mutex;

u32_t sys_jiffies(void)
{
  return REXIS_SysGetTicks();
}

u32_t sys_now(void)
{
    extern int rexis_number_of_msecs_per_tick;
    return REXIS_SysGetTicks() * rexis_number_of_msecs_per_tick;
}

err_t sys_mbox_new( sys_mbox_t *pxMailBox, int iSize )
    {
    *pxMailBox = REXIS_MailboxCreate(iSize);

    return (*pxMailBox != 0) ? ERR_OK : ERR_MEM;
    }

void sys_mbox_free( sys_mbox_t *pxMailBox )
    {
    REXIS_MailboxDestroy(*pxMailBox);
    }

void sys_mbox_post( sys_mbox_t *pxMailBox, void *pxMessageToPost )
    {
    REXIS_MailboxPost(*pxMailBox, pxMessageToPost);
    }

err_t sys_mbox_trypost( sys_mbox_t *pxMailBox, void *pxMessageToPost )
    {
    int stat = REXIS_MailboxTryPost(*pxMailBox, pxMessageToPost);

    return (stat == 0) ? ERR_OK : ERR_WOULDBLOCK;
    }

err_t sys_mbox_trypost_fromisr( sys_mbox_t *pxMailBox, void *pxMessageToPost )
    {
    REXIS_MailboxPostFromISR(*pxMailBox, pxMessageToPost);

    return ERR_OK;
    }

u32_t sys_arch_mbox_fetch( sys_mbox_t *pxMailBox, void **ppvBuffer, u32_t ulTimeOut )
    {
    int stat = REXIS_MailboxFetch(*pxMailBox, ppvBuffer, 0); // ulTimeOut);

    return (stat < 0) ? SYS_ARCH_TIMEOUT : stat;
    }

u32_t sys_arch_mbox_tryfetch( sys_mbox_t *pxMailBox, void **ppvBuffer )
    {
    int stat = REXIS_MailboxTryFetch(*pxMailBox, ppvBuffer);

    return (stat == 0) ? 0 : SYS_MBOX_EMPTY;
    }

int sys_mbox_valid(sys_mbox_t *mbox)
    {
    return (*mbox) != 0;
    }
int sys_sem_valid(sys_sem_t *s)
    {
    return *s != 0;
    }
err_t sys_sem_new( sys_sem_t *pxSemaphore, u8_t ucCount )
    {
    *pxSemaphore = REXIS_SemaphoreCreate(true, ucCount);

    return (*pxSemaphore != 0) ? ERR_OK : ERR_MEM;
    }
void sys_mbox_set_invalid(sys_mbox_t *mbox)
    {
    *mbox = 0;
    }
void sys_sem_set_invalid(sys_sem_t *s)
    {
    *s = 0;
    }

u32_t sys_arch_sem_wait( sys_sem_t *pxSemaphore, u32_t ulTimeOut )
    {
    int stat = 0;

    if (*pxSemaphore)
        stat = REXIS_SemaphoreWait(*pxSemaphore, ulTimeOut);

    return (stat < 0) ? SYS_ARCH_TIMEOUT : stat;
    }

err_t sys_mutex_new( sys_mutex_t *pxMutex )
    {
    *pxMutex = REXIS_MutexCreate();

    return (*pxMutex != 0) ? ERR_OK : ERR_MEM;
    }

void sys_mutex_lock( sys_mutex_t *pxMutex )
    {
    if (*pxMutex)
        REXIS_MutexLock(*pxMutex, 0);
    }

void sys_mutex_unlock(sys_mutex_t *pxMutex )
    {
    if (*pxMutex)
        REXIS_MutexUnlock(*pxMutex);
    }

void sys_mutex_free( sys_mutex_t *pxMutex )
    {
    REXIS_MutexDestroy(*pxMutex);
    }

void sys_sem_signal( sys_sem_t *pxSemaphore )
    {
    if (*pxSemaphore)
        REXIS_SemaphoreSignal(*pxSemaphore);
    }

void sys_sem_signalISR( sys_sem_t *pxSemaphore )
    {
    REXIS_SemaphoreSignalFromISR(*pxSemaphore);
    }

void sys_sem_free( sys_sem_t *pxSemaphore )
    {
    REXIS_SemaphoreDestroy(*pxSemaphore);
    }

void sys_init(void)
    {
    lwIP_system_mutex = REXIS_MutexCreate();
    }

sys_thread_t sys_thread_new( const char *pcName, void( *pxThread )( void *pvParameters ), void *pvArg, int iStackSize, int iPriority )
    {
    if (iPriority < REXIS_PRIO_MIN || iPriority > REXIS_PRIO_MAX)
        iPriority = REXIS_PRIO_NORM;

    return REXIS_TaskCreate((char *)pcName, (void (*)(unsigned))pxThread, iPriority, iStackSize, (unsigned)pvArg);
    }

sys_prot_t sys_arch_protect( void )
    {
    REXIS_MutexLock(lwIP_system_mutex, 0);
    int old_pending_context_switch = pending_context_switch;
    pending_context_switch = 1;
    return old_pending_context_switch;
    }

void sys_arch_unprotect( sys_prot_t xValue )
    {
    pending_context_switch = xValue;
    REXIS_MutexUnlock(lwIP_system_mutex);
    }
void sys_assert( const char *pcMessage )
    {
    printf("lwIP SYS ASSERT: '%s'\n", pcMessage);
    while (1);
    }
