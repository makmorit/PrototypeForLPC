#ifdef __USE_CMSIS
#include "LPC8xx.h"
#endif

#include "lpc8xx_clkconfig.h"
#include "lpc8xx_gpio.h"
#include "lpc8xx_mrt.h"

extern uint32_t mrt_counter;

static void process_on_250ms(void)
{
	static state = 0;

	switch (state) {
	case 0:
		GPIOSetBitValue(0, 0, 0);
		state++;
		break;
	case 1:
		GPIOSetBitValue(0, 0, 1);
		state++;
		break;
	case 2:
		GPIOSetBitValue(0, 1, 0);
		state++;
		break;
	case 3:
		GPIOSetBitValue(0, 1, 1);
		state = 0;
		break;
	default:
		break;
	}
}

int main (void)
{
	// システムクロックの設定
	SystemCoreClockUpdate();

	// ポートの出力設定
	// P0_0, P0_1, P0_2
	GPIOSetDir(0, 0, 1);
	GPIOSetDir(0, 1, 1);
	GPIOSetDir(0, 2, 1);

	// 割込み間隔を 1ms（1/1000秒）に設定
	init_mrt(SystemCoreClock / 1000);
    
	while (1) {
		if (mrt_counter == 250) {
			// 250msごとの処理を実行
			process_on_250ms();
			mrt_counter = 0;
		}
	}
}
