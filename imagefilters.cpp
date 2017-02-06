#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <was/storage_account.h>
#include <was/blob.h>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;
using namespace azure;

/// Global Variables
Mat img; Mat templ; Mat result;

int match_method;
int max_Trackbar = 5;

const utility::string_t storage_connection_string(U("DefaultEndpointsProtocol=https;AccountName=cppstorage;AccountKey=JIGwxpMESw7fCoXZ8qSCtOiyg++RFL6JDGwMVmRhW18OVyP/7DotKRwgQpJ3AQM8b657LSHN8oHy3V2lZGKdZw=="));

/// Function Headers
void MatchingMethod(int, void*);
void grayscale(Mat img, storage::cloud_block_blob);

void run() {
	azure::storage::cloud_storage_account storage_account = azure::storage::cloud_storage_account::parse(storage_connection_string);
	storage::cloud_blob_client blob_client = storage_account.create_cloud_blob_client();
	storage::cloud_blob_container container = blob_client.get_container_reference(U("img-container"));
	storage::cloud_blob_container result_container = blob_client.get_container_reference(U("img-resized-container"));
	result_container.create_if_not_exists();
	azure::storage::list_blob_item_iterator end_of_results;
	
	while (1) {
		for (auto it = container.list_blobs(); it != end_of_results; ++it)
		{
			bool downloadimage = false;
			if (it->is_blob())
			{
				utility::string_t blobname = it->as_blob().name();

				storage::cloud_block_blob blockBlob = container.get_block_blob_reference(blobname);
				storage::cloud_block_blob resultblockBlob = result_container.get_block_blob_reference(blobname);

				// Save blob contents to a file.
				concurrency::streams::container_buffer<std::vector<uint8_t>> buffer;
				concurrency::streams::ostream output_stream(buffer);
				blockBlob.download_to_stream(output_stream);
				//blockBlob.delete_blob();
				std::ofstream outfile("DownloadBlobFile.jpg", std::ofstream::binary);
				std::vector<unsigned char>& data = buffer.collection();

				outfile.write((char *)&data[0], buffer.size());
				outfile.close();

				Mat img = imread("DownloadBlobFile.jpg");

				grayscale(img, resultblockBlob);
			}
		}
		}
}

void grayscale(Mat img, storage::cloud_block_blob resultblockBlob) {
	cvtColor(img, img, CV_BGR2GRAY);
	//cv::resize(img, img, Size(), 0.5, 0.5);
	imwrite("result.jpg", img);
	resultblockBlob.upload_from_file("result.jpg");
}


/** @function main */
int main(int argc, char** argv)
{
	run();
	
	/*
		/// Load image and template
		img = imread(argv[1], 1);
		templ = imread(argv[2], 1);

		/// Create windows
		namedWindow(image_window, CV_WINDOW_AUTOSIZE);
		namedWindow(result_window, CV_WINDOW_AUTOSIZE);

		/// Create Trackbar
		char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
		createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);

		MatchingMethod(0, 0);

		waitKey(0);
		*/
	return 0;
}

/**
* @function MatchingMethod
* @brief Trackbar callback
*/
