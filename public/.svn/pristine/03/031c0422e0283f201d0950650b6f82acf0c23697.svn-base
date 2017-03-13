#include "fslazywindow.h"

FsLazyWindowApplicationBase::FsLazyWindowApplicationBase()
{
	terminateFlag=false;
}

/* virtual */ bool FsLazyWindowApplicationBase::MustTerminate(void) const
{
	return terminateFlag;
}

/* virtual */ long long int FsLazyWindowApplicationBase::GetMinimumSleepPerInterval(void) const
{
	return 10;
}

/* virtual */ bool FsLazyWindowApplicationBase::UserWantToCloseProgram(void)
{
	return true;
}

/* virtual */ void FsLazyWindowApplicationBase::ContextLostAndRecreated(void)
{
}

void FsLazyWindowApplicationBase::SetMustTerminate(bool MustTerminate)
{
	terminateFlag=MustTerminate;
}

/* virtual */ bool FsLazyWindowApplicationBase::StayIdleUntilEvent(void) const
{
	return false;
}
