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
import paho.mqtt.client as mqtt
import threading

parser = argparse.ArgumentParser(description="Aeroponic Image Sampler & Measurement")
parser.add_argument("-th", "--threshold", help="Control Trigger Threshold in Centimeters")
parser.add_argument("-ppm", "--pixel_per_millimeters", help="Measurement Calibration")
parser.add_argument("-x0", "--x_roi_left", help="Left Side of RoI")
parser.add_argument("-x1", "--x_roi_right", help="Right Side of RoI")

args = parser.parse_args()

# Image Processing Setting Variables
if args.threshold is None:
    LIM_CM = 20.0  # Limit in centimeters
else:
    LIM_CM = float(args.threshold)

if args.pixel_per_millimeters is None:
    PPM = 6.0  # Pixel per millimeters calibration result
else:
    PPM = float(args.pixel_per_millimeters)

if args.x_roi_left is None or args.x_roi_right is None:
    DIMX = [700, 1400]  # Manual RoI Assignment
else:
    DIMX = [int(args.x_roi_left), int(args.x_roi_right)]

# MQTT broker information
broker_address = "broker.mqtt-dashboard.com"  # Public broker for testing
port = 1883  # Standard MQTT port
pub_topic = "test/aeroponik/reading"  # Topic to publish
sub_topic = "test/aeroponik/settings/#"  # Topic to subscribe

# # List of IP Camera RTSP URLs
# rtsp_urls = [
#     "rtsp://admin:Admin_TF24!@192.168.1.100",
#     "rtsp://admin:Admin_TF24!@192.168.1.101",
#     "rtsp://admin:Admin_TF24!@192.168.1.102",
#     "rtsp://admin:Admin_TF24!@192.168.1.103"
# ]

# Scheduler instance
scheduler = sched.scheduler(time.time, time.sleep)

# Output directory
output_dir = os.path.expanduser("~/Aeroponik")
im_path = "Sample"
im_path = os.path.join(im_path, random.choice(os.listdir(im_path)))

# MQTT callback when a message is received
def on_message(client, userdata, message):
    print(f"Received message '{message.payload.decode()}' on topic '{message.topic}'")
    topic = message.topic.split('/')[-1]
    if topic == 'lim':
        global LIM_CM
        LIM_CM = int(message.payload.decode())
    if topic == 'ppm':
        global PPM
        PPM = int(message.payload.decode())
    if topic == 'dimx0':
        global DIMX
        DIMX[0] = int(message.payload.decode())
    if topic == 'dimx1':
        DIMX[1] = int(message.payload.decode())

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

def capture_image_offline(im_path, client):
    now = datetime.now()
    timestamp = now.strftime("%Y-%m-%d_%H-%M-%S")
    frame = cv2.imread(im_path)

    # Write captured image
    cv2.imwrite(f"{timestamp}.jpg", frame)
    
    # Simulated function imutils.check_length (as original function details are missing)
    full_im, deb_im, status, length = imutils.check_length(img=frame, dimx=DIMX, ppm=PPM, lim_cm=LIM_CM)
    # Parse message into jsonString
    message = {
        'Params': {
            'Pixel per Millimeters': PPM,
            'Control Threshold': LIM_CM,
            'RoI': DIMX
        },
        'Measurements': {
            'Status': status,
            'Length (cm)': length
        }
    }
    jsonString = json.dumps(message, indent=2)
    # print(jsonString)
    # Publish json sring
    client.publish(pub_topic, jsonString)  # Publish the message using the client
    # Write output image
    cv2.imwrite(f"{timestamp}_measure.jpg", full_im)

def schedule_image_capture(sc, client):
  # for i, rtsp_url in enumerate(rtsp_urls):
  #     capture_image(i, rtsp_url)
    capture_image_offline(im_path, client)
    # Schedule the next call in ten minutes (600 seconds)
    sc.enter(10, 1, schedule_image_capture, (sc, client))

# Thread for MQTT loop
def mqtt_thread(client):
    client.on_message = on_message
    client.connect(broker_address, port)
    client.subscribe(sub_topic)
    client.loop_forever()

# Thread for scheduler
def scheduler_thread(client):
    scheduler.enter(0, 1, schedule_image_capture, (scheduler, client))
    scheduler.run()

if __name__ == "__main__":
    # Create the MQTT client instance
    client = mqtt.Client()

    # Create and start the MQTT thread
    mqtt_t = threading.Thread(target=mqtt_thread, args=(client,))
    mqtt_t.start()

    # Create and start the scheduler thread
    scheduler_t = threading.Thread(target=scheduler_thread, args=(client,))
    scheduler_t.start()

    # Join threads to keep the main program running
    mqtt_t.join()
    scheduler_t.join()
