#include "mcal.h"
#include "utils.h"

void mcal_wdt_init(void)
{
    WDT_ENABLE();
}

void mcal_wdt_set(uint32_t resetTime)
{
}

void mcal_wdt_feed(void)
{
    //WDT_WriteClearCode();
}

void mcal_wdt_disable(void)
{
    WDT_DISABLE();
}
