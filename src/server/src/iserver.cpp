#include "stdafx.h"
#include "asio_service_deamon.h"

int main(int argc, char** args)
{

    try
    {
        AsioServiceDeamon serviceEntry;
        serviceEntry.start("irene_service", 4);
    }
    catch (...)
    {
        std::cout << "An unknown exception occurred." << std::endl;
    }

    return 0;
}