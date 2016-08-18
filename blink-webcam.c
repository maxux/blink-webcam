#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <errno.h>

void diep(char *str) {
	printf("[-] %s: %s\n", str, strerror(errno));
	exit(EXIT_FAILURE);
}

void debug(char *str) {
	printf("[+] %s\n", str);
}

int main(int argc, char *argv[]) {
	int fd;
	char *device = "/dev/video0";
	struct v4l2_capability cap;
	struct v4l2_format format;
	struct v4l2_requestbuffers bufrequest;
	struct v4l2_buffer bufferinfo;

	//
	// checking for device
	//
	if(argc > 1)
		device = argv[argc - 1];

	//
	// open device
	//
	printf("[+] opening: %s\n", device);

	if((fd = open(device, O_RDWR)) < 0)
		diep(device);

	if(ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0)
		diep("VIDIOC_QUERYCAP");

	printf("[+] device: %s\n", cap.card);

	//
	// request small video
	//
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	format.fmt.pix.width = 640;
	format.fmt.pix.height = 480;

	if(ioctl(fd, VIDIOC_S_FMT, &format) < 0)
		diep("VIDIOC_S_FMT");

	//
	// building a buffer
	//
	bufrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	bufrequest.memory = V4L2_MEMORY_MMAP;
	bufrequest.count = 1;
	 
	if(ioctl(fd, VIDIOC_REQBUFS, &bufrequest) < 0)
		diep("VIDIOC_REQBUFS");

	//
	// allocating buffer
	//
	memset(&bufferinfo, 0, sizeof(bufferinfo));
	bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	bufferinfo.memory = V4L2_MEMORY_MMAP;
	bufferinfo.index = 0;

	if(ioctl(fd, VIDIOC_QBUF, &bufferinfo) < 0)
		diep("VIDIOC_QBUF");

	//
	// request a frame
	//
	int type = bufferinfo.type;

	printf("[+] powering on\n");
	
	if(ioctl(fd, VIDIOC_STREAMON, &type) < 0)
		diep("VIDIOC_STREAMON");

	//
	// closing stuff and exit
	//
	printf("[+] closing device\n");

	if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0)
		diep("VIDIOC_STREAMOFF");
	
	return 0;
}
