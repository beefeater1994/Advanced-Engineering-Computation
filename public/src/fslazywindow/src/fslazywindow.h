#ifndef FSLAZYWINDOW_IS_INCLUDED
#define FSLAZYWINDOW_IS_INCLUDED
/* { */

#include <fssimplewindow.h>


#ifdef __cplusplus

/*! 
*/
class FsLazyWindowApplicationBase
{
protected:
	bool terminateFlag;

public:
	/*! This function return a pointer to a sub-class of FsLazyWindowApplicationBase.

	    This function will be called from the lazy-window framework's event handler many times.
	    Therefore, do not re-allocate FsLazyWindowApplicationBase every time.
	*/
	static FsLazyWindowApplicationBase *GetApplication(void);


	/*! This function sets must-terminate flag. 
	*/
	void SetMustTerminate(bool MustTerminate);

	/*! Default constructor.  It sets terminateFlag=false.
	*/
	FsLazyWindowApplicationBase();

	/*! This function is called before the window is created.  If something needs to be done before opening a window
	    (for example, deciding window size from the command parameters) it is the right place to do.

	    Argc and argv are taken from the command parameters.
	*/
	virtual void BeforeEverything(int argc,char *argv[])=0;

	/*! This function must be implemented in the lazy-window application.
	    It should return preferred window location and size, title, etc., in OPT,
	    which may or may not be respected by the lazy-window framework.
	*/
	virtual void GetOpenWindowOption(FsOpenWindowOption &OPT) const=0;

	/*! This function must be implemented in the lazy-window application.
        This function will be called once when the window and OpenGL context is ready.

	    Argc and argv are taken from the command parameters.
	*/
	virtual void Initialize(int argc,char *argv[])=0;

	/*! This function is called regularly on interval.
	*/
	virtual void Interval(void)=0;

	/*! This function is called before the program terminates.
	*/
	virtual void BeforeTerminate(void)=0;

	/*! This function is called when the window needs to be re-drawn. */
	virtual void Draw(void)=0;

	/*! This function is called when the user wants to close the program.
	    For example, when the user clicks on the close button of the window, or
	    pressed Alt+F4 in Windows or Command+Q in Mac OSX. 

	    The default implementation just closes the program by returning true.

	    Return true if the window can be closed.

	    If the window must not be closed right away (for example, need to confirm with the user if the window can really be closed),
	    set up whatever user-inquirey and return false in this function.
	*/
	virtual bool UserWantToCloseProgram(void);

	/*! This function must return true when the program should terminate.
	    The default behavior is to return terminateFlag.
	*/
	virtual bool MustTerminate(void) const;

	/*! This function must return minimum sleep per interval in milli seconds. 
	    It returns 10 by default. */
	virtual long long int GetMinimumSleepPerInterval(void) const;

	/*! This function must return true when the window must be re-drawn.
	*/
	virtual bool NeedRedraw(void) const=0;

	/*! Implement this function in the sub-class and return true when the program needs to stay idle
	    (that means OnInterval won't be called) until an event comes in.
	    For example, this function can return true while waiting for the user to select a menu,
	    and return false when a heavily interactive process (like gaming) starts.
	    Default behavior is to return false.
	*/
	virtual bool StayIdleUntilEvent(void) const;


	/*! EGL library allows the destruction of OpenGL context.  
	    Instead of making OpenGL device put on the stand-by and recover upon power-management event,
	    it may just destroy the context and let the application re-create all resources.
	    Which is pretty bad.  It is similar to the bad design of Direct 3D9, which requires the 
	    application to destroy and re-create context and all resources when the user simply re-sized the window.
	    But, it's life.  They made it bad.
	    OpenGL on UWP is based on EGL, and it can happen.  When it happens, the notification is sent to
	    this function.  The application must re-create GLSL renderers, vertex buffer objects, and every OpenGL resources.

	    This function will be called after the OpenGL context is re-created.
	*/
	virtual void ContextLostAndRecreated(void);
};

#endif

/* } */
#endif
