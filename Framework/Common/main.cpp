#include <stdio.h>
#include "IApplication.hpp"
using namespace Onion;
namespace Onion
{
	extern IApplication* gApp;
}

int main(int argc,char** argv)
{
	int ret;
	ret = gApp->Initialize();
	if(ret!=0){
		printf("App Initialize failed,will exit now.");
		return ret;
	}
	while(!gApp->IsQuit()){
		gApp->Tick();
	}
	gApp->Finalize();
	return 0;
}