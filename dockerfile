FROM debian:jessie

MAINTAINER Ankit Asthana <aasthan@microsoft.com>

RUN apt-get update \
	&& apt-get upgrade -y \
	&& apt-get install -y unzip wget build-essential \
		cmake git pkg-config libswscale-dev \
		python3-dev python3-numpy \
		libtbb2 libtbb-dev libjpeg-dev \
		libpng-dev libtiff-dev libjasper-dev

RUN mkdir /imagecppservice \
	&& cd imagecppservice \
        && wget https://github.com/Itseez/opencv/archive/3.1.0.zip \
	&& unzip 3.1.0.zip \
	&& cd opencv-3.1.0 \
	&& mkdir build \
	&& cd build \
	&& cmake .. \
	&& make -j3 \
	&& make install 

RUN cd /imagecppservice \
       && apt-get install -y g++ git make zlib1g-dev libboost-all-dev libssl-dev cmake \
       && git clone https://github.com/Microsoft/cpprestsdk.git casablanca \
       && cd casablanca/Release \
       && mkdir build.release \
       && cd build.release \
       && cmake .. -DCMAKE_BUILD_TYPE=Release \
       && make


RUN cd /imagecppservice \
    && apt-get install -y libxml++2.6-dev libxml++2.6-doc uuid-dev \
    &&  git clone https://github.com/Azure/azure-storage-cpp.git \
    && cd azure-storage-cpp/Microsoft.WindowsAzure.Storage && mkdir build.release && cd build.release \
    && CASABLANCA_DIR=/imagecppservice/casablanca CXX=g++ cmake .. -DCMAKE_BUILD_TYPE=Release && make

RUN cd /imagecppservice \
    && git clone https://github.com/asthana86/imagefilters \
    && cd imagefilters \
    && ./build.sh 

RUN echo "export LD_LIBRARY_PATH=/imagecppservice/azure-storage-cpp/Microsoft.WindowsAzure.Storage/build.release/Binaries:/imagecppservice/casablanca/Release/build.release/Binaries:/imagecppservice/opencv-3.1.0/build/lib" >> ~/.bashrc

RUN bash 

RUN export LD_LIBRARY_PATH=/imagecppservice/azure-storage-cpp/Microsoft.WindowsAzure.Storage/build.release/Binaries:/imagecppservice/casablanca/Release/build.release/Binaries:/imagecppservice/opencv-3.1.0/build/lib && /imagecppservice/imagefilters/a.out &

   
 