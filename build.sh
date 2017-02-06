 



export LD_LIBRARY_PATH=

g++ -std=c++11 -I/crimecop/azure-storage-cpp/Microsoft.WindowsAzure.Storage/includes -I/crimecop/azure-storage-cpp/Microsoft.WindowsAzure.Storage/casablanca/Release/include -L/crimecop/azure-storage-cpp/Microsoft.WindowsAzure.Storage/build.release/Binaries/ `pkg-config opencv --cflags --libs` -lazurestorage -L/crimecop/azure-storage-cpp/Microsoft.WindowsAzure.Storage/casablanca/Release/build.release/Binaries -lcpprest -lboost_system -lcrypto -lssl -lz imagefilters.cpp


