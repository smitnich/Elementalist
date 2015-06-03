void WiiResetPressed()
{
	HWButton = 0;
}
 void WiiPowerPressed()
{
	//Reverting this to reset for now, since having
	//to reboot the whole system while debugging is a 
	//huge pain
	//Should be changed back eventually
	HWButton = 0;
	//HWButton = SYS_POWEROFF_STANDBY;
}
void WiimotePowerPressed(s32 chan)
{
	HWButton = SYS_POWEROFF_STANDBY;
}
