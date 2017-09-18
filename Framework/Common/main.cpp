#include <stdio.h>
#include "IApplication.hpp"
using namespace Onion;
namespace Onion
{
	extern IApplication* g_pApp;
}

int main(int argc,char** argv)
{
	int ret;
	ret = g_pApp->Initialize();
	if(ret!=0){
		printf("App Initialize failed,will exit now.");
		return ret;
	}
	while(!g_pApp->IsQuit()){
		g_pApp->Tick();
	}
	g_pApp->Finalize();
	return 0;
}