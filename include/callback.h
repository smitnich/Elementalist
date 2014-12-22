void WiiResetPressed()
{
	HWButton = 0;
}
 void WiiPowerPressed()
{
	HWButton = SYS_POWEROFF_STANDBY;
}
void WiimotePowerPressed(s32 chan)
{
	HWButton = SYS_POWEROFF_STANDBY;
}
