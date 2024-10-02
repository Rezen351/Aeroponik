import cv2
import time
import numpy as np
from datetime import datetime
import os
import sched
import imutils
import random
import json
import argparse

parser = argparse.ArgumentParser(description="Aeroponic Image Sampler & Measurement")
parser.add_argument("-th", "--threshold", help="Control Trigger Threshold in Centimeters")
parser.add_argument("-ppm", "--pixel_per_millimeters", help="Measurement Calibration")
parser.add_argument("-x0", "--x_roi_left", help="Left Side of RoI")
parser.add_argument("-x1", "--x_roi_right", help="Right Side of RoI")

args = parser.parse_args()

# Image Processing Setting Variables
if args.threshold == None:
  LIM_CM=20.0 #Limit in centimeters
else: 
  LIM_CM = float(args.threshold)

if args.pixel_per_millimeters == None:
  PPM=6.0 #Pixel per millimeters calibration result
else: 
  PPM=float(args.pixel_per_millimeters)

if (args.x_roi_left == None) or (args.x_roi_right == None):
  DIMX=(700,1400) # Manual RoI Assignment
else: 
  DIMX=(int(args.x_roi_left),int(args.x_roi_right))



# Image Processing
def check_length(im_path,debug=False):
  img = cv2.imread(im_path)
  crop_im = imutils.crop_img(img,DIMX)
  cl = imutils.apply_clahe(crop_im)
  sharp = imutils.sharpening(cl)
  th = imutils.adaptive_threshold(sharp)
  im_open = imutils.morph(th,cv2.MORPH_OPEN)
  final_im,status,length = imutils.show_segment(crop_im,im_open,ppm=PPM,lim_cm=LIM_CM)
  full_im=imutils.get_roi(img,final_im)
  if debug:
    horizontal_concat = np.concatenate((crop_im, cv2.cvtColor(cl, cv2.COLOR_GRAY2BGR), cv2.cvtColor(sharp, cv2.COLOR_GRAY2BGR), cv2.cvtColor(th, cv2.COLOR_GRAY2BGR), cv2.cvtColor(im_open, cv2.COLOR_GRAY2BGR), final_im), axis=1)
    cv2.imshow('Debug Image', horizontal_concat)
    cv2.imshow('Full Image', full_im)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
  return full_im,status,length

# # Scheduler instance
# scheduler = sched.scheduler(time.time, time.sleep)

# # List of IP Camera RTSP URLs
# rtsp_urls = [
#     "rtsp://admin:Admin_TF24!@192.168.1.100",
#     "rtsp://admin:Admin_TF24!@192.168.1.101",
#     "rtsp://admin:Admin_TF24!@192.168.1.102",
#     "rtsp://admin:Admin_TF24!@192.168.1.103"
# ]

# # Output directory
# output_dir = os.path.expanduser("~/Aeroponik")

# def capture_image(camera_index, rtsp_url):
#     cap = cv2.VideoCapture(rtsp_url)
#     if not cap.isOpened():
#         print(f"Error: Cannot open stream for camera {camera_index + 1}")
#         return

#     ret, frame = cap.read()
#     if ret:
#         now = datetime.now()
#         timestamp = now.strftime("%Y-%m-%d_%H-%M-%S")

#         # Create camera subfolder if it doesn't exist
#         camera_folder = f"camera_{camera_index + 1}"
#         camera_dir = os.path.join(output_dir, camera_folder)
#         os.makedirs(camera_dir, exist_ok=True)

#         filename = os.path.join(camera_dir, f"{timestamp}.jpg")
#         cv2.imwrite(filename, frame)
#         print(f"Image saved: {filename} for camera {camera_index + 1}")
#     else:
#         print(f"Error: Cannot capture image for camera {camera_index + 1}")

#     cap.release()

# def schedule_image_capture(sc):
#     for i, rtsp_url in enumerate(rtsp_urls):
#         capture_image(i, rtsp_url)
#     # Schedule the next call in ten minutes (600 seconds)
#     sc.enter(600, 1, schedule_image_capture, (sc,))

if __name__ == "__main__":
    # Schedule the first call immediately
    # scheduler.enter(0, 1, schedule_image_capture, (scheduler,))
    # Start the scheduler
    # scheduler.run()
    #path
    # im_path = "Sample/2024-07-20_15-36-15.jpg"
    # im_path = "Sample/2024-07-22_00-20-29.jpg"
    im_path = "Sample/2024-08-22_10-43-40.jpg"
    # im_path = "Sample/2024-07-26_08-52-33.jpg"
    # im_path = "Sample/2024-08-05_14-44-26.jpg"
    # im_path = "Sample"
    # im_path = os.path.join(im_path,random.choice(os.listdir(im_path)))
    full_im,status,length=check_length(im_path=im_path)
    cv2.imwrite("test.jpg", full_im)
    message = {
      'Params':{
        'Pixel per Millimeters':PPM,
        'Control Threshold':LIM_CM,
        'RoI':DIMX
      },
      'Measurements':{
        'Status':status,
        'Length (cm)':length}
      }
    jsonString = json.dumps(message, indent=2)
    print(jsonString)






