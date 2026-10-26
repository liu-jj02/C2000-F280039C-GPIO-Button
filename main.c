#include "driverlib.h"
#include "device.h"

void main(void)
{
    Device_init();          // 时钟、外设时钟、关看门狗
    Device_initGPIO();      // 解除 GPIO 配置锁

    uint16_t last = 1;          /* 上一次的电平，初始认为"松开" */

    // ---- LED5 = GPIO22 ----
    GPIO_setPadConfig(22, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(22, GPIO_DIR_MODE_OUT);

    /* ---- 按键输入 = GPIO47，开内部上拉 ---- */
    GPIO_setPinConfig(GPIO_47_GPIO47);            /* 引脚复用选 GPIO 功能 */
    GPIO_setPadConfig(47, GPIO_PIN_TYPE_PULLUP);  /* ★ 关键：开上拉 */
    GPIO_setDirectionMode(47, GPIO_DIR_MODE_IN);  /* 输入 */
    GPIO_setQualificationMode(47, GPIO_QUAL_SYNC);/* 同步到 SYSCLK */

    for(;;)
    {
        uint16_t now = GPIO_readPin(47);

        if (last == 1 && now == 0)              /* 只在"松开→按下"这一刻触发 */
        {
            DEVICE_DELAY_US(20000);             /* 消抖：等 20 ms */
            if (GPIO_readPin(47) == 0)      /* 还按着，确认是真按下 */
            {
                GPIO_togglePin(22);           /* 动作只执行一次 */
                while (GPIO_readPin(47) == 0) { }   /* 等松手 */
            }
        }

        last = now;
    }
}
