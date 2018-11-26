void delay_us(void)
{
	nop();nop();nop();nop();nop();nop();nop();nop();
	nop();nop();nop();nop();nop();nop();nop();nop();
}
void delay_10us(void)
{
	delay_us();delay_us();delay_us();delay_us();
	delay_us();delay_us();delay_us();delay_us();
	delay_us();delay_us();
}
void delay_100us(void)
{
	delay_10us();delay_10us();delay_10us();delay_10us();
	delay_10us();delay_10us();delay_10us();delay_10us();
	delay_10us();delay_10us();
}
void delay_1ms(void)
{
	delay_100us();delay_100us();delay_100us();delay_100us();
	delay_100us();delay_100us();delay_100us();delay_100us();
	delay_100us();delay_100us();
}
void delay_xms(u16 counter)
{
	while(counter--)
	{
		delay_1ms();
	}
}