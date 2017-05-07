#ifdef __USE_CMSIS
#include "LPC8xx.h"
#endif

#include "lpc8xx_clkconfig.h"
#include "lpc8xx_gpio.h"
#include "lpc8xx_mrt.h"

extern uint32_t mrt_counter;

// 割込検知用変数
static uint32_t mrt_toggle;

// LED点灯用ステータス
static int led_blink_state;

//
// ボタン押下検知処理
//
// ボタン押下連続検知抑止カウンター（約0.5秒）
#define BTN_PUSH_PREVENT_CNT 500
static unsigned int btn_push_prevent_cnt;

// 基板上のボタン押下時の処理
static int process_on_button_press()
{
    int ret = 1;

    // スイッチOnに対する処理を実行
	if (GPIOGetPinValue(0, 4) == 1) {
		//
		// LED点灯ステータスを変更し、
		// 点滅をストップ／リスタートさせる
		//
		if (led_blink_state == 9) {
			led_blink_state = 0;
		} else {
			led_blink_state = 9;
		}

    } else {
        ret = 0;
    }
    return ret;
}

// 割込みごとに処理
static void switch_prevent()
{
    // カウンターが０の時は終了
    if (0 == btn_push_prevent_cnt) {
        return;
    }

    // ボタン連続押下抑止カウンターを更新
    btn_push_prevent_cnt-- ;
}

// イベントループ内の最後のステップで処理
static void switch_detection()
{
    // カウンターが０でない時は終了
    if (0 < btn_push_prevent_cnt) {
        return;
    }

    // スイッチ押下時の処理を実行
    if (process_on_button_press() != 0) {
        // 押下抑止カウンターを設定
        btn_push_prevent_cnt = BTN_PUSH_PREVENT_CNT;
    } else {
        btn_push_prevent_cnt = 0;
    }
}

static void process_on_250ms(void)
{
	switch (led_blink_state) {
	case 0:
		GPIOSetBitValue(0, 0, 0);
		led_blink_state++;
		break;
	case 1:
		GPIOSetBitValue(0, 0, 1);
		led_blink_state++;
		break;
	case 2:
		GPIOSetBitValue(0, 1, 0);
		led_blink_state++;
		break;
	case 3:
		GPIOSetBitValue(0, 1, 1);
		led_blink_state = 0;
		break;
	default:
		// 点滅をストップさせる
		GPIOSetBitValue(0, 0, 1);
		GPIOSetBitValue(0, 1, 1);
		break;
	}
}

static void process_on_interval()
{
    // スイッチ連続検知抑止
    switch_prevent();
}

static void process_init()
{
	// 割込検知用変数初期化
	mrt_toggle = mrt_counter;
	// ローカル変数の初期化
    btn_push_prevent_cnt = 0;
	led_blink_state = 0;
}

static void setup_port()
{
	// 出力: P0_0, P0_1
	// 入力: P0_4
	GPIOSetDir(0, 0, 1);
	GPIOSetDir(0, 1, 1);
	GPIOSetDir(0, 4, 0);
}

static void setup_clock_and_timer()
{
	// システムクロックの設定
	SystemCoreClockUpdate();

	// 割込み間隔を 1ms（1/1000秒）に設定
	init_mrt(SystemCoreClock / 1000);
}

int main (void)
{
	// クロック／割込間隔設定、
	// ポートの設定、変数初期化
	setup_clock_and_timer();
	setup_port();
	process_init();

	while (1) {
		if (mrt_toggle != mrt_counter) {
			// 割込ごとの処理を実行
			process_on_interval();
			mrt_toggle = mrt_counter;
		}
		if (mrt_counter == 250) {
			// 250msごとの処理を実行
			process_on_250ms();
			mrt_counter = 0;
		}
	    // スイッチ検知処理
	    switch_detection();
	}
}
