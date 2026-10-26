# C2000-F280039C-GPIO-Button

LAUNCHXL-F280039C 上的 GPIO 按键输入：内部上拉 + 20 ms 软件消抖 + 下降沿检测，**按一下翻转一次** LED。

## 硬件

| 项 | 接法 |
|---|---|
| 按键输入 | GPIO47（Port B），代码中开启内部上拉 |
| 按下 | 杜邦线把 GPIO47 接到 GND，读到低电平 |
| 指示灯 | LED5 = GPIO22，低电平点亮 |

## 关键代码

```c
if (last == 1 && now == 0)          /* 下降沿 */
{
    DEVICE_DELAY_US(20000);         /* 消抖 20 ms */
    if (GPIO_readPin(47) == 0)
    {
        GPIO_togglePin(22);         /* 只执行一次 */
        while (GPIO_readPin(47) == 0) { }
    }
}
last = now;
```

## 三个设计要点

1. **边沿触发而不是电平触发**：`if (now == 0)` 会在按住期间反复翻转；`last == 1 && now == 0` 只在按下瞬间触发一次。
2. **20 ms 消抖**：机械按键抖动约 5–20 ms；人的按键动作约 100 ms，20 ms 不会显得迟钝。
3. **等松手**：吸收释放时的抖动，避免一次按下触发多次。

## 实测现象

- 按一下 → 翻转一次
- 按住 → 只翻一次
- 快速连按 5 下 → 翻 5 次

## 编译

- CCS 12.1.0 + C2000Ware 5.04.00.00
- 构建配置：RAM 链接（`F280039_ram_lnk.cmd`）
- 调试提示：GPIO47 属于 Port B，Expressions 中看 `GpioDataRegs.GPBDAT.bit.GPIO47`

## 依赖

本仓库只包含本人编写的代码与工程配置，不含 TI 的 SDK 文件。编译前需新建 C2000 工程，并把 C2000Ware 的 `device_support/f28003x/` 与 `driverlib/f28003x/` 复制到工程目录。
