# imagefilters
Image Filters website using OpenCV

There are two portions to this repo. 

1. The ASP.net website is located under the image-web folder and presented as a Visual Studio Solution.Using the website you can upload a
   picture which gets stored in an Azure Storage Blob. When you click grayscale it will look for an grayscaled version of the image you 
   uploaded in the result Blob Container image. 

2. imagefilters.cpp is the C++ piece of code that is platform agonistic runs on Windows or Linux. It downloads the blob and uses OpenCV
   to convert it to a grayscale version which is then uplodaded back to a blob.
   
   Build Instructions:
   1. ASP.net website
      - In VS2015 build imagefilters.csproj, you will have to replace the following with your storage account:
        CloudStorageAccount storageAccount = CloudStorageAccount.Parse(CloudConfigurationManager.GetSetting("cppstorage_AzureStorageConnectionString"));
       
   2. The C++ piece of code on a Linux container.
      - Create a container using this OpenCV image (https://github.com/kavolorn/Docker-OpenCV)
      - Install and build the Azure Storage C++ SDK for Linux (https://github.com/azure/azure-storage-cpp#getting-started-on-linux)
      - Use the build.sh file to build this C++ code 
        Your LD_LIBRARY_PATH need to point to your storage and cpprest dynamic shared libraries e.g. 
        /crimecop/azure-storage-cpp/Microsoft.WindowsAzure.Storage/build.release/Binaries:/crimecop/azure-storage-cpp/Microsoft.WindowsAzure.Storage/casablanca/Release/build.debug/Binaries
        Your include path needs to point to your Storage and CPP includes, likewise -L needs to point to your storage and Rest libraries
       
