#include "LPC8xx.h"
#include "lpc8xx_clkconfig.h"
#include "lpc8xx_mrt.h"
#include "lpc8xx_uart.h"

extern volatile uint32_t UARTRxCount;
extern volatile uint32_t RxErrorCount;
extern volatile uint8_t UARTRxBuffer[BUFSIZE];
extern uint32_t mrt_counter;

static void setup_clock_and_timer()
{
	// システムクロックの設定
	SystemCoreClockUpdate();

	// 割込み間隔を 1ms（1/1000秒）に設定
	init_mrt(SystemCoreClock / 1000);
}

static void uart0_pin_init()
{
	uint32_t regVal;

	// P0.4 is UART0 TXD, ASSIGN0(7:0)
	regVal = LPC_SWM->PINASSIGN0 & ~( 0xFF << 0 );
	LPC_SWM->PINASSIGN0 = regVal | ( 4 << 0 );

	// P0.0 is UART0 RXD. ASSIGN0(15:8)
	regVal = LPC_SWM->PINASSIGN0 & ~( 0xFF << 8 );
	LPC_SWM->PINASSIGN0 = regVal | ( 0 << 8 );
}

static void uart0_init()
{
	// UARTInit、UARTClock_Initを使用すると
	// 通信スピードが合わなくなる不具合が見られるため、
	// この関数で設定を行う
	NVIC_DisableIRQ(UART0_IRQn);

	// Enable UART clock
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<14);
	// Peripheral reset control to UART, a "1" bring it out of reset.
	LPC_SYSCON->PRESETCTRL &= ~(0x1<<3);
	LPC_SYSCON->PRESETCTRL |= (0x1<<3);

	// baud rate = 19200
	LPC_USART0->BRG = (SystemCoreClock / 16 / 19200) - 1;
	LPC_SYSCON->UARTCLKDIV = 2;
	LPC_SYSCON->UARTFRGDIV = 0xFF;
	LPC_SYSCON->UARTFRGMULT = 0;

	// 8 bits, no Parity, 1 Stop bit
	LPC_USART0->CFG = DATA_LENG_8|PARITY_NONE|STOP_BIT_1;

	// Clear all status bits.
	LPC_USART0->STAT = CTS_DELTA | DELTA_RXBRK;

	// Enable UART interrupt
	NVIC_EnableIRQ(UART0_IRQn);
	LPC_USART0->INTENSET = RXRDY | TXRDY | DELTA_RXBRK;
	LPC_USART0->CFG |= UART_EN;
}

static void print_initial_prompt()
{
	unsigned char *str = "Hello World!\r\n";
	for (int i = 0; i < 10; i++) {
		// UART出力したのち、1000ms 待つ
		UARTSend(LPC_USART0, (uint8_t *)str, strlen(str));
		for (mrt_counter = 0; mrt_counter < 1000; );
	}
}

static void echoback_input_character()
{
	if (UARTRxCount && !RxErrorCount) {
		// Disable RXRDY
		LPC_USART0->INTENCLR = RXRDY;

		UARTSend(LPC_USART0, (uint8_t *)UARTRxBuffer, UARTRxCount);
		UARTRxCount = 0;

		// Re-enable RXRDY
		LPC_USART0->INTENSET = RXRDY;
	}
}

int main (void)
{
	setup_clock_and_timer();
	uart0_pin_init();
	uart0_init();

	// Hello World! を、
	// １秒ごとに１０回 UART 出力する
	print_initial_prompt();
  
	while (1) {
		// UARTに入力された文字列を
		// そのままUARTへエコーバック出力する
		echoback_input_character();
	}
}
