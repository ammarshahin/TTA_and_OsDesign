#include "utils.h"

#define CYCLE_MELLI_DELAY_FACTOR 9
#define CYCLE_MICRO_DELAY_FACTOR 13

#define SECONDS_TO_NANOSECONSD_FACTOR 1000000000
#define SECONDS_TO_MICROSECONSD_FACTOR 1000000
#define SECONDS_TO_MILLISECONSD_FACTOR 1000
#define DUTY_CYCLE_PERCENTAGE_FACTOR 100

#if 0
void utils_ftoa(float x, uint8_t *a)
{
    int i = 0;
    unsigned int integer = (unsigned int)x;
    unsigned int fraction = (((x - integer) < 0.11) ? (0) : (x - integer) * 100);
    if (integer == 0)
        a[i++] = '0';
    else
    {
        while (integer > 0)
        {
            a[i] = (integer % 10) | 0x30;
            integer /= 10;
            i++;
        }
    }
    a[i++] = '.';
    util_reverse(a);
    if (fraction == 0)
    {
        a[i++] = '0';
        a[i] = '\0';
    }
    else
    {
        while (fraction > 0)
        {
            a[i] = (fraction % 10) | 0x30;
            fraction /= 10;
            i++;
        }
        a[i] = '\0';
        util_reverse(&a[i - 2]);
    }
}
#endif

void utils_itoa(int32_t n, uint8_t s[])
{
    int32_t sign = 0;
    uint8_t i = 0;

    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do
    {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    utils_reverse(s);
}

void utils_uitoa(uint32_t n, uint8_t s[])
{
    uint8_t i = 0;

    i = 0;
    do
    {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    s[i] = '\0';
    utils_reverse(s);
}

void utils_reverse(uint8_t *str)
{
    int i, j;
    uint8_t temp;
    for (i = 0, j = utils_strlen(str) - 1; i < j; i++, j--)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

int utils_strlen(uint8_t *s)
{
    int i = 0;
    while (s[i] != 0 && s[i] != '.')
        i++;
    return i;
}

int32_t utils_atoi(uint8_t *str)
{
    int32_t res = 0;   // Initialize result
    int8_t sign = 1;   // Initialize sign as positive
    uint8_t i = 0;     // Initialize index of first digit

    // If number is negative, then update sign
    if (str[0] == '-')
    {
        sign = -1;
        i++;   // Also update index of first digit
    }

    // Iterate through all digits and update the result
    for (; str[i] != '\0'; ++i)
        res = res * 10 + (str[i] - '0');

    // Return result with sign
    return sign * res;
}

void utils_itoahex(uint8_t x, char *a)
{
    uint8_t temp = 0;

    for (uint8_t i = 0; i < 2; i++)
    {
        temp = (x >> (i * 4)) & 0x0f;

        switch (temp)
        {
        case 15:
            a[i] = 'f';
            break;
        case 14:
            a[i] = 'e';
            break;
        case 13:
            a[i] = 'd';
            break;
        case 12:
            a[i] = 'c';
            break;
        case 11:
            a[i] = 'b';
            break;
        case 10:
            a[i] = 'a';
            break;
        default:
            a[i] = temp | 0x30;
            break;
        }
    }
    utils_reverse((uint8_t *)a);
}

uint8_t utils_dataMatch_check(uint8_t *data, uint8_t *ref, uint16_t size)
{
    uint16_t i = 0;

    while (size > i)
    {
        if (data[i] != ref[i])
        {
            return 0;
        }
        i++;
    }
    return 1;
}

void utils_sw_delay(unsigned long x)
{
    while (x-- > 0)
        ;
}

// decimal to packed bcd
uint8_t utils_dToPbcd(uint8_t val)
{
    return (uint8_t)(((val / 10) << 4) + (val % 10));
}

uint8_t utils_PbcdToDec(uint8_t val)
{
    return (uint8_t)(((val >> 4) * 10) + (val & 0x0f));
}

// takes in an array, sticks it together to form the data type of size specified
// example : array[4] --> uint32_t
// NOTE: little endian
void utils_arrayToUint32(uint8_t *array, uint32_t *bytes)
{
    *bytes = 0x00000000 | ((uint32_t)((uint32_t)array[0] | ((uint32_t)array[1] << 8) | ((uint32_t)array[2] << 16) | ((uint32_t)array[3] << 24)));
}

void utils_arrayToFloat(uint8_t *array, float *bytes)
{
    typedef union {
        float f_tmp;
        uint32_t u32_tmp;
    } aTf_t;
    aTf_t aTf;
    aTf.u32_tmp = ((uint32_t)((uint32_t)array[0] | ((uint32_t)array[1] << 8) | ((uint32_t)array[2] << 16) | ((uint32_t)array[3] << 24)));
    *bytes = aTf.f_tmp;
}

uint8_t utils_convergance_check(int32_t value1, int32_t value2, int32_t error)
{
    if (((value1 - value2) <= error) || ((value2 - value1) <= error))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t utils_convergenceValidity_check(uint32_t value, uint32_t threshold, uint32_t range)
{
    if (value <= (threshold + range) && value >= (threshold - range))
    {
        return 1;
    }
    else
        return 0;
}

uint8_t utils_thresholdValidity_check(uint32_t value, uint32_t highThreshold, uint32_t lowThreshold)
{
    if (value <= (highThreshold) && value >= (lowThreshold))
    {
        return 1;
    }
    else
        return 0;
}

/*===========================SW_Delay Functions===============================*/
/**
 * Description: Function to make a software Delay in the Melli seconds 
 * @param NoOfMilleSeconds the number of  Melli seconds to delay
 */
void SwDelay_ms(uint32_t NoOfMilleSeconds)
{
    uint64_t counter;
    counter = NoOfMilleSeconds * (F_CPU / SECONDS_TO_MILLISECONSD_FACTOR);
    while (counter > FALSE)
    {
        counter = counter - CYCLE_MELLI_DELAY_FACTOR;
    }
}

/**
 * Description: Function to make a software Delay in the Micro Seconds. 
 * @param NoOfMicroeSeconds the number of Micro Seconds to delay
 */
void SwDelay_us(uint32_t NoOfMicroeSeconds)
{
    uint64_t counter;
    counter = NoOfMicroeSeconds * (F_CPU / SECONDS_TO_MICROSECONSD_FACTOR);
    while (counter > FALSE)
    {
        counter = counter - CYCLE_MICRO_DELAY_FACTOR;
    }
}