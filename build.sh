
g++ -std=c++11 -I/imagecppservice/azure-storage-cpp/Microsoft.WindowsAzure.Storage/includes -I/imagecppservice/casablanca/Release/include -L/imagecppservice/azure-storage-cpp/Microsoft.WindowsAzure.Storage/build.release/Binaries/  -lazurestorage -L/imagecppservice/casablanca/Release/build.release/Binaries -lcpprest -lboost_system -lcrypto -lssl -lz -L/imagecppservice/opencv-3.1.0/build/lib -lopencv_core -lopencv_imgproc -lopencv_imgcodecs imagefilters.cpp 

export LD_LIBRARY_PATH=/imagecppservice/azure-storage-cpp/Microsoft.WindowsAzure.Storage/build.release/Binaries:/imagecppservice/casablanca/Release/build.release/Binaries:/imagecppservice/opencv-3.1.0/build/lib
