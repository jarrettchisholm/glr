#include "ClientApp.hpp"

int main(int argc, char *argv[])
{
#ifdef _WIN32
	CefMainArgs args(nullptr);
#else
	CefMainArgs args(argc, argv);
#endif
    CefRefPtr<glr::cef_client::ClientApp> app = new glr::cef_client::ClientApp();
	return CefExecuteProcess(args, app.get());
}
