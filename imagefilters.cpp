#include <was/storage_account.h>
#include <was/blob.h>
#include <was/queue.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace std;
using namespace azure;
using namespace cv;

const utility::string_t storage_connection_string(U("DefaultEndpointsProtocol=https;AccountName=cppstorage;AccountKey=JIGwxpMESw7fCoXZ8qSCtOiyg++RFL6JDGwMVmRhW18OVyP/7DotKRwgQpJ3AQM8b657LSHN8oHy3V2lZGKdZw=="));

/// Function Headers
void MatchingMethod(int, void*);
void grayscale(Mat img, storage::cloud_block_blob);
Mat img; Mat templ; Mat result;

void run() {
	try {
		azure::storage::cloud_storage_account storage_account = azure::storage::cloud_storage_account::parse(storage_connection_string);
		storage::cloud_blob_client blob_client = storage_account.create_cloud_blob_client();
		storage::cloud_blob_container container = blob_client.get_container_reference(U("img-container"));
		storage::cloud_blob_container result_container = blob_client.get_container_reference(U("img-resized-container"));
		result_container.create_if_not_exists();
		azure::storage::list_blob_item_iterator end_of_results;

		azure::storage::cloud_queue_client queue_client = storage_account.create_cloud_queue_client();
		azure::storage::cloud_queue input_queue = queue_client.get_queue_reference(U("img-input-queue"));
		input_queue.create_if_not_exists();
		input_queue.download_attributes();

		azure::storage::cloud_queue output_queue = queue_client.get_queue_reference(U("img-output-queue"));
		output_queue.create_if_not_exists();
                
		while (1) {
                    input_queue.download_attributes(); 
                    while (input_queue.approximate_message_count() != 0) {
				azure::storage::cloud_queue_message dequeued_message = input_queue.get_message();
				vector<uint8_t> iterator = dequeued_message.content_as_binary();
				utility::string_t blobname(iterator.begin(), iterator.end());
				storage::cloud_block_blob blockBlob = container.get_block_blob_reference(blobname);
				storage::cloud_block_blob resultblockBlob = result_container.get_block_blob_reference(blobname);
				concurrency::streams::container_buffer<std::vector<uint8_t>> buffer;
				concurrency::streams::ostream output_stream(buffer);
				blockBlob.download_to_stream(output_stream);
			        input_queue.delete_message(dequeued_message);	
                                blockBlob.delete_blob();
                                std::cout << "New message recieved and processed, blob:(" << blobname << ")"<<endl; 
				std::cout.flush();
				std::ofstream outfile("DownloadBlobFile.jpg", std::ofstream::binary);
				std::vector<unsigned char>& data = buffer.collection();
				outfile.write((char *)&data[0], buffer.size());
				outfile.close();
				Mat img = imread("DownloadBlobFile.jpg");
			        std::cout<<"Running GrayScale function on blob:("<<blobname<<")"<<endl;	
                                grayscale(img, resultblockBlob);
				azure::storage::cloud_queue_message message(blobname);
				output_queue.add_message(message);
                                std::cout<<"Output message posted:("<<blobname<<")"<<endl;
		                std::cout.flush();	
                                input_queue.download_attributes();
                    }
			std::cout << "Sleeping, waiting for work!" << endl;
			std::cout.flush();
			std::this_thread::sleep_for(std::chrono::seconds(30));
		}
	}
	catch (exception ex) {
		std::cout<<ex.what();
	}
}

void grayscale(Mat img, storage::cloud_block_blob resultblockBlob) {
	cvtColor(img, img, CV_BGR2GRAY);
	//cv::resize(img, img, Size(), 0.5, 0.5);
	imwrite("result.jpg", img);
	resultblockBlob.upload_from_file("result.jpg");
	std::cout << "Uploading blob with grayscale image";
	std::cout.flush() << endl;
}

/** @function main */
int main(int argc, char** argv)
{
	run();
	return 0;
}

