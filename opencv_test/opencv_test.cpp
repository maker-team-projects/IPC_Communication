#include <iostream>
#include <vector>
#include <string>

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace std;
using namespace cv;

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

bool is_base64( unsigned char c ) 
{ 
	return (isalnum(c) || (c == '+') || (c == '/'));
}

string base64_encode(uchar const* bytes_to_encode, unsigned int in_len) 
{
	string ret;

	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) 
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i <4); i++) 
			{
				ret += base64_chars[char_array_4[i]];
			}
			i = 0;
		}
	}

	if (i) 
	{
		for (j = i; j < 3; j++) 
		{
			char_array_3[j] = '\0';
		}

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++) 
		{
			ret += base64_chars[char_array_4[j]];
		}
		
		while ((i++ < 3)) 
		{
			ret += '=';
		}
	}

	return ret;
}

string base64_decode(string const& encoded_string)
{
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	string ret;

	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) 
	{
		char_array_4[i++] = encoded_string[in_]; in_++;

		if (i == 4) 
		{
			for (i = 0; i < 4; i++) 
			{	
				char_array_4[i] = base64_chars.find(char_array_4[i]);
			}

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
			{
				ret += char_array_3[i];
			}

			i = 0;
		}
	}

	if (i) 
	{
		for (j = i; j < 4; j++) 
		{
			char_array_4[j] = 0;
		}
		
		for (j = 0; j < 4; j++) 
		{	
			char_array_4[j] = base64_chars.find(char_array_4[j]);
		}

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++)
		{	
			ret += char_array_3[j];
		}
	}

	return ret;
}

string mat2str(const Mat& m)
{
	int params[3] = {0};
	params[0] = IMWRITE_JPEG_QUALITY;
	params[1] = 100;

	vector<uchar> buf;
	bool code = imencode(".jpg", m, buf, vector<int>(params, params+2));
	uchar* result = reinterpret_cast<uchar*> (&buf[0]);

	return base64_encode(result, buf.size());

}

Mat str2mat(const string& s)
{
	// Decode data
	string decoded_string = base64_decode(s);
	vector<uchar> data(decoded_string.begin(), decoded_string.end());

	Mat img = imdecode(data, IMREAD_UNCHANGED);
	return img;
}

int main()
{
	Mat img = imread("test_img.jpg");

	if (img.empty()) {
		cerr << "Image laod failed!" << endl;
		return -1;
	}

	string base64 = mat2str(img);
	cout << "Base64 Result: " << base64 << endl;
	
	Mat img_r = str2mat(base64);

	namedWindow("image");
	imshow("image", img_r);
	waitKey();
	destroyAllWindows();
}